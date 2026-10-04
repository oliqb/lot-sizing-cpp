
#include "ProblemInstance.h"

#include <stdexcept>

#include <gtest/gtest.h>

TEST(ProblemInstanceConstructor, ConstructsWithGivenValues){
    std::vector<double> cost_unit_production = {2.0, 3.0};
    std::vector<double> holding_cost         = {0.5, 0.4};
    std::vector<double> setup_cost           = {100.0, 150.0};
    std::vector<double> setup_time           = {2.0, 3.0};
    std::vector<double> unit_production_time = {1.0, 1.5};
    std::vector<double> I0                   = {10.0, 5.0};
    
    std::vector<double> capacity = {800.0, 800.0, 700.0};

    std::vector<std::vector<double>> demand = {
        {20.0, 30.0, 25.0},   // item 0
        {15.0, 20.0, 10.0}   // item 1
    };

    ProblemInstance problem = ProblemInstance(
        cost_unit_production,
        holding_cost, 
        setup_cost, 
        setup_time, 
        unit_production_time,
        I0,
        capacity,
        demand
    );

    EXPECT_EQ((problem.get_item_count()), static_cast<int>(cost_unit_production.size()));
    EXPECT_EQ((problem.get_period_count()), static_cast<int>(capacity.size()));

    EXPECT_DOUBLE_EQ(problem.get_item(0).get_cost_unit_production(), cost_unit_production[0]);
    EXPECT_DOUBLE_EQ(problem.get_item(1).get_cost_unit_production(), cost_unit_production[1]);
    EXPECT_DOUBLE_EQ(problem.get_item(0).get_holding_cost(), holding_cost[0]);
    EXPECT_DOUBLE_EQ(problem.get_item(1).get_holding_cost(), holding_cost[1]);
    EXPECT_DOUBLE_EQ(problem.get_item(0).get_setup_cost(), setup_cost[0]);
    EXPECT_DOUBLE_EQ(problem.get_item(1).get_setup_cost(), setup_cost[1]);
    EXPECT_DOUBLE_EQ(problem.get_item(0).get_setup_time(), setup_time[0]);
    EXPECT_DOUBLE_EQ(problem.get_item(1).get_setup_time(), setup_time[1]);
    EXPECT_DOUBLE_EQ(problem.get_item(0).get_production_unit_time(), unit_production_time[0]);
    EXPECT_DOUBLE_EQ(problem.get_item(1).get_production_unit_time(), unit_production_time[1]);
    EXPECT_DOUBLE_EQ(problem.get_item(0).get_I0(), I0[0]);
    EXPECT_DOUBLE_EQ(problem.get_item(1).get_I0(), I0[1]);
    EXPECT_DOUBLE_EQ(problem.get_period(0).get_capacity(), capacity[0]);
    EXPECT_DOUBLE_EQ(problem.get_period(1).get_capacity(), capacity[1]);
    EXPECT_DOUBLE_EQ(problem.get_period(2).get_capacity(), capacity[2]);
    EXPECT_DOUBLE_EQ(problem.get_period(0).get_demand()[0], demand[0][0]);
    EXPECT_DOUBLE_EQ(problem.get_period(1).get_demand()[0], demand[0][1]);
    EXPECT_DOUBLE_EQ(problem.get_period(2).get_demand()[0], demand[0][2]);
    EXPECT_DOUBLE_EQ(problem.get_period(0).get_demand()[1], demand[1][0]);
    EXPECT_DOUBLE_EQ(problem.get_period(1).get_demand()[1], demand[1][1]);
    EXPECT_DOUBLE_EQ(problem.get_period(2).get_demand()[1], demand[1][2]);
}

class ProblemInstanceFixture : public ::testing::Test{
    protected:
        ProblemInstance problem;
        ProblemInstance other;
    public:
        ProblemInstanceFixture(): problem(
            {2.5, 3.5},
            {1.5, 1.4},
            {115.0, 250.0},
            {2.4, 3.9},
            {2.0, 0.5},
            {14.0, 8.0},
            {860.0, 900.0, 740.0},
            {
                {21.0, 31.0, 26.0},   // item 0
                {16.0, 21.0, 11.0}    // item 1
            }), 
            other(
            {4.5},
            {0.9},
            {75.0},
            {1.1},
            {3.0},
            {6.0},
            {500.0, 450.0},
            {
                {12.0, 18.0}   // item 0
            }) {}
};

TEST_F(ProblemInstanceFixture, MoveAssignmentMoving){
    // Couple spot checks on problem
    ASSERT_DOUBLE_EQ(problem.get_item(0).get_cost_unit_production(), 2.5);
    ASSERT_DOUBLE_EQ(problem.get_item(1).get_setup_cost(), 250);
    ASSERT_EQ(problem.get_item_count(), 2);
    ASSERT_EQ(problem.get_period_count(), 3);

    // Couple spot checks on other
    ASSERT_DOUBLE_EQ(other.get_item(0).get_cost_unit_production(), 4.5);
    ASSERT_DOUBLE_EQ(other.get_item(0).get_setup_cost(), 75);
    ASSERT_EQ(other.get_item_count(), 1);
    ASSERT_EQ(other.get_period_count(), 2);

    // Keep pointer
    Item* before = &problem.get_item(0);
    
    // Move
    other = std::move(problem);

    // problem is empty
    EXPECT_EQ(problem.get_item_count(), 0);
    EXPECT_EQ(problem.get_period_count(), 0);
    EXPECT_ANY_THROW(problem.get_item(0));

    // Couple spot checks on other
    EXPECT_DOUBLE_EQ(other.get_item(0).get_cost_unit_production(), 2.5);
    EXPECT_DOUBLE_EQ(other.get_item(1).get_setup_cost(), 250);
    EXPECT_EQ(other.get_item_count(), 2);
    EXPECT_EQ(other.get_period_count(), 3);
    
    // Check pointer
    EXPECT_EQ(&other.get_item(0), before);
}

