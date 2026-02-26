#include "Swap.h"
#include "FixedLeg.h"
#include "FloatingLeg.h"
#include <ZeroCouponCurve/ZeroCouponCurve.h>
#include <cmath>

Swap::Swap(double nominal,
           double fixed_rate,
           double maturity,
           int    payments_per_year)
: N_(nominal), T_(maturity), m_(payments_per_year)
{
    fixed_    = std::make_unique<FixedLeg>(nominal, fixed_rate, maturity, payments_per_year);
    floating_ = std::make_unique<FloatingLeg>(nominal, maturity, payments_per_year);
}

double Swap::price(const ZeroCouponCurve& curve) const
{
    // NPV desde la perspectiva del receptor fijo:
    //   valor = PV(pata fija) - PV(pata flotante)
    return fixed_->price(curve) - floating_->price(curve);
}

double Swap::par_rate(const ZeroCouponCurve& curve) const
{
    // El par rate es el tipo fijo c* que hace NPV = 0:
    //   c* * N * dt * sum(DF(t_i)) = N * (1 - DF(T))
    //   c* = (1 - DF(T)) / (dt * sum(DF(t_i)))

    const double dt = 1.0 / static_cast<double>(m_);
    const int    n  = static_cast<int>(std::round(T_ / dt));

    double annuity = 0.0;
    for (int i = 1; i <= n; ++i)
        annuity += curve.get_dcf(i * dt);
    annuity *= dt;

    return (1.0 - curve.get_dcf(T_)) / annuity;
}
