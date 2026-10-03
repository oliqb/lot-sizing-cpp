#include "Solver.h"
#include <stdexcept>

Solver::Solver(ProblemInstance problem): _problem(std::move(problem)){}

Solver::~Solver(){}

std::vector<std::vector<double>> Solver::get_var_values(std::string var_name) const{
    if (var_name == "xit"){
        return _xit;
    } else if (var_name == "Iit"){
        return _Iit;
    } else if (var_name == "yit"){
        return _yit;
    } else {
        throw std::invalid_argument("Invalid var name");
    }

}