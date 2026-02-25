#pragma once
#include <memory>

class Instrument;
struct InstrumentDescription;

class InstrumentBuilder {
public:
    virtual ~InstrumentBuilder() = default;

    virtual std::unique_ptr<Instrument>
    build(const InstrumentDescription& desc) const = 0;
};