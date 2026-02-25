#include "InstrumentFactory.h"
#include <stdexcept>

InstrumentFactory& InstrumentFactory::instance()
{
    static InstrumentFactory factory;
    return factory;
}

void InstrumentFactory::register_builder(
        InstrumentDescription::Type type,
        std::unique_ptr<InstrumentBuilder> builder)
{
    builders_[type] = std::move(builder);
}

std::unique_ptr<Instrument>
InstrumentFactory::create(const InstrumentDescription& desc) const
{
    auto it = builders_.find(desc.type);
    if (it == builders_.end())
        throw std::runtime_error("No builder registered");

    return it->second->build(desc);
}