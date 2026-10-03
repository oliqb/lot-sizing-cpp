#include "Item.h"

#include <gtest/gtest.h>


TEST(ItemConstructor, ConstructsWithGivenValues) {
    Item i1 = Item(1.0,
        2.0,
        3.0,
        4.0,
        5.0,
        6.0);
    EXPECT_DOUBLE_EQ(i1.get_cost_unit_production(), 1.0);
    EXPECT_DOUBLE_EQ(i1.get_holding_cost(), 2.0);
    EXPECT_DOUBLE_EQ(i1.get_setup_cost(), 3.0);
    EXPECT_DOUBLE_EQ(i1.get_setup_time(), 4.0);
    EXPECT_DOUBLE_EQ(i1.get_I0(), 5.0);
    EXPECT_DOUBLE_EQ(i1.get_production_unit_time(), 6.0);
}

class ItemFixture: public ::testing::Test {
    protected:
        Item i1;

    public: 
        ItemFixture(): i1(1.0,
            2.0,
            3.0,
            4.0,
            5.0,
            6.0) {}
};

TEST_F(ItemFixture, ItemSetterCostProd){
    i1.set_cost_unit_production(9.0);
    EXPECT_DOUBLE_EQ(i1.get_cost_unit_production(), 9.0);
}

TEST_F(ItemFixture, ItemSetterHoldingCost){
    i1.set_holding_cost(10.0);
    EXPECT_DOUBLE_EQ(i1.get_holding_cost(), 10.0);
}

TEST_F(ItemFixture, ItemSetterSetupCost){
    i1.set_setup_cost(11.0);
    EXPECT_DOUBLE_EQ(i1.get_setup_cost(), 11.0);
}

TEST_F(ItemFixture, ItemSetterSetupTime){
    i1.set_setup_time(12.0);
    EXPECT_DOUBLE_EQ(i1.get_setup_time(), 12.0);
}

TEST_F(ItemFixture, ItemSetterI0){
    i1.set_I0(13.0);
    EXPECT_DOUBLE_EQ(i1.get_I0(), 13.0);
}

TEST_F(ItemFixture, ItemSetterProductionUnitTime){
    i1.set_production_unit_time(14.0);
    EXPECT_DOUBLE_EQ(i1.get_production_unit_time(), 14.0);
}