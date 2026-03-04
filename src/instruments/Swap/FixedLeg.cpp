#include "FixedLeg.h"
#include <utils/DayCount/IDayCount.h>
#include <marketData/ZeroCouponCurve/ZeroCouponCurve.h>
#include <stdexcept>

FixedLeg::FixedLeg(double nominal,
                   double fixedRate,
                   std::shared_ptr<const IDayCount> dayCount,
                   std::vector<std::pair<Date, Date>> periods)
: N_(nominal), fixed_(fixedRate), dc_(std::move(dayCount)), periods_(std::move(periods))
{
    if (!dc_) throw std::invalid_argument("FixedLeg: dayCount is null");
    if (periods_.empty()) throw std::invalid_argument("FixedLeg: empty periods");
}

double FixedLeg::price(const ZeroCouponCurve& curve) const
{
    double pv = 0.0;
    for (const auto& p : periods_) {
        const Date& start = p.first;
        const Date& end   = p.second;

        const double alpha = dc_->yearFraction(start, end);
        const double cf    = N_ * fixed_ * alpha;
        pv += cf * curve.get_dcf(end); 
    }
    return pv;
}
