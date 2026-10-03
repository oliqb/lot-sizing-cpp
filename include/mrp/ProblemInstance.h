#pragma once

#include "Item.h"
#include "Period.h"

#include <vector>

class ProblemInstance{
    private:
        std::vector<Item> _items;
        std::vector<Period> _periods;
        int _n_item;
        int _n_period;

    public:
        ProblemInstance(
            std::vector<double> cost_unit_production,
            std::vector<double> holding_cost, 
            std::vector<double> setup_cost, 
            std::vector<double> setup_time, 
            std::vector<double> unit_production_time,
            std::vector<double> I0,
            std::vector<double> capacity,
            std::vector<std::vector<double>> demand
        );

        ~ProblemInstance();

        ProblemInstance(ProblemInstance&& problem) noexcept;
        
        ProblemInstance(const ProblemInstance& problem) = default;

        ProblemInstance& operator=(ProblemInstance&& problem) noexcept;

        ProblemInstance& operator=(const ProblemInstance& problem) = default;

        Item& get_item(int i);
        Period& get_period(int t);
        int get_item_count() const;
        int get_period_count() const;
};


