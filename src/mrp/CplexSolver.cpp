#include "CplexSolver.h"


std::string CplexSolver::_convertStatus(IloAlgorithm::Status status){
    switch (status){
        case IloAlgorithm::Optimal:
            return "Optimal";
        case IloAlgorithm::Unknown:
            return "Unknown";
        case IloAlgorithm::Feasible:
            return "Feasible";
        case IloAlgorithm::Infeasible:
            return "Infeasible";
        case IloAlgorithm::Unbounded:
            return "Unbounded";
        case IloAlgorithm::InfeasibleOrUnbounded :
            return "Infeasible Or Unbounded";
        case IloAlgorithm::Error:
            return "Error";
    }

    return "Unknown";
}

CplexSolver::CplexSolver(ProblemInstance problem): Solver(std::move(problem)){
    _solver_status = "Not solved";
    try{
        model = IloModel(env);
        n_items = _problem.get_item_count();
        n_periods = _problem.get_period_count();
        double big_M = 1e10;
        
        auto exp_obj = IloExpr(env);
        
        
        IloArray<IloExpr> all_exp_ct_cap = IloArray<IloExpr>(env, n_periods);
        for(int t=0; t<n_periods;t++){
            all_exp_ct_cap[t] = IloExpr(env);
        }

        x_i_t = IloArray<IloArray<IloNumVar>>(env, n_items);
        I_i_t = IloArray<IloArray<IloNumVar>>(env, n_items);
        y_i_t = IloArray<IloArray<IloBoolVar>>(env, n_items);

        for(int i=0; i<n_items; i++){
            x_i_t[i] = IloArray<IloNumVar>(env, n_periods);
            I_i_t[i] = IloArray<IloNumVar>(env, n_periods);
            y_i_t[i] = IloArray<IloBoolVar>(env, n_periods);
            
            IloNumVar I_prec;

            for(int t=0; t<n_periods;t++){
                auto exp_ct_setup_lkg = IloExpr(env);
                auto exp_ct_inv_bal = IloExpr(env);

                // Creating variables
                auto x = IloNumVar(env, 0.0, IloInfinity);
                auto I = IloNumVar(env, 0.0, IloInfinity);
                auto y = IloBoolVar(env);
                x_i_t[i][t] = x;
                I_i_t[i][t] = I;
                y_i_t[i][t] = y;

                // Creating obj
                exp_obj += _problem.get_item(i).get_cost_unit_production() * x;
                exp_obj += _problem.get_item(i).get_holding_cost() * I;
                exp_obj += _problem.get_item(i).get_setup_cost() * y;

                // Creating inv bal cst
                if(t == 0){
                    exp_ct_inv_bal += _problem.get_item(i).get_I0() + x - (_problem.get_period(t).get_demand())[i] - I;
                } else {
                    exp_ct_inv_bal += I_prec + x - (_problem.get_period(t).get_demand())[i] - I;
                }
                model.add(exp_ct_inv_bal == 0);
                exp_ct_inv_bal.end();
                I_prec = I;

                // Creating x and y linking constraints
                exp_ct_setup_lkg += y * big_M - x;
                model.add(exp_ct_setup_lkg >= 0);
                exp_ct_setup_lkg.end();
                
                // Creating Inventory capacity constraints
                all_exp_ct_cap[t] += _problem.get_item(i).get_production_unit_time() * x + _problem.get_item(i).get_setup_time() * y;
            }
        }

        for(int t=0; t<n_periods;t++){
            model.add(all_exp_ct_cap[t] <= _problem.get_period(t).get_capacity());
            all_exp_ct_cap[t].end();
        }

        model.add(IloMinimize(env, exp_obj));
        exp_obj.end();
    }
    catch (IloException& e) {
        std::cerr << "Concert exception: " << e << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception" << std::endl;
    }

}


CplexSolver::~CplexSolver(){
    env.end();
}

std::string CplexSolver::solve(){
    try{
        
        cplex = IloCplex(model);
        cplex.exportModel("model.lp");
        if (cplex.solve()){
            // Extract var values
            _xit = std::vector<std::vector<double>>(n_items);
            _Iit = std::vector<std::vector<double>>(n_items);
            _yit = std::vector<std::vector<double>>(n_items);
            
            for(int i=0; i<n_items; i++){
                auto _xi = std::vector<double>(n_periods);
                auto _Ii = std::vector<double>(n_periods);
                auto _yi = std::vector<double>(n_periods);
                for(int t=0; t<n_periods;t++){
                    _xi[t] = cplex.getValue(x_i_t[i][t]);
                    _Ii[t] = cplex.getValue(I_i_t[i][t]);
                    _yi[t] = cplex.getValue(y_i_t[i][t]);
                }
                _xit[i] = _xi;
                _Iit[i] = _Ii;
                _yit[i] = _yi;
            }
            
            _solver_status = CplexSolver::_convertStatus(cplex.getStatus());
        }
        std::cerr << "solve status: " << CplexSolver::_convertStatus(cplex.getStatus()) << std::endl;
        return _solver_status;
    }
    catch (IloException& e) {
        std::cerr << "Concert exception: " << e << std::endl;
        _solver_status = "Solve failed!";
        return _solver_status;
    }
    catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        _solver_status = "Solve failed!";
        return _solver_status;
    }
}


std::string CplexSolver::get_solver_status(){
    return _solver_status;
}

bool CplexSolver::validateSolution(){
    for (int i=0; i < n_items; i++){
        for (int t=0; t < n_periods; t++){
            if ((_xit[i][t] > 1e-3) & (_yit[i][t] < 0.5)){
                    std::cout << "(_xit[i][t] > 0) & (_yit[i][t] < 0.5): i=" << i << " t=" << t << " _xit[i][t]" << _xit[i][t] << " _yit[i][t]" << _yit[i][t] << std::endl;
                    return false;
            }
        }
    }

    for (int t=0; t < n_periods; t++){
        double actual = 0.0;
        for (int i=0; i < n_items; i++){
            actual += _problem.get_item(i).get_setup_time() * _yit[i][t] + _problem.get_item(i).get_production_unit_time() * _xit[i][t];
        }
    if (actual > _problem.get_period(t).get_capacity()){
        return false;
    }
    }
    return true;
}