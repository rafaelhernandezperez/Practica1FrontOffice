#include "FloatingLeg.h"
#include <ZeroCouponCurve/ZeroCouponCurve.h>
#include <cmath>

FloatingLeg::FloatingLeg(double nominal,
                         double maturity,
                         int payments_per_year)
: N_(nominal), T_(maturity), m_(payments_per_year)
{}

double FloatingLeg::pv(const ZeroCouponCurve& curve) const
{
    // El PV del floating leg (sin intercambio de nominal) es:
    //   sum_i [ N * (DF(t_{i-1})/DF(t_i) - 1) * DF(t_i) ]
    //   = sum_i [ N * (DF(t_{i-1}) - DF(t_i)) ]
    //   = N * (DF(0) - DF(T))
    //   = N * (1 - DF(T))          <- suma telescópica exacta
    return N_ * (1.0 - curve.get_dcf(T_));
}