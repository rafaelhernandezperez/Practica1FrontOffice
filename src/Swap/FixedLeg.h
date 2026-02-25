#pragma once
#include <vector>
#include "Leg.h"

class ZeroCouponCurve;

class FixedLeg : public Leg
{
public:
    FixedLeg(double nominal,
             double fixed_rate,   // ej: 0.05
             double maturity,
             int payments_per_year);

    double price(const ZeroCouponCurve& curve) const;

private:
    double N_;
    double r_;
    double T_;
    int m_;
};