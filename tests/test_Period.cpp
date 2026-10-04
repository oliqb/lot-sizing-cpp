#include "Period.h"

#include <gtest/gtest.h>
// #include <stdexcept>
#include <vector>

TEST (PeriodConstructor, ConstructsWithGivenValues){
    std::vector<double> demand = {2.5, 4.2};
    double capacity = 3.0;
    Period p1 = Period(capacity,
        demand);
    
    EXPECT_DOUBLE_EQ(p1.get_capacity(), capacity);
    EXPECT_EQ(p1.get_demand(), demand);
}

class PeriodFixture : public ::testing::Test {
    protected:
        Period p1;

    public:
        PeriodFixture() : p1(Period(3.0, {2.5, 4.2})) {}
};

TEST_F(PeriodFixture, SetterCapacity){
    double capacity = 2.1;
    p1.set_capacity(capacity);
    EXPECT_DOUBLE_EQ(p1.get_capacity(), capacity);
}


TEST_F(PeriodFixture, SetterDemand){
    std::vector<double> demand = {0.1, 0.5};
    p1.set_demand(demand);
    EXPECT_EQ(p1.get_demand(), demand); // EQ should be fine because there are no operations on demand
}