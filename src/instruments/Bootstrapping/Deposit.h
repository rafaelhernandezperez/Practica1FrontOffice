#pragma once
#include "IPricer.h"
#include <boost/date_time/gregorian/gregorian.hpp>

class Deposit : public IPricer {
public:
    using date = boost::gregorian::date;

    Deposit(const date& t0, const date& t1, double r);

    double computeDiscountFactor(double& annuity) const override;

private:
    date t0_, t1_;
    double r_;
};