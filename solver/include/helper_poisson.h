#ifndef HELPER_POISSON_H
#define HELPER_POISSON_H

#include "common_definitions.h"
#include "helper.h"


class HelperPoisson: public Helper
{
  public:
    
    double get_boundary_value_x(Cell &cell, DIRECTION direction); //!< This is a test in header
    double boundary_function_x(vec &point); ///< This is a test in header
    double rhs_function(vec &point);

};

#endif