#pragma once
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

class DayCountCalculator
{
public:
    static boost::gregorian::date make_date(const std::string& date);
};

class Actual_360 : public DayCountCalculator
{
public:
    static short compute_daycount(const std::string& from, const std::string& to);
    static short compute_daycount(const boost::gregorian::date& from,
                                  const boost::gregorian::date& to);

    template<class DATE>
    double operator()(const DATE& start, const DATE& end) const
    {
        return compute_daycount(start, end) / 360.0;
    }
};

class Thirty_360 : public DayCountCalculator
{
public:
    static short compute_daycount(const std::string& from, const std::string& to);
    static short compute_daycount(const boost::gregorian::date& from,
                                  const boost::gregorian::date& to);
    static short compute_daycount(short years,
                                  short months,
                                  short days_from,
                                  short days_to);

    template<class DATE>
    double operator()(const DATE& start, const DATE& end) const
    {
        return compute_daycount(start, end) / 360.0;
    }
};
