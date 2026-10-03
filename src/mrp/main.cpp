#include <iostream>
#include "ProblemInstance.h"
#include "CplexSolver.h"

int main(){
    std::vector<double> cost_unit_production = {2.0, 3.0, 2.5};
    std::vector<double> holding_cost         = {0.5, 0.4, 0.6};
    std::vector<double> setup_cost           = {100.0, 150.0, 120.0};
    std::vector<double> setup_time           = {2.0, 3.0, 2.5};
    std::vector<double> unit_production_time = {1.0, 1.5, 1.2};
    std::vector<double> I0                   = {10.0, 5.0, 0.0};

    std::vector<double> capacity = {800.0, 800.0, 700.0, 900.0, 600.0, 1000.0};

    std::vector<std::vector<double>> demand = {
        {20.0, 30.0, 25.0, 40.0, 35.0, 20.0},   // item 0
        {15.0, 20.0, 10.0, 25.0, 30.0, 15.0},   // item 1
        {10.0, 15.0, 20.0, 10.0, 25.0, 30.0}    // item 2
    };

    ProblemInstance problem(
        cost_unit_production,
        holding_cost, 
        setup_cost, 
        setup_time, 
        unit_production_time,
        I0,
        capacity,
        demand
    );
    CplexSolver solver(std::move(problem));
    std::cout << "Before solve" << std::endl;
    std::cout << solver.solve() << std::endl;
    std::cout << "The value of i=2, t=2 is " << solver.get_var_values("xit")[2][2] << std::endl;
    if (solver.validateSolution()){
        std::cout << "The solution is valid" << std::endl;
    } else{
        std::cout << "The solution is invalid" << std::endl;
    }
    std::cout << "I'm done" << std::endl;
}