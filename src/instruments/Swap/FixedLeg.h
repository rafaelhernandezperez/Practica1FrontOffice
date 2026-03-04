#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "ILeg.h"

class IDayCount;
class ZeroCouponCurve;

class FixedLeg : public ILeg
{
public:
    using Date = boost::gregorian::date;

    FixedLeg(double nominal,
             double fixedRate,
             std::shared_ptr<const IDayCount> dayCount,
             std::vector<std::pair<Date, Date>> periods);

    double price(const ZeroCouponCurve& curve) const override;

private:
    double N_;
    double fixed_;
    std::shared_ptr<const IDayCount> dc_;
    std::vector<std::pair<Date, Date>> periods_; 
};