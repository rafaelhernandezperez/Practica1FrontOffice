#pragma once
class ZeroCouponCurve;

class Leg {
public:
    virtual ~Leg() = default;
    virtual double price(const ZeroCouponCurve& curve) const = 0;
};