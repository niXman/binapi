
// ----------------------------------------------------------------------------
//                              Apache License
//                        Version 2.0, January 2004
//                     http://www.apache.org/licenses/
//
// This file is part of FlatJSON(https://github.com/niXman/flatjson) project.
//
// Copyright (c) 2019-2021 niXman (github dot nixman dog pm.me). All rights reserved.
// ----------------------------------------------------------------------------

#ifndef __FLATJSON__FLATJSON_HPP
#define __FLATJSON__FLATJSON_HPP

#include <limits>
#include <ostream>
#include <iterator>
#include <vector>
#include <string>
#include <memory>

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>

/*************************************************************************************************/

#if defined(__clang__)
#   define __FLATJSON__FALLTHROUGH [[clang::fallthrough]]
#elif defined(__GNUC__)
#   define __FLATJSON__FALLTHROUGH __attribute__ ((fallthrough))
#elif defined(_MSC_VER)
#   define __FLATJSON__FALLTHROUGH
#else
#   error "Unknown compiler"
#endif //

/*************************************************************************************************/

#define __FLATJSON__STRINGIZE_I(x) #x
#define __FLATJSON__STRINGIZE(x) __FLATJSON__STRINGIZE_I(x)

#define __FLATJSON__MAKE_ERROR_MESSAGE(msg) \
    __FILE__ "(" __FLATJSON__STRINGIZE(__LINE__) "): " msg

#ifdef __FLATJSON__DONT_CHECK_OVERFLOW
#   define __FLATJSON__CHECK_OVERFLOW(expr, type, err)
#else
#   define __FLATJSON__CHECK_OVERFLOW(expr, type, err) \
        if ( (expr) >= (std::numeric_limits<type>::max)() ) return err
#endif //__FLATJSON__SHOULD_CHECK_OVERFLOW

#ifndef __FLATJSON__KLEN_TYPE
#   define __FLATJSON__KLEN_TYPE std::uint8_t
#endif // __FLATJSON__KLEN_TYPE
#ifndef __FLATJSON__VLEN_TYPE
#   define __FLATJSON__VLEN_TYPE std::uint32_t
#endif // __FLATJSON__VLEN_TYPE
#ifndef __FLATJSON__CHILDS_TYPE
#   define __FLATJSON__CHILDS_TYPE std::uint16_t
#endif // __FLATJSON__CHILDS_TYPE

/*************************************************************************************************/

#if __cplusplus >= 201703L
#   include <string_view>
namespace flatjson {
using static_string = std::string_view;
} // ns flatjson
#else
namespace flatjson {
namespace details {

/*************************************************************************************************/

struct static_string {
    static_string() = default;
    template<std::size_t N>
    explicit static_string(const char (&str)[N])
        :m_ptr{str}
        ,m_len{N-1}
    {}
    static_string(const char *ptr, std::size_t len)
        :m_ptr{ptr}
        ,m_len{len}
    {}
    static_string(const char *beg, const char *end)
        :m_ptr{beg}
        ,m_len{static_cast<std::size_t>(end-beg)}
    {}

    std::size_t size() const { return m_len; }
    bool empty() const { return size() == 0; }
    const char* data() const { return m_ptr; }

    friend bool operator==(const static_string &l, const char *r) { return std::strncmp(l.m_ptr, r, l.m_len) == 0; }
    friend bool operator==(const static_string &l, const static_string &r) { return std::strncmp(l.m_ptr, r.m_ptr, l.m_len) == 0; }
    friend bool operator!=(const static_string &l, const char *r) { return !(l == r); }
    friend bool operator!=(const static_string &l, const static_string &r) { return !(l == r); }

    friend std::ostream& operator<< (std::ostream &os, const static_string &s) {
        os.write(s.m_ptr, s.m_len);

        return os;
    }

private:
    const char* m_ptr;
    std::size_t m_len;
};

/*************************************************************************************************/

} // ns details

using static_string = details::static_string;

} // ns flatjson
#endif // __cplusplus >= 201703L

