#define BOOST_TEST_MODULE test_index
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <marketData/Index/Index.h>
#include <marketData/ZeroCouponCurve/ZeroCouponCurve.h>
#include <utils/DayCount/daycount.h>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <map>
#include <memory>
#include <cmath>

BOOST_AUTO_TEST_SUITE(index_suite)

BOOST_AUTO_TEST_CASE(constant_zc_gives_constant_forward_after_conversion)
{
    using boost::gregorian::date;
    using boost::gregorian::months;

    date ref(2026, 1, 1);
    auto dc = std::make_shared<Actual360>();

    // curva plana 5%
    const double zc = 0.05;
    std::map<double,double> zcmap = {
        {0.5, zc},
        {10.0, zc}
    };

    ZeroCouponCurve curve(ref, dc, zcmap);

    // Index 6M 
    Index eur6m("Euribor6M", 2);

    date start = ref + months(12);
    date end   = ref + months(18);

    const double r = eur6m.get_rate(curve, start, end);

    // En curva plana, forward continuo = zc
    const double expected = 2.0 * (std::exp(zc / 2.0) - 1.0);

    BOOST_TEST(r == expected, boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_SUITE_END()