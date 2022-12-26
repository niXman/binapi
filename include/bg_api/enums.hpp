
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

#ifndef __bg_api__enums_hpp
#define __bg_api__enums_hpp

#include <cstdint>
#include <set>
#include <string>

#include "simdjson.h"

namespace bg_api {

//------------------------------------------------------------------------------

    // Trade direction
    enum class _side : size_t {
        _sell,   // Sell
        _buy     // Buy
    };

    _side side_from_string(const char* str);
    _side side_from_string(const std::string& str);
    _side side_from_string(const std::string_view str);
    const char* side_to_string(_side s);

//------------------------------------------------------------------------------

    // Order type
    enum class _order_type : size_t {
        _limit,  // Limit order
        _market  // Market order
    };

    _order_type order_type_from_string(const char* str);
    _order_type order_type_from_string(const std::string& str);
    _order_type order_type_from_string(const std::string_view str);
    const char* order_type_to_string(_order_type o);

//------------------------------------------------------------------------------

    // Order type (in-force)
    enum class _force : size_t {
        _normal,     // Good till cancel
        _post_only,  // Maker only
        _fok,        // Fill or kill
        _ioc         // Immediate or cancel
    };

    _force force_from_string(const char* str);
    _force force_from_string(const std::string& str);
    _force force_from_string(const std::string_view str);
    const char* force_to_string(_force f);

//------------------------------------------------------------------------------

    // Order status
    enum class _status : size_t {
        _init,           // Initial, inserted into DB
        _new,            // Unfilled, pending in orderbook
        _partial_fill,   // Partially filled
        _full_fill,      // Fully filled
        _canceled       // Canceled
    };

    _status status_from_string(const char* str);
    _status status_from_string(const std::string& str);
    _status status_from_string(const std::string_view str);
    const char* status_to_string(_status s);

//------------------------------------------------------------------------------

    // Major types of transactions
    enum class _group_type : size_t {
        _deposit,        // Deposit
        _withdraw,       // Withdrawal
        _transaction,    // Trade
        _transfer,       // Transfer
        _other          // Others
    };

    _group_type group_type_from_string(const char* str);
    _group_type group_type_from_string(const std::string& str);
    _group_type group_type_from_string(const std::string_view str);
    const char* group_type_to_string(_group_type g);

//------------------------------------------------------------------------------

    // Business type
    enum class _biz_type : size_t {
        _deposit,       // Deposit
        _withdraw,      // Withdrawal
        _buy,           // Buy
        _sell,          // Sell
        _deduct_hand,   // Deduction of handling fee
        _transfer_in,   // Transfer in
        _transfer_out,  // Transfer out
        _rebate_rwd,    // Rebate rewards  
        _airdrop_rwd,   // Airdrop rewards
        _USDT_rwd,      // USDT contract rewards
        _mix_rwd,       // Mix contract rewards
        _system_lock,   // System lock
        _user_lock      // User lock 
    };

    _biz_type biz_type_from_string(const char* str);
    _biz_type biz_type_from_string(const std::string& str);
    _biz_type biz_type_from_string(const std::string_view str);
    const char* biz_type_to_string(_biz_type b);

//------------------------------------------------------------------------------

    // Deposit withdrawal order status
    enum class _dwos : size_t {
        _cancel,         // Cancel
        _reject,         // Reject
        _success,        // Success
        _wallet_fail,    // Wallet failed
        _wallet_proc,    // Wallet processing
        _first_audit,    // First audit
        _second_audit,   // Second audit
        _first_reject,   // First audit rejected
        _second_reject   // Second audit rejected
    };

    _dwos dwos_from_string(const char* str);
    _dwos dwos_from_string(const std::string& str);
    _dwos dwos_from_string(const std::string_view str);
    const char* dwos_to_string(_dwos d);

//------------------------------------------------------------------------------

    // User withdrawal address query
    enum class _withdraw_type : size_t {
        _chain,  // On blockchain
        _inner   // Internal address
    };

    _withdraw_type withdraw_type_from_string(const char* str);
    _withdraw_type withdraw_type_from_string(const std::string& str);
    _withdraw_type withdraw_type_from_string(const std::string_view str);
    const char* withdraw_type_to_string(_withdraw_type w);

//------------------------------------------------------------------------------

    // Account type
    enum class _account_type : size_t {
        _EXCHANGE,   // Spot account
        _OTC_SGD,    // OTC account
        _CONTRACT,   // Contract account
        _USD_MIX,    // Mix account
        _USDT_MIX    // USDT futures account
    };

    _account_type account_type_from_string(const char* str);
    _account_type account_type_from_string(const std::string& str);
    _account_type account_type_from_string(const std::string_view str);
    const char* account_type_to_string(_account_type a);

//------------------------------------------------------------------------------