namespace flatjson {

/*************************************************************************************************/

enum e_fj_token_type: std::uint8_t {
     FJ_TYPE_INVALID = 0
    ,FJ_TYPE_STRING
    ,FJ_TYPE_NUMBER
    ,FJ_TYPE_BOOL
    ,FJ_TYPE_NULL
    ,FJ_TYPE_OBJECT
    ,FJ_TYPE_OBJECT_END
    ,FJ_TYPE_ARRAY
    ,FJ_TYPE_ARRAY_END
    ,__MAX_FJ_TYPE_ID // must be latest
};

inline const char *fj_token_type_name(e_fj_token_type t) {
    static const char* strs[] = {
         "INVALID"
        ,"STRING"
        ,"NUMBER"
        ,"BOOL"
        ,"NULL"
        ,"OBJECT"
        ,"OBJECT_END"
        ,"ARRAY"
        ,"ARRAY_END"
    };

    std::size_t idx = static_cast<std::size_t>(t);
    if ( idx < sizeof(strs)/sizeof(strs[0]) ) {
        return strs[idx];
    }

    return "UNKNOWN TYPE";
}

enum e_fj_error_code {
     FJ_EC_OK = 0
    ,FJ_EC_INVALID = -1
    ,FJ_EC_INCOMPLETE = -2
    ,FJ_EC_NO_FREE_TOKENS = -3
    ,FJ_EC_KLEN_OVERFLOW = -4
    ,FJ_EC_VLEN_OVERFLOW = -5
    ,FJ_EC_CHILDS_OVERFLOW = -6
};

inline const char* fj_error_string(e_fj_error_code e) {
    static const char* strs[] = {
         "OK"
        ,"INVALID"
        ,"INCOMPLETE"
        ,"NO_FREE_TOKENS"
        ,"KLEN_OVERFLOW"
        ,"VLEN_OVERFLOW"
        ,"CHILDS_OVERFLOW"
    };
    int idx = static_cast<int>(e);
    idx = -idx;
    if ( static_cast<unsigned>(idx) < sizeof(strs)/sizeof(strs[0]) ) {
        return strs[idx];
    }

    return "UNKNOWN ERROR";
}

/*************************************************************************************************/

namespace details {

#define _FJ_CASE_1(x) \
    case x:

#define _FJ_CASE_3(S) \
    _FJ_CASE_1(S) _FJ_CASE_1(S+1) _FJ_CASE_1(S+2)

#define _FJ_CASE_5(S) \
    _FJ_CASE_3(S) _FJ_CASE_1(S+3) _FJ_CASE_1(S+4)

#define _FJ_CASE_10(S) \
    _FJ_CASE_5(S) _FJ_CASE_5(S+5)

#define _FJ_CASE_20(S) \
    _FJ_CASE_10(S) _FJ_CASE_10(S+10)

#define _FJ_CASE_40(S) \
    _FJ_CASE_20(S) _FJ_CASE_20(S+20)

template<typename CharT>
bool fj_is_simple_type(CharT v) {
    switch ( static_cast<std::uint8_t>(v) ) {
        _FJ_CASE_1(0) \
            return false;
        _FJ_CASE_3(1) \
        _FJ_CASE_1(4) \
            return true;
        default:
            return false;
    }
}

template<typename CharT>
bool fj_is_digit(CharT ch) {
    switch ( static_cast<std::uint8_t>(ch) ) {
        _FJ_CASE_20(0) \
        _FJ_CASE_20(20) \
        _FJ_CASE_5(40) \
        _FJ_CASE_3(45) \
            return false;
        _FJ_CASE_10(48) \
            return true;
        default:
            return false;
    }
}

template<typename CharT>
bool fj_is_hex_digit(CharT ch) {
    if ( fj_is_digit(ch) ) {
        return true;
    }

    switch ( static_cast<std::uint8_t>(ch) ) {
        _FJ_CASE_20(0) \
        _FJ_CASE_20(20) \
        _FJ_CASE_20(40) \
        _FJ_CASE_5(60) \
            return false;
        _FJ_CASE_3(65) \
        _FJ_CASE_3(68) \
            return true;
        _FJ_CASE_20(71) \
        _FJ_CASE_3(91) \
        _FJ_CASE_3(94) \
            return false;
         _FJ_CASE_3(97) \
         _FJ_CASE_3(100) \
            return true;
        default:
            return false;
    }
}

template<typename CharT>
bool fj_is_hex_digit4(CharT ch0, CharT ch1, CharT ch2, CharT ch3) {
    return fj_is_hex_digit(ch0) && fj_is_hex_digit(ch1) && fj_is_hex_digit(ch2) && fj_is_hex_digit(ch3);
}

template<typename CharT>
std::size_t fj_utf8_char_len(CharT ch) {
    switch ( static_cast<std::uint8_t>(ch) ) {
        _FJ_CASE_40(0) \
        _FJ_CASE_40(40) \
        _FJ_CASE_40(80) \
        _FJ_CASE_5 (120) \
        _FJ_CASE_3 (125) \
            return 1;
        _FJ_CASE_40(128) \
        _FJ_CASE_40(168) \
        _FJ_CASE_10(208) \
        _FJ_CASE_5 (218) \
        _FJ_CASE_1 (223) \
            return 2;
        _FJ_CASE_10(224) \
        _FJ_CASE_5 (234) \
        _FJ_CASE_1 (239) \
            return 3;
        _FJ_CASE_10(240) \
        _FJ_CASE_5 (250) \
            return 4;
        default:
            assert("unrechable!" == nullptr);
    }
}

#undef _FJ_CASE_1
#undef _FJ_CASE_3
#undef _FJ_CASE_5
#undef _FJ_CASE_10
#undef _FJ_CASE_20
#undef _FJ_CASE_40

/*************************************************************************************************/

template<typename To>
static typename std::enable_if<
    (std::is_integral<To>::value && std::is_unsigned<To>::value) &&
    !std::is_same<To, bool>::value, To
>::type
conv_to(const char *ptr, std::size_t len) {
    const auto *str = reinterpret_cast<const std::uint8_t *>(ptr);
    std::uint64_t v = 0;
    switch ( len ) {
        case 20: v = v + (str[len - 20] - '0') * 10000000000000000000ull; __FLATJSON__FALLTHROUGH;
        case 19: v = v + (str[len - 19] - '0') * 1000000000000000000ull; __FLATJSON__FALLTHROUGH;
        case 18: v = v + (str[len - 18] - '0') * 100000000000000000ull; __FLATJSON__FALLTHROUGH;
        case 17: v = v + (str[len - 17] - '0') * 10000000000000000ull; __FLATJSON__FALLTHROUGH;
        case 16: v = v + (str[len - 16] - '0') * 1000000000000000ull; __FLATJSON__FALLTHROUGH;
        case 15: v = v + (str[len - 15] - '0') * 100000000000000ull; __FLATJSON__FALLTHROUGH;
        case 14: v = v + (str[len - 14] - '0') * 10000000000000ull; __FLATJSON__FALLTHROUGH;
        case 13: v = v + (str[len - 13] - '0') * 1000000000000ull; __FLATJSON__FALLTHROUGH;
        case 12: v = v + (str[len - 12] - '0') * 100000000000ull; __FLATJSON__FALLTHROUGH;
        case 11: v = v + (str[len - 11] - '0') * 10000000000ull; __FLATJSON__FALLTHROUGH;
        case 10: v = v + (str[len - 10] - '0') * 1000000000ull; __FLATJSON__FALLTHROUGH;
        case 9 : v = v + (str[len - 9 ] - '0') * 100000000ull; __FLATJSON__FALLTHROUGH;
        case 8 : v = v + (str[len - 8 ] - '0') * 10000000ull; __FLATJSON__FALLTHROUGH;
        case 7 : v = v + (str[len - 7 ] - '0') * 1000000ull; __FLATJSON__FALLTHROUGH;
        case 6 : v = v + (str[len - 6 ] - '0') * 100000ull; __FLATJSON__FALLTHROUGH;
        case 5 : v = v + (str[len - 5 ] - '0') * 10000ull; __FLATJSON__FALLTHROUGH;
        case 4 : v = v + (str[len - 4 ] - '0') * 1000ull; __FLATJSON__FALLTHROUGH;
        case 3 : v = v + (str[len - 3 ] - '0') * 100ull; __FLATJSON__FALLTHROUGH;
        case 2 : v = v + (str[len - 2 ] - '0') * 10ull; __FLATJSON__FALLTHROUGH;
        case 1 : v = v + (str[len - 1 ] - '0') * 1ull; __FLATJSON__FALLTHROUGH;
        default: break;
    }

    return static_cast<To>(v);
}

template<typename To>
static typename std::enable_if<
    (std::is_integral<To>::value && std::is_signed<To>::value) &&
    !std::is_same<To, bool>::value, To
>::type
conv_to(const char *ptr, std::size_t len) {
    if ( *ptr == '-' ) {
        ++ptr;
        return -conv_to<typename std::make_unsigned<To>::type>(ptr, len - 1);
    }

    return conv_to<typename std::make_unsigned<To>::type>(ptr, len);
}

template<typename To>
static typename std::enable_if<std::is_same<To, bool>::value, To>::type
conv_to(const char *ptr, std::size_t len) {
    return *ptr == 't' && len == 4;
}

template<typename To>
static typename std::enable_if<std::is_same<To, double>::value, To>::type
conv_to(const char *ptr, std::size_t len) {
    char buf[std::numeric_limits<To>::max_exponent10 + 20];
    std::memcpy(buf, ptr, len);
    buf[len] = 0;

    return std::strtod(buf, nullptr);
}

template<typename To>
static typename std::enable_if<std::is_same<To, float>::value, To>::type
conv_to(const char *ptr, std::size_t len) {
    char buf[std::numeric_limits<To>::max_exponent10 + 20];
    std::memcpy(buf, ptr, len);
    buf[len] = 0;

    return std::strtof(buf, nullptr);
}

template<typename To>
static typename std::enable_if<std::is_same<To, std::string>::value, To>::type
conv_to(const char *ptr, std::size_t len) { return {ptr, len}; }

template<typename To>
static typename std::enable_if<std::is_same<To, static_string>::value, To>::type
conv_to(const char *ptr, std::size_t len) { return {ptr, len}; }

} // ns details

/*************************************************************************************************/

#pragma pack(push, 1)

template<typename Iterator>
struct fj_token {
    fj_token()
        :__type{}
        ,__key{}
        ,__klen{}
        ,__val{}
        ,__vlen{}
        ,__parent{}
        ,__childs{}
        ,__end{}
    {}

