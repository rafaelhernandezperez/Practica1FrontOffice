#pragma once
#include <string>

struct InstrumentDescription
{
    enum class Type {
        Bond,
        Swap
    };

    Type type;

    // Campos comunes
    double nominal;
    double maturity;
    int payments_per_year;

    // Solo para bond
    double coupon_rate;

    // Solo para swap
    double fixed_rate;
};