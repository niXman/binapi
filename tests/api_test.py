from flask import Flask, request
import json
import time

app = Flask(__name__)

def verify_signature(headers, data):
    pass

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
@app.get('/api/spot/v1/public/ticker')
def get_():
    with open('data/spot/ticker.json', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/market/tickers
@app.get('/api/spot/v1/public/tickers')
def get_():
    with open('data/spot/tickers.json', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/market/fills
@app.get('/api/spot/v1/public/')
def get_():
    with open('', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/market/candles
@app.get('')
def get_():
    with open('', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/market/depth
@app.get('/api/spot/v1/public/depth')
def get_():
    with open('data/spot/depth.json', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/wallet/transfer
@app.get('/api/spot/v1/public/transfer')
def get_():
    res = {
                "code":"00000",
                "msg":"success"
        }       
    return res

# POST /api/spot/v1/wallet/subTransfer
@app.get('/api/spot/v1/public/subTransfer')
def get_():
    res = {
    "code":"00000",
    "msg":"success"
    }
    return res

# GET /api/spot/v1/wallet/deposit-address
@app.get('/api/spot/v1/public/')
def get_():
    with open('', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/wallet/withdrawal
@app.get('/api/spot/v1/public/withdraw')
def get_():
    res = {
  "code": "00000",
  "msg": "success",
  "data": "888291686266343424"
}
    return res

# POST /api/spot/v1/wallet/withdrawal-inner
@app.get('/api/spot/v1/public/withdraw-inner')
def get_():
    res = {
  "code": "00000",
  "msg": "success",
  "data": "888291686266343424"
}
    return res

# GET /api/spot/v1/wallet/withdrawal-list
@app.get('/api/spot/v1/public/withdraw-list')
def get_():
    with open('data/spot/withdraws.json', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/wallet/deposit-list
@app.get('/api/spot/v1/public/deposit_list')
def get_():
    with open('data/spot/deposits.json', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/account/getInfo
@app.get('/api/spot/v1/public/getInfo')
def get_():
    with open('data/spot/apikey_info.json', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/account/assets
@app.get('/api/spot/v1/public/assets')
def get_():
    with open('data/spot/assets.json', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/account/bills
@app.get('/api/spot/v1/public/bills')
def get_():
    with open('data/spot/bills.json', 'r') as f:
        res = json.load(f)
        return res

# GET /api/spot/v1/account/transferRecords
@app.get('/api/spot/v1/public/')
def get_():
    with open('', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/trade/orders
@app.get('/api/spot/v1/public/orders')
def get_():
    res = {
    "code":"00000",
    "msg":"success",
    "data":{
        "orderId":"1001",
        "clientOrderId":"hgXjh89AsxleMSw"
    }
}
    return res

# POST /api/spot/v1/trade/batch-orders
@app.get('/api/spot/v1/public/batch-order')
def get_():
    with open('data/spot/batchOrder.json', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/trade/cancel-order
@app.get('/api/spot/v1/public/cancel-order')
def get_():
    res = {

  "code":"00000",
   "message":"success",
  "data": "202934892814667"
}  
    return res

# POST /api/spot/v1/trade/cancel-batch-orders
@app.get('/api/spot/v1/public/')
def get_():
    with open('', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/trade/orderInfo
@app.get('/api/spot/v1/public/orderInfo')
def get_():
    with open('data/spot/orderInfo.json', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/trade/open-orders
@app.get('/api/spot/v1/public/')
def get_():
    with open('', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/trade/history
@app.get('/api/spot/v1/public/orderHistory')
def get_():
    with open('data/spot/orderHistory.json', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/trade/fills
@app.get('/api/spot/v1/public/')
def get_():
    with open('', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/plan/placePlan
@app.get('/api/spot/v1/public/placePlan')
def get_():
    with open('data/spot/placePlan.json', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/plan/modifyPlan
@app.get('/api/spot/v1/public/modifyPlan')
def get_():
    with open('data/spot/modifyPlan.json', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/plan/cancelPlan
@app.get('/api/spot/v1/public/cancelPlan')
def get_():
    res = {
  "code": "00000",
  "msg": "success",
  "requestTime": 1668134497496,
  "data": "974792060738441216"
}
    return res

# POST /api/spot/v1/plan/currentPlan
@app.get('/api/spot/v1/public/currentPlan')
def get_():
    with open('data/spot/currentPlan.json', 'r') as f:
        res = json.load(f)
        return res

# POST /api/spot/v1/plan/historyPlan
@app.get('/api/spot/v1/public/historyPlan')
def get_():
    with open('data/spot/historyPlan.json', 'r') as f:
        res = json.load(f)
        return res

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
