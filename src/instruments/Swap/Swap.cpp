#include "Swap.h"
#include "ILeg.h"
#include <marketData/ZeroCouponCurve/ZeroCouponCurve.h>

Swap::Swap(std::unique_ptr<ILeg> fixedLeg,
           std::unique_ptr<ILeg> floatLeg)
: fixed_(std::move(fixedLeg)), float_(std::move(floatLeg))
{}

double Swap::price(const ZeroCouponCurve& curve) const
{
    return float_->price(curve) - fixed_->price(curve);
}
