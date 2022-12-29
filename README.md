# bg_api
Bitget API implemented in C++ for both synchronous and asynchronous way. Based on niXman's Binance C++ API.

Note: This project is currently a work in progress. The expected date of completion is January 15, 2023.

## TODO:

Here are the files that need to be implemented before I get started on the REST API and WebSocket API.

- [x] enums.hpp
- [x] enums.cpp
- [x] errors.hpp
- [x] errors.cpp
- [x] types.hpp
- [x] types.cpp


## REST API

Here is a breakdown of what needs to be done for the REST API:

- **Spot methods**:
    - [x] getServerTime()
    - [x] getCoinList()
    - [x] getSymbol()
    - [x] getSymbols()
    - [x] getSpotTicker()
    - [x] getSpotTickers()
    - [x] getSpotTrades()
    - [x] getSpotCandles()
    - [x] getSpotDepth()
    - [x] transfer()
    - [x] getAddress()
    - [x] withdraw()
    - [x] innerWithdraw()
    - [ ] getWithdrawalList()
    - [ ] getDepositList()
    - [ ] getInfo()
    - [ ] getAssets()
    - [ ] getBills()
    - [ ] getTransferList()
    - [ ] placeOrder()
    - [ ] placeOrders()
    - [ ] cancelOrder()
    - [ ] cancelOrders()
    - [ ] getOrderDetails()
    - [ ] getOpenOrders()
    - [ ] getOrderHistory()
    - [ ] getTransactionDetails()
    - [ ] placePlanOrder()
    - [ ] modifyPlanOrder()
    - [ ] cancelPlanOrder()
    - [ ] getPlanOrders()
    - [ ] getHistoryPlanOrders()
- **Futures methods**:
    - [ ] Get All Symbols
    - [ ] Get Depth
    - [ ] Get Single Symbol Ticker
    - [ ] Get All Symbol Ticker
    - [ ] Get Fills
    - [ ] Get Candle Data
    - [ ] Get Symbol Index Price
    - [ ] Get Symbol Next Funding
    - [ ] Get History Funding Rate
    - [ ] Get Current Funding Rate
    - [ ] Get Open Interest
    - [ ] Get Symbol Mark Price
    - [ ] Get Symbol Leverage
    - [ ] Get Single Account
    - [ ] Get Account List
    - [ ] Get Open Count
    - [ ] Change Leverage
    - [ ] Change Margin
    - [ ] Change Margin Mode
    - [ ] Change Hold Mode
    - [ ] Get Symbol Position
    - [ ] Get All Position
    - [ ] Get Account Bill
    - [ ] Get Business Account Bill
    - [ ] Place Order
    - [ ] Reversal
    - [ ] Batch Order
    - [ ] Cancel Order
    - [ ] Batch Cancel Order
    - [ ] Cancel All Order
    - [ ] Get Open Order
    - [ ] Get All Open Order
    - [ ] Get History Orders
    - [ ] Get ProductType History Orders
    - [ ] Get Order Details
    - [ ] Get Order Fill Detail
    - [ ] Get ProductType Order Fill Detail
    - [ ] Place Plan Order
    - [ ] Modify Plan Order
    - [ ] Modify Plan Order TPSL
    - [ ] Place Stop Order
    - [ ] Place Trailing Stop Order
    - [ ] Place Position TPSL
    - [ ] Modify Stop Order
    - [ ] Cancel Plan Order (TPSL)
    - [ ] Cancel All Trigger Order (TPSL)
    - [ ] Get Plan Order (TPSL) List
    - [ ] Get History Plan Orders (TPSL)

## WebSocket API

Here is a breakdown of what needs to be done for the WebSocket API:
- **Asynchronous**:
    - [ ] Login
    - [ ] Subscribe and Unsubscribe
    - [ ] Public Channels
        - [ ] Tickers Channel
        - [ ] Candlesticks Channel
        - [ ] Order Book Channel
        - [ ] Trades Channel
        - [ ] New Trades Channel
    - [ ] Private Channels
        - [ ] Account Channel
        - [ ] Positions Channel
        - [ ] Order Channel
        - [ ] Plan Order Channel
    - [ ] Checksum
- **Synchronous**:
    - [ ] Login
    - [ ] Subscribe and Unsubscribe
    - [ ] Public Channels
        - [ ] Tickers Channel
        - [ ] Candlesticks Channel
        - [ ] Order Book Channel
        - [ ] Trades Channel
        - [ ] New Trades Channel
    - [ ] Private Channels
        - [ ] Account Channel
        - [ ] Positions Channel
        - [ ] Order Channel
        - [ ] Plan Order Channel
    - [ ] Checksum