    e_fj_token_type type() const { return __type; }
    const char* type_name() const { return fj_token_type_name(type()); }
    bool valid() const { return type() != FJ_TYPE_INVALID; }
    bool is_array() const { return type() == FJ_TYPE_ARRAY; }
    bool is_object() const { return type() == FJ_TYPE_OBJECT; }
    bool is_null() const { return type() == FJ_TYPE_NULL; }
    bool is_bool() const { return type() == FJ_TYPE_BOOL; }
    bool is_number() const { return type() == FJ_TYPE_NUMBER; }
    bool is_string() const { return type() == FJ_TYPE_STRING; }
    bool is_simple_type() const { return details::fj_is_simple_type(type()); }

    static_string to_sstring() const {
        if ( is_simple_type() ) {
            if ( !is_null() ) {
                return value();
            }

            return {};
        }

        throw std::logic_error(__FLATJSON__MAKE_ERROR_MESSAGE("not STRING/NUMBER/BOOL/NULL type"));
    }
    std::string to_string() const { auto s = to_sstring(); return {s.data(), s.size()}; }
    template<typename T>
    T to() const { auto s = to_sstring(); return details::conv_to<T>(s.data(), s.size()); }
    bool to_bool() const { return to<bool>(); }
    std::uint32_t to_uint() const { return to<std::uint32_t>(); }
    std::int32_t to_int() const { return to<std::int32_t>(); }
    std::uint64_t to_uint64() const { return to<std::uint64_t>(); }
    std::int64_t to_int64() const { return to<std::int64_t>(); }
    double to_double() const { return to<double>(); }
    float to_float() const { return to<float>(); }

    static_string key() const { return {__key, __klen}; }
    static_string value() const { return {__val, __vlen}; }
    __FLATJSON__CHILDS_TYPE childs() const { return __childs; }
    const fj_token* parent() const { return __parent; }
    const fj_token* end() const { return __end; }

    e_fj_token_type __type;
    Iterator __key;
    __FLATJSON__KLEN_TYPE __klen;
    Iterator __val;
    __FLATJSON__VLEN_TYPE __vlen;
    fj_token<Iterator> *__parent;
    __FLATJSON__CHILDS_TYPE __childs;
    fj_token<Iterator> *__end; // pointing to the last token for arrays and objects
};

#pragma pack(pop)

/*************************************************************************************************/

namespace details {

template<typename Iterator>
struct fj_parser {
    Iterator js_cur;
    Iterator js_end;

