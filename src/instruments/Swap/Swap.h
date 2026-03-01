#pragma once
#include <memory>
#include <instruments/Instrument/IInstrument.h>
#include <instruments/Swap/ILeg.h>

class ZeroCouponCurve;

// Convención para replicar el Excel:
// Valor Swap = PV(pata flotante) - PV(pata fija)
class Swap : public IInstrument
{
public:
    Swap(std::unique_ptr<ILeg> fixedLeg,
         std::unique_ptr<ILeg> floatLeg);

    double price(const ZeroCouponCurve& curve) const override;

private:
    std::unique_ptr<ILeg> fixed_;
    std::unique_ptr<ILeg> float_;
};