#include "daycount.h"
#include <algorithm>

using boost::gregorian::date;
using boost::gregorian::from_string;

boost::gregorian::date DayCountCalculator::make_date(const std::string& dateStr)
{
    return from_string(dateStr);
}

double Actual360::yearFraction(const date& from, const date& to) const
{
    return static_cast<double>(compute_daycount(from, to)) / 360.0;
}

long Actual360::compute_daycount(const std::string& from, const std::string& to)
{
    return compute_daycount(from_string(from), from_string(to));
}

long Actual360::compute_daycount(const date& from, const date& to)
{
    return (to - from).days();
}

double Thirty360::yearFraction(const date& from, const date& to) const
{
    return static_cast<double>(compute_daycount(from, to)) / 360.0;
}

long Thirty360::compute_daycount(const std::string& from, const std::string& to)
{
    return compute_daycount(from_string(from), from_string(to));
}

long Thirty360::compute_daycount(const date& from, const date& to)
{
    auto from_date = from.year_month_day();
    auto to_date   = to.year_month_day();

    auto years  = static_cast<long>(to_date.year  - from_date.year);
    auto months = static_cast<long>(to_date.month - from_date.month);

    return compute_daycount(years, months,
                            static_cast<long>(from_date.day),
                            static_cast<long>(to_date.day));
}

long Thirty360::compute_daycount(long years,
                                 long months,
                                 long days_from,
                                 long days_to)
{
    return (360 * years) + 30 * (months - 1) +
           std::max<long>(0, 30 - days_from) +
           std::min<long>(30, days_to);
}


std::unique_ptr<IDayCount> DayCountFactory::create(DayCountType type)
{
    switch (type) {
        case DayCountType::Actual360:
            return std::make_unique<Actual360>();
        case DayCountType::Thirty360:
            return std::make_unique<Thirty360>();
        default:
            throw std::invalid_argument("Unsupported DayCountType");
    }
}