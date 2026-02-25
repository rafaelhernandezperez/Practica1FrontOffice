#pragma once
#include "InstrumentBuilder.h"

class BondBuilder : public InstrumentBuilder {
public:
    std::unique_ptr<Instrument>
    build(const InstrumentDescription& desc) const override;
};