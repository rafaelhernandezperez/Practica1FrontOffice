#pragma once
#include <memory>
#include <instruments/Instrument/IInstrument.h>

struct InstrumentDescription;

// Abstract builder: cada subclase sabe construir un tipo concreto de IInstrument.
class InstrumentBuilder {
public:
    virtual ~InstrumentBuilder() = default;

    virtual std::unique_ptr<IInstrument>
    build(const InstrumentDescription& desc) const = 0;
};
