#define BOOST_TEST_MODULE test_zerocouponcurve
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include <ZeroCouponCurve.h>
#include <instruments/Bond/Bond.h>
#include <utils/GoalSeeker/GoalSeeker.h>
#include <map>
#include <cmath>

BOOST_AUTO_TEST_SUITE(zc_curve_suite)


// Factores de descuento exactos en los nodos de la curva
BOOST_AUTO_TEST_CASE(zc_exact_points_and_df)
{
    std::map<double, double> zc = {
        {0.5, 0.050},
        {1.0, 0.058},
        {1.5, 0.064},
        {2.0, 0.068}
    };
    ZeroCouponCurve curve(zc);

    BOOST_TEST(curve.get_zc(0.5) == 0.050, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_zc(1.0) == 0.058, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_zc(1.5) == 0.064, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_zc(2.0) == 0.068, boost::test_tools::tolerance(1e-12));

    BOOST_TEST(curve.get_dcf(0.5) == std::exp(-0.5 * 0.050), boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_dcf(1.0) == std::exp(-1.0 * 0.058), boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_dcf(1.5) == std::exp(-1.5 * 0.064), boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_dcf(2.0) == std::exp(-2.0 * 0.068), boost::test_tools::tolerance(1e-12));

    BOOST_TEST(curve.get_dcf(0.0) == 1.0, boost::test_tools::tolerance(1e-12));
}


