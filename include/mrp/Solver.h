#pragma once

#include "ProblemInstance.h"
#include "Item.h"
#include "Period.h"

#include <vector>
#include <string>


class Solver{
    protected:
        ProblemInstance _problem;
        std::string _solver_status = "Not solved";
        std::vector<std::vector<double>> _xit;
        std::vector<std::vector<double>> _Iit;
        std::vector<std::vector<double>> _yit;

    public:
        Solver(ProblemInstance problem);
        virtual ~Solver() = 0;

        virtual std::string solve() = 0;

        std::vector<std::vector<double>> get_var_values(std::string var_name) const;

        virtual std::string get_solver_status() = 0;

        bool validateSolution();

};
