#define BOOST_TEST_MODULE test_zerocouponcurve
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <ZeroCouponCurve/ZeroCouponCurve.h>
#include <map>
#include <cmath>

BOOST_AUTO_TEST_SUITE(zc_curve_suite)

BOOST_AUTO_TEST_CASE(zc_exact_points_and_df)
{
    // Curva del ejemplo del bono (tipos continuamente compuestos)
    std::map<double, double> zc = {
        {0.5, 0.050},
        {1.0, 0.058},
        {1.5, 0.064},
        {2.0, 0.068}
    };

    ZeroCouponCurve curve(zc);

    // ZC exactos
    BOOST_TEST(curve.get_zc(0.5) == 0.050, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_zc(1.0) == 0.058, boost::test_tools::tolerance(1e-12));

    // DF(0,t) = exp(-t*zc)
    BOOST_TEST(curve.get_dcf(0.5) == std::exp(-0.5 * 0.050), boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_dcf(2.0) == std::exp(-2.0 * 0.068), boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_CASE(zc_linear_interpolation)
{
    std::map<double, double> zc = {
        {1.0, 0.050},
        {2.0, 0.070}
    };
    ZeroCouponCurve curve(zc);

    // En 1.5 debería ser 0.060 por interpolación lineal
    BOOST_TEST(curve.get_zc(1.5) == 0.060, boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_SUITE_END()

