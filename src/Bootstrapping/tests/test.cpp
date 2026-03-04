#define BOOST_TEST_MODULE test_bootstrapping
#include <boost/test/unit_test.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <memory>

#include <Bootstrapping/Bootstrapping.h>
#include <Bootstrapping/Deposit.h>
#include <Bootstrapping/SwapQuote.h>

BOOST_AUTO_TEST_SUITE(bootstrap_suite)

BOOST_AUTO_TEST_CASE(exercise_3_1_curve_points)
{
    using boost::gregorian::date;
    using boost::gregorian::months;

    const date t0(2016,4,1);

    const date t6m  = t0 + months(6);   // 2016-10-01
    const date t12m = t0 + months(12);  // 2017-04-01
    const date t18m = t0 + months(18);  // 2017-10-01
    const date t24m = t0 + months(24);  // 2018-04-01

    Bootstrapping::InstrumentsMapType insts;
    insts[t6m]  = std::make_shared<Deposit>(t0,  t6m,  0.05);
    insts[t12m] = std::make_shared<SwapQuote>(t6m,  t12m, 0.055);
    insts[t18m] = std::make_shared<SwapQuote>(t12m, t18m, 0.06);
    insts[t24m] = std::make_shared<SwapQuote>(t18m, t24m, 0.064);

    Bootstrapping bs;
    auto curve = bs(insts);

    // Valores esperados con ACT/360 y fórmula incremental (3.1 + 3.6)
    BOOST_TEST(curve.at(t6m)  == 0.9752133279154815, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.at(t12m) == 0.9464189558512760, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.at(t18m) == 0.9136806907692924, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.at(t24m) == 0.8794703193758833, boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_SUITE_END()