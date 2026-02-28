#include "Index.h"

#include <cmath>
#include <stdexcept>
#include <ZeroCouponCurve/ZeroCouponCurve.h>

Index::Index(std::string name,
             std::shared_ptr<const ZeroCouponCurve> curve,
             int compoundingPerYear)
: name_(std::move(name)), curve_(std::move(curve)), m_(compoundingPerYear)
{
    if (!curve_) throw std::invalid_argument("Index: curve is null");
    if (m_ <= 0) throw std::invalid_argument("Index: m must be > 0");
}

double Index::get_rate(const Date& start, const Date& end) const
{
    if (end <= start) throw std::invalid_argument("Index::get_rate: end <= start");

    // T1,T2 desde la refDate de la curva
    const double T1 = curve_->time_from_ref(start);
    const double T2 = curve_->time_from_ref(end);
    if (T2 <= T1) throw std::invalid_argument("Index::get_rate: invalid times");

    const double z1 = curve_->get_zc(T1);
    const double z2 = curve_->get_zc(T2);

    // (como en clase) forward continuamente compuesto:
    // Rc = (z2*T2 - z1*T1) / (T2 - T1)
    const double Rc = (z2 * T2 - z1 * T1) / (T2 - T1);

    // conversión a capitalización m veces al año:
    // Rm = m*(exp(Rc/m) - 1)
    const double m = static_cast<double>(m_);
    return m * (std::exp(Rc / m) - 1.0);
}