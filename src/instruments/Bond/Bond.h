#pragma once
#include <vector>
#include <utility>
#include <marketData/ZeroCouponCurve/ZeroCouponCurve.h>
#include <instruments/Instrument/IInstrument.h>

class Bond : public IInstrument
{
public:
    Bond(double nominal,
         double annual_coupon_rate,   
         double maturity_years,       
         int coupons_per_year);      

    // PV = suma(cupones*DF) + nominal*DF(maturity)
    double price(const ZeroCouponCurve& curve) const override;

    // Para tests/debug
    std::vector<double> payment_times() const;

    std::vector<std::pair<double,double>> cashflows() const;

private:
    double nominal_;
    double c_;            
    double T_;            
    int m_;               
};
