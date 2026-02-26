// Tests del Swap (IRS) - Tema 2 del curso
// Curva de referencia (tipos zero-coupon, composicion continua):
//   T=0.5  => 5.0%
//   T=1.0  => 5.8%
//   T=1.5  => 6.4%
//   T=2.0  => 6.8%
//
// Valores exactos calculados con la formula:
//   DF(t) = exp(-t * r(t))
//   DF(0.5)=0.97531, DF(1.0)=0.94366, DF(1.5)=0.90847, DF(2.0)=0.87284
//   Anualidad (dt=0.5): A = 0.5*(0.97531+0.94366+0.90847+0.87284) = 1.85014
//   FloatingLeg PV    = N*(1-DF(2)) = 100*0.12716 = 12.716
//   Par rate c*       = 12.716 / (100*1.85014) = 6.873%
//   FixedLeg PV (5%)  = 100*0.05*0.5*(sum DF) = 2.5*3.70028 = 9.2507
//   NPV (rec.fijo,5%) = 9.2507 - 12.716 = -3.465

#define BOOST_TEST_MODULE test_swap
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <map>

#include <ZeroCouponCurve/ZeroCouponCurve.h>
#include <Swap/Swap.h>
#include <Swap/FixedLeg.h>
#include <Swap/FloatingLeg.h>
#include <Factory/InstrumentFactory.h>
#include <Factory/InstrumentDescription.h>
#include <Factory/SwapBuilder.h>
#include <Factory/BondBuilder.h>

// Curva del Tema 2 (reutilizada en todos los tests)
static std::map<double,double> tema2_zc()
{
    return { {0.5, 0.050}, {1.0, 0.058}, {1.5, 0.064}, {2.0, 0.068} };
}

BOOST_AUTO_TEST_SUITE(swap_suite)

// -----------------------------------------------------------------
// 1. Pata fija: verificacion de flujos descontados
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(fixed_leg_price)
{
    ZeroCouponCurve curve(tema2_zc());
    FixedLeg leg(100.0, 0.05, 2.0, 2);

    // PV = 100*0.05*0.5 * (DF(0.5)+DF(1.0)+DF(1.5)+DF(2.0))
    //    = 2.5 * 3.70028 = 9.2507
    const double pv = leg.price(curve);
    BOOST_TEST_MESSAGE("FixedLeg PV (5%, 2y, semi) = " << pv);
    BOOST_TEST(pv == 9.2507, boost::test_tools::tolerance(1e-3));
}

// -----------------------------------------------------------------
// 2. Pata flotante: formula telescopica exacta
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(floating_leg_price)
{
    ZeroCouponCurve curve(tema2_zc());
    FloatingLeg leg(100.0, 2.0, 2);

    // PV = N*(1 - DF(T)) = 100*(1 - exp(-2*0.068)) = 100*0.12716 = 12.716
    const double pv = leg.price(curve);
    BOOST_TEST_MESSAGE("FloatingLeg PV (2y, semi) = " << pv);
    BOOST_TEST(pv == 12.716, boost::test_tools::tolerance(1e-3));
}

// -----------------------------------------------------------------
// 3. Par rate del Swap (Tema 2, ejemplo swap 2 anios semestral)
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(par_rate_tema2)
{
    ZeroCouponCurve curve(tema2_zc());
    Swap swap(100.0, 0.05, 2.0, 2);

    // c* = (1 - DF(2)) / (dt * sumDF) = 12.716 / (100*1.85014) = 6.873%
    const double par = swap.par_rate(curve);
    BOOST_TEST_MESSAGE("Par rate = " << par * 100.0 << "%");
    BOOST_TEST(par == 0.06873, boost::test_tools::tolerance(1e-4));
}

// -----------------------------------------------------------------
// 4. Swap fuera de equilibrio: tipo fijo < par => NPV < 0 (rec. fijo)
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(swap_npv_below_par)
{
    ZeroCouponCurve curve(tema2_zc());
    Swap swap(100.0, 0.05, 2.0, 2);   // r=5% < par=6.873%

    // NPV = PV(fija) - PV(flotante) = 9.2507 - 12.716 = -3.465
    const double npv = swap.price(curve);
    BOOST_TEST_MESSAGE("NPV (rec.fijo, r=5%) = " << npv);
    BOOST_TEST(npv < 0.0);
    BOOST_TEST(npv == -3.465, boost::test_tools::tolerance(1e-2));
}

// -----------------------------------------------------------------
// 5. Swap al par: NPV = 0
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(par_swap_zero_npv)
{
    ZeroCouponCurve curve(tema2_zc());
    Swap swap_par(100.0, 0.05, 2.0, 2);
    const double c_par = swap_par.par_rate(curve);

    Swap at_par(100.0, c_par, 2.0, 2);
    const double npv = at_par.price(curve);
    BOOST_TEST_MESSAGE("NPV del swap al par = " << npv);
    BOOST_TEST(std::fabs(npv) < 1e-10);
}

// -----------------------------------------------------------------
// 6. Patron Factory: crea Swap via InstrumentFactory + SwapBuilder
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(factory_creates_swap)
{
    auto& factory = InstrumentFactory::instance();
    factory.register_builder(InstrumentDescription::Type::Swap,
                             std::make_unique<SwapBuilder>());
    factory.register_builder(InstrumentDescription::Type::Bond,
                             std::make_unique<BondBuilder>());

    InstrumentDescription desc;
    desc.type             = InstrumentDescription::Type::Swap;
    desc.nominal          = 100.0;
    desc.fixed_rate       = 0.05;
    desc.maturity         = 2.0;
    desc.payments_per_year = 2;

    auto instrument = factory.create(desc);

    ZeroCouponCurve curve(tema2_zc());
    const double npv = instrument->price(curve);
    BOOST_TEST_MESSAGE("Factory Swap NPV (5%) = " << npv);
    BOOST_TEST(npv == -3.465, boost::test_tools::tolerance(1e-2));
}

// -----------------------------------------------------------------
// 7. Consistencia: par rate no depende del tipo fijo inicial
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(par_rate_independent_of_initial_rate)
{
    ZeroCouponCurve curve(tema2_zc());

    Swap s1(100.0, 0.03, 2.0, 2);
    Swap s2(100.0, 0.08, 2.0, 2);

    BOOST_TEST(s1.par_rate(curve) == s2.par_rate(curve),
               boost::test_tools::tolerance(1e-12));
}

BOOST_AUTO_TEST_SUITE_END()
