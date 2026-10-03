#include "Item.h"


Item::Item(
    double cost_unit_production,
    double holding_cost, 
    double setup_cost, 
    double setup_time, 
    double I0,
    double unit_production_time) :
    _cost_unit_production(cost_unit_production), 
    _holding_cost(holding_cost), 
    _setup_cost(setup_cost), 
    _setup_time(setup_time), 
    _I0(I0), 
    _unit_production_time(unit_production_time)
{
}

Item::~Item()
{
}

double Item::get_cost_unit_production() const{
    return _cost_unit_production;
}
double Item::get_holding_cost() const{
    return _holding_cost;
}
double Item::get_setup_cost() const{
    return _setup_cost;
}
double Item::get_setup_time() const{
    return _setup_time;
}
double Item::get_I0() const{
    return _I0;
}
double Item::get_production_unit_time() const{
    return _unit_production_time;
}

void Item::set_cost_unit_production(double val){
    _cost_unit_production = val;
}
void Item::set_holding_cost(double val){
    _holding_cost = val;
}
void Item::set_setup_cost(double val){
    _setup_cost = val;
}
void Item::set_setup_time(double val){
    _setup_time = val;
}
void Item::set_I0(double val){
    _I0 = val;
}
void Item::set_production_unit_time(double val){
    _unit_production_time = val;
}