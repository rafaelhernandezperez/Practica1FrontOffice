#pragma once
#include <memory>
#include "ILeg.h"
#include <instruments/Instrument/IInstrument.h>

class ZeroCouponCurve;

// Swap de tipo IRS (Interest Rate Swap).
// Convencion: el receptor recibe cupon fijo (FixedLeg) y paga flotante (FloatingLeg).
// NPV = PV(fixed) - PV(floating)
class Swap : public IInstrument
{
public:
    // Constructor principal: crea internamente FixedLeg y FloatingLeg.
    Swap(double nominal,
         double fixed_rate,        // tipo fijo anual (ej: 0.05)
         double maturity,          // vencimiento en anios (ej: 2.0)
         int    payments_per_year); // pagos por anio (ej: 2 = semestral)

    // IInstrument: precio del swap (NPV desde la perspectiva del receptor fijo)
    double price(const ZeroCouponCurve& curve) const override;

    // Tipo de equilibrio (par rate): tipo fijo que hace NPV = 0
    // par = (1 - DF(T)) / (dt * sum_i DF(t_i))
    double par_rate(const ZeroCouponCurve& curve) const;

private:
    std::unique_ptr<ILeg> fixed_;    // pata fija (receptor)
    std::unique_ptr<ILeg> floating_; // pata flotante (pagador)

    double N_;  // nominal
    double T_;  // vencimiento
    int    m_;  // pagos por anio
};
