#include "Deposit.h"

Deposit::Deposit(const date& t0, const date& t1, double r)
: t0_(t0), t1_(t1), r_(r)
{}

double Deposit::computeDiscountFactor(double& annuity) const
{
    // ACT/360
    const double b = (t1_ - t0_).days() / 360.0;

    // DF = 1 / (1 + r * b)  (depósito simple)
    const double df = 1.0 / (1.0 + r_ * b);

    // actualizar la annuity para el siguiente instrumento
    annuity += b * df;

    return df;
}