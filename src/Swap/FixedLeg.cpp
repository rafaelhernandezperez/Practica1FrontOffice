#include "FixedLeg.h"
#include <ZeroCouponCurve/ZeroCouponCurve.h>
#include <cmath>

FixedLeg::FixedLeg(double nominal,
                   double fixed_rate,
                   double maturity,
                   int payments_per_year)
: N_(nominal), r_(fixed_rate), T_(maturity), m_(payments_per_year)
{}

double FixedLeg::pv(const ZeroCouponCurve& curve) const
{
    const double dt = 1.0 / m_;
    const int n = static_cast<int>(std::round(T_ / dt));
    const double coupon_amount = N_ * r_ * dt;

    double pv = 0.0;
    for (int i = 1; i <= n; ++i) {
        const double t = i * dt;
        pv += coupon_amount * curve.get_dcf(t);
    }
    pv += N_ * curve.get_dcf(T_);
    return pv;
}
