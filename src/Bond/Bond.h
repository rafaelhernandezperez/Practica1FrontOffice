#pragma once
#include <vector>
#include <utility>
#include <ZeroCouponCurve/ZeroCouponCurve.h>
#include <Instrument/IInstrument.h>

class Bond : public IInstrument
{
public:
    Bond(double nominal,
         double annual_coupon_rate,   // ej: 0.06
         double maturity_years,       // ej: 2.0
         int coupons_per_year);       // ej: 2 (semestral)

    // PV = suma(cupones*DF) + nominal*DF(maturity)
    double price(const ZeroCouponCurve& curve) const override;

    // Para tests/debug
    std::vector<double> payment_times() const;

    std::vector<std::pair<double,double>> cashflows() const;

private:
    double nominal_;
    double c_;            // cupón anual en tanto por uno
    double T_;            // vencimiento en años
    int m_;               // pagos por año
};
