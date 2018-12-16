 #include "helper_flow.h"


// Rubbish code, change later
const double mu = 0.2;

void HelperFlow::interpolate(VectorXd &solution /* To do: There should be a function pointer as another argument here */)
{

  for (int id = 0; id < solution.size(); ++id)
  {
    Cell cell(id);
    // To fill up
  }

}


void HelperFlow::interpolate_S(VectorXd &solution /* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < solution.size(); ++i)
  {
    solution(i) = 0;
  } 
}
void HelperFlow::interpolate_S(VectorXd &solution, double time /* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < solution.size(); ++i)
  {
    Cell cell(i);
    solution(i) = boundary_function_S(cell.cell_center_, time);
  } 
}



void HelperFlow::interpolate_p(VectorXd &solution /* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < solution.size(); ++i)
  {
    Cell cell(i);
    solution(i) = boundary_function_p(cell.cell_center_);
  } 
}
void HelperFlow::interpolate_p(VectorXd &solution, double time/* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < solution.size(); ++i)
  {
    Cell cell(i);
    solution(i) = boundary_function_p(cell.cell_center_, time);
  } 
}



double HelperFlow::compute_K(Cell &cell, DIRECTION direction)
{
  // return 1;


  if (direction == WEST || direction == EAST)
  {
    return 1;   
  }
  else
  {
    return 1;
  }

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



double HelperFlow::get_kappa(double S)
{
  return -1./(0.5 - 0.2*S);
}


double HelperFlow::get_mobility(double S)
{
  return 1./mu*pow(S, 2) + pow(1-S, 2);
}

double HelperFlow::get_fraction(double S)
{
  return pow(S, 2)/(pow(S, 2) + mu*pow(1-S, 2));
}




// Garbage code
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



double HelperFlow::get_boundary_value_p(Cell &cell, DIRECTION direction)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_p(cell.face_centers_[direction]);
}
double HelperFlow::get_boundary_value_p(Cell &cell, DIRECTION direction, double time)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_p(cell.face_centers_[direction], time);
}


double HelperFlow::get_boundary_value_S(Cell &cell, DIRECTION direction)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_S(cell.face_centers_[direction]);
}
double HelperFlow::get_boundary_value_S(Cell &cell, DIRECTION direction, double time)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_S(cell.face_centers_[direction], time);
}



double HelperFlow::boundary_function_p(vec &point)
{
  return 2 - point[0];
}
double HelperFlow::boundary_function_p(vec &point, double time)
{
  return 0.2/M_PI*cos(M_PI*(point[0] + point[1] - 2*time)) + 0.5*(point[0] + point[1]);
}



double HelperFlow::boundary_function_S(vec &point)
{
  if (point[0] < 1e-10)
    return 1;
  else
    return 0;
}

double HelperFlow::boundary_function_S(vec &point, double time)
{
  return sin(M_PI*(point[0] + point[1] - 2*time));
}