TEST_F(ProblemInstanceFixture, MoveConstructorMoving){
    // Couple spot checks
    ASSERT_DOUBLE_EQ(problem.get_item(0).get_cost_unit_production(), 2.5);
    ASSERT_DOUBLE_EQ(problem.get_item(1).get_setup_cost(), 250);
    ASSERT_EQ(problem.get_item_count(), 2);
    ASSERT_EQ(problem.get_period_count(), 3);

    // Keep pointer
    Item* before = &problem.get_item(0);
    
    // Move
    ProblemInstance p2 = std::move(problem);

    // problem is empty
    EXPECT_EQ(problem.get_item_count(), 0);
    EXPECT_EQ(problem.get_period_count(), 0);
    EXPECT_ANY_THROW(problem.get_item(0));

    // Couple spot checks on p2
    EXPECT_DOUBLE_EQ(p2.get_item(0).get_cost_unit_production(), 2.5);
    EXPECT_DOUBLE_EQ(p2.get_item(1).get_setup_cost(), 250);
    EXPECT_EQ(p2.get_item_count(), 2);
    EXPECT_EQ(p2.get_period_count(), 3);
    
    // Check pointer
    EXPECT_EQ(&p2.get_item(0), before);
}

TEST_F(ProblemInstanceFixture, SelfMoveAssignmentKeepsData){
    // Couple spot checks
    ASSERT_DOUBLE_EQ(problem.get_item(0).get_cost_unit_production(), 2.5);
    ASSERT_DOUBLE_EQ(problem.get_item(1).get_setup_cost(), 250);
    ASSERT_EQ(problem.get_item_count(), 2);
    ASSERT_EQ(problem.get_period_count(), 3);

    // Keep pointer
    Item* before = &problem.get_item(0);
    
    // Move
    problem = std::move(problem);

    // Couple spot checks
    EXPECT_DOUBLE_EQ(problem.get_item(0).get_cost_unit_production(), 2.5);
    EXPECT_DOUBLE_EQ(problem.get_item(1).get_setup_cost(), 250);
    EXPECT_EQ(problem.get_item_count(), 2);
    EXPECT_EQ(problem.get_period_count(), 3);

    // Check pointer
    EXPECT_EQ(&problem.get_item(0), before);
}

TEST_F(ProblemInstanceFixture, OutOfRangeFailure){
    EXPECT_THROW(problem.get_item(2), std::out_of_range);
    EXPECT_THROW(problem.get_period(3), std::out_of_range);
}

TEST_F(ProblemInstanceFixture, CopyAssignment){
    // Couple spot checks on problem
    ASSERT_DOUBLE_EQ(problem.get_item(0).get_cost_unit_production(), 2.5);
    ASSERT_DOUBLE_EQ(problem.get_item(1).get_setup_cost(), 250);
    ASSERT_EQ(problem.get_item_count(), 2);
    ASSERT_EQ(problem.get_period_count(), 3);

    // Couple spot checks on other
    ASSERT_DOUBLE_EQ(other.get_item(0).get_cost_unit_production(), 4.5);
    ASSERT_DOUBLE_EQ(other.get_item(0).get_setup_cost(), 75);
    ASSERT_EQ(other.get_item_count(), 1);
    ASSERT_EQ(other.get_period_count(), 2);

    // Keep pointer
    Item* before = &problem.get_item(0);
    
    // Copy
    other = problem;

    // Source unchanged
    EXPECT_DOUBLE_EQ(problem.get_item(0).get_cost_unit_production(), 2.5);
    EXPECT_DOUBLE_EQ(problem.get_item(1).get_setup_cost(), 250);
    EXPECT_EQ(problem.get_item_count(), 2);
    EXPECT_EQ(problem.get_period_count(), 3);

    // Couple spot checks on other
    EXPECT_DOUBLE_EQ(other.get_item(0).get_cost_unit_production(), 2.5);
    EXPECT_DOUBLE_EQ(other.get_item(1).get_setup_cost(), 250);
    EXPECT_EQ(other.get_item_count(), 2);
    EXPECT_EQ(other.get_period_count(), 3);
    
    // Check pointer
    EXPECT_NE(&other.get_item(0), before);
}

TEST_F(ProblemInstanceFixture, CopyConstructor){
    // Couple spot checks
    ASSERT_DOUBLE_EQ(problem.get_item(0).get_cost_unit_production(), 2.5);
    ASSERT_DOUBLE_EQ(problem.get_item(1).get_setup_cost(), 250);
    ASSERT_EQ(problem.get_item_count(), 2);
    ASSERT_EQ(problem.get_period_count(), 3);

    // Keep pointer
    Item* before = &problem.get_item(0);
    
    // Copy
    ProblemInstance p2 = problem;

    // Source unchanged
    EXPECT_DOUBLE_EQ(problem.get_item(0).get_cost_unit_production(), 2.5);
    EXPECT_DOUBLE_EQ(problem.get_item(1).get_setup_cost(), 250);
    EXPECT_EQ(problem.get_item_count(), 2);
    EXPECT_EQ(problem.get_period_count(), 3);

    // Couple spot checks on p2
    EXPECT_DOUBLE_EQ(p2.get_item(0).get_cost_unit_production(), 2.5);
    EXPECT_DOUBLE_EQ(p2.get_item(1).get_setup_cost(), 250);
    EXPECT_EQ(p2.get_item_count(), 2);
    EXPECT_EQ(p2.get_period_count(), 3);
    
    // Check pointer
    EXPECT_NE(&p2.get_item(0), before);
}