    fj_token<Iterator> *jstok_beg;
    fj_token<Iterator> *jstok_cur;
    fj_token<Iterator> *jstok_end;
};

/*************************************************************************************************/

template<typename Iterator>
inline void fj_skip_ws(fj_parser<Iterator> *p) {
    for (
        ;p->js_cur < p->js_end && (*p->js_cur == ' ' || *p->js_cur == '\t' || *p->js_cur == '\r' || *p->js_cur == '\n')
        ;++p->js_cur
    )
        ;
}

#define __FLATJSON__CUR_CHAR(p) \
    ((fj_skip_ws(p)), (p->js_cur >= p->js_end ? ((char)-1) : *(p->js_cur)))

template<typename Iterator>
inline int fj_check_and_skip(fj_parser<Iterator> *p, char expected) {
    char ch = __FLATJSON__CUR_CHAR(p);
    if ( ch == expected ) {
        p->js_cur++;

        return FJ_EC_OK;
    }

    if ( ch == ((char)-1) ) {
        return FJ_EC_INCOMPLETE;
    }

    return FJ_EC_INVALID;
}

template<typename Iterator>
inline int fj_escape_len(Iterator s, std::ptrdiff_t len) {
    switch ( *s ) {
        case 'u':
            return len < 6
                ? FJ_EC_INCOMPLETE
                : fj_is_hex_digit4(*(s+1), *(s+2), *(s+3), *(s+4))
                    ? 5
                    : FJ_EC_INVALID
            ;
        case '"':
        case '\\':
        case '/':
        case 'b':
        case 'f':
        case 'n':
        case 'r':
        case 't':
            return len < 2 ? FJ_EC_INCOMPLETE : 1;
        default:
            return FJ_EC_INVALID;
    }
}

/*************************************************************************************************/

enum class parser_mode {
     parse
    ,count_tokens
};

template<parser_mode M, typename Iterator, std::size_t ExLen>
inline int fj_expect(fj_parser<Iterator> *p, const char (&s)[ExLen], Iterator *ptr, std::size_t *size) {
    if ( p->js_cur + (ExLen-1) > p->js_end )
        return FJ_EC_INCOMPLETE;

    if ( std::strncmp(p->js_cur, s, ExLen-1) != 0 ) {
        return FJ_EC_INVALID;
    }

    if ( M == parser_mode::parse ) {
        *ptr = p->js_cur;
        *size = ExLen-1;
    }

    p->js_cur += ExLen-1;

    return FJ_EC_OK;
}

template<parser_mode M, typename Iterator>
int fj_parse_string(fj_parser<Iterator> *p, Iterator *ptr, std::size_t *size) {
    int ec = fj_check_and_skip(p, '"');
    if ( ec ) return ec;

    int ch = 0;
    Iterator start = p->js_cur;
    for ( std::size_t len = 0; p->js_cur < p->js_end; p->js_cur += len ) {
        ch = static_cast<unsigned char>(*(p->js_cur));
        len = fj_utf8_char_len((unsigned char)ch);
        if ( !(ch >= 32 && len > 0) ) return FJ_EC_INVALID;
        if ( static_cast<std::ptrdiff_t>(len) > (p->js_end - p->js_cur) ) return FJ_EC_INCOMPLETE;

        if ( ch == '\\' ) {
            int n = fj_escape_len(p->js_cur + 1, p->js_end - p->js_cur);
            if ( n <= 0 ) return n;
            len += n;
        } else if ( ch == '"' ) {
            if ( M == parser_mode::parse ) {
                *ptr = start;
                *size = p->js_cur - start;
            }

            ++p->js_cur;

            break;
        }
    }

    return ch == '"' ? FJ_EC_OK : FJ_EC_INCOMPLETE;
}

template<parser_mode M, typename Iterator>
int fj_parse_number(fj_parser<Iterator> *p, Iterator *ptr, std::size_t *size) {
    Iterator start = p->js_cur;
    if ( __FLATJSON__CUR_CHAR(p)== '-' ) p->js_cur++;

    if ( p->js_cur >= p->js_end ) return FJ_EC_INCOMPLETE;
    if ( p->js_cur + 1 < p->js_end && *(p->js_cur) == '0' && *(p->js_cur+1) == 'x' ) {
        p->js_cur += 2;

        if ( p->js_cur >= p->js_end ) return FJ_EC_INCOMPLETE;
        if ( !details::fj_is_hex_digit(*(p->js_cur)) ) return FJ_EC_INVALID;

        for ( ; p->js_cur < p->js_end && details::fj_is_hex_digit(*(p->js_cur)); ++p->js_cur )
            ;
    } else {
        if ( !details::fj_is_digit(*(p->js_cur)) ) return FJ_EC_INVALID;
        for ( ; p->js_cur < p->js_end && details::fj_is_digit(*(p->js_cur)); ++p->js_cur )
            ;

        if ( p->js_cur < p->js_end && *(p->js_cur) == '.' ) {
            p->js_cur++;

            if ( p->js_cur >= p->js_end ) return FJ_EC_INCOMPLETE;
            if ( !details::fj_is_digit(*(p->js_cur)) ) return FJ_EC_INVALID;

            for ( ; p->js_cur < p->js_end && details::fj_is_digit(*(p->js_cur)); ++p->js_cur )
                ;
        }
        if ( p->js_cur < p->js_end && (*(p->js_cur) == 'e' || *(p->js_cur) == 'E') ) {
            p->js_cur++;

            if ( p->js_cur >= p->js_end ) return FJ_EC_INCOMPLETE;

            if ( *(p->js_cur) == '+' || *(p->js_cur) == '-' )
                p->js_cur++;

            if ( p->js_cur >= p->js_end ) return FJ_EC_INCOMPLETE;
            if ( !details::fj_is_digit(*(p->js_cur)) ) return FJ_EC_INVALID;

            for ( ; p->js_cur < p->js_end && details::fj_is_digit(*(p->js_cur)); ++p->js_cur )
                ;
        }
    }

    if ( (p->js_cur - start) > 1 && (start[0] == '0' && start[1] != '.') ) {
        return FJ_EC_INVALID;
    }

    if ( M == parser_mode::parse ) {
        *ptr = start;
        *size = p->js_cur - start;
    }

    return FJ_EC_OK;
}

template<parser_mode M, typename Iterator>
int fj_parse_value(fj_parser<Iterator> *p, Iterator *ptr, std::size_t *size, e_fj_token_type *toktype, fj_token<Iterator> *parent);

template<parser_mode M, typename Iterator>
int fj_parse_array(fj_parser<Iterator> *p, fj_token<Iterator> *parent) {
    int ec = fj_check_and_skip(p, '[');
    if ( ec ) return ec;

    if ( M == parser_mode::parse && p->jstok_cur == p->jstok_end )
        return FJ_EC_NO_FREE_TOKENS;

    fj_token<Iterator> *startarr = p->jstok_cur++;
    if ( M == parser_mode::parse ) {
        startarr->__type = FJ_TYPE_ARRAY;
        startarr->__parent = parent;
        if ( startarr->__parent ) {
            __FLATJSON__CHECK_OVERFLOW(startarr->__parent->__childs, __FLATJSON__CHILDS_TYPE, FJ_EC_CHILDS_OVERFLOW);
            ++startarr->__parent->__childs;
        }
    }

    while ( __FLATJSON__CUR_CHAR(p) != ']' ) {
        if ( M == parser_mode::parse && p->jstok_cur == p->jstok_end )
            return FJ_EC_NO_FREE_TOKENS;

        fj_token<Iterator> *pair = p->jstok_cur++;

        char ch = __FLATJSON__CUR_CHAR(p);
        if ( ch == '{' || ch == '[' ) {
            p->jstok_cur -= 1;
        } else {
            if ( M == parser_mode::parse ) {
                pair->__parent = startarr;

                __FLATJSON__CHECK_OVERFLOW(startarr->__childs, __FLATJSON__CHILDS_TYPE, FJ_EC_CHILDS_OVERFLOW);
                ++startarr->__childs;
            }
        }

        std::size_t size = 0;
        ec = fj_parse_value<M>(
             p
            ,&(pair->__val)
            ,&size
            ,&(pair->__type)
            ,startarr
        );
        if ( ec ) return ec;
        if ( M == parser_mode::parse ) {
            __FLATJSON__CHECK_OVERFLOW(size, __FLATJSON__VLEN_TYPE, FJ_EC_VLEN_OVERFLOW);
            pair->__vlen = size;
        }

        if ( __FLATJSON__CUR_CHAR(p) == ',' ) {
            p->js_cur++;
            if ( *(p->js_cur) == ']' ) return FJ_EC_INVALID;
        }
    }

    ec = fj_check_and_skip(p, ']');
    if ( ec ) return ec;

    if ( M == parser_mode::parse ) {
        if ( p->jstok_cur == p->jstok_end ) return FJ_EC_NO_FREE_TOKENS;
        fj_token<Iterator> *endarr = p->jstok_cur++;
        endarr->__type = FJ_TYPE_ARRAY_END;
        endarr->__parent = startarr;
        __FLATJSON__CHECK_OVERFLOW(endarr->__parent->__childs, __FLATJSON__CHILDS_TYPE, FJ_EC_CHILDS_OVERFLOW);
        ++endarr->__parent->__childs;
        startarr->__end = endarr;
    } else {
        ++p->jstok_cur;
    }

    return 0;
}

template<parser_mode M, typename Iterator>
int fj_parse_object(fj_parser<Iterator> *p, fj_token<Iterator> *parent) {
    int ec = fj_check_and_skip(p, '{');
    if ( ec ) return ec;

    if ( M == parser_mode::parse && p->jstok_cur == p->jstok_end )
        return FJ_EC_NO_FREE_TOKENS;

    fj_token<Iterator> *startobj = p->jstok_cur++;
    if ( M == parser_mode::parse ) {
        startobj->__type = FJ_TYPE_OBJECT;
        startobj->__parent = parent;
        if ( startobj->__parent ) {
            __FLATJSON__CHECK_OVERFLOW(startobj->__parent->__childs, __FLATJSON__CHILDS_TYPE, FJ_EC_CHILDS_OVERFLOW);
            ++startobj->__parent->__childs;
        }
    }

    while ( __FLATJSON__CUR_CHAR(p) != '}' ) {
        char ch = __FLATJSON__CUR_CHAR(p);
        if ( ch != '"' ) {
            if ( ch == ((char)-1) ) {
                return FJ_EC_INCOMPLETE;
            }

            return FJ_EC_INVALID;
        }

        if ( M == parser_mode::parse && p->jstok_cur == p->jstok_end )
            return FJ_EC_NO_FREE_TOKENS;

        fj_token<Iterator> *pair = p->jstok_cur++;

        std::size_t size = 0;
        ec = fj_parse_value<M>(
             p
            ,&(pair->__key)
            ,&size
            ,&(pair->__type)
            ,startobj
        );
        if ( ec ) return ec;
        if ( M == parser_mode::parse ) {
            __FLATJSON__CHECK_OVERFLOW(size, __FLATJSON__KLEN_TYPE, FJ_EC_KLEN_OVERFLOW);
            pair->__klen = size;
        }

        ec = fj_check_and_skip(p, ':');
        if ( ec ) return ec;

        ch = __FLATJSON__CUR_CHAR(p);
        if ( ch == '[' || ch == '{' ) {
            p->jstok_cur -= 1;
            static Iterator unused_str{};
            std::size_t unused_size{};
            ec = fj_parse_value<M>(
                 p
                ,&unused_str
                ,&unused_size
                ,&(pair->__type)
                ,startobj
            );
        } else {
            if ( M == parser_mode::parse ) {
                pair->__parent = startobj;
                __FLATJSON__CHECK_OVERFLOW(startobj->__childs, __FLATJSON__CHILDS_TYPE, FJ_EC_CHILDS_OVERFLOW);
                ++startobj->__childs;
            }

            ec = fj_parse_value<M>(
                 p
                ,&(pair->__val)
                ,&size
                ,&(pair->__type)
                ,startobj
            );
            __FLATJSON__CHECK_OVERFLOW(size, __FLATJSON__VLEN_TYPE, FJ_EC_VLEN_OVERFLOW);
            if ( M == parser_mode::parse ) pair->__vlen = size;
        }

        if ( ec ) return ec;

        if ( __FLATJSON__CUR_CHAR(p) == ',' ) {
            p->js_cur++;
            if ( *(p->js_cur) == '}' ) return FJ_EC_INVALID;
        }
// don't remove the code below!
//        if ( p->jstok_end && startobj->__childs > 1 ) {
//            std::size_t num = startobj->__childs-1;
//            fj_token *beg = startobj+1;
//            fj_token *cur = startobj+1+num;
//
//            fj_token *it = nullptr;
//            while ( num > 0 ) {
//                it = beg;
//                std::size_t step = num / 2;
//                it += step;
//                std::size_t minlen = std::min(it->__klen, cur->__klen);
//                if ( std::strncmp(it->__key, cur->__key, minlen) < 0 ) { // strNcmp?
//                    beg = ++it;
//                    num -= step + 1;
//                } else {
//                    num = step;
//                }
//            }
////            printf("__key=%.*s\n", beg->__klen, beg->__key);
//            fj_token tmp = *cur;
//            std::memmove(cur, beg, sizeof(tmp));
//            *beg = tmp;
//        }
    }

    ec = fj_check_and_skip(p, '}');
    if ( ec ) return ec;

    if ( M == parser_mode::parse ) {
        if ( p->jstok_cur == p->jstok_end ) return FJ_EC_NO_FREE_TOKENS;
        fj_token<Iterator> *endobj = p->jstok_cur++;
        endobj->__type = FJ_TYPE_OBJECT_END;
        endobj->__parent = startobj;
        __FLATJSON__CHECK_OVERFLOW(endobj->__parent->__childs, __FLATJSON__CHILDS_TYPE, FJ_EC_CHILDS_OVERFLOW);
        ++endobj->__parent->__childs;
        startobj->__end = endobj;
    } else {
        ++p->jstok_cur;
    }

    return FJ_EC_OK;
}

template<parser_mode M, typename Iterator>
int fj_parse_value(fj_parser<Iterator> *p, Iterator *ptr, std::size_t *size, e_fj_token_type *toktype, fj_token<Iterator> *parent) {
    auto ch = __FLATJSON__CUR_CHAR(p);
    switch ( ch ) {
        case '{': {
            int ec = fj_parse_object<M>(p, parent);
            if ( ec ) return ec;
            if ( M == parser_mode::parse ) *toktype = FJ_TYPE_OBJECT;
            break;
        }
        case '[': {
            int ec = fj_parse_array<M>(p, parent);
            if ( ec ) return ec;
            if ( M == parser_mode::parse ) *toktype = FJ_TYPE_ARRAY;
            break;
        }
        case 'n': {
            int ec = fj_expect<M>(p, "null", ptr, size);
            if ( ec ) return ec;
            // on root token
            if ( p->jstok_cur == p->jstok_beg ) {
                ++p->jstok_cur;
            }
            if ( M == parser_mode::parse ) *toktype = FJ_TYPE_NULL;
            break;
        }
        case 't': {
            int ec = fj_expect<M>(p, "true", ptr, size);
            if ( ec ) return ec;
            // on root token
            if ( p->jstok_cur == p->jstok_beg ) {
                ++p->jstok_cur;
            }
            if ( M == parser_mode::parse ) *toktype = FJ_TYPE_BOOL;
            break;
        }
        case 'f': {
            int ec = fj_expect<M>(p, "false", ptr, size);
            if ( ec ) return ec;
            // on root token
            if ( p->jstok_cur == p->jstok_beg ) {
                ++p->jstok_cur;
            }
            if ( M == parser_mode::parse ) *toktype = FJ_TYPE_BOOL;
            break;
        }
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            int ec = fj_parse_number<M>(p, ptr, size);
            if ( ec ) return ec;
            // on root token
            if ( p->jstok_cur == p->jstok_beg ) {
                ++p->jstok_cur;
            }
            if ( M == parser_mode::parse ) *toktype = FJ_TYPE_NUMBER;
            break;
        }
        case '"': {
            int ec = fj_parse_string<M>(p, ptr, size);
            if ( ec ) return ec;
            // on root token
            if ( p->jstok_cur == p->jstok_beg ) {
                ++p->jstok_cur;
            }
            if ( M == parser_mode::parse ) *toktype = FJ_TYPE_STRING;
            break;
        }
        default:
            if ( ch == ((char)-1) ) {
                return FJ_EC_INCOMPLETE;
            } else {
                return FJ_EC_INVALID;
            }
    }

