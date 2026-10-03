#include "ProblemInstance.h"
#include "Item.h"
#include "Period.h"

#include <vector>

ProblemInstance::ProblemInstance(
            std::vector<double> cost_unit_production,
            std::vector<double> holding_cost, 
            std::vector<double> setup_cost, 
            std::vector<double> setup_time, 
            std::vector<double> unit_production_time,
            std::vector<double> I0,
            std::vector<double> capacity,
            std::vector<std::vector<double>> demand)
            {
                ProblemInstance::_n_item = cost_unit_production.size();
                ProblemInstance::_n_period = capacity.size();
                for(int i=0; i < _n_item; i++){
                    _items.emplace_back(
                        cost_unit_production[i],
                        holding_cost[i], 
                        setup_cost[i],
                        setup_time[i],
                        I0[i],
                        unit_production_time[i]);
                    }

                for(int t=0; t<_n_period; t++){
                    std::vector<double> demand_i;
                    for(int i=0; i<_n_item; i++){
                        demand_i.push_back(demand[i][t]);
                    }
                    _periods.emplace_back(capacity[t], demand_i);
                }

            }

ProblemInstance::~ProblemInstance()
{
}

ProblemInstance::ProblemInstance(ProblemInstance&& problem) noexcept : 
    _items(std::move(problem._items)), 
    _periods(std::move(problem._periods)), 
    _n_item(problem._n_item), 
    _n_period(problem._n_period) {
        problem._n_item = 0;
        problem._n_period = 0;
    }

ProblemInstance& ProblemInstance::operator=(ProblemInstance&& problem) noexcept{
    if (this != &problem) {
        _items = std::move(problem._items); 
        _periods = std::move(problem._periods);
        _n_item = problem._n_item;
        _n_period = problem._n_period;
        problem._n_item = 0;
        problem._n_period = 0;
    }
    return *this;
}

Item& ProblemInstance::get_item(int i){
    return ProblemInstance::_items.at(i);
}

int ProblemInstance::get_item_count() const{
    return ProblemInstance::_n_item;
}

int ProblemInstance::get_period_count() const{
    return ProblemInstance::_n_period;
}

Period& ProblemInstance::get_period(int t){
    return ProblemInstance::_periods.at(t);
}