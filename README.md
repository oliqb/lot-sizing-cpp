# Capacitated Lot-Sizing Solver

C++ implementation of the Capacitated Lot-Sizing Problem, solvable with either IBM CPLEX or FICO Xpress as the backend.

Uses a `Solver` abstract base class so the same `ProblemInstance` can be solved with either CPLEX (`CplexSolver`) or Xpress (`XpressSolver`).

## Problem

Lot-sizing problem: given a planning horizon and a set of items, the objective is to plan the quantity of each item to produce in each period. The variables considered are

x[i,t]: quantity of item i produced in period t
y[i,t]: binary variable indicating whether setup for item i occurs in t
I[i,t]: inventory of item i at the end of period t

The objective is to minimize total cost: unit production cost (c_i), setup cost (s_i), and unit holding cost (h_i).

Production is limited by total available capacity per period: setup time (st_i) and unit production time (p_i) together must stay under that period's capacity.

## Build

Requires CMake 3.15+, an MSVC toolchain, IBM CPLEX, and FICO Xpress.

```
cmake -B build
cmake --build build --config Release --target mrpCplex   # CPLEX
cmake --build build --config Release --target mrpXpress  # Xpress
```

Exe ends up in `build/Release/`.

## Run

```
./build/Release/mrpCplex.exe
```

No arguments — the problem instance (items, demand, capacity) is hardcoded in `main.cpp`. It builds and solves that instance, then prints the solver status and a sample variable value to stdout. There's no CLI or file-based input yet; swapping instances means editing `main.cpp` directly.

## Status

CPLEX side works end to end. Xpress port still in progress. No tests yet.

