#pragma once


class Item{
    private:
        double _cost_unit_production;
        double _holding_cost;
        double _setup_cost;
        double _setup_time;
        double _I0;
        double _unit_production_time;

    public:
        Item(
            double cost_unit_production,
            double holding_cost, 
            double setup_cost, 
            double setup_time, 
            double I0,
            double unit_production_time
        );

        ~Item();

        double get_cost_unit_production() const;
        double get_holding_cost() const;
        double get_setup_cost() const;
        double get_setup_time() const;
        double get_I0() const;
        double get_production_unit_time() const;

        void set_cost_unit_production(double val);
        void set_holding_cost(double val);
        void set_setup_cost(double val);
        void set_setup_time(double val);
        void set_I0(double val);
        void set_production_unit_time(double val);
    };


