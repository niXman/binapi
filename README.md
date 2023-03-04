# bg_api
Bitget API implemented in C++ for both synchronous and asynchronous way. Based on niXman's [Binance C++ API](https://github.com/niXman/binapi).

Note: This project is currently a work in progress. The expected date of completion is the end of May 2023.

![](https://progress-bar.dev/0/?scale=100&title=REST%20API%20&width=240)

![](https://progress-bar.dev/0/?scale=100&title=WebSocket%20API&width=216)

## TODO:

#### REST API

Here is a breakdown of what needs to be done for the REST API:

##### Spot methods:

| Method                        | Written               | Tested                |
| :---                          | :----:                | :----:                |
| getServerTime()               | :x:                   | :x:                   |
| getCoinList()                 | :x:                   | :x:                   |
| getSymbol()                   | :x:                   | :x:                   |
| getSymbols()                  | :x:                   | :x:                   |
| getSpotTicker()               | :x:                   | :x:                   |
| getSpotTickers()              | :x:                   | :x:                   |
| getSpotTrades()               | :x:                   | :x:                   |
| getSpotCandles()              | :x:                   | :x:                   |
| getSpotDepth()                | :x:                   | :x:                   |
| transfer()                    | :x:                   | :x:                   |
| getAddress()                  | :x:                   | :x:                   |
| withdraw()                    | :x:                   | :x:                   |
| innerWithdraw()               | :x:                   | :x:                   |
| getWithdrawalList()           | :x:                   | :x:                   |
| getDepositList()              | :x:                   | :x:                   |
| getApiKeyInfo()               | :x:                   | :x:                   |
| getSpotAccount()              | :x:                   | :x:                   |
| getSpotBills()                | :x:                   | :x:                   |
| getSpotTransferList()         | :x:                   | :x:                   |
| placeSpotOrder()              | :x:                   | :x:                   |
| placeSpotOrders()             | :x:                   | :x:                   |
| cancelSpotOrder()             | :x:                   | :x:                   |
| cancelSpotOrders()            | :x:                   | :x:                   |
| getSpotOrderDetails()         | :x:                   | :x:                   |
| getSpotOpenOrders()           | :x:                   | :x:                   |
| getSpotOrderHistory()         | :x:                   | :x:                   |
| getSpotFills()                | :x:                   | :x:                   |
| placeSpotPlanOrder()          | :x:                   | :x:                   |
| modifySpotPlanOrder()         | :x:                   | :x:                   |
| cancelSpotPlanOrder()         | :x:                   | :x:                   |
| getSpotPlanOrders()           | :x:                   | :x:                   |
| getSpotHistoryPlanOrders()    | :x:                   | :x:                   |

##### Futures methods:

| Method                            | Written               | Tested                |
| :---                              | :----:                | :----:                |
| getContracts()                    | :x:                   | :x:                   |
| getFuturesDepth()                 | :x:                   | :x:                   |
| getFuturesTicker()                | :x:                   | :x:                   |
| getFuturesTickers()               | :x:                   | :x:                   |
| getFuturesFills()                 | :x:                   | :x:                   |
| getFuturesCandles()               | :x:                   | :x:                   |
| getFuturesIndex()                 | :x:                   | :x:                   |
| getNextFundingTime()              | :x:                   | :x:                   |
| getHistoryFundRate()              | :x:                   | :x:                   |
| getFundRate()                     | :x:                   | :x:                   |
| getOpenInterest()                 | :x:                   | :x:                   |
| getSymbolMarkPrice()              | :x:                   | :x:                   |
| getSymbolLeverage()               | :x:                   | :x:                   |
| getFuturesAccount()               | :x:                   | :x:                   |
| getFuturesAccounts()              | :x:                   | :x:                   |
| getOpenCount()                    | :x:                   | :x:                   |
| setLeverage()                     | :x:                   | :x:                   |
| setMargin()                       | :x:                   | :x:                   |
| setMarginMode()                   | :x:                   | :x:                   |
| setPositionMode()                 | :x:                   | :x:                   |
| getFuturesPosition()              | :x:                   | :x:                   |
| getFuturesPositions()             | :x:                   | :x:                   |
| getFuturesAccountBills()          | :x:                   | :x:                   |
| getBizAccountBills()              | :x:                   | :x:                   |
| placeFuturesOrder()               | :x:                   | :x:                   |
| placeFuturesReversal()            | :x:                   | :x:                   |
| placeFuturesBatchOrder()          | :x:                   | :x:                   |
| cancelFuturesOrder()              | :x:                   | :x:                   |
| batchCancelFuturesOrder()         | :x:                   | :x:                   |
| cancelAllFuturesOrders()          | :x:                   | :x:                   |
| getOpenFuturesOrder()             | :x:                   | :x:                   |
| getOpenFuturesOrders()            | :x:                   | :x:                   |
| getFuturesHistory()               | :x:                   | :x:                   |
| getProductTypeHistory()           | :x:                   | :x:                   |
| getFuturesOrderDetail()           | :x:                   | :x:                   |
| getFuturesOrderFills()            | :x:                   | :x:                   |
| getAllFuturesOrderFills()         | :x:                   | :x:                   |
| placeFuturesPlanOrder()           | :x:                   | :x:                   |
| modifyFuturesPlanOrder()          | :x:                   | :x:                   |
| modifyFuturesPlanOrderTPSL()      | :x:                   | :x:                   |
| placeFuturesStopOrder()           | :x:                   | :x:                   |
| placeFuturesTrailingStopOrder()   | :x:                   | :x:                   |
| placeFuturesPositionTPSL()        | :x:                   | :x:                   |
| modifyFuturesStopOrder()          | :x:                   | :x:                   |
| cancelFuturesPlanOrder()          | :x:                   | :x:                   |
| cancelFuturesPlanOrders()         | :x:                   | :x:                   |
| getFuturesPlanOrders()            | :x:                   | :x:                   |
| getFuturesHistoryPlanOrders()     | :x:                   | :x:                   |
| getTraderOpenOrder()              | :x:                   | :x:                   |
| getTraderOpenOrders()             | :x:                   | :x:                   |
| getFollowerHistoryOrders()        | :x:                   | :x:                   |
| closePosition()                   | :x:                   | :x:                   |
| modifyTPSL()                      | :x:                   | :x:                   |
| getTradersHistoryOrders()         | :x:                   | :x:                   |
| getTraderProfitSummary()          | :x:                   | :x:                   |
| getTraderHistoryProfit()          | :x:                   | :x:                   |
| getTraderProfitDetails()          | :x:                   | :x:                   |
| getCopyTradeSymbols()             | :x:                   | :x:                   |
| changeCopyTradeSymbol()           | :x:                   | :x:                   |

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
