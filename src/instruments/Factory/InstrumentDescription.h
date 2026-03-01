#pragma once
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

struct InstrumentDescription
{
    enum class Type {
        Bond,
        Swap
    };

    Type type;

    // Comunes
    double nominal;
    double maturity;
    int payments_per_year;

    // Bond
    double coupon_rate;

    // Swap
    double fixed_rate;
    boost::gregorian::date start_date;
    bool has_first_fixing = false;
    double first_fixing = 0.0;
};