
#include <iostream>

#include "xprb_cpp.h"

using namespace dashoptimization;

int main(){
    XPRBprob prob("mrp");
    auto var_x = prob.newVar("x", XPRB_PL, 0.0, XPRB_INFINITY);
    auto var_I = prob.newVar("I", XPRB_PL, 0.0, XPRB_INFINITY);

    XPRBexpr obj;
    obj += var_x;
    obj += var_I;

    prob.setObj(obj);
    prob.setSense(XPRB_MINIM);
    auto sol = prob.solve();

    std::cout << "Solution " << sol << std::endl;

    return 0;
}