    return FJ_EC_OK;
}

struct parse_result {
    e_fj_error_code ec;
    std::size_t toknum;
};

template<typename Iterator>
inline void fj_init(fj_parser<Iterator> *parser, Iterator beg, Iterator end, fj_token<Iterator> *tokbuf, std::size_t toksize) {
    // root token
    tokbuf[0].__type = FJ_TYPE_INVALID;
    tokbuf[0].__key = Iterator{};
    tokbuf[0].__klen = 0;
    tokbuf[0].__val = Iterator{};
    tokbuf[0].__vlen = 0;
    tokbuf[0].__parent = nullptr;
    tokbuf[0].__childs = 0;

    *parser = {beg, end, tokbuf, tokbuf, tokbuf+toksize};
}

template<typename Iterator>
inline parse_result fj_parse(fj_parser<Iterator> *parser) {
    parse_result res{};

    std::size_t vlen = 0;
    res.ec = static_cast<e_fj_error_code>(fj_parse_value<parser_mode::parse, Iterator>(
         parser
        ,&(parser->jstok_beg->__val)
        ,&vlen
        ,&(parser->jstok_beg->__type)
        ,nullptr
    ));
    assert(vlen <= std::numeric_limits<__FLATJSON__VLEN_TYPE>::max());
    parser->jstok_beg->__vlen = static_cast<__FLATJSON__VLEN_TYPE>(vlen);

    if ( res.ec ) {
        res.toknum = 0;
    }

    res.toknum = parser->jstok_cur - parser->jstok_beg;

    return res;
}

template<typename Iterator>
inline fj_parser<Iterator> fj_make_parser(fj_token<Iterator> *tokbuf, std::size_t toksize, Iterator beg, Iterator end) {
    fj_parser<Iterator> parser;
    fj_init(std::addressof(parser), beg, end, tokbuf, toksize);

    return parser;
}

template<typename Iterator>
inline parse_result fj_num_tokens(Iterator beg, Iterator end) {
    parse_result res{};
    fj_parser<Iterator> parser{};
    parser.js_cur = beg;
    parser.js_end = end;

    static fj_token<Iterator> fake;
    parser.jstok_beg = &fake;
    parser.jstok_cur = &fake;
    parser.jstok_end = nullptr;

    std::size_t vlen = 0;
    e_fj_token_type toktype{};
    res.ec = static_cast<e_fj_error_code>(fj_parse_value<parser_mode::count_tokens, Iterator>(
         &parser
        ,&(parser.jstok_beg->__val)
        ,&vlen
        ,&toktype
        ,nullptr
    ));

    if ( !res.ec && parser.js_cur+1 != parser.js_end ) {
        fj_skip_ws(&parser);
        if ( parser.js_cur != parser.js_end ) {
            res.ec = FJ_EC_INVALID;
        }
    }

    assert(vlen <= std::numeric_limits<__FLATJSON__VLEN_TYPE>::max());
    parser.jstok_beg->__vlen = static_cast<__FLATJSON__VLEN_TYPE>(vlen);

    res.toknum = parser.jstok_cur - parser.jstok_beg;

    return res;
}

// for debugging purposes
template<typename Iterator>
inline void fj_dump_tokens(std::FILE *stream, fj_token<Iterator> *toks, std::size_t num) {
    for ( auto *it = toks; it != toks+num; ++it ) {
        std::fprintf(stream, "%2d: type=%12s, parent=%p, addr=%p, end=%p, childs=%d, key=%.*s, val=%.*s\n"
            ,(int)(it-toks)
            ,fj_token_type_name(it->__type)
            ,it->__parent
            ,it
            ,it->__end
            ,it->__childs
            ,(int)it->__klen, it->__key
            ,(int)it->__vlen, it->__val
        );
        std::fflush(stream);
    }
}

using fj_gather_cb_t = void(*)(void *userdata, const char *ptr, std::size_t len);

template<typename Iterator, bool CalcLength = false>
inline std::size_t fj_get_tokens(const fj_token<Iterator> *toks, std::size_t num, std::size_t indent, void *userdata, fj_gather_cb_t cb) {
    static const char indent_str[] = "                                                                                ";
    std::size_t indent_scope = 0;
    std::size_t length = 0;
    for ( auto *it = toks; it != toks+num; ++it ) {
        if ( it != toks ) {
            e_fj_token_type ctype = it->__type;
            e_fj_token_type ptype = (it-1)->__type;
            if ( (ctype != FJ_TYPE_ARRAY_END && ctype != FJ_TYPE_OBJECT_END ) &&
                 (ptype != FJ_TYPE_OBJECT && ptype != FJ_TYPE_ARRAY) )
            {
                if ( !CalcLength ) {
                    cb(userdata, ",", 1);
                    if ( indent ) cb(userdata, "\n", 1);
                }
                length += 1;
                if ( indent ) length += 1;
            }
        }

        switch ( it->__type ) {
            case FJ_TYPE_OBJECT: {
                if ( it->__key ) {
                    if ( !CalcLength ) {
                        if ( indent ) cb(userdata, indent_str, indent_scope);
                        cb(userdata, "\"", 1);
                        cb(userdata, it->__key, it->__klen);
                        cb(userdata, "\":", 2);
                    }
                    length += 1+2;
                    length += it->__klen;
                    if ( indent ) length += indent_scope;
                }
                if ( !CalcLength ) {
                    cb(userdata, "{", 1);
                    if ( indent ) cb(userdata, "\n", 1);
                }
                length += 1;
                if ( indent ) {
                    length += 1;
                    indent_scope += indent;
                }
                break;
            }
            case FJ_TYPE_OBJECT_END: {
                if ( !CalcLength ) {
                    if ( indent ) {
                        cb(userdata, "\n", 1);
                        cb(userdata, indent_str, indent_scope-indent);
                    }
                    cb(userdata, "}", 1);
                }
                length += 1;
                if ( indent ) {
                    length += 1;
                    indent_scope -= indent;
                    length += indent_scope;
                }
                break;
            }
            case FJ_TYPE_ARRAY: {
                if ( it->__key ) {
                    if ( !CalcLength ) {
                        if ( indent ) cb(userdata, indent_str, indent_scope);
                        cb(userdata, "\"", 1);
                        cb(userdata, it->__key, it->__klen);
                        cb(userdata, "\":", 2);
                    }
                    length += 1+2;
                    length += it->__klen;
                    if ( indent ) length += indent_scope;
                }
                if ( !CalcLength ) {
                    cb(userdata, "[", 1);
                    if ( indent ) cb(userdata, "\n", 1);
                }
                length += 1;
                if ( indent ) {
                    length += 1;
                    indent_scope += indent;
                }
                break;
            }
            case FJ_TYPE_ARRAY_END: {
                if ( !CalcLength ) {
                    if ( indent ) {
                        cb(userdata, "\n", 1);
                        cb(userdata, indent_str, indent_scope-indent);
                    }
                    cb(userdata, "]", 1);
                }
                length += 1;
                if ( indent ) {
                    length += 1;
                    indent_scope -= indent;
                    length += indent_scope;
                }
                break;
            }
            case FJ_TYPE_NULL:
            case FJ_TYPE_BOOL:
            case FJ_TYPE_NUMBER:
            case FJ_TYPE_STRING: {
                if ( it->__parent->__type != FJ_TYPE_ARRAY ) {
                    if ( !CalcLength ) {
                        if ( indent ) cb(userdata, indent_str, indent_scope);
                        cb(userdata, "\"", 1);
                        cb(userdata, it->__key, it->__klen);
                        cb(userdata, "\":", 2);
                    }
                    length += 1+2;
                    length += it->__klen;
                    if ( indent ) length += indent_scope;
                } else if ( it->__parent->__type == FJ_TYPE_ARRAY ) {
                    if ( !CalcLength ) {
                        if ( indent ) cb(userdata, indent_str, indent_scope);
                    }
                    if ( indent ) length += indent_scope;
                }
                switch ( it->__type ) {
                    case FJ_TYPE_NULL:
                    case FJ_TYPE_BOOL:
                    case FJ_TYPE_NUMBER: {
                        if ( !CalcLength ) cb(userdata, it->__val, it->__vlen);
                        length += it->__vlen;
                        break;
                    }
                    case FJ_TYPE_STRING: {
                        if ( !CalcLength ) {
                            cb(userdata, "\"", 1);
                            cb(userdata, it->__val, it->__vlen);
                            cb(userdata, "\"", 1);
                        }
                        length += 2;
                        length += it->__vlen;
                        break;
                    }
                    default: break;
                }

                break;
            }
            default: break;
        }
    }

    return length;
}

/*************************************************************************************************/

static void tokens_to_stream_cb_0(void *userdata, const char *ptr, std::size_t len) {
    auto *stream = static_cast<std::FILE*>(userdata);
    std::fwrite(ptr, len, 1, stream);
}

template<typename Iterator>
inline std::size_t fj_tokens_to_stream(std::FILE *stream, const fj_token<Iterator> *toks, std::size_t num, std::size_t indent = 0) {
    return fj_get_tokens(toks, num, indent, stream, tokens_to_stream_cb_0);
}

/*************************************************************************************************/

static void tokens_to_stream_cb_1(void *userdata, const char *ptr, std::size_t len) {
    auto *stream = static_cast<std::ostream *>(userdata);
    stream->write(ptr, len);
}

template<typename Iterator>
inline std::size_t fj_tokens_to_stream(std::ostream &stream, const fj_token<Iterator> *toks, std::size_t num, std::size_t indent = 0) {
    return fj_get_tokens(toks, num, indent, &stream, tokens_to_stream_cb_1);
}

/*************************************************************************************************/

struct tokens_to_buf_userdata {
    char *ptr;
    const char *end;
};

static void tokens_to_buf_cb(void *userdata, const char *ptr, std::size_t len) {
    auto *p = static_cast<tokens_to_buf_userdata *>(userdata);
    assert(p->ptr + len <= p->end);
    std::memcpy(p->ptr, ptr, len);
    p->ptr += len;
}

template<typename Iterator>
inline std::size_t fj_tokens_to_buf(const fj_token<Iterator> *toks, std::size_t num, char *buf, std::size_t size, std::size_t indent = 0) {
    tokens_to_buf_userdata userdata{buf, buf+size};
    return fj_get_tokens(toks, num, indent, &userdata, tokens_to_buf_cb);
}

/*************************************************************************************************/

template<typename Iterator>
inline std::size_t fj_str_length(const fj_token<Iterator> *toks, std::size_t num, std::size_t indent = 0) {
    return fj_get_tokens<Iterator, true>(toks, num, indent, nullptr, nullptr);
}

/*************************************************************************************************/

template<typename Iterator>
inline std::size_t fj_get_keys(const fj_token<Iterator> *toks, std::size_t num, void *userdata, fj_gather_cb_t cb) {
    std::size_t cnt{};

    if ( !toks->is_object() ) {
        return 0;
    }

    const fj_token<Iterator> *parent = toks;
    for ( const fj_token<Iterator> *it = toks; it != toks+num; ++it ) {
        if ( it->parent() != parent || it->type() == FJ_TYPE_OBJECT_END ) {
            continue;
        }

        cb(userdata, it->__key, it->__klen);

        ++cnt;
    }

    return cnt;
}

} // ns details

