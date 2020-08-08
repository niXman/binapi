
#include <binapi/websocket.hpp>
#include <binapi/types.hpp>
#include <binapi/invoker.hpp>
#include <binapi/fnv1a.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

#include <boost/callable_traits.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <map>
#include <set>
#include <cstring>
#include <iostream> // TODO: comment out

namespace binapi {
namespace ws {

/*************************************************************************************************/

#define __CB_ON_ERROR(cb, ec) \
    cb(__FILE__ "(" BOOST_PP_STRINGIZE(__LINE__) ")", ec.value(), ec.message(), nullptr, 0);

struct websocket::impl {
    impl(boost::asio::io_context &ioctx)
        :m_ioctx{ioctx}
        ,m_ssl{boost::asio::ssl::context::sslv23_client}
        ,m_resolver{m_ioctx}
        ,m_ws{m_ioctx, m_ssl}
        ,m_buf{}
        ,m_host{}
        ,m_target{}
    {}

    void async_start(const std::string &host, const std::string &port, const std::string &target, on_message_received_cb cb) {
        m_host = host;
        m_target = target;

        m_resolver.async_resolve(m_host, port,
            [this, cb=std::move(cb)]
            (boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type res) mutable {
                if ( ec ) {
                    __CB_ON_ERROR(cb, ec);
                } else {
                    async_connect(std::move(res), std::move(cb));
                }
            }
        );
    }
    void async_connect(boost::asio::ip::tcp::resolver::results_type res, on_message_received_cb cb) {
        boost::asio::async_connect(m_ws.next_layer().next_layer(), res.begin(), res.end(),
            [this, cb=std::move(cb)]
            (boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator) mutable {
                if ( ec ) {
                    __CB_ON_ERROR(cb, ec);
                } else {
                    on_connected(std::move(cb));
                }
            }
        );
    }
    void on_connected(on_message_received_cb cb) {
        m_ws.control_callback(
            [this](boost::beast::websocket::frame_type kind, boost::beast::string_view payload){
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
            ,[this, cb=std::move(cb)]
            (boost::system::error_code ec) mutable {
                if ( ec ) {
                    __CB_ON_ERROR(cb, ec);
                } else {
                    on_async_ssl_handshake(std::move(cb));
                }
            }
        );
    }
    void on_async_ssl_handshake(on_message_received_cb cb) {
        m_ws.async_handshake(m_host, m_target,
            [this, cb=std::move(cb)](boost::system::error_code ec) mutable
            { start_read(ec, std::move(cb)); }
        );
    }

    void stop() {
        if ( m_ws.next_layer().next_layer().is_open() ) {
            boost::system::error_code ec;
            m_ws.close(boost::beast::websocket::close_code::normal, ec);
        }
    }

    void start_read(boost::system::error_code ec, on_message_received_cb cb) {
        if ( ec ) {
            __CB_ON_ERROR(cb, ec);

            return;
        }

        m_ws.async_read(m_buf,
            [this, cb=std::move(cb)]
            (boost::system::error_code ec, std::size_t rd) mutable
            { on_read(ec, rd, std::move(cb)); }
        );
    }
    void on_read(boost::system::error_code ec, std::size_t rd, on_message_received_cb cb) {
        if ( ec ) { return; }

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
            start_read(boost::system::error_code{}, std::move(cb));
        }
    }

    boost::asio::io_context &m_ioctx;
    boost::asio::ssl::context m_ssl;
    boost::asio::ip::tcp::resolver m_resolver;
    boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> m_ws;
    boost::beast::multi_buffer m_buf;
    std::string m_host;
    std::string m_target;
};

/*************************************************************************************************/

websocket::websocket(boost::asio::io_context &ioctx)
    :pimpl{std::make_unique<impl>(ioctx)}
{}

websocket::~websocket()
{}

/*************************************************************************************************/

void websocket::start(const std::string &host, const std::string &port, const std::string &target, on_message_received_cb cb)
{ return pimpl->async_start(host, port, target, std::move(cb)); }

void websocket::stop() { return pimpl->stop(); }

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

struct websockets_pool::impl {
    impl(boost::asio::io_context &ioctx, const std::string &host, const std::string &port, on_message_received_cb cb)
        :m_ioctx{ioctx}
        ,m_host{host}
        ,m_port{port}
        ,m_on_message{std::move(cb)}
        ,m_map{}
    {}

    static std::string make_channel(const char *pair, const char *channel) {
        std::string res{"/ws/"};
        if ( pair ) {
            res += pair;
            boost::algorithm::to_lower(res);
            res += '@';
        }

        res += channel;

        return res;
    }

    template<typename F>
    websockets_pool::handle start_channel(const char *pair, const char *channel, F cb) {
        using args_tuple = typename boost::callable_traits::args<decltype(cb)>::type;
        using message_type = typename std::tuple_element<3, args_tuple>::type;
        using return_type = typename boost::callable_traits::return_type<F>::type;
        using invoker_type = detail::invoker<return_type, message_type, F>;

        auto invoker = std::make_shared<invoker_type>(std::move(cb));
        websockets_pool::handle h = invoker.get();
        std::string schannel = make_channel(pair, channel);

        websocket ws{m_ioctx};
        ws.start(
             m_host
            ,m_port
            ,schannel
            ,[this, schannel, inv=std::move(invoker)]
             (const char *fl, int ec, std::string errmsg, const char *ptr, std::size_t size) {
                 try {
                     if ( m_on_message ) { m_on_message(schannel.c_str(), ptr, size); }
                 } catch (const std::exception &ex) {
                     std::fprintf(stderr, "%s(%d): %s", __FILE__, __LINE__, ex.what());
                 }

                 return inv->invoke(fl, ec, std::move(errmsg), ptr, size);
             }
        );

        m_map.emplace(h, std::move(ws));

        return h;
    }

