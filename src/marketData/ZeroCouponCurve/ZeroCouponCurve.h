#pragma once

#include <map>
#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>

class IDayCount;

class ZeroCouponCurve {
public:
    using Date = boost::gregorian::date;

    // Curva definida por (T -> zc(T)), con interpolación lineal
    ZeroCouponCurve(Date refDate,
                    std::shared_ptr<const IDayCount> dayCount,
                    std::map<double, double> zcByTime);

    const Date& ref_date() const { return refDate_; }

    // t medido desde refDate usando daycount (lo usa Index y discounting)
    double time_from_ref(const Date& d) const;

    // ZC y DF
    double get_zc(double t) const;
    double get_zc(const Date& d) const { return get_zc(time_from_ref(d)); }

    // DF = exp(-zc(t)*t)
    double get_dcf(double t) const;
    double get_dcf(const Date& d) const { return get_dcf(time_from_ref(d)); }

private:
    Date refDate_;
    std::shared_ptr<const IDayCount> dc_;
    std::map<double, double> zc_;
};

