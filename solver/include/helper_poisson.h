/**
 * \class HelperPoisson
 *
 * This class provides functions that are necessary for Poisson's problem. These functions are 
 * very specific. 
 *
 */

#ifndef HELPER_POISSON_H
#define HELPER_POISSON_H

#include "common_definitions.h"
#include "helper.h"
#include "cell.h"

using namespace PoissonModel;

class HelperPoisson: public Helper
{
  public:
    
    double get_boundary_value_x(Cell &cell, DIRECTION direction); 
    double boundary_function_x(vec &point); 
    double rhs_function(vec &point); 

};

#endif