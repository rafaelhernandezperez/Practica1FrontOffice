#include "BondBuilder.h"
#include <Bond/Bond.h>
#include "InstrumentDescription.h"
#include <stdexcept>

std::unique_ptr<Instrument>
BondBuilder::build(const InstrumentDescription& desc) const
{
    return std::make_unique<Bond>(
        desc.nominal,
        desc.coupon_rate,
        desc.maturity,
        desc.payments_per_year
    );
}