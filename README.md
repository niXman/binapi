# bg_api
Bitget API implemented in C++ for both synchronous and asynchronous way. Based on niXman's Binance C++ API.

Note: This project is currently a work in progress. The expected date of completion is January 15, 2023.

![25%](https://progress-bar.dev/25/?scale=100&title=REST%20API%20&width=240)

![0%](https://progress-bar.dev/0/?scale=100&title=WebSocket%20API&width=216)

## TODO:

#### REST API

Here is a breakdown of what needs to be done for the REST API:

##### Spot methods:
| Method                        | Written               | Request Working       | Response Verified     |
| :---                          | :----:                | :----:                | :---:                 |
| getServerTime()               | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| getCoinList()                 | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| getSymbol()                   | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| getSymbols()                  | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotTicker()               | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotTickers()              | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotTrades()               | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotCandles()              | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotDepth()                | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| transfer()                    | :heavy_check_mark:    | :x:                   | :x:                   |
| getAddress()                  | :heavy_check_mark:    | :x:                   | :x:                   |
| withdraw()                    | :heavy_check_mark:    | :x:                   | :x:                   |
| innerWithdraw()               | :heavy_check_mark:    | :x:                   | :x:                   |
| getWithdrawalList()           | :heavy_check_mark:    | :heavy_check_mark:    | :x:                   |
| getDepositList()              | :heavy_check_mark:    | :heavy_check_mark:    | :x:                   |
| getApiKeyInfo()               | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotAccount()              | :heavy_check_mark:    | :heavy_check_mark:    | :heavy_check_mark:    |
| getSpotBills()                | :heavy_check_mark:    | :heavy_check_mark:    | :x:                   |
| getSpotTransferList()         | :heavy_check_mark:    | :heavy_check_mark:    | :x:                   |
| placeSpotOrder()              | :heavy_check_mark:    | :x:                   | :x:                   |
| placeSpotOrders()             | :heavy_check_mark:    | :x:                   | :x:                   |
| cancelSpotOrder()             | :heavy_check_mark:    | :x:                   | :x:                   |
| cancelSpotOrders()            | :heavy_check_mark:    | :x:                   | :x:                   |
| getSpotOrderDetails()         | :x:                   | :x:                   | :x:                   |
| getSpotOpenOrders()           | :x:                   | :x:                   | :x:                   |
| getSpotOrderHistory()         | :heavy_check_mark:    | :heavy_check_mark:    | :x:                   |
| getSpotFills()                | :x:                   | :x:                   | :x:                   |
| placeSpotPlanOrder()          | :x:                   | :x:                   | :x:                   |
| modifySpotPlanOrder()         | :x:                   | :x:                   | :x:                   |
| cancelSpotPlanOrder()         | :x:                   | :x:                   | :x:                   |
| getSpotPlanOrders()           | :x:                   | :x:                   | :x:                   |
| getSpotHistoryPlanOrders()    | :x:                   | :x:                   | :x:                   |

##### Futures methods:
| Method                        | Written               | Request Working       | Response Verified     |
| :---                          | :----:                | :----:                | :---:                 |
| getContracts()                | :x:                   | :x:                   | :x:                   |
| getMixDepth()                 | :x:                   | :x:                   | :x:                   |
| getMixTicker()                | :x:                   | :x:                   | :x:                   |
| getMixTickers()               | :x:                   | :x:                   | :x:                   |
| getMixFills()                 | :x:                   | :x:                   | :x:                   |
| getMixCandles()               | :x:                   | :x:                   | :x:                   |
| getMixIndex()                 | :x:                   | :x:                   | :x:                   |
| getNextFundingTime()          | :x:                   | :x:                   | :x:                   |
| getHistoryFundRate()          | :x:                   | :x:                   | :x:                   |
| getFundRate()                 | :x:                   | :x:                   | :x:                   |
| getOpenInterest()             | :x:                   | :x:                   | :x:                   |
| getSymbolMarkPrice()          | :x:                   | :x:                   | :x:                   |
| getSymbolLeverage()           | :x:                   | :x:                   | :x:                   |
| getMixAccount()               | :x:                   | :x:                   | :x:                   |
| getMixAccounts()              | :x:                   | :x:                   | :x:                   |
| getOpenCount()                | :x:                   | :x:                   | :x:                   |
| setLeverage()                 | :x:                   | :x:                   | :x:                   |
| setMargin()                   | :x:                   | :x:                   | :x:                   |
| setMarginMode()               | :x:                   | :x:                   | :x:                   |
| setPositionMode()             | :x:                   | :x:                   | :x:                   |
| getMixPosition()              | :x:                   | :x:                   | :x:                   |
| getMixPositions()             | :x:                   | :x:                   | :x:                   |
| getMixAccountBill()           | :x:                   | :x:                   | :x:                   |
| getBizAccountBill()           | :x:                   | :x:                   | :x:                   |
| placeMixOrder()               | :x:                   | :x:                   | :x:                   |
| placeMixReversal()            | :x:                   | :x:                   | :x:                   |
| placeMixBatchOrder()          | :x:                   | :x:                   | :x:                   |
| cancelMixOrder()              | :x:                   | :x:                   | :x:                   |
| batchCancelMixOrder()         | :x:                   | :x:                   | :x:                   |
| cancelAllMixOrders()          | :x:                   | :x:                   | :x:                   |
| getOpenMixOrder()             | :x:                   | :x:                   | :x:                   |
| getOpenMixOrders()            | :x:                   | :x:                   | :x:                   |
| getMixHistory()               | :x:                   | :x:                   | :x:                   |
| getProductTypeHistory()       | :x:                   | :x:                   | :x:                   |
| getMixOrderDetail()           | :x:                   | :x:                   | :x:                   |
| getMixOrderFills()            | :x:                   | :x:                   | :x:                   |
| getAllMixOrderFills()         | :x:                   | :x:                   | :x:                   |
| placeMixPlanOrder()           | :x:                   | :x:                   | :x:                   |
| modifyMixPlanOrder()          | :x:                   | :x:                   | :x:                   |
| modifyMixPlanOrderTPSL()      | :x:                   | :x:                   | :x:                   |
| placeMixStopOrder()           | :x:                   | :x:                   | :x:                   |
| placeMixTrailingStopOrder()   | :x:                   | :x:                   | :x:                   |
| placeMixPositionTPSL()        | :x:                   | :x:                   | :x:                   |
| modifyMixStopOrder()          | :x:                   | :x:                   | :x:                   |
| cancelMixPlanOrder()          | :x:                   | :x:                   | :x:                   |
| cancelMixPlanOrders()         | :x:                   | :x:                   | :x:                   |
| getMixPlanOrders()            | :x:                   | :x:                   | :x:                   |
| getMixHistoryPlanOrders()     | :x:                   | :x:                   | :x:                   |

#### WebSocket API

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
