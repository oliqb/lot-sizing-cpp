cmake_minimum_required(VERSION 3.15)
project(or_portfolio)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests 
    tests/test_Item.cpp
    tests/test_Period.cpp
    tests/test_ProblemInstance.cpp
    tests/test_Solver.cpp
    src/mrp/Item.cpp
    src/mrp/Period.cpp
    src/mrp/ProblemInstance.cpp
    src/mrp/Solver.cpp)
target_link_libraries(unit_tests PRIVATE 
    gtest_main)
target_include_directories(unit_tests PRIVATE 
    src
    include/mrp)

include(GoogleTest)
gtest_discover_tests(unit_tests)

if (MSVC)
    set(CPLEX_ROOT "C:/Program Files/IBM/ILOG/CPLEX_Studio_Community222")
    set(CPLEX_ARCH "x64_windows_msvc14/stat_mda")

    set(XPRESS_ROOT "C:/xpressmp")

    add_executable(mrpCplex 
        src/mrp/CplexSolver.cpp
        src/mrp/Item.cpp
        src/mrp/Period.cpp
        src/mrp/ProblemInstance.cpp
        src/mrp/Solver.cpp
        src/mrp/main.cpp)

    target_include_directories(mrpCplex PRIVATE
        src/mrp
        include/mrp
        ${CPLEX_ROOT}/cplex/include
        ${CPLEX_ROOT}/concert/include
    )

    target_link_directories(mrpCplex PRIVATE
        ${CPLEX_ROOT}/cplex/lib/${CPLEX_ARCH}
        ${CPLEX_ROOT}/concert/lib/${CPLEX_ARCH}
    )

    target_link_libraries(mrpCplex PRIVATE
        ilocplex
        cplex2220
        concert
    )

    target_compile_definitions(mrpCplex PRIVATE IL_STD)

    add_executable(mrpXpress
        src/mrp/Item.cpp
        src/mrp/Period.cpp
        src/mrp/ProblemInstance.cpp
        src/mrp/Solver.cpp
        src/mrp/mainXpress.cpp)

    target_include_directories(mrpXpress PRIVATE
        src/mrp
        include/mrp
        ${XPRESS_ROOT}/include
    )

    target_link_directories(mrpXpress PRIVATE
        ${XPRESS_ROOT}/lib
    )

    target_link_libraries(mrpXpress PRIVATE
        xprs
    )



endif()