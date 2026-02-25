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
    const double dt = 1.0 / m_;
    const int n = static_cast<int>(std::round(T_ / dt));

    double pv = 0.0;

    for (int i = 1; i <= n; ++i) {

        const double t1 = (i-1) * dt;
        const double t2 = i * dt;

        const double zc1 = (t1 == 0.0) ? curve.get_zc(dt) : curve.get_zc(t1);
        const double zc2 = curve.get_zc(t2);

        // Forward continuo
        const double rf =
            (zc2 * t2 - zc1 * t1) / (t2 - t1);

        // Convertir a tasa del periodo
        const double r_period =
            m_ * (std::exp(rf / m_) - 1.0);

        const double flow = N_ * r_period * dt;

        pv += flow * curve.get_dcf(t2);
    }

    // Para un floating leg con par bonds, el PV es simplemente:
    // N * (1 - DF(T)) = N - N * DF(T)
    // Esto es porque en par, cada cupón es par_rate * period * N
    // y los flujos descontados equivalen a N minus el VF descontado
    
    //return N_ * (1.0 - curve.get_dcf(T_));

    return pv;
}