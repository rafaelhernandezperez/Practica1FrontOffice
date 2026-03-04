#pragma once
#include "IPricer.h"
#include <boost/date_time/gregorian/gregorian.hpp>

class SwapQuote : public IPricer {
public:
    using date = boost::gregorian::date;

    SwapQuote(const date& prev, const date& end, double S);

    double computeDiscountFactor(double& annuity) const override;

private:
    date prev_, end_;
    double S_;
};