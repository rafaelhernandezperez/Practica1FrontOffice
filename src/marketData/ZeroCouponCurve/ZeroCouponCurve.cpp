#include "ZeroCouponCurve.h"

#include <cmath>
#include <stdexcept>
#include <utils/DayCount/IDayCount.h>

ZeroCouponCurve::ZeroCouponCurve(Date refDate,
                                 std::shared_ptr<const IDayCount> dayCount,
                                 std::map<double, double> zcByTime)
: refDate_(refDate), dc_(std::move(dayCount)), zc_(std::move(zcByTime))
{
    if (!dc_) throw std::invalid_argument("ZeroCouponCurve: dayCount is null");
    if (zc_.empty()) throw std::invalid_argument("ZeroCouponCurve: curve is empty");
}

ZeroCouponCurve::ZeroCouponCurve(std::map<double, double> zcByTime)
: refDate_(boost::gregorian::date(1970, 1, 1)), dc_(nullptr), zc_(std::move(zcByTime))
{
    if (zc_.empty()) throw std::invalid_argument("ZeroCouponCurve: curve is empty");
}

double ZeroCouponCurve::time_from_ref(const Date& d) const
{
    if (!dc_) throw std::logic_error("ZeroCouponCurve: no dayCount set (use full constructor)");
    if (d < refDate_) throw std::invalid_argument("ZeroCouponCurve: date < refDate");
    return dc_->yearFraction(refDate_, d);
}

double ZeroCouponCurve::get_zc(double t) const
{
    if (t < 0.0) throw std::invalid_argument("ZeroCouponCurve::get_zc: t must be >= 0");

    // Exacto en un nodo
    auto it = zc_.find(t);
    if (it != zc_.end()) return it->second;

    // t = 0 exacto y no hay nodo en 0: devolver el primer tipo
    if (t == 0.0) return zc_.begin()->second;

    // Interpolación lineal entre nodos
    auto upper = zc_.upper_bound(t);
    if (upper == zc_.begin() || upper == zc_.end())
        throw std::out_of_range("ZeroCouponCurve::get_zc: t out of range");

    auto lower = std::prev(upper);

    const double t1 = lower->first, z1 = lower->second;
    const double t2 = upper->first, z2 = upper->second;
    const double w  = (t - t1) / (t2 - t1);
    return z1 + w * (z2 - z1);
}

double ZeroCouponCurve::get_dcf(double t) const
{
    if (t == 0.0) return 1.0;    
    const double z = get_zc(t);
    return std::exp(-z * t);
}