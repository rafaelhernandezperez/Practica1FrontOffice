#pragma once

#include <map>
#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "IPricer.h"

class Bootstrapping {
public:
    Bootstrapping() = default;

    using date = boost::gregorian::date;

    using DiscountCurveType   = std::map<date, double>;
    using InstrumentsMapType  = std::map<date, std::shared_ptr<IPricer>>;

    DiscountCurveType operator()(const InstrumentsMapType& instruments) const;

    virtual ~Bootstrapping() = default;
};