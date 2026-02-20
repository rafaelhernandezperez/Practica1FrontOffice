#include "Bond.h"
#include <stdexcept>
#include <cmath>
#include <ZeroCouponCurve/ZerocouponCurve.h>

Bond::Bond(double nominal, double annual_coupon_rate, double maturity_years, int coupons_per_year)
: nominal_(nominal), c_(annual_coupon_rate), T_(maturity_years), m_(coupons_per_year)
{
    if (nominal_ <= 0.0) throw std::invalid_argument("Bond: nominal must be > 0");
    if (c_ < 0.0) throw std::invalid_argument("Bond: coupon rate must be >= 0");
    if (T_ <= 0.0) throw std::invalid_argument("Bond: maturity must be > 0");
    if (m_ <= 0) throw std::invalid_argument("Bond: coupons_per_year must be > 0");
}

std::vector<double> Bond::payment_times() const
{
    std::vector<double> times;
    const double dt = 1.0 / static_cast<double>(m_);
    // asumimos que T es múltiplo exacto de dt (en el ejemplo: 2.0 y dt=0.5)
    const int n = static_cast<int>(std::round(T_ / dt));
    for (int i = 1; i <= n; ++i) {
        times.push_back(i * dt);
    }
    return times;
}

double Bond::price(const ZerocouponCurve& curve) const
{
    const double dt = 1.0 / static_cast<double>(m_);
    const int n = static_cast<int>(std::round(T_ / dt));
    const double coupon_amount = nominal_ * c_ * dt; // cupón por periodo

    double pv = 0.0;
    for (int i = 1; i <= n; ++i) {
        const double t = i * dt;
        pv += coupon_amount * curve.get_dcf(t);
    }
    pv += nominal_ * curve.get_dcf(T_);
    return pv;
}
