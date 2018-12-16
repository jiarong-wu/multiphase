#ifndef HELPER_VERTICAL_H
#define HELPER_VERTICAL_H

#include "common_definitions.h"
#include "helper.h"

class HelperVertical: public Helper
{
  public:
    
    void interpolate(VectorXd &solution /* To do: There should be a function pointer as another argument here */);

    void interpolate_S(VectorXd &solution /* This function is a rubbish version. Got to be deleted and replaced. */);

    void interpolate_p(VectorXd &solution /* This function is a rubbish version. Got to be deleted and replaced. */);

    double compute_K(Cell &cell, DIRECTION direction);

    double get_mobility(double S);

    double get_fraction(double S);

    bool determine_upwind(Cell &cell, DIRECTION direction, VectorXd &p_solution);

    double get_saturation(Cell &cell, DIRECTION direction, VectorXd &p_solution, VectorXd &S_solution);

    double get_boundary_value_p(Cell &cell, DIRECTION direction);

    double get_boundary_value_S(Cell &cell, DIRECTION direction);

    double boundary_function_p(vec &point);
  
    double boundary_function_S(vec &point);





};

#endif