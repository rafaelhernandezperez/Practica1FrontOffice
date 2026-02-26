#include "ZeroCouponCurve.h"
#include <cmath>

ZeroCouponCurve::ZeroCouponCurve(const std::map<double, double>& zc_by_maturity)
: zc_(zc_by_maturity)
{
    if (zc_.empty())
        throw std::invalid_argument("ZeroCouponCurve: empty curve");
}

double ZeroCouponCurve::get_zc(double t) const
{
    if (t <= 0.0)
        throw std::invalid_argument("ZeroCouponCurve::get_zc: t must be > 0");

    // Exact match
    auto it = zc_.find(t);
    if (it != zc_.end())
        return it->second;

    // Interpolar: buscamos el primer punto > t (upper)
    auto upper = zc_.upper_bound(t);

    // Si t está fuera del rango, por simplicidad: error (más adelante podrías extrapolar)
    if (upper == zc_.begin() || upper == zc_.end())
        throw std::out_of_range("ZeroCouponCurve::get_zc: t out of curve range");

    auto lower = std::prev(upper);

    const double t1 = lower->first;
    const double z1 = lower->second;
    const double t2 = upper->first;
    const double z2 = upper->second;

    // Interpolación lineal
    const double w = (t - t1) / (t2 - t1);
    return z1 + w * (z2 - z1);
}

double ZeroCouponCurve::get_dcf(double t) const
{
    const double zc = get_zc(t);
    return std::exp(-zc * t);
}

