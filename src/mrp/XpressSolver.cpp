
#include "XpressSolver.h"

#include <iostream>
#include <string>

using namespace dashoptimization;

// Using the deprecated library

XpressSolver::XpressSolver(ProblemInstance problem) : Solver(std::move(problem)) {

    try{
        n_items = _problem.get_item_count();
        n_periods = _problem.get_period_count();
        _solver_status = "Not solved";
        double big_M = 1e10;
        x_i_t = std::vector<std::vector<XPRBvar>>(n_items);
        y_i_t = std::vector<std::vector<XPRBvar>>(n_items);
        I_i_t = std::vector<std::vector<XPRBvar>>(n_items);

        XPRBexpr exp_obj;

        std::string ctrName;
        
        // Constraint production capacity
        std::vector<XPRBexpr> all_exp_ct_cap = std::vector<XPRBexpr>(n_periods);

        for(int i=0; i<n_items; i++){
            x_i_t[i] = std::vector<XPRBvar>(n_periods);
            I_i_t[i] = std::vector<XPRBvar>(n_periods);
            y_i_t[i] = std::vector<XPRBvar>(n_periods);
            
            XPRBvar I_prec;
            XPRBexpr exp_invBal;

            for(int t=0; t<n_periods;t++){
                // Creating variables
                std::string varName;
                varName = "x_" + std::to_string(i) + "_" + std::to_string(t);
                auto x = prob.newVar(varName.c_str() , XPRB_PL, 0.0, XPRB_INFINITY);
                varName = "I_" + std::to_string(i) + "_" + std::to_string(t);
                auto I = prob.newVar(varName.c_str() , XPRB_PL, 0.0, XPRB_INFINITY);
                varName = "y_" + std::to_string(i) + "_" + std::to_string(t);
                auto y = prob.newVar(varName.c_str() , XPRB_BV);
                x_i_t[i][t] = x;
                I_i_t[i][t] = I;
                y_i_t[i][t] = y;

                // Creating obj
                exp_obj += _problem.get_item(i).get_cost_unit_production() * x;
                exp_obj += _problem.get_item(i).get_holding_cost() * I;
                exp_obj += _problem.get_item(i).get_setup_cost() * y;

                // Constraint setup linking
                ctrName = "c_link_" + std::to_string(i) + "_" + std::to_string(t);
                prob.newCtr(ctrName.c_str(), y * big_M >= x);

                // Constraint inventory balance
                if (t == 0){
                    exp_invBal = _problem.get_item(i).get_I0() + x - (_problem.get_period(t).get_demand())[i] - I;
                } else {
                    exp_invBal = I_prec + x - (_problem.get_period(t).get_demand())[i] - I;
                }
                ctrName = "c_invBal_" + std::to_string(i) + "_" + std::to_string(t);
                prob.newCtr(ctrName.c_str(),exp_invBal == 0.0);

                // Constraint production capacity
                all_exp_ct_cap[t] += _problem.get_item(i).get_production_unit_time() * x + _problem.get_item(i).get_setup_time() * y;

                I_prec = I;
            }
        }

        // Constraint production capacity
        for(int t=0; t<n_periods;t++){
            ctrName = "c_cap_" + std::to_string(t);
            prob.newCtr(ctrName.c_str(), all_exp_ct_cap[t] <= _problem.get_period(t).get_capacity());
        }

        prob.setObj(exp_obj);
        prob.setSense(XPRB_MINIM);
    } catch (std::exception& e) {
        std::cerr << "Exception when building the model: " << e.what() << std::endl;
        _solver_status = "Solver failed during construction!";
    }
    catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        _solver_status = "Solver failed during construction!";
    }
}

std::string XpressSolver::solve() {
    if(_solver_status == "Solver failed during construction!"){
        return "Solver failed during construction!";
    }
    try{

        int temp_valu =prob.mipOptimize();
        int sol_status = prob.getMIPStat();
        std::cout << "Value of solver " << std::to_string(sol_status) << " or " << std::to_string(temp_valu) << std::endl;
        _solver_status = _convertStatus(sol_status);

        if (sol_status == XPRB_MIP_OPTIMAL || sol_status == XPRB_MIP_SOLUTION){
            std::cout << "Optimal (or feasible) solution found." << std::endl;
            _xit = std::vector<std::vector<double>>(n_items);
            _Iit = std::vector<std::vector<double>>(n_items);
            _yit = std::vector<std::vector<double>>(n_items);
            
            for(int i=0; i<n_items; i++){
                auto _xi = std::vector<double>(n_periods);
                auto _Ii = std::vector<double>(n_periods);
                auto _yi = std::vector<double>(n_periods);
                for(int t=0; t<n_periods;t++){
                    _xi[t] = (x_i_t[i][t]).getSol();
                    _Ii[t] = (I_i_t[i][t]).getSol();
                    _yi[t] = (y_i_t[i][t]).getSol();
                }
                _xit[i] = _xi;
                _Iit[i] = _Ii;
                _yit[i] = _yi;
            }

        } else {
            std::cout << _solver_status << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Exception when solving the model: " << e.what() << std::endl;
        _solver_status = "Solver failed during solving!";
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        _solver_status = "Solver failed during solving!";
    }
    

    return _solver_status;
}

std::string XpressSolver::get_solver_status(){
    return _solver_status;
}

std::string XpressSolver::_convertStatus(int status){

    switch (status){
        case XPRB_MIP_OPTIMAL:
            return "Optimal";
        case XPRB_MIP_NO_SOL_FOUND:
            return "Unknown";
        case XPRB_MIP_LP_NOT_OPTIMAL:
            return "Unknown";
        case XPRB_MIP_LP_OPTIMAL:
            return "Unknown";
        case XPRB_MIP_NOT_LOADED:
            return "Unknown";
        case XPRB_MIP_SOLUTION:
            return "Feasible";
        case XPRB_MIP_INFEAS:
            return "Infeasible";
        case XPRB_MIP_UNBOUNDED:
            return "Unbounded";
    }

    return "Unknown";
}
