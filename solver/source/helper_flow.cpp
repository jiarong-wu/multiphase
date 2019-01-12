#include "helper_flow.h"


/*!
 * \note The function is to be implemented. There should be a function pointer as another argument here.
*/
void HelperFlow::interpolate(VectorXd &solution)
{

  for (int id = 0; id < solution.size(); ++id)
  {
    Cell cell(id);
    // To fill up
  }

}

/*!
 * \details Interpolte the initial value for S
*/
void HelperFlow::interpolate_S(VectorXd &solution)
{
  for (int i = 0; i < solution.size(); ++i)
  {
    solution(i) = 0;
  } 
}

/*!
 * \details Interpolte the initial value for S given time
*/
void HelperFlow::interpolate_S(VectorXd &solution, double time /* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < solution.size(); ++i)
  {
    Cell cell(i);
    solution(i) = boundary_function_S(cell.cell_center_, time);
  } 
}


/*!
 * \details Interpolte the initial value for p
*/
void HelperFlow::interpolate_p(VectorXd &solution /* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < solution.size(); ++i)
  {
    Cell cell(i);
    solution(i) = boundary_function_p(cell.cell_center_);
  } 
}

/*!
 * \details Interpolte the initial value for S given time
*/
void HelperFlow::interpolate_p(VectorXd &solution, double time/* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < solution.size(); ++i)
  {
    Cell cell(i);
    solution(i) = boundary_function_p(cell.cell_center_, time);
  } 
}


/*!
 * \details Compute the permeability tensor K. Could be a random permeability tensor.
 * \note The function needs to be improved.
*/
double HelperFlow::compute_K(Cell &cell, DIRECTION direction)
{

  return 1;

  // if (direction == WEST || direction == EAST)
  // {
  //   srand(cell.id_);
  //   return (rand()%RONDOM_N + 1)/(double)RONDOM_N;   
  // }
  // else
  // {
  //   srand(cell.id_ + CELL_NUMBER);
  //   return (rand()%RONDOM_N + 1)/(double)RONDOM_N;
  // }
}


/*!
 * \details Compute kappa used in the analytical solution
*/
double HelperFlow::get_kappa(double S)
{
  return -1./(0.5 - 0.2*S);
}


/*!
 * \details Compute mobility
*/
double HelperFlow::get_mobility(double S)
{
  return 1./mu*pow(S, 2) + pow(1-S, 2);
}


/*!
 * \details Compute mobility
*/
double HelperFlow::get_fraction(double S)
{
  return pow(S, 2)/(pow(S, 2) + mu*pow(1-S, 2));
}


/*!
 * \details Use the information from pressure to determin upwinding
*/
bool HelperFlow::determine_upwind(Cell &cell, DIRECTION direction, VectorXd &p_solution)
{
  double p_self = p_solution(cell.id_);

  double p_other;
  if (cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID)
    p_other = get_boundary_value_p(cell, direction);
  else
    p_other = p_solution(cell.neighbour_ids_[direction]);

  return p_self - p_other >= 0 ? true : false; 
}

/*!
 * \details Use the information from pressure to determin upwinding given time
*/
bool HelperFlow::determine_upwind(Cell &cell, DIRECTION direction, VectorXd &p_solution, double time)
{
  double p_self = p_solution(cell.id_);

  double p_other;
  if (cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID)
    p_other = get_boundary_value_p(cell, direction, time);
  else
    p_other = p_solution(cell.neighbour_ids_[direction]);

  return p_self - p_other >= 0 ? false : true; 
}


/*!
 * \details Compute saturation
*/
double HelperFlow::get_saturation(Cell &cell, DIRECTION direction, VectorXd &p_solution, VectorXd &S_solution)
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

/*!
 * \details Compute saturation given time
*/
double HelperFlow::get_saturation(Cell &cell, DIRECTION direction, VectorXd &p_solution, VectorXd &S_solution, double time)
{
  bool is_upwind = determine_upwind(cell, direction, p_solution, time);

  if (is_upwind)
  {
    return S_solution(cell.id_);
  }
  else
  {
    if (cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID)
      return get_boundary_value_S(cell, direction, time);
    else  
      return S_solution(cell.neighbour_ids_[direction]);
  }
}


/*!
 * \details Get boundary value for p
*/
double HelperFlow::get_boundary_value_p(Cell &cell, DIRECTION direction)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_p(cell.face_centers_[direction]);
}

/*!
 * \details Get boundary value for p given time
*/
double HelperFlow::get_boundary_value_p(Cell &cell, DIRECTION direction, double time)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_p(cell.face_centers_[direction], time);
}

/*!
 * \details Get boundary value for S
*/
double HelperFlow::get_boundary_value_S(Cell &cell, DIRECTION direction)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_S(cell.face_centers_[direction]);
}

/*!
 * \details Get boundary value for S given time
*/
double HelperFlow::get_boundary_value_S(Cell &cell, DIRECTION direction, double time)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_S(cell.face_centers_[direction], time);
}


/*!
 * \details Analytical boundary functions for p
*/
double HelperFlow::boundary_function_p(vec &point)
{
  return 2 - point[0];
}

/*!
 * \details Analytical boundary functions for p given time
*/
double HelperFlow::boundary_function_p(vec &point, double time)
{
  return 0.2/M_PI*cos(M_PI*(point[0] + point[1] - 2*time)) + 0.5*(point[0] + point[1]);
}


/*!
 * \details Analytical boundary functions for S
*/
double HelperFlow::boundary_function_S(vec &point)
{
  if (point[0] < 1e-10)
    return 1;
  else
    return 0;
}

/*!
 * \details Analytical boundary functions for S given time
*/
double HelperFlow::boundary_function_S(vec &point, double time)
{
  return sin(M_PI*(point[0] + point[1] - 2*time));
}



