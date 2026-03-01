#include "GoalSeeker.h"
#include <cmath>

GoalSeeker::Result GoalSeeker::solve(const std::function<double(double)>& f,
                                     double x0,
                                     double tol,
                                     int max_iter,
                                     double h)
{
    double x = x0;

    for (int it = 1; it <= max_iter; ++it) {
        const double fx = f(x);
        if (std::fabs(fx) < tol) {
            return {x, it, true};
        }

        const double dfx = (f(x + h) - f(x - h)) / (2.0 * h);

        if (std::fabs(dfx) < 1e-14) {
            return {x, it, false};
        }

        x = x - fx / dfx;
    }

    return {x, max_iter, false};
}