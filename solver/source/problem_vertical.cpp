#include "problem_vertical.h"


ProblemVertical::ProblemVertical()
:
new_p_solution(CELL_NUMBER), 
new_S_solution(CELL_NUMBER), 
old_p_solution(CELL_NUMBER), 
old_S_solution(CELL_NUMBER), 
system_rhs(CELL_NUMBER),
flag_vector(CELL_NUMBER),
system_matrix(CELL_NUMBER, CELL_NUMBER),
time(0)
{
  helper_vertical_ptr = new HelperVertical;
}


void ProblemVertical::run()
{

  helper_vertical_ptr->interpolate_S(old_S_solution);
  helper_vertical_ptr->interpolate_p(old_p_solution);

  cout << "Total number of dofs is " << CELL_NUMBER << endl;

  for (int i = 0; i < 30; ++i)
  {

    time += DELTA_T;

    triplet_list.clear();
    system_rhs.setZero();
    system_matrix.setZero();

    cout << "This is time step " << i << endl;
    run_one_step();
    if (i%10 == 0)
    {
      output_results(i/10);
    }

    // assert(("Numerical issue! Saturation is out of bound!",  new_p_solution.maxCoeff() < 2 &&  new_p_solution.maxCoeff() >  -1));
  }

  post_processing();
}


void ProblemVertical::run_one_step()
{
  assemble_system_Pb();
  solve_Pb();
  compute_Pc();
  compute_Sc();
  compute_Sb();
}


void ProblemVertical::assemble_system_Pb()
{

  int counter = 0;
  for (int id = 0; id < CELL_NUMBER; ++id)
  {
    Cell cell(id);

    map<DIRECTION, int>::iterator it;
    for (it = cell.neighbour_ids_.begin(); it != cell.neighbour_ids_.end(); it++)
    {
      double K_component = helper_vertical_ptr->compute_K(cell, it->first);
      double saturation = helper_vertical_ptr->get_saturation(cell, it->first, old_p_solution, old_S_solution);
      double mobility = helper_vertical_ptr->get_mobility(saturation);

      // if (saturation > 0.9)
      // {
      //   // We can have a test here. When will saturation be greater than 0.9? Must be left boundary.
      //   cout << "(" << cell.face_centers_[it->first][0] << " , " << cell.face_centers_[it->first][1] << ")" << endl << endl; 
      // }

      // Face contribution
      if (it->second == OUTSIDE_CELL_ID)
      {
        double p_boundary_value = helper_vertical_ptr->get_boundary_value_p(cell, it->first);
        system_rhs(cell.id_) += 2*K_component*mobility*p_boundary_value;
        triplet_list.push_back(T(cell.id_, cell.id_, 2*K_component*mobility));
      }
      else
      {
        triplet_list.push_back(T(cell.id_, cell.id_, 1*K_component*mobility));
        triplet_list.push_back(T(cell.id_, it->second, -1*K_component*mobility));
      }

    }

    // Volume contribution
    system_rhs(cell.id_) += 0*Area;
  }

  // This is a very efficient way of assembling the matrix A. The function is provided by the external library Eigen.
  system_matrix.setFromTriplets(triplet_list.begin(), triplet_list.end());

}


void ProblemVertical::solve_Pb()
{

  // SimplicialCholesky<SparseMatrix<double>> chol(A);  // performs a Cholesky factorization of A
  // VectorXd x = chol.solve(b);         // use the factorization to solve for the given right hand side

  SparseLU<SparseMatrix<double>>   solver;
  solver.analyzePattern(system_matrix); 
  solver.factorize(system_matrix); 
  new_p_solution = solver.solve(system_rhs); 

  // cout << MatrixXd(system_matrix) << endl;
  // cout << new_p_solution << endl;
  // cout << system_rhs << endl;
  // cout << new_p_solution.maxCoeff() << endl;

  old_p_solution = new_p_solution;

}

void ProblemVertical::compute_Pc()
{

}

void ProblemVertical::compute_Sc()
{

}



void ProblemVertical::compute_Sb()
{

  new_S_solution.setZero();

  for (int id = 0; id < CELL_NUMBER; ++id)
  {
    Cell cell(id);
    map<DIRECTION, int>::iterator it;
    for (it = cell.neighbour_ids_.begin(); it != cell.neighbour_ids_.end(); it++)
    {

      double K_component = helper_vertical_ptr->compute_K(cell, it->first);
      double saturation = helper_vertical_ptr->get_saturation(cell, it->first, old_p_solution, old_S_solution);
      double mobility = helper_vertical_ptr->get_mobility(saturation);
      double fraction = helper_vertical_ptr->get_fraction(saturation);

      double wind;

      // Face contribution
      if (it->second == OUTSIDE_CELL_ID)
        wind = (helper_vertical_ptr->get_boundary_value_p(cell, it->first) - old_p_solution(cell.id_))/(H/2.);
      else
        wind = (old_p_solution(it->second) - old_p_solution(cell.id_))/H;

      new_S_solution(cell.id_) += fraction*mobility*K_component*wind*H*DELTA_T/Area; 

    }

    // Volume contribution
    new_S_solution(cell.id_) += old_S_solution(cell.id_);


  }
  old_S_solution = new_S_solution;

}


void ProblemVertical::output_results(int cycle)
{


  string p_name = "p";
  string S_name = "S";
  OutputManager output_manager(cycle);

  output_manager.scalar_output(old_p_solution, p_name);
  output_manager.scalar_output(new_S_solution, S_name);  

 
}



void ProblemVertical::post_processing()
{

}


ProblemVertical::~ProblemVertical()
{
  delete helper_vertical_ptr;
}
