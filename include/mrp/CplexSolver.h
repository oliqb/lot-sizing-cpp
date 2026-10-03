#pragma once

#include "Solver.h"
#include "ProblemInstance.h"
#include "Item.h"
#include "Period.h"

#include <ilcplex/ilocplex.h>
#include <vector>


class CplexSolver: public Solver{
    private:
        IloEnv env;
        IloModel model;
        IloCplex cplex;
        int n_items;
        int n_periods;
        IloArray<IloArray<IloNumVar>> x_i_t;
        IloArray<IloArray<IloBoolVar>> y_i_t;
        IloArray<IloArray<IloNumVar>> I_i_t;
        std::string _solver_status;

        static std::string _convertStatus(IloAlgorithm::Status status);

    public:
        CplexSolver(ProblemInstance problem);
        ~CplexSolver();

        virtual std::string solve();

        virtual std::string get_solver_status();

        bool validateSolution();
};




