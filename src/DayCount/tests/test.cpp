#define BOOST_TEST_MODULE test_daycount
#include <boost/test/unit_test.hpp>

#include <DayCount/daycount.h> 

BOOST_AUTO_TEST_SUITE(daycount_suite)

// ------------------------------------------------------------
// Actual/360
// ------------------------------------------------------------
BOOST_AUTO_TEST_CASE(actual_360_string_dates)
{
    BOOST_TEST_MESSAGE("Testing Actual/360 with string dates");

    Actual_360 act360;

    auto yf = act360("2016-04-01", "2016-10-01");

    BOOST_TEST_MESSAGE(" - year fraction: " << yf);

    // 183 días / 360
    BOOST_TEST(yf == 183.0/360.0,
               boost::test_tools::tolerance(1e-12));
}

// ------------------------------------------------------------
// 30/360
// ------------------------------------------------------------
BOOST_AUTO_TEST_CASE(thirty_360_string_dates)
{
    BOOST_TEST_MESSAGE("Testing 30/360 with string dates");

    Thirty_360 th360;

    auto yf = th360("2016-04-01", "2016-10-01");

    BOOST_TEST_MESSAGE(" - year fraction: " << yf);

    // Exactamente medio año
    BOOST_TEST(yf == 180.0/360.0,
               boost::test_tools::tolerance(1e-12));
}

// ------------------------------------------------------------
// Test usando boost::date directamente
// ------------------------------------------------------------
BOOST_AUTO_TEST_CASE(actual_360_date_objects)
{
    using boost::gregorian::date;

    BOOST_TEST_MESSAGE("Testing Actual/360 with date objects");

    Actual_360 act360;

    date d1(2016, 4, 1);
    date d2(2016, 10, 1);

    auto yf = act360(d1, d2);

    BOOST_TEST_MESSAGE(" - year fraction: " << yf);

    BOOST_TEST(yf == 183.0/360.0,
               boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_SUITE_END()
