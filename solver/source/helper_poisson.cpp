 #include "helper_poisson.h"


double HelperPoisson::get_boundary_value_x(Cell &cell, DIRECTION direction)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_x(cell.face_centers_[direction]);
}


double HelperPoisson::boundary_function_x(vec &point)
{
  return sin(2*M_PI*(point[0] + point[1]));
}

double HelperPoisson::rhs_function(vec &point)
{
  // Mannualy set the rhs function so that the home made analytical solution is satisfied in the poisson equation
  return 8*pow(M_PI, 2)*sin(2*M_PI*(point[0] + point[1]));
}