/*************************************************************************************************/

struct fjson {
    using InputIterator = const char *;
    using element_type = fj_token<InputIterator>;
    using storage_type = std::vector<element_type>;
    using storage_ptr = std::shared_ptr<storage_type>;

private:
    template<typename T>
    struct tokens_iterator_impl {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;

        explicit tokens_iterator_impl(pointer c)
            :m_cur{c}
        {}

        pointer operator->() const { return m_cur; }
        tokens_iterator_impl& operator++() { ++m_cur; return *this; }
        tokens_iterator_impl operator++(int) { tokens_iterator_impl tmp{m_cur}; ++(*this); return tmp; }
        reference operator* () { return *m_cur; }

        friend difference_type operator- (const tokens_iterator_impl &l, const tokens_iterator_impl &r)
        { return l.m_cur - r.m_cur; }
        friend bool operator== (const tokens_iterator_impl &l, const tokens_iterator_impl &r)
        { return l.m_cur == r.m_cur; }
        friend bool operator!= (const tokens_iterator_impl &l, const tokens_iterator_impl &r)
        { return !operator==(l, r); }

    private:
        pointer m_cur;
    };

public:
    using iterator = tokens_iterator_impl<element_type>;
    using const_iterator = tokens_iterator_impl<const element_type>;

    iterator begin()              { return iterator{m_beg}; }
    iterator end()                { return iterator{m_end}; }
    const_iterator begin()  const { return const_iterator{m_beg}; }
    const_iterator end()    const { return const_iterator{m_end}; }
    const_iterator cbegin() const { return const_iterator{m_beg}; }
    const_iterator cend()   const { return const_iterator{m_end}; }

