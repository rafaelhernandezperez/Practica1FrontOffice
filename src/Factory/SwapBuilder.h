#pragma once
#include "InstrumentBuilder.h"

class SwapBuilder : public InstrumentBuilder {
public:
    std::unique_ptr<Instrument>
    build(const InstrumentDescription& desc) const override;
};