#include "Swap.h"
#include "Leg.h"

Swap::Swap(std::unique_ptr<Leg> receiver,
           std::unique_ptr<Leg> payer)
: receiver_(std::move(receiver)),
  payer_(std::move(payer))
{}

double Swap::price(const ZeroCouponCurve& curve) const
{
    return receiver_->price(curve)
         - payer_->price(curve);
}