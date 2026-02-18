#include "daycount.h"
#include <algorithm>

boost::gregorian::date DayCountCalculator::make_date(const std::string& date)
{
    return boost::gregorian::from_string(date);
}

short Actual_360::compute_daycount(const std::string& from, const std::string& to)
{
    boost::gregorian::date from_date(boost::gregorian::from_string(from));
    boost::gregorian::date to_date(boost::gregorian::from_string(to));
    return compute_daycount(from_date, to_date);
}

short Actual_360::compute_daycount(const boost::gregorian::date& from,
                                   const boost::gregorian::date& to)
{
    return (to - from).days();
}

short Thirty_360::compute_daycount(const std::string& from, const std::string& to)
{
    auto from_date(boost::gregorian::date(boost::gregorian::from_string(from)));
    auto to_date(boost::gregorian::date(boost::gregorian::from_string(to)));
    return compute_daycount(from_date, to_date);
}

short Thirty_360::compute_daycount(const boost::gregorian::date& from,
                                   const boost::gregorian::date& to)
{
    auto from_date(from.year_month_day());
    auto to_date(to.year_month_day());
    auto years  = static_cast<short>(to_date.year  - from_date.year);
    auto months = static_cast<short>(to_date.month - from_date.month);
    return compute_daycount(years, months,
                            static_cast<short>(from_date.day),
                            static_cast<short>(to_date.day));
}

short Thirty_360::compute_daycount(short years,
                                   short months,
                                   short days_from,
                                   short days_to)
{
    return static_cast<short>(
        (360 * years) + 30 * (months - 1) +
        std::max<short>(0, static_cast<short>(30 - days_from)) +
        std::min<short>(30, days_to)
    );
}
