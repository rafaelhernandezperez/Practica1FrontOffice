#include "BondBuilder.h"
#include <Bond/Bond.h>
#include "InstrumentDescription.h"

std::unique_ptr<IInstrument>
BondBuilder::build(const InstrumentDescription& desc) const
{
    return std::make_unique<Bond>(
        desc.nominal,
        desc.coupon_rate,
        desc.maturity,
        desc.payments_per_year
    );
}
