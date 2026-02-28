#pragma once

#include <memory>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

class ZeroCouponCurve;

// Índice flotante: calcula tipo del periodo (start,end) a partir de ZC
class Index {
public:
    using Date = boost::gregorian::date;

    // m=2 para 6M (como en clase)
    Index(std::string name,
          std::shared_ptr<const ZeroCouponCurve> curve,
          int compoundingPerYear = 2);

    double get_rate(const Date& start, const Date& end) const;

private:
    std::string name_;
    std::shared_ptr<const ZeroCouponCurve> curve_;
    int m_;
};