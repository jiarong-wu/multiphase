#include "tools.h"

// Rubbish code, change later
const double mu = 0.2;

void interpolate(VectorXd &solution /* To do: There should be a function pointer as another argument here */)
{

  for (int id = 0; id < solution.size(); ++id)
  {
    Cell cell(id);
    // To fill up
  }

}


void interpolate_S(VectorXd &solution /* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < solution.size(); ++i)
  {
    solution(i) = 0;
  } 
}

void interpolate_p(VectorXd &solution /* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < solution.size(); ++i)
  {
    Cell cell(i);
    solution(i) = boundary_function_p(cell.cell_center_);
  } 
}

double compute_K(Cell &cell, DIRECTION direction)
{
  if (direction == WEST || direction == EAST)
  {
    // return 100*fabs(sin(2*M_PI*cell.face_centers_[direction][1])) + 1e-3;
    return 100;   
  }
  else
  {
    return 1;
  }
}

double get_mobility(double S)
{
  return 1./mu*pow(S, 2) + pow(1-S, 2);
}

double get_fraction(double S)
{
  return pow(S, 2)/(pow(S, 2) + mu*pow(1-S, 2));
}


// Garbage code
bool determine_upwind(Cell &cell, DIRECTION direction, VectorXd &p_solution)
{
  double p_self = p_solution(cell.id_);

  double p_other;
  if (cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID)
    p_other = get_boundary_value_p(cell, direction);
  else
    p_other = p_solution(cell.neighbour_ids_[direction]);

  return p_self - p_other > 0 ? true : false; 

}

double get_saturation(Cell &cell, DIRECTION direction, VectorXd &p_solution, VectorXd &S_solution)
{
  bool is_upwind = determine_upwind(cell, direction, p_solution);

  if (is_upwind)
  {
    return S_solution(cell.id_);
  }
  else
  {
    if (cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID)
      return get_boundary_value_S(cell, direction);
    else  
      return S_solution(cell.neighbour_ids_[direction]);
  }
}

double get_boundary_value_p(Cell &cell, DIRECTION direction)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_p(cell.face_centers_[direction]);
}

double get_boundary_value_S(Cell &cell, DIRECTION direction)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_S(cell.face_centers_[direction]);
}

double get_boundary_value_x(Cell &cell, DIRECTION direction)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_x(cell.face_centers_[direction]);
}

double boundary_function_p(vec &point)
{
  return 2 - point[0];
}

double boundary_function_S(vec &point)
{
  if (point[0] < 1e-10)
    return 1;
  else
    return 0;
}

double boundary_function_x(vec &point)
{
  return sin(2*M_PI*(point[0] + point[1]));
}

double rhs_function(vec &point)
{
  // Mannualy set the rhs function so that the home made analytical solution is satisfied in the poisson equation
  return 8*pow(M_PI, 2)*sin(2*M_PI*(point[0] + point[1]));
}