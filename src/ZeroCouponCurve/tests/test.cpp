// Tests de la curva zero-coupon y calibracion por bootstrapping (Tema 3)
//
// Curva objetivo: { 0.5: 5.0%, 1.0: 5.8%, 1.5: 6.4%, 2.0: 6.8% }
//
// Bootstrapping (Tema 3):
//   Dados precios de bonos en el mercado, se recupera la curva ZC iterativamente.
//   Para cada vencimiento T_i, usando las tasas ya calibradas para T < T_i,
//   se resuelve (via GoalSeeker/Newton-Raphson):
//     Precio_bono(r_i) - Precio_mercado = 0

#define BOOST_TEST_MODULE test_zerocouponcurve
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <ZeroCouponCurve/ZeroCouponCurve.h>
#include <Bond/Bond.h>
#include <GoalSeeker/GoalSeeker.h>
#include <map>
#include <cmath>

BOOST_AUTO_TEST_SUITE(zc_curve_suite)

// -----------------------------------------------------------------
// 1. Factores de descuento exactos en los nodos de la curva
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(zc_exact_points_and_df)
{
    std::map<double, double> zc = {
        {0.5, 0.050},
        {1.0, 0.058},
        {1.5, 0.064},
        {2.0, 0.068}
    };
    ZeroCouponCurve curve(zc);

    // Tipos exactos en los nodos
    BOOST_TEST(curve.get_zc(0.5) == 0.050, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_zc(1.0) == 0.058, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_zc(1.5) == 0.064, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_zc(2.0) == 0.068, boost::test_tools::tolerance(1e-12));

    // DF(0,t) = exp(-t*zc(t))
    BOOST_TEST(curve.get_dcf(0.5) == std::exp(-0.5 * 0.050), boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_dcf(1.0) == std::exp(-1.0 * 0.058), boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_dcf(1.5) == std::exp(-1.5 * 0.064), boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_dcf(2.0) == std::exp(-2.0 * 0.068), boost::test_tools::tolerance(1e-12));
}

// -----------------------------------------------------------------
// 2. Interpolacion lineal entre nodos
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(zc_linear_interpolation)
{
    std::map<double, double> zc = {
        {1.0, 0.050},
        {2.0, 0.070}
    };
    ZeroCouponCurve curve(zc);

    // Interpolacion lineal: zc(1.5) = 0.060
    BOOST_TEST(curve.get_zc(1.5) == 0.060, boost::test_tools::tolerance(1e-12));

    // Interpolacion al 25% del tramo: zc(1.25) = 0.055
    BOOST_TEST(curve.get_zc(1.25) == 0.055, boost::test_tools::tolerance(1e-12));
}

// -----------------------------------------------------------------
// 3. Error al consultar fuera del rango de la curva
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(zc_out_of_range_throws)
{
    ZeroCouponCurve curve({ {1.0, 0.05}, {2.0, 0.07} });
    BOOST_CHECK_THROW(curve.get_zc(3.0), std::out_of_range);
    BOOST_CHECK_THROW(curve.get_zc(0.5), std::out_of_range);
}

