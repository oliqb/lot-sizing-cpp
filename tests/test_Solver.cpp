#include "Solver.h"

#include <gtest/gtest.h>
#include <stdexcept>

class TestSolver : public Solver {
    public:
        TestSolver(ProblemInstance problem): Solver(std::move(problem)){
        }

        std::string solve() override{
            return "";
        }

        std::string get_solver_status() override{
            return Solver::_solver_status;
        }

        void set_xit(std::vector<std::vector<double>> xit){
            Solver::_xit = xit;
        }

        void set_Iit(std::vector<std::vector<double>> Iit){
            Solver::_Iit = Iit;
        }

        void set_yit(std::vector<std::vector<double>> yit){
            Solver::_yit = yit;
        }

    protected:

};


namespace {

ProblemInstance makeProblem(){
    return ProblemInstance(
        {2.5, 3.5},                  // cost_unit_production
        {1.5, 1.4},                  // holding_cost
        {115.0, 250.0},              // setup_cost
        {2.4, 3.9},                  // setup_time
        {2.0, 0.5},                  // unit_production_time
        {14.0, 8.0},                 // I0
        {860.0, 900.0, 740.0},       // capacity
        {
            {21.0, 31.0, 26.0},      // demand, item 0
            {16.0, 21.0, 11.0}       // demand, item 1
        }
    );
}

}  // namespace

class SolverFixture : public ::testing::Test{
    protected:
        TestSolver solver;

    public:
        SolverFixture() : solver(makeProblem()) {}
};

TEST_F(SolverFixture, getXit){
    std::vector<std::vector<double>> xit = {
        {10.0, 0.0, 5.0},    // item 0
        {0.0, 7.0, 3.0}      // item 1
    };
    solver.set_xit(xit);

    ASSERT_EQ(solver.get_var_values("xit"), xit);

}

TEST_F(SolverFixture, getYit){
    std::vector<std::vector<double>> yit = {
        {1, 0, 1},    // item 0
        {0, 1, 1}     // item 1
    };
    solver.set_yit(yit);

    ASSERT_EQ(solver.get_var_values("yit"), yit);

}

TEST_F(SolverFixture, getIit){
    std::vector<std::vector<double>> Iit = {
        {2.0, 3.0, 4.0},    // item 0
        {7.0, 8.0, 9.0}      // item 1
    };
    solver.set_Iit(Iit);

    ASSERT_EQ(solver.get_var_values("Iit"), Iit);
}

TEST_F(SolverFixture, InvalidName){
    ASSERT_THROW(solver.get_var_values("Invalid"), std::invalid_argument);
}

TEST_F(SolverFixture, ValidateSolutionFeasible){
    std::vector<std::vector<double>> xit = {
        {10.0, 0.0, 5.0},    // item 0
        {0.0, 7.0, 3.0}      // item 1
    };
    solver.set_xit(xit);

    std::vector<std::vector<double>> yit = {
        {1, 0, 1},    // item 0
        {0, 1, 1}     // item 1
    };
    solver.set_yit(yit);

    EXPECT_TRUE(solver.validateSolution());
}

TEST_F(SolverFixture, ValidateSolutionInfeasibleYitWrong){
    std::vector<std::vector<double>> xit = {
        {10.0, 0.0, 5.0},    // item 0
        {0.0, 7.0, 3.0}      // item 1
    };
    solver.set_xit(xit);

    std::vector<std::vector<double>> yit = {
        {0, 0, 1},    // item 0
        {0, 1, 1}     // item 1
    };
    solver.set_yit(yit);

    EXPECT_FALSE(solver.validateSolution());
}


TEST_F(SolverFixture, ValidateSolutionInfeasibleOverCapacity){
    std::vector<std::vector<double>> xit = {
        {300.0, 0.0, 5.0},   // item 0
        {600.0, 7.0, 3.0}    // item 1
    };
    solver.set_xit(xit);

    std::vector<std::vector<double>> yit = {
        {1, 0, 1},    // item 0
        {1, 1, 1}     // item 1
    };
    solver.set_yit(yit);

    EXPECT_FALSE(solver.validateSolution());
}
