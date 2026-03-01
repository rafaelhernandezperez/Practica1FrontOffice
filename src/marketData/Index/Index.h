#pragma once

#include <memory>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

class ZeroCouponCurve;

class Index {
public:
    using Date = boost::gregorian::date;

    Index(const std::string& name, int comp_per_year);

    double get_rate(const ZeroCouponCurve& curve,
                    const Date& start,
                    const Date& end) const;

private:
    std::string name_;
    int m_;
};