#pragma once
#include <functional>

class GoalSeeker
{
public:
    struct Result {
        double x;
        int iterations;
        bool converged;
    };

    static Result solve(const std::function<double(double)>& f,
                        double x0,
                        double tol = 1e-12,
                        int max_iter = 100,
                        double h = 1e-6);
};
