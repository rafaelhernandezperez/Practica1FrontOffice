#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "ILeg.h"

class IDayCount;
class ZeroCouponCurve;
class Index;

class FloatingLeg : public ILeg
{
public:
    using Date = boost::gregorian::date;

    FloatingLeg(double nominal,
                std::shared_ptr<const IDayCount> dayCount,
                std::vector<std::pair<Date, Date>> periods,
                std::shared_ptr<const Index> index,
                bool hasFirstFixing = false,
                double firstFixing = 0.0,
                double spread = 0.0);

    double price(const ZeroCouponCurve& curve) const override;

private:
    double N_;
    double spread_;
    bool hasFirstFixing_;
    double firstFixing_;

    std::shared_ptr<const IDayCount> dc_;
    std::shared_ptr<const Index> index_;
    std::vector<std::pair<Date, Date>> periods_;
};