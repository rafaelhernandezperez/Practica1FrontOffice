#include "SwapBuilder.h"
#include "InstrumentDescription.h"

#include <instruments/Swap/Swap.h>
#include <instruments/Swap/FixedLeg.h>
#include <instruments/Swap/FloatingLeg.h>

#include <marketData/Index/Index.h>
#include <utils/DayCount/daycount.h>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <vector>
#include <memory>

std::unique_ptr<IInstrument>
SwapBuilder::build(const InstrumentDescription& desc) const
{
    using boost::gregorian::months;
    using Date = boost::gregorian::date;

    auto dc = std::make_shared<Actual360>();

    const int monthsStep = 12 / desc.payments_per_year;
    const int totalMonths = static_cast<int>(desc.maturity * 12 + 0.5);

    Date start = desc.start_date;
    Date end   = desc.start_date;
    Date finalDate = desc.start_date + months(totalMonths);

    std::vector<std::pair<Date,Date>> periods;

    while (end < finalDate)
    {
        Date next = end + months(monthsStep);
        if (next > finalDate)
            next = finalDate;

        periods.emplace_back(end, next);
        end = next;
    }

    auto index = std::make_shared<Index>("Euribor", desc.payments_per_year);

    auto fixedLeg = std::make_unique<FixedLeg>(
        desc.nominal,
        desc.fixed_rate,
        dc,
        periods
    );

    auto floatLeg = std::make_unique<FloatingLeg>(
        desc.nominal,
        dc,
        periods,
        index,
        desc.has_first_fixing,
        desc.first_fixing
    );

    return std::make_unique<Swap>(
        std::move(fixedLeg),
        std::move(floatLeg)
    );
}
