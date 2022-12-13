
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of bg_api(https://github.com/patrickk33/bg_api) project. A fork of 
// niXman's binapi(https://github.com/niXman/binapi) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#include <bg_api/websocket.hpp>
#include <bg_api/types.hpp>
#include <bg_api/message.hpp>
#include <bg_api/fnv1a.hpp>
#include <bg_api/flatjson.hpp>
#include <bg_api/errors.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

#include <boost/callable_traits.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <boost/intrusive/set.hpp>

#include <map>
#include <set>
#include <cstring>

//#include <iostream> // TODO: comment out

#define __bg_api_CB_ON_ERROR(cb, ec) \
    cb(__FILE__ "(" BOOST_PP_STRINGIZE(__LINE__) ")", ec.value(), ec.message(), nullptr, 0);

namespace bg_api {
namespace ws {

struct websockets;

/*************************************************************************************************/

struct websocket: std::enable_shared_from_this<websocket> {
    friend struct websockets;

    explicit websocket(boost::asio::io_context &ioctx)
        :m_ioctx{ioctx}
        ,m_ssl{boost::asio::ssl::context::sslv23_client}
        ,m_resolver{m_ioctx}
        ,m_ws{m_ioctx, m_ssl}
        ,m_buf{}
        ,m_host{}
        ,m_target{}
        ,m_stop_requested{}
    {}
    virtual ~websocket()
    {}

    using on_message_received_cb = std::function<
        bool(const char *fl, int ec, std::string errmsg, const char *ptr, std::size_t size)
    >; // when 'false' returned the stop will called

    using holder_type = std::shared_ptr<websocket>;
    void start(const std::string &host, const std::string &port, const std::string &target, on_message_received_cb cb, holder_type holder)
    { return async_start(host, port, target, std::move(cb), std::move(holder)); }

    void stop() {
        m_stop_requested = true;

        if ( m_ws.next_layer().next_layer().is_open() ) {
            boost::system::error_code ec;
            m_ws.close(boost::beast::websocket::close_code::normal, ec);
        }
    }

    void async_stop() {
        m_stop_requested = true;
        holder_type holder = shared_from_this();

        if ( m_ws.next_layer().next_layer().is_open() ) {
            m_ws.async_close(
                 boost::beast::websocket::close_code::normal
                ,[holder=std::move(holder)](const boost::system::error_code &){}
            );
        }
    }

private:
    void async_start(const std::string &host, const std::string &port, const std::string &target, on_message_received_cb cb, holder_type holder) {
        m_host = host;
        m_target = target;

        m_resolver.async_resolve(
             m_host
            ,port
            ,[this, cb=std::move(cb), holder=std::move(holder)]
             (boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type res) mutable {
                if ( ec ) {
                    if ( !m_stop_requested ) { __bg_api_CB_ON_ERROR(cb, ec); }
                } else {
                    async_connect(std::move(res), std::move(cb), std::move(holder));
                }
            }
        );
    }
    void async_connect(boost::asio::ip::tcp::resolver::results_type res, on_message_received_cb cb, holder_type holder) {
        if( !SSL_set_tlsext_host_name(m_ws.next_layer().native_handle() ,m_host.c_str())) {
            auto error_code = boost::beast::error_code(
                 static_cast<int>(::ERR_get_error())
                ,boost::asio::error::get_ssl_category()
            );

            __bg_api_CB_ON_ERROR(cb, error_code);

            return;
        }

        boost::asio::async_connect(
             m_ws.next_layer().next_layer()
            ,res.begin()
            ,res.end()
            ,[this, cb=std::move(cb), holder=std::move(holder)]
             (boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator) mutable {
                if ( ec ) {
                    if ( !m_stop_requested ) { __bg_api_CB_ON_ERROR(cb, ec); }
                } else {
                    on_connected(std::move(cb), std::move(holder));
                }
            }
        );
    }
    void on_connected(on_message_received_cb cb, holder_type holder) {
        m_ws.control_callback(
            [this]
            (boost::beast::websocket::frame_type kind, boost::beast::string_view payload) mutable {
                (void)kind; (void) payload;
                //std::cout << "control_callback(" << this << "): kind=" << static_cast<int>(kind) << ", payload=" << payload.data() << std::endl;
                m_ws.async_pong(
                     boost::beast::websocket::ping_data{}
                    ,[](boost::beast::error_code ec)
                     { (void)ec; /*std::cout << "control_callback_cb(" << this << "): ec=" << ec << std::endl;*/ }
                );
            }
        );

        m_ws.next_layer().async_handshake(
             boost::asio::ssl::stream_base::client
            ,[this, cb=std::move(cb), holder=std::move(holder)]
             (boost::system::error_code ec) mutable {
                if ( ec ) {
                    if ( !m_stop_requested ) { __bg_api_CB_ON_ERROR(cb, ec); }
                } else {
                    on_async_ssl_handshake(std::move(cb), std::move(holder));
                }
            }
        );
    }
    void on_async_ssl_handshake(on_message_received_cb cb, holder_type holder) {
        m_ws.async_handshake(
             m_host
            ,m_target
            ,[this, cb=std::move(cb), holder=std::move(holder)]
             (boost::system::error_code ec) mutable
             { start_read(ec, std::move(cb), std::move(holder)); }
        );
    }

    void start_read(boost::system::error_code ec, on_message_received_cb cb, holder_type holder) {
        if ( ec ) {
            if ( !m_stop_requested ) {
                __bg_api_CB_ON_ERROR(cb, ec);
            }

            stop();

            return;
        }

        m_ws.async_read(
             m_buf
            ,[this, cb=std::move(cb), holder=std::move(holder)]
             (boost::system::error_code ec, std::size_t rd) mutable
             { on_read(ec, rd, std::move(cb), std::move(holder)); }
        );
    }
    void on_read(boost::system::error_code ec, std::size_t rd, on_message_received_cb cb, holder_type holder) {
        if ( ec ) {
            if ( !m_stop_requested ) {
                __bg_api_CB_ON_ERROR(cb, ec);
            }

            stop();

            return;
        }

        auto size = m_buf.size();
        assert(size == rd);

        std::string strbuf;
        strbuf.reserve(size);

        for ( const auto &it: m_buf.data() ) {
            strbuf.append(static_cast<const char *>(it.data()), it.size());
        }
        m_buf.consume(m_buf.size());

        bool ok = cb(nullptr, 0, std::string{}, strbuf.data(), strbuf.size());
        if ( !ok ) {
            stop();
        } else {
            start_read(boost::system::error_code{}, std::move(cb), std::move(holder));
        }
    }

    boost::asio::io_context &m_ioctx;
    boost::asio::ssl::context m_ssl;
    boost::asio::ip::tcp::resolver m_resolver;
    boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> m_ws;
    boost::beast::multi_buffer m_buf;
    std::string m_host;
    std::string m_target;
    bool m_stop_requested;
    boost::intrusive::set_member_hook<> m_intrusive_set_hook;
};

struct websocket_id_getter {
    using type = const void *;
    type operator()(const websocket &sock) const { return std::addressof(sock); }
};

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

struct websockets::impl {
    impl(
        boost::asio::io_context &ioctx,
        std::string host,
        std::string port,
        std::string pk,
        std::string passphrase,
        std::string sk,
        on_message_received_cb cb
    ) :
        m_ioctx{ioctx},
        m_host{std::move(host)},
        m_port{std::move(port)},
        m_pk{std::move(pk)},
        m_passphrase{std::move(passphrase)},
        m_sk{std::move(sk)},
        m_on_message{std::move(cb)},
        m_set{}
    {}
    ~impl() {
        unsubscribe_all();
    }

    static std::string make_channel_name(const char *pair, const char *channel) {
        std::string res{"/ws/"};
        if ( pair ) {
            res += pair;
            if ( *pair != '!' ) {
                boost::algorithm::to_lower(res);
            }

            res += '@';
        }

        res += channel;

        return res;
    }

    template<typename F>
    websockets::handle start_channel(const char *pair, const char *channel, F cb) {
        using args_tuple = typename boost::callable_traits::args<F>::type;
        using message_type = typename std::tuple_element<3, args_tuple>::type;

        static const auto deleter = [this](websocket *ws) {
            auto it = m_set.find(ws);
            if ( it != m_set.end() ) {
                m_set.erase(it);
            }

            delete ws;
        };
        std::shared_ptr<websocket> ws{new websocket(m_ioctx), deleter};
        std::string schannel = make_channel_name(pair, channel);

        auto wscb = [this, schannel, cb=std::move(cb)]
            (const char *fl, int ec, std::string errmsg, const char *ptr, std::size_t size) -> bool
        {
            if ( ec ) {
                try {
                    cb(fl, ec, std::move(errmsg), message_type{});
                } catch (const std::exception &ex) {
                    std::fprintf(stderr, "%s: %s\n", __MAKE_FILELINE, ex.what());
                    std::fflush(stderr);
                }

                return false;
            }

            const flatjson::fjson json{ptr, size};
            if ( json.is_object() && bg_api::rest::is_api_error(json) ) {
                auto error = bg_api::rest::construct_error(json);
                auto ecode = error.first;
                auto emsg  = std::move(error.second);

                try {
                    message_type message{};
                    return cb(__MAKE_FILELINE, ecode, std::move(emsg), std::move(message));
                } catch (const std::exception &ex) {
                    std::fprintf(stderr, "%s: %s\n", __MAKE_FILELINE, ex.what());
                    std::fflush(stderr);
                }
            }

            try {
                if ( m_on_message ) { m_on_message(schannel.c_str(), ptr, size); }
            } catch (const std::exception &ex) {
                std::fprintf(stderr, "%s: %s\n", __MAKE_FILELINE, ex.what());
                std::fflush(stderr);
            }

            try {
                message_type message = message_type::construct(json);
                return cb(fl, ec, std::move(errmsg), std::move(message));
            } catch (const std::exception &ex) {
                std::fprintf(stderr, "%s: %s\n", __MAKE_FILELINE, ex.what());
                std::fflush(stderr);
            }

            return false;
        };

        auto *ptr = ws.get();
        ptr->start(
             m_host
            ,m_port
            ,schannel
            ,std::move(wscb)
            ,std::move(ws)
        );

        m_set.insert(*ptr);

        return ptr;
    }

    template<typename F>
    void stop_channel_impl(handle h, F f) {
        auto it = m_set.find(h);
        if ( it == m_set.end() ) { return; }

        auto *ws = static_cast<websocket *>(&(*it));
        f(ws);

        m_set.erase(it);
    }

    void stop_channel(handle h) {
        return stop_channel_impl(h, [](auto sp){ sp->stop(); });
    }
    void async_stop_channel(handle h) {
        return stop_channel_impl(h, [](auto sp){ sp->async_stop(); });
    }

    template<typename F>
    void unsubscribe_all_impl(F f) {
        for ( auto it = m_set.begin(); it != m_set.end(); ) {
            auto *ws = static_cast<websocket *>(&(*it));
            f(ws);

            it = m_set.erase(it);
        }
    }
    void unsubscribe_all() {
        return unsubscribe_all_impl([](auto sp){ sp->stop(); });
    }
    void async_unsubscribe_all() {
        return unsubscribe_all_impl([](auto sp){ sp->async_stop(); });
    }

    boost::asio::io_context &m_ioctx;
    std::string m_host;
    std::string m_port;
    std::string m_pk;
    std::string m_passphrase;
    std::string m_sk;
    on_message_received_cb m_on_message;
    boost::intrusive::set<
         websocket
        ,boost::intrusive::key_of_value<websocket_id_getter>
        ,boost::intrusive::member_hook<
             websocket
            ,boost::intrusive::set_member_hook<>
            ,&websocket::m_intrusive_set_hook
        >
    > m_set;
};

/*************************************************************************************************/

websockets::websockets(
     boost::asio::io_context &ioctx,
    std::string host,
    std::string port,
    std::string pk,
    std::string passphrase,
    std::string sk,
    on_message_received_cb cb
)
    :pimpl{
        std::make_unique<impl>(ioctx,
        std::move(host),
        std::move(port),
        std::move(pk),
        std::move(passphrase),
        std::move(sk),
        std::move(cb))
    }
{}

websockets::~websockets()
{}


} // ns ws
} // ns bg_api
