#pragma once
#include <memory>

class ZeroCouponCurve;
class Leg;

class Swap
{
public:
    Swap(std::unique_ptr<Leg> receiver,
         std::unique_ptr<Leg> payer);

    double price(const ZeroCouponCurve& curve) const;

private:
    std::unique_ptr<Leg> receiver_;
    std::unique_ptr<Leg> payer_;
};