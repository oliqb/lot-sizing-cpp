#include "Solver.h"
#include <stdexcept>
#include <iostream>

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

bool Solver::validateSolution(){
    for (int i=0; i < _problem.get_item_count(); i++){
        for (int t=0; t < _problem.get_period_count(); t++){
            if ((_xit[i][t] > 1e-3) & (_yit[i][t] < 0.5)){
                    std::cout << "(_xit[i][t] > 0) & (_yit[i][t] < 0.5): i=" << i << " t=" << t << " _xit[i][t]" << _xit[i][t] << " _yit[i][t]" << _yit[i][t] << std::endl;
                    return false;
            }
        }
    }

    for (int t=0; t < _problem.get_period_count(); t++){
        double actual = 0.0;
        for (int i=0; i < _problem.get_item_count(); i++){
            actual += _problem.get_item(i).get_setup_time() * _yit[i][t] + _problem.get_item(i).get_production_unit_time() * _xit[i][t];
        }
    if (actual > _problem.get_period(t).get_capacity()){
        return false;
    }
    }
    return true;
}