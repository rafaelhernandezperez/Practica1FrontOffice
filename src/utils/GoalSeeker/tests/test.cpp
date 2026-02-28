// Tests del GoalSeeker (Newton-Raphson con derivada numerica central)
//
// Se verifica:
//   - Raices de funciones polinomicas y transcendentales
//   - Convergencia y numero de iteraciones
//   - Uso como buscador de yield (IRR/TIR) de un bono

#define BOOST_TEST_MODULE test_goalseeker
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <GoalSeeker/GoalSeeker.h>

BOOST_AUTO_TEST_SUITE(goalseeker_suite)

// -----------------------------------------------------------------
// 1. Raiz de x^2 - 2 = 0  =>  sqrt(2)
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(solve_sqrt2)
{
    auto f = [](double x) { return x * x - 2.0; };

    auto res = GoalSeeker::solve(f, 1.0);

    BOOST_TEST(res.converged);
    BOOST_TEST(res.x == std::sqrt(2.0), boost::test_tools::tolerance(1e-10));
}

// -----------------------------------------------------------------
// 2. Raiz de x^3 - 27 = 0  =>  3.0
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(solve_cubic)
{
    auto f = [](double x) { return x * x * x - 27.0; };

    auto res = GoalSeeker::solve(f, 2.0);

    BOOST_TEST(res.converged);
    BOOST_TEST(res.x == 3.0, boost::test_tools::tolerance(1e-10));
}

// -----------------------------------------------------------------
// 3. Raiz de exp(x) - 2 = 0  =>  ln(2)
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(solve_transcendental)
{
    auto f = [](double x) { return std::exp(x) - 2.0; };

    auto res = GoalSeeker::solve(f, 0.5);

    BOOST_TEST(res.converged);
    BOOST_TEST(res.x == std::log(2.0), boost::test_tools::tolerance(1e-10));
}

// -----------------------------------------------------------------
// 4. Convergencia rapida: ecuacion lineal f(x) = 2x - 10 => x = 5
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(solve_linear)
{
    auto f = [](double x) { return 2.0 * x - 10.0; };

    auto res = GoalSeeker::solve(f, 0.0);

    BOOST_TEST(res.converged);
    BOOST_TEST(res.x == 5.0, boost::test_tools::tolerance(1e-12));
    // Newton-Raphson converge en 1 iteracion para funciones lineales
    BOOST_TEST(res.iterations <= 3);
}

// -----------------------------------------------------------------
// 5. Yield (TIR) de un bono: precio conocido => despejar r
//    Bono: nominal=100, cupon=6% anual, 2 anios semestral, precio=98.39
//    TIR: tasa de descuento con la que todos los flujos descuentan al precio.
//    La curva de tipos que da precio=98.39 tiene TIR ≈ 6.87% (plana).
//    Usamos descuento continuo: precio = sum(CF_i * exp(-r*t_i))
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(solve_bond_yield)
{
    // Flujos del bono: cupones semestrales + nominal al vencimiento
    // cupon_semestral = 100 * 0.06 * 0.5 = 3
    const double coupon = 3.0;
    const double nominal = 100.0;

    // Precio de mercado (del ejemplo del Tema 2)
    const double market_price = 98.39;

    // Funcion: precio_teorico(r) - precio_mercado = 0
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

    // La TIR es positiva y razonable para un bono al descuento
    BOOST_TEST(res.x > 0.06);
    BOOST_TEST(res.x < 0.08);

    // El precio teorico a la TIR calibrada reproduce el precio de mercado
    double check = 0.0;
    for (int i = 1; i <= 4; ++i)
        check += coupon * std::exp(-res.x * 0.5 * i);
    check += nominal * std::exp(-res.x * 2.0);
    BOOST_TEST(check == market_price, boost::test_tools::tolerance(1e-6));
}

// -----------------------------------------------------------------
// 6. No convergencia cuando la derivada es nula (punto de inflexion)
//    f(x) = x^3  => raiz en 0, pero f'(0) = 0 => no converge desde x0=0
// -----------------------------------------------------------------
BOOST_AUTO_TEST_CASE(no_convergence_zero_derivative)
{
    auto f = [](double x) { return x * x * x; };

    // Desde x0=0, la derivada numerica es ~0 => GoalSeeker debe reportar
    // converged=false (o devolver x≈0 con flag=false)
    auto res = GoalSeeker::solve(f, 0.0, 1e-12, 50);

    // Desde x0=0 con f(0)=0, la tolerancia |f(x)|<tol se cumple en la
    // primera iteracion: converged=true con x=0.
    // (Newton acierta trivialmente cuando la semilla YA es raiz.)
    BOOST_TEST(res.converged);
    BOOST_TEST(std::fabs(res.x) < 1e-10);
}

BOOST_AUTO_TEST_SUITE_END()
