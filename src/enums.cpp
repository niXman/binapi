
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

#include <bg_api/enums.hpp>
#include <bg_api/fnv1a.hpp>

#include <algorithm>
#include <cassert>

namespace bg_api {

//------------------------------------------------------------------------------

    _side side_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch(hash) {
            case fnv1a("buy"): return _side::_buy;
            case fnv1a("sell"): return _side::_sell;
        }

        assert(!"unreachable");
    }

    const char* side_to_string(_side s) {
        switch (s) {
            case _side::_buy: return "buy";
            case _side::_sell: return "sell";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _order_type order_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch(hash) {
            case fnv1a("limit"): return _order_type::_limit;
            case fnv1a("market"): return _order_type::_market;
        }

        assert(!"unreachable");
    }

    const char* order_type_to_string(_order_type o) {
        switch(o) {
            case _order_type::_limit: return "limit";
            case _order_type::_market: return "market";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _force force_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch(hash) {
            case fnv1a("normal"): return _force::_normal;
            case fnv1a("post_only"): return _force::_post_only;
            case fnv1a("fok"): return _force::_fok;
            case fnv1a("ioc"): return _force::_ioc;
        }

        assert(!"unreachable");
    }

    const char* force_to_string(_force f) {
        switch(f) {
            case _force::_normal: return "normal";
            case _force::_post_only: return "post_only";
            case _force::_fok: return "fok";
            case _force::_ioc: return "ioc";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _status status_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch(hash) {
            case fnv1a("init"): return _status::_init;
            case fnv1a("new"): return _status::_new;
            case fnv1a("partial_fill"): return _status::_partial_fill;
            case fnv1a("full_fill"): return _status::_full_fill;
            case fnv1a("cancelled"): return _status::_canceled;
        }

        assert(!"unreachable");
    }

    const char* status_to_string(_status s) {
        switch(s) {
            case _status::_init: return "init";
            case _status::_new: return "new";
            case _status::_partial_fill: return "partial_fill";
            case _status::_full_fill: return "full_fill";
            case _status::_canceled: return "cancelled";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _group_type group_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch(hash) {
            case fnv1a("deposit"): return _group_type::_deposit;
            case fnv1a("withdraw"): return _group_type::_withdraw;
            case fnv1a("transaction"): return _group_type::_transaction;
            case fnv1a("transfer"): return _group_type::_transfer;
            case fnv1a("other"): return _group_type::_other;
        }

        assert(!"unreachable");
    }

    const char* group_type_to_string(_group_type g) {
        switch(g) {
            case _group_type::_deposit: return "deposit";
            case _group_type::_withdraw: return "withdraw";
            case _group_type::_transaction: return "transaction";
            case _group_type::_transfer: return "transfer";
            case _group_type::_other: return "other";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _biz_type biz_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("deposit"): return _biz_type::_deposit;
            case fnv1a("withdraw"): return _biz_type::_withdraw;
            case fnv1a("buy"): return _biz_type::_buy;
            case fnv1a("sell"): return _biz_type::_sell;
            case fnv1a("transfer-in"): return _biz_type::_transfer_in;
            case fnv1a("transfer-out"): return _biz_type::_transfer_out;
        }
        
        assert(!"unreachable");
    }

    const char* biz_type_to_string(_biz_type b) {
        switch(b) {
            case _biz_type::_deposit: return "deposit";
            case _biz_type::_withdraw: return "withdraw";
            case _biz_type::_buy: return "buy";
            case _biz_type::_sell: return "sell";
            case _biz_type::_transfer_in: return "transfer-in";
            case _biz_type::_transfer_out: return "transfer-out";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _dwos dwos_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("cancel"): return _dwos::_cancel;
            case fnv1a("reject"): return _dwos::_reject;
            case fnv1a("success"): return _dwos::_success;
            case fnv1a("wallet-fail"): return _dwos::_wallet_fail;
            case fnv1a("wallet-processing"): return _dwos::_wallet_proc;
            case fnv1a("first-audit"): return _dwos::_first_audit;
            case fnv1a("recheck"): return _dwos::_second_audit;
            case fnv1a("first-reject"): return _dwos::_first_reject;
            case fnv1a("recheck-reject"): return _dwos::_second_reject;
        }

        assert(!"unreachable");
    }

    const char* dwos_to_string(_dwos d) {
        switch(d) {
            case _dwos::_cancel: return "cancel";
            case _dwos::_reject: return "reject";
            case _dwos::_success: return "success";
            case _dwos::_wallet_fail: return "wallet-fail";
            case _dwos::_wallet_proc: return "wallet-processing";
            case _dwos::_first_audit: return "first-audit";
            case _dwos::_second_audit: return "recheck";
            case _dwos::_first_reject: return "first-reject";
            case _dwos::_second_reject: return "recheck-reject";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _withdraw_type withdraw_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("chain-on"): return _withdraw_type::_chain;
            case fnv1a("inner-transfer"): return _withdraw_type::_inner;
        }

        assert(!"unreachable");
    }

    const char* withdraw_type_to_string(_withdraw_type w) {
        switch(w) {
            case _withdraw_type::_chain: return "chain-on";
            case _withdraw_type::_inner: return "inner-transfer";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _account_type account_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("EXCHANGE"): return _account_type::_EXCHANGE;
            case fnv1a("OTC_SGD"): return _account_type::_OTC_SGD;
            case fnv1a("CONTRACT"): return _account_type::_CONTRACT;
            case fnv1a("USD_MIX"): return _account_type::_USD_MIX;
            case fnv1a("USDT_MIX"): return _account_type::_USDT_MIX;
        }

        assert(!"unreachable");
    }

    const char* account_type_to_string(_account_type a) {
        switch (a) {
            case _account_type::_EXCHANGE: return "EXCHANGE";
            case _account_type::_OTC_SGD: return "OTC_SGD";
            case _account_type::_CONTRACT: return "CONTRACT";
            case _account_type::_USD_MIX: return "USD_MIX";
            case _account_type::_USDT_MIX: return "USDT_MIX";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _candle_gran candle_gran_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("1min"): return _candle_gran::_1m;
            case fnv1a("5min"): return _candle_gran::_5m;
            case fnv1a("15min"): return _candle_gran::_15m;
            case fnv1a("30min"): return _candle_gran::_30m;
            case fnv1a("1h"): return _candle_gran::_1h;
            case fnv1a("4h"): return _candle_gran::_4h;
            case fnv1a("6h"): return _candle_gran::_6h;
            case fnv1a("12h"): return _candle_gran::_12h;
            case fnv1a("1day"): return _candle_gran::_1D;
            case fnv1a("3day"): return _candle_gran::_3D;
            case fnv1a("1week"): return _candle_gran::_1W;
            case fnv1a("1M"): return _candle_gran::_1M;
            case fnv1a("6Hutc"): return _candle_gran::_6Hutc;
            case fnv1a("12Hutc"): return _candle_gran::_12Hutc;
            case fnv1a("1Dutc"): return _candle_gran::_1Dutc;
            case fnv1a("3Dutc"): return _candle_gran::_3Dutc;
            case fnv1a("1Wutc"): return _candle_gran::_1Wutc;
            case fnv1a("1Mutc"): return _candle_gran::_1Mutc;
        }

        assert(!"unreachable");
    }

    const char* candle_gran_to_string(_candle_gran c) {
        switch (c) {
            case _candle_gran::_1m: return "1min";
            case _candle_gran::_5m: return "5min";
            case _candle_gran::_15m: return "15min";
            case _candle_gran::_30m: return "30min";
            case _candle_gran::_1h: return "1h";
            case _candle_gran::_4h: return "4h";
            case _candle_gran::_6h: return "6h";
            case _candle_gran::_12h: return "12h";
            case _candle_gran::_1D: return "1day";
            case _candle_gran::_3D: return "3day";
            case _candle_gran::_1W: return "1week";
            case _candle_gran::_1M: return "1M";
            case _candle_gran::_6Hutc: return "6Hutc";
            case _candle_gran::_12Hutc: return "12Hutc";
            case _candle_gran::_1Dutc: return "1Dutc";
            case _candle_gran::_3Dutc: return "3Dutc";
            case _candle_gran::_1Wutc: return "1Wutc";
            case _candle_gran::_1Mutc: return "1Mutc";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _from_to_type from_to_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("spot"): return _from_to_type::_spot;
            case fnv1a("mix_usdt"): return _from_to_type::_mix_usdt;
            case fnv1a("mix_usd"): return _from_to_type::_mix_usd;
            case fnv1a("mix_usdc"): return _from_to_type::_mix_usdc;
        }

        assert(!"unreachable");
    }

    const char* from_to_type_to_string(_from_to_type f) {
        switch(f) {
            case _from_to_type::_spot: return "spot";
            case _from_to_type::_mix_usdt: return "mix_usdt";
            case _from_to_type::_mix_usd: return "mix_usd";
            case _from_to_type::_mix_usdc: return "mix_usdc";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _product_type product_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("umcbl"): return _product_type::_umcbl;
            case fnv1a("dmcbl"): return _product_type::_dmcbl;
            case fnv1a("cmcbl"): return _product_type::_cmcbl;
            case fnv1a("sumcbl"): return _product_type::_sumcbl;
            case fnv1a("sdmcbl"): return _product_type::_sdmcbl;
            case fnv1a("scmcbl"): return _product_type::_scmcbl;
        }

        assert(!"unreachable");
    }

    const char* product_type_to_string(_product_type p) {
        switch (p) {
            case _product_type::_umcbl: return "umcbl";
            case _product_type::_dmcbl: return "dmcbl";
            case _product_type::_cmcbl: return "cmcbl";
            case _product_type::_sumcbl: return "sumcbl";
            case _product_type::_sdmcbl: return "sdmcbl";
            case _product_type::_scmcbl: return "scmcbl";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _margin_mode margin_mode_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("fixed"): return _margin_mode::_fixed;
            case fnv1a("crossed"): return _margin_mode::_crossed;
        }

        assert(!"unreachable");
    }

    const char* margin_mode_to_string(_margin_mode m) {
        switch (m) {
            case _margin_mode::_fixed: return "fixed";
            case _margin_mode::_crossed: return "crossed";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _hold_mode hold_mode_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("single_hold"): return _hold_mode::_single;
            case fnv1a("double_hold"): return _hold_mode::_double;
        }

        assert(!"unreachable");
    }

    const char* hold_mode_to_string(_hold_mode h) {
        switch (h) {
            case _hold_mode::_single: return "single_hold";
            case _hold_mode::_double: return "double_hold";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _hold_side hold_side_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("long"): return _hold_side::_long;
            case fnv1a("short"): return _hold_side::_short;
        }

        assert(!"unreachable");
    }

    const char* hold_side_to_string(_hold_side h) {
        switch (h) {
            case _hold_side::_long: return "long";
            case _hold_side::_short: return "short";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _business business_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("open_long"): return _business::_open_long;
            case fnv1a("open_short"): return _business::_open_short;
            case fnv1a("close_long"): return _business::_close_long;
            case fnv1a("close_short"): return _business::_close_short;
            case fnv1a("trans_from_exchange"): return _business::_trans_from_ex;
            case fnv1a("trans_to_exchange"): return _business::_trans_to_ex;
            case fnv1a("contract_main_settle_fee"): return _business::_cntrct_main_settle_fee;
            case fnv1a("contract_margin_settle_fee"): return _business::_cntrct_margin_settle_fee;
            case fnv1a("tracking_trader_income"): return _business::_tracking_trader_income;
        }

        assert(!"unreachable");
    }

    const char* business_to_string(_business b) {
        switch (b) {
            case _business::_open_long: return "open_long";
            case _business::_open_short: return "open_short";
            case _business::_close_long: return "close_long";
            case _business::_close_short: return "close_short";
            case _business::_trans_from_ex: return "trans_from_exchange";
            case _business::_trans_to_ex: return "trans_to_exchange";
            case _business::_cntrct_main_settle_fee: return "contract_main_settle_fee";
            case _business::_cntrct_margin_settle_fee: return "contract_margin_settle_fee";
            case _business::_tracking_trader_income: return "tracking_trader_income";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _mix_side mix_side_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("open_long"): return _mix_side::_open_long;
            case fnv1a("open_short"): return _mix_side::_open_short;
            case fnv1a("close_long"): return _mix_side::_close_long;
            case fnv1a("close_short"): return _mix_side::_close_short;
            case fnv1a("buy_single"): return _mix_side::_buy_single;
            case fnv1a("sell_single"): return _mix_side::_sell_single;
        }

        assert(!"unreachable");
    }

    const char* mix_side_to_string(_mix_side s) {
        switch (s) {
            case _mix_side::_open_long: return "open_long";
            case _mix_side::_open_short: return "open_short";
            case _mix_side::_close_long: return "close_long";
            case _mix_side::_close_short: return "close_short";
            case _mix_side::_buy_single: return "buy_single";
            case _mix_side::_sell_single: return "sell_single";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _trade_side trade_side_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("open_long"): return _trade_side::_open_long;
            case fnv1a("open_short"): return _trade_side::_open_short;
            case fnv1a("close_long"): return _trade_side::_close_long;
            case fnv1a("close_short"): return _trade_side::_close_short;
            case fnv1a("reduce_close_long"): return _trade_side::_reduce_close_long;
            case fnv1a("reduce_close_short"): return _trade_side::_reduce_close_short;
            case fnv1a("offset_close_long"): return _trade_side::_offset_close_long;
            case fnv1a("offset_close_short"): return _trade_side::_offset_close_short;
            case fnv1a("burst_close_long"): return _trade_side::_burst_close_long;
            case fnv1a("burst_close_short"): return _trade_side::_burst_close_short;
            case fnv1a("delivery_close_long"): return _trade_side::_delivery_close_long;
            case fnv1a("delivery_close_short"): return _trade_side::_delivery_close_short;
            case fnv1a("buy_single"): return _trade_side::_buy_single;
            case fnv1a("sell_single"): return _trade_side::_sell_single;
            case fnv1a("reduce_buy_single"): return _trade_side::_reduce_buy_single;
            case fnv1a("reduce_sell_single"): return _trade_side::_reduce_sell_single;
            case fnv1a("burst_buy_single"): return _trade_side::_burst_buy_single;
            case fnv1a("burst_sell_single"): return _trade_side::_burst_sell_single;
            case fnv1a("delivery_buy_single"): return _trade_side::_delivery_buy_single;
            case fnv1a("delivery_sell_single"): return _trade_side::_delivery_sell_single;
        }

        assert(!"unreachable");
    }

    const char* trade_side_to_string(_trade_side t) {
        switch (t) {
            case _trade_side::_open_long: return "open_long";
            case _trade_side::_open_short: return "open_short";
            case _trade_side::_close_long: return "close_long";
            case _trade_side::_close_short: return "close_short";
            case _trade_side::_reduce_close_long: return "reduce_close_long";
            case _trade_side::_reduce_close_short: return "reduce_close_short";
            case _trade_side::_offset_close_long: return "offset_close_long";
            case _trade_side::_offset_close_short: return "offset_close_short";
            case _trade_side::_burst_close_long: return "burst_close_long";
            case _trade_side::_burst_close_short: return "burst_close_short";
            case _trade_side::_delivery_close_long: return "delivery_close_long";
            case _trade_side::_delivery_close_short: return "delivery_close_short";
            case _trade_side::_buy_single: return "buy_single";
            case _trade_side::_sell_single: return "sell_single";
            case _trade_side::_reduce_buy_single: return "reduce_buy_single";
            case _trade_side::_reduce_sell_single: return "reduce_sell_single";
            case _trade_side::_burst_buy_single: return "burst_buy_single";
            case _trade_side::_burst_sell_single: return "burst_sell_single";
            case _trade_side::_delivery_buy_single: return "delivery_buy_single";
            case _trade_side::_delivery_sell_single: return "delivery_sell_single";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _state state_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch(hash) {
            case fnv1a("init"): return _state::_init;
            case fnv1a("new"): return _state::_new;
            case fnv1a("partially_filled"): return _state::_partially_filled;
            case fnv1a("fill"): return _state::_filled;
            case fnv1a("cancelled"): return _state::_canceled;
        }

        assert(!"unreachable");
    }

    const char* state_to_string(_state s) {
        switch(s) {
            case _state::_init: return "init";
            case _state::_new: return "new";
            case _state::_partially_filled: return "partial_fill";
            case _state::_filled: return "full_fill";
            case _state::_canceled: return "canceled";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _trigger_type trigger_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("fill_price"): return _trigger_type::_fill_price;
            case fnv1a("market_price"): return _trigger_type::_market_price;
        }

        assert(!"unreachable");
    }

    const char* trigger_type_to_string(_trigger_type t) {
        switch (t) {
            case _trigger_type::_fill_price: return "fill_price";
            case _trigger_type::_market_price: return "market_price";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _plan_type plan_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("profit_plan"): return _plan_type::_profit_plan;
            case fnv1a("loss_plan"): return _plan_type::_loss_plan;
            case fnv1a("normal_plan"): return _plan_type::_normal_plan;
            case fnv1a("pos_profit"): return _plan_type::_pos_profit;
            case fnv1a("pos_loss"): return _plan_type::_pos_loss;
            case fnv1a("moving_plan"): return _plan_type::_moving_plan;
            case fnv1a("track_plan"): return _plan_type::_track_plan;
        }

        assert(!"unreachable");
    }

    const char* plan_type_to_string(_plan_type p) {
        switch (p) {
            case _plan_type::_profit_plan: return "profit_plan";
            case _plan_type::_loss_plan: return "loss_plan";
            case _plan_type::_normal_plan: return "normal_plan";
            case _plan_type::_pos_profit: return "pos_profit";
            case _plan_type::_pos_loss: return "pos_loss";
            case _plan_type::_moving_plan: return "moving_plan";
            case _plan_type::_track_plan: return "track_plan";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _is_plan is_plan_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("plan"): return _is_plan::_plan;
            case fnv1a("profit_loss"): return _is_plan::_profit_loss;
        }

        assert(!"unreachable");
    }

    const char* is_plan_to_string(_is_plan p) {
        switch (p) {
            case _is_plan::_plan: return "plan";
            case _is_plan::_profit_loss: return "profit_loss";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _plan_status plan_status_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("not_trigger"): return _plan_status::_not_trigger;
            case fnv1a("triggered"): return _plan_status::_triggered;
            case fnv1a("fail_trigger"): return _plan_status::_fail_trigger;
            case fnv1a("cancel"): return _plan_status::_cancel;
        }
        
        assert(!"unreachable");
    }

    const char* plan_status_to_string(_plan_status p) {
        switch (p) {
            case _plan_status::_not_trigger: return "not_trigger";
            case _plan_status::_triggered: return "triggered";
            case _plan_status::_fail_trigger: return "fail_trigger";
            case _plan_status::_cancel: return "cancel";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _stop_type stop_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("profit"): return _stop_type::_profit;
            case fnv1a("loss"): return _stop_type::_loss;
        }

        assert(!"unreachable");
    }

    const char* stop_type_to_string(_stop_type s) {
        switch (s) {
            case _stop_type::_profit: return "profit";
            case _stop_type::_loss: return "loss";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    _ws_plan_type ws_plan_type_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("pl"): return _ws_plan_type::_pl;
            case fnv1a("tp"): return _ws_plan_type::_tp;
            case fnv1a("sl"): return _ws_plan_type::_sl;
            case fnv1a("ptp"): return _ws_plan_type::_ptp;
            case fnv1a("psl"): return _ws_plan_type::_psl;
        }

        assert(!"unreachable");
    }

    const char* ws_plan_type_to_string(_ws_plan_type p) {
        switch (p) {
            case _ws_plan_type::_pl: return "pl";
            case _ws_plan_type::_tp: return "tp";
            case _ws_plan_type::_sl: return "sl";
            case _ws_plan_type::_ptp: return "ptp";
            case _ws_plan_type::_psl: return "psl";
        }

        assert(!"unreachable");
    }

//------------------------------------------------------------------------------

    _account_status account_status_from_string(const char* str) {
        const auto hash = fnv1a(str);
        switch (hash) {
            case fnv1a("normal"): return _account_status::_normal;
            case fnv1a("freeze"): return _account_status::_freeze;
            case fnv1a("del"): return _account_status::_del;
        }

        assert(!"unreachable");
    }

    const char* account_status_to_string(_account_status a) {
        switch (a) {
            case _account_status::_normal: return "normal";
            case _account_status::_freeze: return "freeze";
            case _account_status::_del: return "del";
        }

        assert(!"unreachable");
        return nullptr;
    }

//------------------------------------------------------------------------------

    unsigned int auths_from_string_set(const std::set<std::string> auths) {
        unsigned int a = 0;
        auths.find("withdraw") != auths.end() ? a || 1 : a;
        auths.find("recharge") != auths.end() ? a || 2 : a;
        auths.find("transfer") != auths.end() ? a || 4 : a;
        auths.find("spot_trade") != auths.end() ? a || 8 : a;
        auths.find("contract_trade") != auths.end() ? a || 16 : a;
        auths.find("readonly") != auths.end() ? a || 32 : a;
        auths.find("trade") != auths.end() ? a || 64 : a;
        return a;
    }

    std::set<std::string> auths_to_string_set(unsigned int a) {
        std::set<std::string> auths;
        if (a & 1) 
            auths.insert("withdraw");
        if (a & 2)
            auths.insert("recharge");
        if (a & 4)
            auths.insert("transfer");
        if (a & 8)
            auths.insert("spot_trade");
        if (a & 16)
            auths.insert("contract_trade");
        if (a & 32)
            auths.insert("readonly");
        if (a & 64)
            auths.insert("trade");
        
        return auths;
    }

//------------------------------------------------------------------------------

} // ns bg_api
