#define BOOST_TEST_MODULE test_daycount
#include <boost/test/unit_test.hpp>

#include <memory>
#include "daycount.h"

BOOST_AUTO_TEST_SUITE(daycount_suite)


// Actual/360
BOOST_AUTO_TEST_CASE(actual_360_string_dates)
{
    BOOST_TEST_MESSAGE("Testing Actual/360 with string dates");

    Actual360 act360;

    auto yf = act360("2016-04-01", "2016-10-03");

    BOOST_TEST_MESSAGE(" - year fraction: " << yf);

    BOOST_TEST(yf == 185.0/360.0,
               boost::test_tools::tolerance(1e-12));
}

// 30/360
BOOST_AUTO_TEST_CASE(thirty_360_string_dates)
{
    BOOST_TEST_MESSAGE("Testing 30/360 with string dates");

    Thirty360 th360;

    auto yf = th360("2016-04-01", "2016-10-03");

    BOOST_TEST_MESSAGE(" - year fraction: " << yf);

    BOOST_TEST(yf == 182.0/360.0,
               boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_CASE(actual_360_date_objects)
{
    using boost::gregorian::date;

    BOOST_TEST_MESSAGE("Testing Actual/360 with date objects");

    Actual360 act360;

    date d1(2016, 4, 1);
    date d2(2016, 10, 3);

    auto yf = act360(d1, d2);

    BOOST_TEST_MESSAGE(" - year fraction: " << yf);

    BOOST_TEST(yf == 185.0/360.0,
               boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_CASE(strategy_interface_actual360)
{
    using boost::gregorian::date;

    std::unique_ptr<IDayCount> dc = DayCountFactory::create(DayCountType::Actual360);

    double yf = dc->yearFraction(date(2016,4,1), date(2016,10,3));

    BOOST_TEST(yf == 185.0/360.0,
               boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_CASE(strategy_interface_thirty360)
{
    using boost::gregorian::date;

    std::unique_ptr<IDayCount> dc = DayCountFactory::create(DayCountType::Thirty360);

    double yf = dc->yearFraction(date(2016,4,1), date(2016,10,3));

    BOOST_TEST(yf == 182.0/360.0,
               boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_CASE(zero_length_period)
{
    using boost::gregorian::date;

    Actual360 act360;
    Thirty360 th360;

    date d(2016,4,1);

    BOOST_TEST(act360.yearFraction(d, d) == 0.0,
               boost::test_tools::tolerance(1e-12));
    BOOST_TEST(th360.yearFraction(d, d) == 0.0,
               boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_SUITE_END()