    fjson(const fjson &) = default;
    fjson(fjson &&) = default;

    fjson& operator= (const fjson &) = default;
    fjson& operator= (fjson &&) = default;

    explicit fjson(std::size_t reserved = 0)
        :m_storage{std::make_shared<storage_type>(reserved)}
        ,m_src_beg{nullptr}
        ,m_src_end{nullptr}
        ,m_beg{nullptr}
        ,m_end{nullptr}
        ,m_err{}
    {}
    template<
         std::size_t L
        ,typename CharT = typename std::iterator_traits<InputIterator>::value_type
    >
    explicit fjson(const CharT (&str)[L], std::size_t reserved = 0)
        :m_storage{L-1 ? std::make_shared<storage_type>(reserved) : storage_ptr{}}
        ,m_src_beg{str}
        ,m_src_end{str+L-1}
        ,m_beg{nullptr}
        ,m_end{nullptr}
        ,m_err{}
    {
        load(str, L-1);
    }
    fjson(InputIterator ptr, std::size_t size, std::size_t reserved = 0)
        :m_storage{size ? std::make_shared<storage_type>(reserved) : storage_ptr{}}
        ,m_src_beg{ptr}
        ,m_src_end{ptr+size}
        ,m_beg{nullptr}
        ,m_end{nullptr}
        ,m_err{}
    {
        load(ptr, size);
    }
    fjson(InputIterator beg, InputIterator end, std::size_t reserved = 0)
        :m_storage{beg != end ? std::make_shared<storage_type>(reserved) : storage_ptr{}}
        ,m_src_beg{beg}
        ,m_src_end{end}
        ,m_beg{nullptr}
        ,m_end{nullptr}
        ,m_err{}
    {
        load(beg, end);
    }

    virtual ~fjson() = default;

private:
    fjson(storage_ptr storage, element_type *beg, element_type *end)
        :m_storage{std::move(storage)}
        ,m_src_beg{nullptr}
        ,m_src_end{nullptr}
        ,m_beg{beg}
        ,m_end{end}
        ,m_err{}
    {}

public:
    bool is_valid() const { return m_beg && m_end && !m_storage->empty() && m_err == FJ_EC_OK; }
    e_fj_error_code error() const { return m_err; }
    const char* error_string() const { return fj_error_string(m_err); }

    std::size_t size() const {
        return (!details::fj_is_simple_type(m_beg->__type))
           ? m_beg->__childs - 1
           : static_cast<std::size_t>(m_beg->__type != FJ_TYPE_INVALID)
        ;
    }
    std::size_t tokens() const {
        return (!details::fj_is_simple_type(m_beg->__type))
            ? (m_end-m_beg) + (m_beg->__parent && m_beg->__parent->__type == FJ_TYPE_ARRAY ? 1 : 0)
            : static_cast<std::size_t>(m_beg->__type != FJ_TYPE_INVALID)
        ;
    }
    bool is_empty() const { return size() == 0; }
    void clear() {
        m_storage->clear();
        m_beg = m_end = nullptr;
    }

    e_fj_token_type type() const { return m_beg->type(); }
    const char* type_name() const { return m_beg->type_name(); }

    bool is_array() const { return m_beg->is_array(); }
    bool is_object() const { return m_beg->is_object(); }
    bool is_null() const { return m_beg->is_null(); }
    bool is_bool() const { return m_beg->is_bool(); }
    bool is_number() const { return m_beg->is_number(); }
    bool is_string() const { return m_beg->is_string(); }
    bool is_simple_type() const { return m_beg->is_simple_type(); }

    static_string to_sstring() const { return m_beg->to_sstring(); }
    std::string to_string() const { return m_beg->to_string(); }
    template<typename T>
    T to() const { return m_beg->template to<T>(); }
    bool to_bool() const { return m_beg->to_bool(); }
    std::uint32_t to_uint() const { return m_beg->to_uint(); }
    std::int32_t to_int() const { return m_beg->to_int(); }
    std::uint64_t to_uint64() const { return m_beg->to_uint64(); }
    std::int64_t to_int64() const { return m_beg->to_int64(); }
    double to_double() const { return m_beg->to_double(); }
    float to_float() const { return m_beg->to_float(); }

    template<std::size_t KL>
    bool contains(const char (&key)[KL]) const { return contains(key, KL-1); }
    template<
        typename ConstCharPtr
        ,typename = typename std::enable_if<
            std::is_same<ConstCharPtr, const char*>::value
        >::type
    >
    bool contains(ConstCharPtr key) const { return contains(key, std::strlen(key)); }
    bool contains(const char *key, std::size_t len) const {
        auto res = find(key, len);
        return res.first != nullptr;
    }

