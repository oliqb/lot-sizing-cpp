#include "Period.h"

#include <vector>


Period::Period(
    double c, 
    std::vector<double> d): _capacity(c), _demand(std::move(d))
{
}

Period::~Period()
{
}

double Period::get_capacity() const {
    return _capacity;
}
const std::vector<double>& Period::get_demand() const {
    return _demand;
}

void Period::set_capacity(double val) {
    _capacity = val;
}

void Period::set_demand(std::vector<double> val) {
    _demand = std::move(val);
}