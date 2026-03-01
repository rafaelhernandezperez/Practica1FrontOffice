#include "Index.h"
#include <marketData/ZeroCouponCurve/ZeroCouponCurve.h>
#include <cmath>

Index::Index(const std::string& name, int comp_per_year)
: name_(name), m_(comp_per_year)
{}

double Index::get_rate(const ZeroCouponCurve& curve,
                       const Date& start,
                       const Date& end) const
{
    const double T1 = curve.time_from_ref(start);
    const double T2 = curve.time_from_ref(end);

    const double z1 = curve.get_zc(T1);
    const double z2 = curve.get_zc(T2);

    // forward continuo
    const double f_cont = (z2 * T2 - z1 * T1) / (T2 - T1);

    // convertir a tasa simple m-veces por año
    return m_ * (std::exp(f_cont / m_) - 1.0);
}