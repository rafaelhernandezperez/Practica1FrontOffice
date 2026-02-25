#include "Swap.h"
#include "FixedLeg.h"
#include "FloatingLeg.h"
#include <GoalSeeker/GoalSeeker.h>
#include <iostream>

Swap::Swap(double nominal,
           double fixed_rate,
           double maturity,
           int payments_per_year)
: N_(nominal), r_(fixed_rate), T_(maturity), m_(payments_per_year)
{}

double Swap::price(const ZeroCouponCurve& curve) const
{
    FixedLeg fixed(N_, r_, T_, m_);
    FloatingLeg floating(N_, T_, m_);

    return floating.price(curve) - fixed.price(curve);
}

double Swap::par_rate(const ZeroCouponCurve& curve) const
{
    auto f = [&](double r){

        FixedLeg fixed(N_, r, T_, m_);
        FloatingLeg floating(N_, T_, m_);

        return fixed.price(curve) - floating.price(curve);
    };

    auto res = GoalSeeker::solve(f, 0.05);

    return res.x;
}