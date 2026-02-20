#pragma once
#include <map>
#include <stdexcept>

class ZeroCouponCurve
{
public:
    // Guarda puntos (T -> zc) donde zc está en tanto por uno (ej: 0.05)
    explicit ZeroCouponCurve(const std::map<double, double>& zc_by_maturity);

    // Tipo cero cupón (interpolación lineal entre puntos)
    double get_zc(double t) const;

    // Discount factor continuo: DF(0,t) = exp(-t * zc(t))
    double get_dcf(double t) const;

private:
    std::map<double, double> zc_;
};

