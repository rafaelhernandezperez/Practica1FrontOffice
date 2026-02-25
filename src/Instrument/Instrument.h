#pragma once

class ZeroCouponCurve;

class Instrument {
public:
    virtual ~Instrument() = default;
    virtual double price(const ZeroCouponCurve& curve) const = 0;
};