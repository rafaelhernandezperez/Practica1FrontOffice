#include "FloatingLeg.h"
#include <utils/DayCount/IDayCount.h>
#include <marketData/ZeroCouponCurve/ZeroCouponCurve.h>
#include <marketData/Index/Index.h>
#include <stdexcept>

FloatingLeg::FloatingLeg(double nominal,
                         std::shared_ptr<const IDayCount> dayCount,
                         std::vector<std::pair<Date, Date>> periods,
                         std::shared_ptr<const Index> index,
                         bool hasFirstFixing,
                         double firstFixing,
                         double spread)
: N_(nominal),
  spread_(spread),
  hasFirstFixing_(hasFirstFixing),
  firstFixing_(firstFixing),
  dc_(std::move(dayCount)),
  index_(std::move(index)),
  periods_(std::move(periods))
{
    if (!dc_) throw std::invalid_argument("FloatingLeg: dayCount null");
    if (!index_) throw std::invalid_argument("FloatingLeg: index null");
    if (periods_.empty()) throw std::invalid_argument("FloatingLeg: empty periods");
}

double FloatingLeg::price(const ZeroCouponCurve& curve) const
{
    double pv = 0.0;

    for (size_t i = 0; i < periods_.size(); ++i)
    {
        const Date& start = periods_[i].first;
        const Date& end   = periods_[i].second;

        const double alpha = dc_->yearFraction(start, end);

        double r = 0.0;

        if (i == 0 && hasFirstFixing_)
            r = firstFixing_;
        else
            r = index_->get_rate(curve, start, end);

        r += spread_;

        const double cf = N_ * r * alpha;
        pv += cf * curve.get_dcf(end);
    }

    return pv;
}