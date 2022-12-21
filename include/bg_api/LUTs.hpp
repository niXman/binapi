#ifndef LUTS_H
#define LUTS_H
#include "double_type.hpp"

namespace bg_api {

    struct spot_coin_info {
        const char* symbol;
        const char* symbolName;
        const char* baseCoin;
        const char* quoteCoin;
        double_type minTradeAmount;
        double_type maxTradeAmount;
        double_type takerFeeRate;
        double_type makerFeeRate;
        unsigned int priceScale;
        unsigned int quantityScale;
        double_type minTradeUSDT;
    };

    int getSpotSymbolIndex(const char* const sym);
    int getSpotSymbolIndex(const char* const base, const char* const quote);

    double_type getSpotMinTradeAmt(const char* const sym);
    double_type getSpotMinTradeAmt(const char* const base, const char* const quote);

    double_type getSpotMaxTradeAmt(const char* const sym);
    double_type getSpotMaxTradeAmt(const char* const base, const char* const quote);

    double_type getSpotTakerFeeRate(const char* const sym);
    double_type getSpotTakerFeeRate(const char* const base, const char* const quote);

    double_type getSpotMakerFeeRate(const char* const sym);
    double_type getSpotMakerFeeRate(const char* const base, const char* const quote);

    unsigned int getSpotPriceScale(const char* const sym);
    unsigned int getSpotPriceScale(const char* const base, const char* const quote);

    unsigned int getSpotQtyScale(const char* const sym);
    unsigned int getSpotQtyScale(const char* const base, const char* const quote);

    double_type getSpotMinTradeUSDT(const char* const sym);
    double_type getSpotMinTradeUSDT(const char* const base, const char* const quote);
    
    struct futures_coin_info {
        const char* symbol;
        double_type makerFeeRate;
        double_type takerFeeRate;
        double_type feeRateUpRatio;
        double_type openCostUpRatio;
        double_type buyLimitPriceRatio;
        double_type sellLimitPriceRatio;
        double_type minTradeNum;
        unsigned int priceEndStep;
        unsigned int volumePlace;
        unsigned int pricePlace;
        double_type sizeMultiplier;
    };

    double_type getFuturesMakerFeeRate(const char* const sym);
    double_type getFuturesTakerFeeRate(const char* const sym);
    double_type getFuturesFeeRateUpRatio(const char* const sym);
    double_type getFuturesOpenCostUpRatio(const char* const sym);
    double_type getFuturesBuyLimitPriceRatio(const char* const sym);
    double_type getFuturesSellLimitPriceRatio(const char* const sym);
    double_type getFuturesMinTradeNum(const char* const sym);
    unsigned int getFuturesPriceEndStep(const char* const sym);
    unsigned int getFuturesVolumePlace(const char* const sym);
    unsigned int getFuturesPricePlace(const char* const sym);
    double_type getFuturesSizeMultiplier(const char* const sym);
}

#endif