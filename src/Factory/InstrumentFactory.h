#pragma once
#include <map>
#include <memory>

#include "InstrumentDescription.h"
#include "InstrumentBuilder.h"

class InstrumentFactory
{
public:
    static InstrumentFactory& instance();

    void register_builder(InstrumentDescription::Type type,
                          std::unique_ptr<InstrumentBuilder> builder);

    std::unique_ptr<Instrument>
    create(const InstrumentDescription& desc) const;

private:
    std::map<InstrumentDescription::Type,
             std::unique_ptr<InstrumentBuilder>> builders_;
};