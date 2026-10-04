#pragma once

#include <vector>

#include "xprb_cpp.h"

#include "Solver.h"


class XpressSolver : public Solver{
    private:
        dashoptimization::XPRBprob prob;
        std::vector<std::vector<dashoptimization::XPRBvar>> x_i_t;
        std::vector<std::vector<dashoptimization::XPRBvar>> y_i_t;
        std::vector<std::vector<dashoptimization::XPRBvar>> I_i_t;

        int n_items;
        int n_periods;
        static std::string _convertStatus(int status);

    public:
        XpressSolver(ProblemInstance problem);
        std::string solve() override;
        std::string get_solver_status() override;
};

