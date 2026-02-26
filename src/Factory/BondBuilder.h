#pragma once
#include "InstrumentBuilder.h"

class BondBuilder : public InstrumentBuilder {
public:
    std::unique_ptr<IInstrument>
    build(const InstrumentDescription& desc) const override;
};