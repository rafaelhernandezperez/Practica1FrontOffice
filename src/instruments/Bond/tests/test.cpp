#define BOOST_TEST_MODULE test_bond
#include <boost/test/unit_test.hpp>
#include <map>
#include <cmath>

#include <marketData/ZeroCouponCurve/ZeroCouponCurve.h>
#include <instruments/Bond/Bond.h>

BOOST_AUTO_TEST_SUITE(bond_suite)

BOOST_AUTO_TEST_CASE(bond_price_matches_example)
{
    // Curva del ejemplo del Tema 2 (zc en tanto por uno, comp. continua)
    std::map<double, double> zc = {
        {0.5, 0.050},
        {1.0, 0.058},
        {1.5, 0.064},
        {2.0, 0.068}
    };

    ZeroCouponCurve curve(zc);

    // Bono: nominal 100, cupón 6% anual, vencimiento 2y, semestral
    Bond bond(100.0, 0.06, 2.0, 2);

    const double price = bond.price(curve);

    BOOST_TEST_MESSAGE("Bond PV = " << price);

    // Valor exacto (del enunciado): ~98.39
    BOOST_TEST(price == 98.39, boost::test_tools::tolerance(5e-3));
}

BOOST_AUTO_TEST_SUITE_END()


