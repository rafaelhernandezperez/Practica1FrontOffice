#pragma once

class ZeroCouponCurve;

class IInstrument {
public:
    virtual ~IInstrument() = default;
    virtual double price(const ZeroCouponCurve& curve) const = 0;
};