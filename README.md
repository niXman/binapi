# bg_api
Bitget API implemented in C++ for both synchronous and asynchronous way. Based on niXman's Binance C++ API.

Note: This project is currently a work in progress. The expected date of completion is January 31, 2023.

![](https://progress-bar.dev/26/?scale=100&title=REST%20API%20&width=240)

![](https://progress-bar.dev/0/?scale=100&title=WebSocket%20API&width=216)

## TODO:

#### REST API

Here is a breakdown of what needs to be done for the REST API:

##### Spot methods:

| Method                        | Written               | Tested                |
| :---                          | :----:                | :----:                |
| getServerTime()               | :heavy_check_mark:    | :heavy_check_mark:    |
| getCoinList()                 | :heavy_check_mark:    | :heavy_check_mark:    |
| getSymbol()                   | :heavy_check_mark:    | :heavy_check_mark:    |
| getSymbols()                  | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotTicker()               | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotTickers()              | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotTrades()               | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotCandles()              | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotDepth()                | :heavy_check_mark:    | :heavy_check_mark:    |
| transfer()                    | :heavy_check_mark:    | :x:                   |
| getAddress()                  | :heavy_check_mark:    | :x:                   |
| withdraw()                    | :heavy_check_mark:    | :x:                   |
| innerWithdraw()               | :heavy_check_mark:    | :x:                   |
| getWithdrawalList()           | :heavy_check_mark:    | :heavy_check_mark:    |
| getDepositList()              | :heavy_check_mark:    | :heavy_check_mark:    |
| getApiKeyInfo()               | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotAccount()              | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotBills()                | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotTransferList()         | :heavy_check_mark:    | :heavy_check_mark:    |
| placeSpotOrder()              | :heavy_check_mark:    | :x:                   |
| placeSpotOrders()             | :heavy_check_mark:    | :x:                   |
| cancelSpotOrder()             | :heavy_check_mark:    | :x:                   |
| cancelSpotOrders()            | :heavy_check_mark:    | :x:                   |
| getSpotOrderDetails()         | :heavy_check_mark:    | :x:                   |
| getSpotOpenOrders()           | :heavy_check_mark:    | :x:                   |
| getSpotOrderHistory()         | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotFills()                | :heavy_check_mark:    | :x:                   |
| placeSpotPlanOrder()          | :heavy_check_mark:    | :x:                   |
| modifySpotPlanOrder()         | :heavy_check_mark:    | :x:                   |
| cancelSpotPlanOrder()         | :heavy_check_mark:    | :x:                   |
| getSpotPlanOrders()           | :heavy_check_mark:    | :x:                   |
| getSpotHistoryPlanOrders()    | :heavy_check_mark:    | :x:                   |

##### Futures methods:

| Method                        | Written               | Tested                |
| :---                          | :----:                | :----:                |
| getContracts()                | :x:                   | :x:                   |
| getMixDepth()                 | :x:                   | :x:                   |
| getMixTicker()                | :x:                   | :x:                   |
| getMixTickers()               | :x:                   | :x:                   |
| getMixFills()                 | :x:                   | :x:                   |
| getMixCandles()               | :x:                   | :x:                   |
| getMixIndex()                 | :x:                   | :x:                   |
| getNextFundingTime()          | :x:                   | :x:                   |
| getHistoryFundRate()          | :x:                   | :x:                   |
| getFundRate()                 | :x:                   | :x:                   |
| getOpenInterest()             | :x:                   | :x:                   |
| getSymbolMarkPrice()          | :x:                   | :x:                   |
| getSymbolLeverage()           | :x:                   | :x:                   |
| getMixAccount()               | :x:                   | :x:                   |
| getMixAccounts()              | :x:                   | :x:                   |
| getOpenCount()                | :x:                   | :x:                   |
| setLeverage()                 | :x:                   | :x:                   |
| setMargin()                   | :x:                   | :x:                   |
| setMarginMode()               | :x:                   | :x:                   |
| setPositionMode()             | :x:                   | :x:                   |
| getMixPosition()              | :x:                   | :x:                   |
| getMixPositions()             | :x:                   | :x:                   |
| getMixAccountBill()           | :x:                   | :x:                   |
| getBizAccountBill()           | :x:                   | :x:                   |
| placeMixOrder()               | :x:                   | :x:                   |
| placeMixReversal()            | :x:                   | :x:                   |
| placeMixBatchOrder()          | :x:                   | :x:                   |
| cancelMixOrder()              | :x:                   | :x:                   |
| batchCancelMixOrder()         | :x:                   | :x:                   |
| cancelAllMixOrders()          | :x:                   | :x:                   |
| getOpenMixOrder()             | :x:                   | :x:                   |
| getOpenMixOrders()            | :x:                   | :x:                   |
| getMixHistory()               | :x:                   | :x:                   |
| getProductTypeHistory()       | :x:                   | :x:                   |
| getMixOrderDetail()           | :x:                   | :x:                   |
| getMixOrderFills()            | :x:                   | :x:                   |
| getAllMixOrderFills()         | :x:                   | :x:                   |
| placeMixPlanOrder()           | :x:                   | :x:                   |
| modifyMixPlanOrder()          | :x:                   | :x:                   |
| modifyMixPlanOrderTPSL()      | :x:                   | :x:                   |
| placeMixStopOrder()           | :x:                   | :x:                   |
| placeMixTrailingStopOrder()   | :x:                   | :x:                   |
| placeMixPositionTPSL()        | :x:                   | :x:                   |
| modifyMixStopOrder()          | :x:                   | :x:                   |
| cancelMixPlanOrder()          | :x:                   | :x:                   |
| cancelMixPlanOrders()         | :x:                   | :x:                   |
| getMixPlanOrders()            | :x:                   | :x:                   |
| getMixHistoryPlanOrders()     | :x:                   | :x:                   |
| getTraderOpenOrder()          | :x:                   | :x:                   |
| getTraderOpenOrders()         | :x:                   | :x:                   |
| getFollowerHistoryOrders()    | :x:                   | :x:                   |
| closePosition()               | :x:                   | :x:                   |
| modifyTPSL()                  | :x:                   | :x:                   |
| getTradersHistoryOrders()     | :x:                   | :x:                   |
| getTraderProfitSummary()      | :x:                   | :x:                   |
| getTraderHistoryProfit()      | :x:                   | :x:                   |
| getTraderProfitDetails()      | :x:                   | :x:                   |
| getCopyTradeSymbols()         | :x:                   | :x:                   |
| changeCopyTradeSymbol()       | :x:                   | :x:                   |

#### WebSocket API

Here is a breakdown of what needs to be done for the WebSocket API:

| Channel       | Written   | Subscribe Tested  | Unsubscribe Tested    |
| :---          | :----:    | :----:            | :----:                |
| Login         | :x:       | N/A               | N/A                   |
| Tickers       | :x:       | :x:               | :x:                   |
| Candlesticks  | :x:       | :x:               | :x:                   |
| Order Book    | :x:       | :x:               | :x:                   |
| Trades        | :x:       | :x:               | :x:                   |
| New Trades    | :x:       | :x:               | :x:                   |
| Account       | :x:       | :x:               | :x:                   |
| Positions     | :x:       | :x:               | :x:                   |
| Order         | :x:       | :x:               | :x:                   |
| Plan Order    | :x:       | :x:               | :x:                   |
| Checksum      | :x:       | N/A               | N/A                   |