    // Candlestick granularity
    enum class _candle_gran : size_t {
        _1m = 60,           // 1 minute
        _5m = 300,          // 5 minute
        _15m = 900,         // 15 minute
        _30m = 1800,        // 30 minute
        _1h = 3600,         // 1 hour
        _4h = 1440,         // 4 hour
        _6h = 21600,        // 6 hour
        _12h = 43200,       // 12 hour
        _1D = 86400,        // 1 day
        _3D = 259200,       // 3 day
        _1W = 604800,       // 1 week
        _1M = 2592000,      // 1 month
        _6Hutc = -21600,     // 6 hour (UTC0)
        _12Hutc = -43200,    // 12 hour (UTC0)
        _1Dutc = -86400,     // 1 day (UTC0)
        _3Dutc = -259200,    // 3 day (UTC0)
        _1Wutc = -604800,    // 1 week (UTC0)
        _1Mutc = -2592000    // 1 month (UTC0)
    };

    _candle_gran candle_gran_from_string(const char* str);
    _candle_gran candle_gran_from_string(const std::string& str);
    _candle_gran candle_gran_from_string(const std::string_view str);
    const char* candle_gran_to_string(_candle_gran c);

//------------------------------------------------------------------------------

    // Transfer in type, transfer out type
    enum class _from_to_type : size_t {
        _spot,       // Spot asset coin
        _mix_usdt,   // USDT transfer only
        _mix_usd,    // BTC, ETH, EOS, XRP, USDC
        _mix_usdc    // USDC transfer only
    };

    _from_to_type from_to_type_from_string(const char* str);
    _from_to_type from_to_type_from_string(const std::string& str);
    _from_to_type from_to_type_from_string(const std::string_view str);
    const char* from_to_type_to_string(_from_to_type f);

//------------------------------------------------------------------------------

    // Product type
    enum class _product_type : size_t {
        _umcbl,  // USDT perpetual contract
        _dmcbl,  // Universal margin perpetual contract
        _cmcbl,  // USDC perpetual contract
        _sumcbl, // USDT simulation perpetual contract
        _sdmcbl, // Universal margin simulation perpetual contract
        _scmcbl  // USDC simulation perpetual contract
    };

    _product_type product_type_from_string(const char* str);
    _product_type product_type_from_string(const std::string& str);
    _product_type product_type_from_string(const std::string_view str);
    const char* product_type_to_string(_product_type p);

//------------------------------------------------------------------------------

    // Position mode
    enum class _margin_mode : size_t {
        _fixed,      // Isolated margin
        _crossed     // Cross margin    
    };

    _margin_mode margin_mode_from_string(const char* str);
    _margin_mode margin_mode_from_string(const std::string& str);
    _margin_mode margin_mode_from_string(const std::string_view str);
    const char* margin_mode_to_string(_margin_mode m);

//------------------------------------------------------------------------------

    // Position mode
    enum class _hold_mode : size_t {
        _single,     // One-way position
        _double      // Two-way position
    };

    _hold_mode hold_mode_from_string(const char* str);
    _hold_mode hold_mode_from_string(const std::string& str);
    _hold_mode hold_mode_from_string(const std::string_view str);
    const char* hold_mode_to_string(_hold_mode h);

//------------------------------------------------------------------------------

    // Position direction
    enum class _hold_side : size_t {
        _long,   // Long position
        _short   // Short position
    };

    _hold_side hold_side_from_string(const char* str);
    _hold_side hold_side_from_string(const std::string& str);
    _hold_side hold_side_from_string(const std::string_view str);
    const char* hold_side_to_string(_hold_side h);

//------------------------------------------------------------------------------

    enum class _business : size_t {
        _open_long,                  // Open long
        _open_short,                 // Open short
        _close_long,                 // Close long
        _close_short,                // Close short
        _trans_from_ex,              // Transfer in from spot account
        _trans_to_ex,                // Transfer out to spot account
        _cntrct_main_settle_fee,     // Funding rate for crossed
        _cntrct_margin_settle_fee,   // Fixed margin funding rate
        _tracking_trader_income      // ?
    };

    _business business_from_string(const char* str);
    _business business_from_string(const std::string& str);
    _business business_from_string(const std::string_view str);
    const char* business_to_string(_business b);

//------------------------------------------------------------------------------

    enum class _mix_side : size_t {
        _open_long,      // Open long
        _open_short,     // Open short
        _close_long,     // Close long
        _close_short,    // Close short
        _buy_single,     // Open long under single hold mode
        _sell_single     // Open short under single hold mode
    };

    _mix_side mix_side_from_string(const char* str);
    _mix_side mix_side_from_string(const std::string& str);
    _mix_side mix_side_from_string(const std::string_view str);
    const char* mix_side_to_string(_mix_side s);

//------------------------------------------------------------------------------

    enum class _trade_side : size_t {
        // Values for double hold mode
        _open_long,              // Open long
        _open_short,             // Open short
        _close_long,             // Close long
        _close_short,            // Close short
        _reduce_close_long,      // Force reduce long position
        _reduce_close_short,     // Force reduce short position
        _offset_close_long,      // Force netting: close long position
        _offset_close_short,     // Force netting: close short position
        _burst_close_long,       // Force liquidation: close long position
        _burst_close_short,      // Force liquidation: close short position
        _delivery_close_long,    // Future delivery close long
        _delivery_close_short,   // Future delivery close short

