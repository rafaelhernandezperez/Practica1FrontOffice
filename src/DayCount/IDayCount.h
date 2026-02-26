#pragma once
#include <boost/date_time/gregorian/gregorian.hpp>

class IDayCount {
public:
    virtual ~IDayCount() = default;

    virtual double yearFraction(const boost::gregorian::date& from,
                                const boost::gregorian::date& to) const = 0;
};