#include "SwapBuilder.h"
#include <Swap/Swap.h>
#include "InstrumentDescription.h"

std::unique_ptr<IInstrument>
SwapBuilder::build(const InstrumentDescription& desc) const
{
    // Usa el constructor de conveniencia de Swap que crea FixedLeg y FloatingLeg internamente.
    return std::make_unique<Swap>(
        desc.nominal,
        desc.fixed_rate,
        desc.maturity,
        desc.payments_per_year
    );
}
