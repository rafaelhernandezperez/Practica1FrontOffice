#include "SwapQuote.h"

SwapQuote::SwapQuote(const date& prev, const date& end, double S)
: prev_(prev), end_(end), S_(S)
{}

double SwapQuote::computeDiscountFactor(double& annuity) const
{
    // ACT/360 para el último periodo (semiannual en el ejercicio)
    const double s_last = (end_ - prev_).days() / 360.0;

    // DF_n = (1 - S * A_{n-1}) / (1 + S * s_last)
    const double df = (1.0 - S_ * annuity) / (1.0 + S_ * s_last);

    // actualizar annuity: A_n = A_{n-1} + s_last * DF_n
    annuity += s_last * df;

    return df;
}