    // for objects
    template<std::size_t KL>
    fjson at(const char (&key)[KL]) const { return at(key, KL-1); }
    template<
        typename ConstCharPtr
        ,typename = typename std::enable_if<
            std::is_same<ConstCharPtr, const char*>::value
        >::type
    >
    fjson at(ConstCharPtr key) const { return at(key, std::strlen(key)); }
    fjson at(const char *key, std::size_t len) const {
        auto res = find(key, len);
        if ( res.first ) {
            return {m_storage, res.first, res.second};
        }

        throw std::runtime_error(__FLATJSON__MAKE_ERROR_MESSAGE("key not found"));
    }
    // for arrays
    fjson at(std::size_t idx) const {
        auto res = find(idx);
        if ( res.first ) {
            return {m_storage, res.first, res.second};
        }

        throw std::out_of_range(__FLATJSON__MAKE_ERROR_MESSAGE("out of range"));
    }

    // for arrays
    fjson operator[](std::size_t idx) const { return at(idx); }

    // for objects
    template<std::size_t KL>
    fjson operator[](const char (&key)[KL]) const { return at(key, KL-1); }
    template<
         typename ConstCharPtr
        ,typename = typename std::enable_if<
            std::is_same<ConstCharPtr, const char*>::value
        >::type
    >
    fjson operator[](ConstCharPtr key) const { return at(key, std::strlen(key)); }

    template<std::size_t N, typename CharT = typename std::iterator_traits<InputIterator>::value_type>
    bool load(const char (&str)[N]) { return load(str, str+N-1); }
    bool load(InputIterator beg, std::size_t size) { return load(beg, beg+size); }
    bool load(InputIterator beg, InputIterator end) {
        if ( beg == end ) {
            return false;
        }

        if ( m_storage->empty() ) {
            auto res = details::fj_num_tokens(beg, end);
            if ( res.ec ) {
                m_err = res.ec;

                return false;
            } else {
                m_storage->resize(res.toknum);
            }
        }

        details::fj_parser<InputIterator> parser{};
        details::fj_init(
             &parser
            ,beg
            ,end
            ,&(*m_storage->begin())
            ,m_storage->size()
        );
        details::parse_result res = details::fj_parse(&parser);
        if ( res.ec ) {
            m_err = res.ec;
            return false;
        }

        m_storage->resize(res.toknum);
        m_beg = m_storage->data();
        m_end = m_beg + m_storage->size();

        return true;
    }

    std::string dump(std::size_t indent = 0) const {
        std::size_t strlen = details::fj_str_length(m_beg, m_storage->size(), indent);
        std::string res(strlen, 0);
        assert(strlen == details::fj_tokens_to_buf(
                 m_beg
                ,m_storage->size()
                ,&res[0]
                ,res.size()
                ,indent
            )
        );

        return res;
    }
    std::ostream& dump(std::ostream &os, std::size_t indent = 0) const {
        details::fj_tokens_to_stream(os, m_beg, m_end - m_beg, indent);

        return os;
    }
    friend std::ostream& operator<< (std::ostream &os, const fjson &fj) {
        details::fj_tokens_to_stream(os, fj.m_beg, fj.m_end - fj.m_beg);
        return os;
    }

    std::pair<const fj_token<InputIterator>*, std::size_t>
    data() const { return {m_beg, m_end-m_beg}; }

    std::vector<static_string>
    get_keys() const {
        std::vector<static_string> res{};

        const auto d = data();
        details::fj_get_keys(d.first, d.second, &res, &get_keys_cb);

        return res;
    }

    std::pair<InputIterator, InputIterator>
    get_source_data() const { return {m_src_beg, m_src_end}; }

private:
    static void get_keys_cb(void *userdata, const char *ptr, std::size_t len) {
        auto *vec = static_cast<std::vector<static_string> *>(userdata);
        vec->push_back(static_string{ptr, len});
    }

private:
    std::pair<element_type *, element_type *>
    find(const char *key, std::size_t len) const {
        if ( type() != FJ_TYPE_OBJECT )
            throw std::logic_error(__FLATJSON__MAKE_ERROR_MESSAGE("not OBJECT type"));

        element_type *parent = m_beg;
        element_type *beg = parent+1;
        element_type *end = parent->__end;
        while ( beg != end ) {
            if ( beg->__type == FJ_TYPE_OBJECT_END ) {
                return {nullptr, nullptr};
            }
            if ( beg->__klen == len && std::strncmp(beg->__key, key, len) == 0 ) {
                break;
            }

            beg = details::fj_is_simple_type(beg->__type) ? beg + 1 : beg->__end + 1;
        }

        const auto type = beg->__type;
        if ( details::fj_is_simple_type(type) ) {
            return {beg, beg+1};
        } else if ( type == FJ_TYPE_OBJECT || type == FJ_TYPE_ARRAY ) {
            return {beg, beg->__end};
        } else if ( beg == end && type == FJ_TYPE_OBJECT_END ) {
            return {nullptr, nullptr};
        }

        throw std::logic_error(__FLATJSON__MAKE_ERROR_MESSAGE("unreachable!"));
    }
    std::pair<element_type *, element_type *>
    find(std::size_t idx) const {
        if ( type() != FJ_TYPE_ARRAY )
            throw std::logic_error(__FLATJSON__MAKE_ERROR_MESSAGE("not ARRAY type"));

        if ( idx >= static_cast<__FLATJSON__CHILDS_TYPE>(m_beg->__childs - 1) ) // one for END token
            return {nullptr, nullptr};

        element_type *parent = m_beg;
        element_type *beg = parent+1;
        element_type *end = parent->__end;
        for ( ; beg != end && idx; --idx ) {
            beg = details::fj_is_simple_type(beg->__type) ? beg + 1 : beg->__end + 1;
        }

        const auto type = beg->__type;
        if ( details::fj_is_simple_type(type) ) {
            return {beg, beg+1};
        } else if ( type == FJ_TYPE_OBJECT || type == FJ_TYPE_ARRAY ) {
            return {beg, beg->__end};
        } else if ( beg == end && type == FJ_TYPE_ARRAY_END ) {
            return {nullptr, nullptr};
        }

        throw std::logic_error(__FLATJSON__MAKE_ERROR_MESSAGE("unreachable!"));
    }

private:
    storage_ptr m_storage;
    InputIterator m_src_beg;
    InputIterator m_src_end;
    element_type *m_beg;
    element_type *m_end;
    e_fj_error_code m_err;
};

/*************************************************************************************************/

template<typename Iterator>
fjson  parse(Iterator beg, Iterator end) {
    fjson json{beg, end};

    return json;
}

template<typename Iterator>
fjson parse(Iterator beg) {
    auto end = beg + std::strlen(beg);

    return parse(beg, end);
}

} // ns flatjson

/*************************************************************************************************/

// undef internally used macro-vars
#undef __FLATJSON__FALLTHROUGH
#undef __FLATJSON__STRINGIZE_I
#undef __FLATJSON__STRINGIZE
#undef __FLATJSON__MAKE_ERROR_MESSAGE
#undef __FLATJSON__CHECK_OVERFLOW
#undef __FLATJSON__KLEN_TYPE
#undef __FLATJSON__VLEN_TYPE
#undef __FLATJSON__CHILDS_TYPE
#undef __FLATJSON__CUR_CHAR

/*************************************************************************************************/

#endif // __FLATJSON__FLATJSON_HPP