        // Values for single hold mode
        _buy_single,             // Buy in single hold mode
        _sell_single,            // Sell in single hold mode
        _reduce_buy_single,      // Force reduce buy in single hold mode
        _reduce_sell_single,     // Force reduce sell in single hold mode
        _burst_buy_single,       // Force liquidiation: buy in single hold mode
        _burst_sell_single,      // Force liquidation: sell in single hold mode
        _delivery_buy_single,    // Future delivery buy in single hold mode
        _delivery_sell_single    // Future delivery sell in single hold mode
    };

    _trade_side trade_side_from_string(const char* str);
    _trade_side trade_side_from_string(const std::string& str);
    _trade_side trade_side_from_string(const std::string_view str);
    const char* trade_side_to_string(_trade_side t);

//------------------------------------------------------------------------------

    enum class _state : size_t {
        _init,              // Initial order, inserted into DB
        _new,               // New order, pending match in orderbook
        _partially_filled,  // Partially filled
        _filled,            // Filled
        _canceled           // Canceled
    };

    _state state_from_string(const char* str);
    _state state_from_string(const std::string& str);
    _state state_from_string(const std::string_view str);
    const char* state_to_string(_state s);

//------------------------------------------------------------------------------

    enum class _trigger_type : size_t {
        _fill_price,     // Fill price
        _market_price    // Market price
    };

    _trigger_type trigger_type_from_string(const char* str);
    _trigger_type trigger_type_from_string(const std::string& str);
    _trigger_type trigger_type_from_string(const std::string_view str);
    const char* trigger_type_to_string(_trigger_type t);

//------------------------------------------------------------------------------

    enum class _plan_type : size_t {
        _profit_plan,    // Profit order
        _loss_plan,      // Loss order
        _normal_plan,    // Plan order
        _pos_profit,     // Position profit
        _pos_loss,       // Position loss
        _moving_plan,    // Trailing TP/SL
        _track_plan      // Trailing stop
    };

    _plan_type plan_type_from_string(const char* str);
    _plan_type plan_type_from_string(const std::string& str);
    _plan_type plan_type_from_string(const std::string_view str);
    const char* plan_type_to_string(_plan_type p);

//------------------------------------------------------------------------------

    enum class _is_plan : size_t {
        _plan,
        _profit_loss
    };

    _is_plan is_plan_from_string(const char* str);
    _is_plan is_plan_from_string(const std::string& str);
    _is_plan is_plan_from_string(const std::string_view str);
    const char* is_plan_to_string(_is_plan p);

//------------------------------------------------------------------------------

    enum class _plan_status : size_t {
        _not_trigger,
        _triggered,
        _fail_trigger,
        _cancel
    };

    _plan_status plan_status_from_string(const char* str);
    _plan_status plan_status_from_string(const std::string& str);
    _plan_status plan_status_from_string(const std::string_view str);
    const char* plan_status_to_string(_plan_status p);

//------------------------------------------------------------------------------

    enum class _stop_type : size_t {
        _profit,
        _loss
    };

    _stop_type stop_type_from_string(const char* str);
    _stop_type stop_type_from_string(const std::string& str);
    _stop_type stop_type_from_string(const std::string_view str);
    const char* stop_type_to_string(_stop_type s);

//------------------------------------------------------------------------------

    enum class _ws_plan_type : size_t {
        // CCMT = created/canceled/modified/triggered
        _pl,     // Push data when a plan order is CCMT (default)
        _tp,     // Push data when a take profit order is CCMT
        _sl,     // Push data when a stop loss order is CCMT
        _ptp,    // Push data when a position take profit order is CCMT
        _psl     // Push data when a position stop loss order is CCMT
    };

    _ws_plan_type ws_plan_type_from_string(const char* str);
    _ws_plan_type ws_plan_type_from_string(const std::string& str);
    _ws_plan_type ws_plan_type_from_string(const std::string_view str);
    const char* ws_plan_type_to_string(_ws_plan_type p);

//------------------------------------------------------------------------------

    enum class _account_status : size_t {
        _normal,
        _freeze,
        _del
    };

    _account_status account_status_from_string(const char* str);
    _account_status account_status_from_string(const std::string& str);
    _account_status account_status_from_string(const std::string_view str);
    const char* account_status_to_string(_account_status a);

//------------------------------------------------------------------------------

    // no_auths = 0             // 0000 0000
    // withdraw = 1             // 0000 0001
    // recharge = 2             // 0000 0010
    // transfer = 4             // 0000 0100
    // spot_trade = 8           // 0000 1000
    // contract_trade = 16      // 0001 0000
    // readonly = 32            // 0010 0000
    // trade = 64 (spot only)   // 0100 0000

    unsigned int auths_from_string_set(const std::set<std::string> auths);
    std::set<std::string> auths_to_string_set(unsigned int a);

//------------------------------------------------------------------------------

} // ns bg_api

#endif // __bg_api__enums_hpp
