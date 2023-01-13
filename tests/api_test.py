from flask import Flask, request
import json
import time

app = Flask(__name__)

####################################################################################################
# Spot Endpoints
# GET /api/spot/v1/public/time
@app.get('/api/spot/v1/public/time')
def get_time():
    res = {"code": "00000",
           "msg": "success",
           "requestTime": 0,
           "data": round(time.time()*1000)}
    return res

# GET /api/spot/v1/public/currencies
@app.get('/api/spot/v1/public/currencies')
def get_currencies():
    with open('data/spot/currencies.json', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/public/products
@app.get('/api/spot/v1/public/products')
def get_products():
    with open('data/spot/products.json', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/public/product
@app.get('/api/spot/v1/public/product')
def get_product():
    with open('data/spot/product.json', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/market/ticker

# GET /api/spot/v1/market/tickers

# GET /api/spot/v1/market/fills

# GET /api/spot/v1/market/candles

# GET /api/spot/v1/market/depth

# POST /api/spot/v1/wallet/transfer

# POST /api/spot/v1/wallet/subTransfer

# GET /api/spot/v1/wallet/deposit-address

# POST /api/spot/v1/wallet/withdrawal

# POST /api/spot/v1/wallet/withdrawal-inner

# GET /api/spot/v1/wallet/withdrawal-list

# GET /api/spot/v1/wallet/deposit-list

# GET /api/spot/v1/account/getInfo

# GET /api/spot/v1/account/assets

# POST /api/spot/v1/account/bills

# GET /api/spot/v1/account/transferRecords

# POST /api/spot/v1/trade/orders

# POST /api/spot/v1/trade/batch-orders

# POST /api/spot/v1/trade/cancel-order

# POST /api/spot/v1/trade/cancel-batch-orders

# POST /api/spot/v1/trade/orderInfo

# POST /api/spot/v1/trade/open-orders

# POST /api/spot/v1/trade/history

# POST /api/spot/v1/trade/fills

# POST /api/spot/v1/plan/placePlan

# POST /api/spot/v1/plan/modifyPlan

# POST /api/spot/v1/plan/cancelPlan

# POST /api/spot/v1/plan/currentPlan

# POST /api/spot/v1/plan/historyPlan

####################################################################################################
# Futures Endpoints
# GET /api/mix/v1/market/contracts

# GET /api/mix/v1/market/depth

# GET /api/mix/v1/market/ticker

# GET /api/mix/v1/market/tickers

# GET /api/mix/v1/market/fills

# GET /api/mix/v1/market/candles

# GET /api/mix/v1/market/index

# GET /api/mix/v1/market/funding-time

# GET /api/mix/v1/market/history-fundRate

# GET /api/mix/v1/market/current-fundRate

# GET /api/mix/v1/market/open-interest

# GET /api/mix/v1/market/mark-price

# GET /api/mix/v1/market/symbol-leverage

# GET /api/mix/v1/account/account

# GET /api/mix/v1/account/accounts

# POST /api/mix/v1/account/sub-account-contract-assets

# POST /api/mix/v1/account/open-count

# POST /api/mix/v1/account/setLeverage

# POST /api/mix/v1/account/setMargin

# POST /api/mix/v1/account/setMarginMode

# POST /api/mix/v1/account/setPositionMode

# GET /api/mix/v1/position/singlePosition

# GET /api/mix/v1/position/allPosition

# GET /api/mix/v1/account/accountBill

# GET /api/mix/v1/account/accountBusinessBill

# POST /api/mix/v1/order/placeOrder

# POST /api/mix/v1/order/placeOrder

# POST /api/mix/v1/order/batch-orders

# POST /api/mix/v1/order/cancel-order

# POST /api/mix/v1/order/cancel-batch-orders

# POST /api/mix/v1/order/cancel-all-orders

# GET /api/mix/v1/order/current

# GET /api/mix/v1/order/marginCoinCurrent

# GET /api/mix/v1/order/history

# GET /api/mix/v1/order/historyProductType

# GET /api/mix/v1/order/detail

# GET /api/mix/v1/order/fills

# GET /api/mix/v1/order/allFills

# POST /api/mix/v1/plan/placePlan

# POST /api/mix/v1/plan/modifyPlan

# POST /api/mix/v1/plan/modifyPlanPreset

# POST /api/mix/v1/plan/placeTPSL

# POST /api/mix/v1/plan/placeTrailStop

# POST /api/mix/v1/plan/placePositionsTPSL

# POST /api/mix/v1/plan/modifyTPSLPlan

# POST /api/mix/v1/plan/cancelPlan

# POST /api/mix/v1/plan/cancelAllPlan

# GET /api/mix/v1/plan/currentPlan

# GET /api/mix/v1/plan/historyPlan

# GET /api/mix/v1/trace/currentTrack

# GET /api/mix/v1/trace/followerOrder

# GET /api/mix/v1/trace/followerHistoryOrders

# POST /api/mix/v1/trace/closeTrackOrder

# POST /api/mix/v1/trace/modifyTPSL

# GET /api/mix/v1/trace/historyTrack

# GET /api/mix/v1/trace/summary

# GET /api/mix/v1/trace/profitSettleTokenIdGroup

# GET /api/mix/v1/trace/profitDateGroupList

# GET /api/mix/v1/trace/profitDateList

# GET /api/mix/v1/trace/waitProfitDateList

# GET /api/mix/v1/trace/traderSymbols

# POST /api/mix/v1/trace/setUpCopySymbols
