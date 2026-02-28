#pragma once

#include <string>
#include <memory>
#include <stdexcept>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "IDayCount.h"

class DayCountCalculator
{
public:
    static boost::gregorian::date make_date(const std::string& date);
};


class Actual360 final : public IDayCount, public DayCountCalculator
{
public:
    double yearFraction(const boost::gregorian::date& from,
                        const boost::gregorian::date& to) const override;

    static long compute_daycount(const std::string& from, const std::string& to);
    static long compute_daycount(const boost::gregorian::date& from,
                                 const boost::gregorian::date& to);

    template<class DATE>
    double operator()(const DATE& start, const DATE& end) const
    {
        return compute_daycount(start, end) / 360.0;
    }
};


class Thirty360 final : public IDayCount, public DayCountCalculator
{
public:
    double yearFraction(const boost::gregorian::date& from,
                        const boost::gregorian::date& to) const override;

    static long compute_daycount(const std::string& from, const std::string& to);
    static long compute_daycount(const boost::gregorian::date& from,
                                 const boost::gregorian::date& to);
    static long compute_daycount(long years,
                                 long months,
                                 long days_from,
                                 long days_to);

    template<class DATE>
    double operator()(const DATE& start, const DATE& end) const
    {
        return compute_daycount(start, end) / 360.0;
    }
};

enum class DayCountType {
    Actual360,
    Thirty360
};

class DayCountFactory
{
public:
    static std::unique_ptr<IDayCount> create(DayCountType type);
};