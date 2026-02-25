#include "SwapBuilder.h"
#include <Swap/FixedLeg.h>
#include <Swap/FloatingLeg.h>
#include <Swap/Swap.h>
#include "InstrumentDescription.h"
#include <stdexcept>

std::unique_ptr<Instrument>
SwapBuilder::build(const InstrumentDescription& desc) const
{
    auto fixed = std::make_unique<FixedLeg>(
        desc.nominal,
        desc.fixed_rate,
        desc.maturity,
        desc.payments_per_year
    );

    auto floating = std::make_unique<FloatingLeg>(
        desc.nominal,
        desc.maturity,
        desc.payments_per_year
    );

    return std::make_unique<Swap>(
        std::move(floating),
        std::move(fixed)
    );
}