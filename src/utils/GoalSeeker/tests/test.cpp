#define BOOST_TEST_MODULE test_goalseeker
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <GoalSeeker.h>

BOOST_AUTO_TEST_SUITE(goalseeker_suite)


BOOST_AUTO_TEST_CASE(solve_sqrt2)
{
    auto f = [](double x) { return x * x - 2.0; };

    auto res = GoalSeeker::solve(f, 1.0);

    BOOST_TEST(res.converged);
    BOOST_TEST(res.x == std::sqrt(2.0), boost::test_tools::tolerance(1e-10));
}

BOOST_AUTO_TEST_CASE(solve_cubic)
{
    auto f = [](double x) { return x * x * x - 27.0; };

    auto res = GoalSeeker::solve(f, 2.0);

    BOOST_TEST(res.converged);
    BOOST_TEST(res.x == 3.0, boost::test_tools::tolerance(1e-10));
}

BOOST_AUTO_TEST_CASE(solve_transcendental)
{
    auto f = [](double x) { return std::exp(x) - 2.0; };

    auto res = GoalSeeker::solve(f, 0.5);

    BOOST_TEST(res.converged);
    BOOST_TEST(res.x == std::log(2.0), boost::test_tools::tolerance(1e-10));
}

BOOST_AUTO_TEST_CASE(solve_linear)
{
    auto f = [](double x) { return 2.0 * x - 10.0; };

    auto res = GoalSeeker::solve(f, 0.0);

    BOOST_TEST(res.converged);
    BOOST_TEST(res.x == 5.0, boost::test_tools::tolerance(1e-12));
    BOOST_TEST(res.iterations <= 3);
}


BOOST_AUTO_TEST_CASE(solve_bond_yield)
{

    const double coupon = 3.0;
    const double nominal = 100.0;
    const double market_price = 98.39;

    auto f = [&](double r) {
        double pv = 0.0;
        for (int i = 1; i <= 4; ++i)
            pv += coupon * std::exp(-r * 0.5 * i);
        pv += nominal * std::exp(-r * 2.0);
        return pv - market_price;
    };

    auto res = GoalSeeker::solve(f, 0.06);

    BOOST_TEST(res.converged);
    BOOST_TEST_MESSAGE("Bond yield (TIR) = " << res.x * 100.0 << "%");

    BOOST_TEST(res.x > 0.06);
    BOOST_TEST(res.x < 0.08);

    double check = 0.0;
    for (int i = 1; i <= 4; ++i)
        check += coupon * std::exp(-res.x * 0.5 * i);
    check += nominal * std::exp(-res.x * 2.0);
    BOOST_TEST(check == market_price, boost::test_tools::tolerance(1e-6));
}

BOOST_AUTO_TEST_CASE(no_convergence_zero_derivative)
{
    auto f = [](double x) { return x * x * x; };
    auto res = GoalSeeker::solve(f, 0.0, 1e-12, 50);

    BOOST_TEST(res.converged);
    BOOST_TEST(std::fabs(res.x) < 1e-10);
}

BOOST_AUTO_TEST_SUITE_END()
