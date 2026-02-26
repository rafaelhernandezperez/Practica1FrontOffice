#pragma once
#include "InstrumentBuilder.h"

class SwapBuilder : public InstrumentBuilder {
public:
    std::unique_ptr<IInstrument>
    build(const InstrumentDescription& desc) const override;
};