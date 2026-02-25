#define BOOST_TEST_MODULE test_goalseeker
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <GoalSeeker/GoalSeeker.h>

BOOST_AUTO_TEST_SUITE(goalseeker_suite)

BOOST_AUTO_TEST_CASE(solve_simple_root)
{
    // f(x)=x^2-2 => raíz sqrt(2)
    auto f = [](double x){ return x*x - 2.0; };

    auto res = GoalSeeker::solve(f, 1.0);

    BOOST_TEST(res.converged);
    BOOST_TEST(res.x == std::sqrt(2.0), boost::test_tools::tolerance(1e-10));
}

BOOST_AUTO_TEST_SUITE_END()
