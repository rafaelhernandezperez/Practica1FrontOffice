#define BOOST_TEST_MODULE test_swap
#include <boost/test/unit_test.hpp>
#include <map>

#include <ZeroCouponCurve/ZeroCouponCurve.h>
#include <Swap/Swap.h>

BOOST_AUTO_TEST_SUITE(swap_suite)

BOOST_AUTO_TEST_CASE(par_rate_positive)
{
    std::map<double,double> zc = {
        {0.5, 0.050},
        {1.0, 0.058},
        {1.5, 0.064},
        {2.0, 0.068}
    };

    ZeroCouponCurve curve(zc);

    Swap swap(100.0, 0.05, 2.0, 2);

    double par = swap.par_rate(curve);

    BOOST_TEST_MESSAGE("Par rate = " << par);

    BOOST_TEST(par > 0.0);
    BOOST_TEST(par < 0.10);
}

BOOST_AUTO_TEST_SUITE_END()