// -----------------------------------------------------------------
// 4. Bootstrapping de la curva ZC (Tema 3)
//    Precios de mercado generados desde la curva objetivo:
//      {0.5: 5.0%, 1.0: 5.8%, 1.5: 6.4%, 2.0: 6.8%}
//    Se recuperan los tipos con precision < 1e-6 usando Newton-Raphson.
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(bootstrap_curve_calibration)
{
    // Curva objetivo (que queremos recuperar)
    const double r05_target = 0.050;
    const double r10_target = 0.058;
    const double r15_target = 0.064;
    const double r20_target = 0.068;

    // Precios de mercado calculados con esa curva
    // Bond(nominal, cupon_anual, vencimiento, pagos_por_anio)
    // Usamos cupon 0% para el ZCB de 6 meses
    const double DF05 = std::exp(-0.5 * r05_target);
    const double DF10 = std::exp(-1.0 * r10_target);
    const double DF15 = std::exp(-1.5 * r15_target);
    const double DF20 = std::exp(-2.0 * r20_target);

    // Bono 1: ZCB 6M (cupon=0, semestral => unico flujo = nominal en T=0.5)
    const double p1 = 100.0 * DF05;
    // Bono 2: 1Y, cupon 5% semestral
    const double p2 = 2.5 * DF05 + 102.5 * DF10;
    // Bono 3: 1.5Y, cupon 5% semestral
    const double p3 = 2.5 * DF05 + 2.5 * DF10 + 102.5 * DF15;
    // Bono 4: 2Y, cupon 5% semestral
    const double p4 = 2.5 * DF05 + 2.5 * DF10 + 2.5 * DF15 + 102.5 * DF20;

    std::map<double, double> calibrated;

    // --- Paso 1: resolver r(0.5) ---
    // Bond(100, 0%, 0.5y, 2) => precio = 100*DF(0.5)
    {
        auto res = GoalSeeker::solve(
            [&](double r) {
                ZeroCouponCurve curve({{0.5, r}});
                Bond zcb(100.0, 0.0, 0.5, 2);
                return zcb.price(curve) - p1;
            },
            0.04  // semilla inicial
        );
        BOOST_TEST(res.converged);
        calibrated[0.5] = res.x;
    }

    // --- Paso 2: resolver r(1.0) usando r(0.5) ya calibrado ---
    {
        auto res = GoalSeeker::solve(
            [&](double r) {
                auto zc = calibrated;
                zc[1.0] = r;
                ZeroCouponCurve curve(zc);
                Bond bond(100.0, 0.05, 1.0, 2);
                return bond.price(curve) - p2;
            },
            0.05  // semilla inicial
        );
        BOOST_TEST(res.converged);
        calibrated[1.0] = res.x;
    }

    // --- Paso 3: resolver r(1.5) usando r(0.5) y r(1.0) ya calibrados ---
    {
        auto res = GoalSeeker::solve(
            [&](double r) {
                auto zc = calibrated;
                zc[1.5] = r;
                ZeroCouponCurve curve(zc);
                Bond bond(100.0, 0.05, 1.5, 2);
                return bond.price(curve) - p3;
            },
            0.06
        );
        BOOST_TEST(res.converged);
        calibrated[1.5] = res.x;
    }

    // --- Paso 4: resolver r(2.0) usando todos los anteriores ---
    {
        auto res = GoalSeeker::solve(
            [&](double r) {
                auto zc = calibrated;
                zc[2.0] = r;
                ZeroCouponCurve curve(zc);
                Bond bond(100.0, 0.05, 2.0, 2);
                return bond.price(curve) - p4;
            },
            0.07
        );
        BOOST_TEST(res.converged);
        calibrated[2.0] = res.x;
    }

    // Verificacion: los tipos calibrados deben coincidir con el objetivo
    BOOST_TEST_MESSAGE("r(0.5) calibrado = " << calibrated[0.5] * 100 << "%");
    BOOST_TEST_MESSAGE("r(1.0) calibrado = " << calibrated[1.0] * 100 << "%");
    BOOST_TEST_MESSAGE("r(1.5) calibrado = " << calibrated[1.5] * 100 << "%");
    BOOST_TEST_MESSAGE("r(2.0) calibrado = " << calibrated[2.0] * 100 << "%");

    BOOST_TEST(calibrated[0.5] == r05_target, boost::test_tools::tolerance(1e-6));
    BOOST_TEST(calibrated[1.0] == r10_target, boost::test_tools::tolerance(1e-6));
    BOOST_TEST(calibrated[1.5] == r15_target, boost::test_tools::tolerance(1e-6));
    BOOST_TEST(calibrated[2.0] == r20_target, boost::test_tools::tolerance(1e-6));

    // Verificacion adicional: la curva calibrada reproduce los precios de mercado
    ZeroCouponCurve final_curve(calibrated);
    BOOST_TEST(Bond(100.0, 0.05, 2.0, 2).price(final_curve) == p4,
               boost::test_tools::tolerance(1e-8));
}

BOOST_AUTO_TEST_SUITE_END()
