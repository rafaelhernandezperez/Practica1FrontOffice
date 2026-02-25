#pragma once
#include <ZeroCouponCurve/ZeroCouponCurve.h>

class FloatingLeg
{
public:
    FloatingLeg(double nominal,
                double maturity,
                int payments_per_year);

    double pv(const ZeroCouponCurve& curve) const;

private:
    double N_;
    double T_;
    int m_;
};