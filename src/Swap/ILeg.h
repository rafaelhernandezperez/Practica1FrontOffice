#pragma once
class ZeroCouponCurve;

class ILeg {
public:
    virtual ~ILeg() = default;
    virtual double price(const ZeroCouponCurve& curve) const = 0;
};