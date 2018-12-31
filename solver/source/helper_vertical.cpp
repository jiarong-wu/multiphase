 #include "helper_vertical.h"

void HelperVertical::initial_condition_Sc(VectorXd &old_Sc_solution /* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < old_Sc_solution.size(); ++i)
  {
    old_Sc_solution(i) = 0;
  } 
}

void HelperVertical::initial_condition_Sb(VectorXd &old_Sb_solution, VectorXd &old_Sc_solution)
{
  for (int i = 0; i < old_Sb_solution.size(); ++i)
  {
    old_Sb_solution(i) = 1 - old_Sc_solution[i];
  } 
}



void HelperVertical::initial_condition_Pb(VectorXd &old_Pb_solution /* This function is a rubbish version. Got to be deleted and replaced. */)
{
  for (int i = 0; i < old_Pb_solution.size(); ++i)
  {
    Cell cell(i);
    old_Pb_solution(i) = rg_b * (Lz - 1./2.*dz);
  } 
}




void HelperVertical::initial_condition_Pcap(VectorXd &old_Pcap_solution,  VectorXd &old_Sc_solution)
{
  for (int i = 0; i < old_Pcap_solution.size(); ++i)
  {
    Cell cell(i);
    old_Pcap_solution(i) = Pcap_entry + ((1-old_Sc_solution[i])*Lz - 1./2.*dz)*(rg_c-rg_b);

  } 
}




void HelperVertical::initial_condition_Pc(VectorXd &old_Pc_solution, VectorXd &old_Pb_solution, VectorXd &old_Pcap_solution)
{

  for (int i = 0; i < old_Pc_solution.size(); ++i)
  {
    Cell cell(i);
    old_Pc_solution(i) = old_Pb_solution[i] + old_Pcap_solution[i];
  } 
}



void HelperVertical::get_Pcap(VectorXd &new_Pcap_solution, VectorXd &old_Sc_solution)
{
  initial_condition_Pcap(new_Pcap_solution, old_Sc_solution);
}

void HelperVertical::get_Pc(VectorXd &new_Pc_solution, VectorXd &new_Pb_solution, VectorXd &new_Pcap_solution)
{
  initial_condition_Pc(new_Pc_solution, new_Pb_solution, new_Pcap_solution);
}

void HelperVertical::get_Sb(VectorXd &old_Sb_solution, VectorXd &old_Sc_solution)
{
  initial_condition_Sb(old_Sb_solution, old_Sc_solution);
}



double HelperVertical::compute_K(Cell &cell, DIRECTION direction)
{

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







// Garbage code
bool HelperVertical::determine_upwind(Cell &cell, DIRECTION direction, VectorXd &Pb_solution, VectorXd &Pc_solution, VectorXd &Pcap_solution)
{
  double p_self = Pc_solution(cell.id_);
  double p_other;

  if (cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID)
    p_other = get_ghost_value_Pc(cell, direction, Pb_solution, Pcap_solution);
  else
    p_other = Pc_solution(cell.neighbour_ids_[direction]);

  return p_self - p_other >= 0 ? true : false; 
}




double HelperVertical::get_saturation(Cell &cell, DIRECTION direction, VectorXd &Sc_solution,  
                                      VectorXd &Pb_solution, VectorXd &Pc_solution, VectorXd &Pcap_solution)
{
  bool is_upwind = determine_upwind(cell, direction, Pb_solution, Pc_solution, Pcap_solution);

  if (is_upwind)
  {
    return Sc_solution(cell.id_);
  }
  else
  {
    if (cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID)
      return Sc_solution(cell.id_);
    else  
      return Sc_solution(cell.neighbour_ids_[direction]);
  }
}



double HelperVertical::get_coeff_b(double Sc)
{
  return K * (1./mu_b - Sc/mu_b/(1.-sb_res));
}


double HelperVertical::get_coeff_c(double Sc)
{
  return K * krc_star*Sc/mu_c/(1.-sb_res);
}



double HelperVertical::boundary_function_Pb(vec &point)
{
  return rg_b * (Lz - 1./2.*dz);
}

double HelperVertical::get_ghost_value_Pb(Cell &cell, DIRECTION direction, VectorXd &Pb_solution)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return 2*boundary_function_Pb(cell.face_centers_[direction]) -  Pb_solution(cell.id_);
}



double HelperVertical::get_ghost_value_Pcap(Cell &cell, DIRECTION direction, VectorXd &Pcap_solution)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return  Pcap_solution(cell.id_);
}


double HelperVertical::get_ghost_value_Pc(Cell &cell, DIRECTION direction, VectorXd &Pb_solution,  VectorXd &Pcap_solution)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return get_ghost_value_Pb(cell, direction, Pb_solution) + get_ghost_value_Pcap(cell, direction, Pcap_solution);
}



double HelperVertical::get_boundary_value_Pb(Cell &cell, DIRECTION direction)
{
  assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
  return boundary_function_Pb(cell.face_centers_[direction]);
}




// double HelperVertical::get_boundary_value_Pcap(Cell &cell, DIRECTION direction, VectorXd &Pcap_solution)
// {
//   assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
//   return Pcap_solution(cell.id_);
// }





// double HelperVertical::get_boundary_value_Pc(Cell &cell, DIRECTION direction, VectorXd &Pcap_solution)
// {
//   assert(("Bug! This face should be a boundary face but it is not!", cell.neighbour_ids_[direction] == OUTSIDE_CELL_ID));
//   return boundary_function_Pb(cell.face_centers_[direction]) + get_boundary_value_Pcap(cell, direction, Pcap_solution);
// }