    void stop_channel(const handle h) {
        auto it = m_map.find(h);
        if ( it == m_map.end() ) { return; }

        it->second.stop();

        m_map.erase(it);
    }

    void unsubscribe_all() {
        for ( auto it = m_map.begin(); it != m_map.end(); ++it ) {
            it->second.stop();
            it = m_map.erase(it);
        }
    }

    boost::asio::io_context &m_ioctx;
    std::string m_host;
    std::string m_port;
    on_message_received_cb m_on_message;
    std::map<websockets_pool::handle, websocket> m_map;
};

/*************************************************************************************************/

websockets_pool::websockets_pool(
     boost::asio::io_context &ioctx
    ,const std::string &host
    ,const std::string &port
    ,on_message_received_cb cb
)
    :pimpl{std::make_unique<impl>(ioctx, host, port, std::move(cb))}
{}

websockets_pool::~websockets_pool()
{}

/*************************************************************************************************/

websockets_pool::handle websockets_pool::subscribe_depth(const char *pair, on_depth_received_cd cb)
{ return pimpl->start_channel(pair, "depth", std::move(cb)); }

void websockets_pool::unsubscribe_depth(const handle h) { return pimpl->stop_channel(h); }

/*************************************************************************************************/

websockets_pool::handle websockets_pool::subscribe_klines(const char *pair, const char *period, on_kline_received_cd cb) {
    static const auto switch_ = [](const char *period) -> const char * {
        const auto hash = fnv1a(period);
        switch ( hash ) {
            // mins
            case fnv1a("1m"): return "kline_1m";
            case fnv1a("3m"): return "kline_3m";
            case fnv1a("5m"): return "kline_5m";
            case fnv1a("15m"): return "kline_15m";
            case fnv1a("30m"): return "kline_30m";
            // hours
            case fnv1a("1h"): return "kline_1h";
            case fnv1a("2h"): return "kline_2h";
            case fnv1a("4h"): return "kline_4h";
            case fnv1a("6h"): return "kline_6h";
            case fnv1a("8h"): return "kline_8h";
            case fnv1a("12h"): return "kline_12h";
            // days
            case fnv1a("1d"): return "kline_1d";
            case fnv1a("3d"): return "kline_3d";
            // other
            case fnv1a("1w"): return "kline_1w";
            case fnv1a("1M"): return "kline_1M";
            //
            default: return nullptr;
        }
    };

    const char *p = switch_(period);
    assert(p != nullptr);

    return pimpl->start_channel(pair, p, std::move(cb));
}

void websockets_pool::unsubscribe_klines(const handle h) { return pimpl->stop_channel(h); }

/*************************************************************************************************/

websockets_pool::handle websockets_pool::subscribe_trade(const char *pair, on_trade_received_cd cb)
{ return pimpl->start_channel(pair, "trade", std::move(cb)); }

void websockets_pool::unsubscribe_trade(const handle h) { return pimpl->stop_channel(h); }

/*************************************************************************************************/

websockets_pool::handle websockets_pool::subscribe_agg_trade(const char *pair, on_agg_trade_received_cd cb)
{ return pimpl->start_channel(pair, "aggTrade", std::move(cb)); }

void websockets_pool::unsubscribe_agg_trade(const handle h) { return pimpl->stop_channel(h); }

/*************************************************************************************************/

websockets_pool::handle websockets_pool::subscribe_market(const char *pair, on_market_received_cd cb)
{ return pimpl->start_channel(pair, "ticker", std::move(cb)); }

void websockets_pool::unsubscribe_market(const handle h) { return pimpl->stop_channel(h); }

websockets_pool::handle websockets_pool::subscribe_markets(on_markets_received_cd cb)
{ return pimpl->start_channel("!ticker", "arr", std::move(cb)); }

void websockets_pool::unsubscribe_markets(const handle h) { return pimpl->stop_channel(h); }

/*************************************************************************************************/

websockets_pool::handle websockets_pool::subscribe_userdata(const char *lkey, on_order_update_cb ocb, on_account_update_cb acb) {
    auto cb = [ocb=std::move(ocb), acb=std::move(acb)](const char *fl, int ec, std::string errmsg, userdata::userdata_stream_t msg) {
        if ( ec ) {
            ocb(fl, ec, errmsg, userdata::order_update_t{});
            acb(fl, ec, errmsg, userdata::account_update_t{});

            return false;
        }

        if ( msg.data.find("outboundAccountInfo") != std::string::npos ) {
            userdata::account_update_t res = userdata::account_update_t::parse(msg.data.c_str(), msg.data.length());
            return acb(fl, ec, errmsg, std::move(res));
        } else {
            userdata::order_update_t res = userdata::order_update_t::parse(msg.data.c_str(), msg.data.length());
            return ocb(fl, ec, errmsg, std::move(res));
        }
    };

    return pimpl->start_channel(nullptr, lkey, std::move(cb));
}

void websockets_pool::unsubscribe_userdata(const handle h) { return pimpl->stop_channel(h); }

/*************************************************************************************************/

void websockets_pool::unsubscribe_all() { return pimpl->unsubscribe_all(); }

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

} // ns ws
} // ns binapi
