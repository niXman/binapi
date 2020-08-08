# binapi
Binance API implemented in C++

# Motivation
This implementation has been developed as a consequence of the lack of suitable alternatives as part of my multiuser trading platform project.

# REST API
- ping
- time
- exchange info
- account info
- depth
- trades
- agg trades
- 24hr ticker
- price
- order put
- order query
- order test put
- order test query
- order cancel
- open orders query
- all orders query
- my trades
- user-data-stream new
- user-data-stream ping
- user-data-stream close

# WebSocket API
- depth
- klines
- trades
- agg trades
- ticker
- all market tickers
- userdata

# Implementation details
The project is written using C++14 and [boost](https://www.boost.org/). [boost.beast](https://www.boost.org/doc/libs/1_73_0/libs/beast/index.html) is used to interact with the network.
