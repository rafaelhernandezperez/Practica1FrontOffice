#pragma once

class IPricer {
public:
    virtual ~IPricer() = default;
    virtual double computeDiscountFactor(double& annuity) const = 0;
};