// Interpolación lineal entre nodos
BOOST_AUTO_TEST_CASE(zc_linear_interpolation)
{
    std::map<double, double> zc = {
        {1.0, 0.050},
        {2.0, 0.070}
    };
    ZeroCouponCurve curve(zc);

    BOOST_TEST(curve.get_zc(1.5)  == 0.060, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(curve.get_zc(1.25) == 0.055, boost::test_tools::tolerance(1e-12));
}

// Error al consultar fuera del rango de la curva
BOOST_AUTO_TEST_CASE(zc_out_of_range_throws)
{
    ZeroCouponCurve curve({ {1.0, 0.05}, {2.0, 0.07} });
    BOOST_CHECK_THROW(curve.get_zc(3.0), std::out_of_range);
    BOOST_CHECK_THROW(curve.get_zc(0.5), std::out_of_range);
}

// Bootstrapping de la curva ZC 
BOOST_AUTO_TEST_CASE(bootstrap_tema3_exercise)
{
    // P(0, 0.5) = 1 / (1 + r*t)
    const double depo_rate = 0.05;
    const double dt        = 0.5;

    std::map<double, double> calibrated;

    {
        auto res = GoalSeeker::solve(
            [&](double r) {
                ZeroCouponCurve curve({{0.5, r}});
                // DF = exp(-r*0.5) = 1/(1+0.05*0.5)
                return curve.get_dcf(0.5) - 1.0/(1.0 + depo_rate*dt);
            },
            0.04
        );
        BOOST_TEST(res.converged);
        calibrated[0.5] = res.x;
        BOOST_TEST_MESSAGE("ZC(0.5) = " << res.x * 100.0 << "%");
    }

    // 1 = S*dt*DF(0.5) + (1+S*dt)*DF(1.0)
    // DF(1.0) = (1 - S*dt*DF(0.5)) / (1 + S*dt)
    const double s1y = 0.055;
    const double P05 = std::exp(-calibrated[0.5] * 0.5);
    calibrated[1.0] = (1.0 - s1y*dt*P05) / (1.0 + s1y*dt);
    {
        // Verificar con GoalSeeker que el Swap 1y vale 0
        double r10 = calibrated[1.0];
        // ZC rate desde DF
        calibrated[1.0] = -std::log(r10) / 1.0;
        BOOST_TEST_MESSAGE("ZC(1.0) = " << calibrated[1.0] * 100.0 << "%");
    }

    const double s18m = 0.06;
    {
        auto res = GoalSeeker::solve(
            [&](double r) {
                auto zc = calibrated;
                zc[1.5] = r;
                ZeroCouponCurve curve(zc);
                // PV fixed = S*dt*(DF1+DF2+DF3) + DF3 = 1
                double annuity = curve.get_dcf(0.5) + curve.get_dcf(1.0) + curve.get_dcf(1.5);
                return s18m * dt * annuity + curve.get_dcf(1.5) - 1.0;
            },
            0.06
        );
        BOOST_TEST(res.converged);
        calibrated[1.5] = res.x;
        BOOST_TEST_MESSAGE("ZC(1.5) = " << res.x * 100.0 << "%");
    }

    const double s2y = 0.064;
    {
        auto res = GoalSeeker::solve(
            [&](double r) {
                auto zc = calibrated;
                zc[2.0] = r;
                ZeroCouponCurve curve(zc);
                double annuity = curve.get_dcf(0.5) + curve.get_dcf(1.0)
                               + curve.get_dcf(1.5) + curve.get_dcf(2.0);
                return s2y * dt * annuity + curve.get_dcf(2.0) - 1.0;
            },
            0.07
        );
        BOOST_TEST(res.converged);
        calibrated[2.0] = res.x;
        BOOST_TEST_MESSAGE("ZC(2.0) = " << res.x * 100.0 << "%");
    }

    // Verificación: DFs implícitos deben coincidir con bootstrapping analítico
    ZeroCouponCurve final_curve(calibrated);
    BOOST_TEST(final_curve.get_dcf(0.5) == 1.0/(1.0 + 0.05*0.5),
               boost::test_tools::tolerance(1e-8));
    BOOST_TEST(final_curve.get_dcf(1.0) ==
               (1.0 - s1y*dt*(1.0/(1.0+0.05*0.5))) / (1.0+s1y*dt),
               boost::test_tools::tolerance(1e-8));
}

// Bootstrapping desde precios de bonos
BOOST_AUTO_TEST_CASE(bootstrap_from_bond_prices)
{
    const double r05 = 0.050, r10 = 0.058, r15 = 0.064, r20 = 0.068;

    const double DF05 = std::exp(-0.5*r05);
    const double DF10 = std::exp(-1.0*r10);
    const double DF15 = std::exp(-1.5*r15);
    const double DF20 = std::exp(-2.0*r20);

    const double p1 = 100.0 * DF05;
    const double p2 = 2.5*DF05 + 102.5*DF10;
    const double p3 = 2.5*DF05 + 2.5*DF10 + 102.5*DF15;
    const double p4 = 2.5*DF05 + 2.5*DF10 + 2.5*DF15 + 102.5*DF20;

    std::map<double,double> calibrated;

    {
        auto res = GoalSeeker::solve([&](double r){
            ZeroCouponCurve c({{0.5,r}});
            return Bond(100.0,0.0,0.5,2).price(c) - p1;
        }, 0.04);
        BOOST_TEST(res.converged);
        calibrated[0.5] = res.x;
    }
    {
        auto res = GoalSeeker::solve([&](double r){
            auto zc = calibrated; zc[1.0]=r;
            ZeroCouponCurve c(zc);
            return Bond(100.0,0.05,1.0,2).price(c) - p2;
        }, 0.05);
        BOOST_TEST(res.converged);
        calibrated[1.0] = res.x;
    }
    {
        auto res = GoalSeeker::solve([&](double r){
            auto zc = calibrated; zc[1.5]=r;
            ZeroCouponCurve c(zc);
            return Bond(100.0,0.05,1.5,2).price(c) - p3;
        }, 0.06);
        BOOST_TEST(res.converged);
        calibrated[1.5] = res.x;
    }
    {
        auto res = GoalSeeker::solve([&](double r){
            auto zc = calibrated; zc[2.0]=r;
            ZeroCouponCurve c(zc);
            return Bond(100.0,0.05,2.0,2).price(c) - p4;
        }, 0.07);
        BOOST_TEST(res.converged);
        calibrated[2.0] = res.x;
    }

    BOOST_TEST_MESSAGE("r(0.5) = " << calibrated[0.5]*100 << "%");
    BOOST_TEST_MESSAGE("r(1.0) = " << calibrated[1.0]*100 << "%");
    BOOST_TEST_MESSAGE("r(1.5) = " << calibrated[1.5]*100 << "%");
    BOOST_TEST_MESSAGE("r(2.0) = " << calibrated[2.0]*100 << "%");

    BOOST_TEST(calibrated[0.5] == r05, boost::test_tools::tolerance(1e-6));
    BOOST_TEST(calibrated[1.0] == r10, boost::test_tools::tolerance(1e-6));
    BOOST_TEST(calibrated[1.5] == r15, boost::test_tools::tolerance(1e-6));
    BOOST_TEST(calibrated[2.0] == r20, boost::test_tools::tolerance(1e-6));
}

BOOST_AUTO_TEST_SUITE_END()
