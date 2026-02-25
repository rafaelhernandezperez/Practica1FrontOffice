#pragma once
#include <ZeroCouponCurve/ZeroCouponCurve.h>
#include "Leg.h"

class FloatingLeg : public Leg
{
public:
    FloatingLeg(double nominal,
                double maturity,
                int payments_per_year);

    double price(const ZeroCouponCurve& curve) const;

private:
    double N_;
    double T_;
    int m_;
};