#pragma once

class ZeroCouponCurve;

class Swap
{
public:
    Swap(double nominal,
         double fixed_rate,
         double maturity,
         int payments_per_year);

    double pv(const ZeroCouponCurve& curve) const;

    // Par rate (fixed que hace PV=0)
    double par_rate(const ZeroCouponCurve& curve) const;

private:
    double N_;
    double r_;
    double T_;
    int m_;
};