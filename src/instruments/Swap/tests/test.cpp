#define BOOST_TEST_MODULE test_swap
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <instruments/Swap/Swap.h>
#include <instruments/Swap/FixedLeg.h>
#include <instruments/Swap/FloatingLeg.h>

#include <marketData/ZeroCouponCurve/ZeroCouponCurve.h>
#include <marketData/Index/Index.h>

#include <utils/DayCount/daycount.h>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <map>
#include <memory>
#include <vector>
#include <utility>
#include <cmath>
#include <optional>
#include <iomanip>

BOOST_AUTO_TEST_SUITE(swap_excel_suite)

BOOST_AUTO_TEST_CASE(swap_matches_excel_tema2_example)
{
    using boost::gregorian::date;

    // Inputs
    const double N = 100000000.0;
    const double fixedRate = 0.05;
    const double firstFixing6m = 0.048;
    const int m = 2; // 6M

    // Fecha valoración
    const date ref(2016, 4, 1);
    auto dc = std::make_shared<Actual360>();

    // Fechas de pago
    const date d1(2016, 10, 3);
    const date d2(2017, 4, 3);
    const date d3(2017, 10, 2);
    const date d4(2018, 4, 2);

    // Curva ZC del Excel (en esas fechas)
    std::map<double,double> zc_by_T;
    zc_by_T[dc->yearFraction(ref, d1)] = 0.0474;
    zc_by_T[dc->yearFraction(ref, d2)] = 0.0500;
    zc_by_T[dc->yearFraction(ref, d3)] = 0.0510;
    zc_by_T[dc->yearFraction(ref, d4)] = 0.0520;

    auto curve = std::make_shared<ZeroCouponCurve>(ref, dc, zc_by_T);

    // Index 6M
    auto index = std::make_shared<Index>("Euribor6M", m);

    // Periodos (start,end)
    std::vector<std::pair<date,date>> periods = {
        {ref, d1},
        {d1,  d2},
        {d2,  d3},
        {d3,  d4}
    };

    auto fixedLeg = std::make_unique<FixedLeg>(N, fixedRate, dc, periods);
    auto floatLeg = std::make_unique<FloatingLeg>(N, dc, periods, index, true, firstFixing6m);

    // Valorar las legs por separado antes de moverlas al swap
    const double pvFixed = fixedLeg->price(*curve);
    const double pvFloat = floatLeg->price(*curve);

    BOOST_TEST_MESSAGE("========================================");
    BOOST_TEST_MESSAGE(std::fixed << std::setprecision(6)
        << "PV Fixed Leg  : " << pvFixed);
    BOOST_TEST_MESSAGE(std::fixed << std::setprecision(6)
        << "PV Float Leg  : " << pvFloat);
    BOOST_TEST_MESSAGE(std::fixed << std::setprecision(6)
        << "PV Swap (Float - Fixed): " << (pvFloat - pvFixed));
    BOOST_TEST_MESSAGE("========================================");

    Swap swap(std::move(fixedLeg), std::move(floatLeg));
    const double pvSwap = swap.price(*curve);

    BOOST_TEST_MESSAGE(std::fixed << std::setprecision(6)
        << "PV Swap (via Swap::price): " << pvSwap);
    BOOST_TEST_MESSAGE("Expected               : " << std::setprecision(6) << 497431.43214853667);
    BOOST_TEST_MESSAGE("========================================");

    const double expected = 497431.43214853667;
    BOOST_TEST(pvSwap == expected, boost::test_tools::tolerance(1e-10));
}

BOOST_AUTO_TEST_SUITE_END()