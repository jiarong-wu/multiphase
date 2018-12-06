#include "problem_flow.h"


ProblemFlow::ProblemFlow()
:
new_p_solution(CELL_NUMBER), 
new_S_solution(CELL_NUMBER), 
old_p_solution(CELL_NUMBER), 
old_S_solution(CELL_NUMBER), 
system_rhs(CELL_NUMBER),
system_matrix(CELL_NUMBER, CELL_NUMBER)
{
  // Do we need to initialize those vectors and matrix with all zero? It seems to be automatically done.
}


void ProblemFlow::run()
{
  interpolate_S(old_S_solution);
  interpolate_p(old_p_solution);
  run_one_step();
}


void ProblemFlow::run_one_step()
{
  assemble_system_p();
  solve_p();
  compute_S();
  output_results();

}


void ProblemFlow::assemble_system_p()
{

  int counter = 0;
  for (int id = 0; id < CELL_NUMBER; ++id)
  {
    Cell cell(id);

    map<DIRECTION, int>::iterator it;
    for (it = cell.neighbour_ids_.begin(); it != cell.neighbour_ids_.end(); it++)
    {
      double K_component = compute_K(cell, it->first);
      double saturation = get_saturation(cell, it->first, old_p_solution, old_S_solution);
      double mobility = get_mobility(saturation);

      mobility = 1;
      // K_component = 1;

      // if (saturation > 0.9)
      // {
      //   // We can have a test here. When will saturation be greater than 0.9? Must be left boundary.
      //   cout << "(" << cell.face_centers_[it->first][0] << " , " << cell.face_centers_[it->first][1] << ")" << endl << endl; 
      // }

      cout << "K_component value is " << K_component << endl; 



      // Face contribution
      if (it->second == OUTSIDE_CELL_ID)
      {
        double p_boundary_value = get_boundary_value_p(cell, it->first);
        // cout << p_boundary_value << endl;
        system_rhs(cell.id_) += -2*K_component*mobility*p_boundary_value;
        triplet_list.push_back(T(cell.id_, cell.id_, -2*K_component*mobility));
      }
      else
      {
        triplet_list.push_back(T(cell.id_, cell.id_, -1*K_component*mobility));
        triplet_list.push_back(T(cell.id_, it->second, 1*K_component*mobility));
      }
    }

    // Volume contribution
    // system_rhs(cell.id_) += -1*Area;
  }

  // This is a very efficient way of assembling the matrix A. The function is provided by the external library Eigen.
  system_matrix.setFromTriplets(triplet_list.begin(), triplet_list.end());

}


void ProblemFlow::solve_p()
{

  // SimplicialCholesky<SparseMatrix<double>> chol(A);  // performs a Cholesky factorization of A
  // VectorXd x = chol.solve(b);         // use the factorization to solve for the given right hand side

  SparseLU<SparseMatrix<double>>   solver;
  solver.analyzePattern(system_matrix); 
  solver.factorize(system_matrix); 
  new_p_solution = solver.solve(system_rhs); 

  cout << MatrixXd(system_matrix) << endl;
  // cout << new_p_solution << endl;
  // cout << system_rhs << endl;
  // cout << new_p_solution.maxCoeff() << endl;

}


void ProblemFlow::compute_S()
{

  // VectorXd test_v(10);
  // test_v(2) = 1;
  // cout << test_v << endl << endl;
  // VectorXd test_v_1(10);
  // test_v_1(3) = 2;
  // test_v = test_v_1;
  // cout << test_v << endl;


}


void ProblemFlow::output_results()
{
  OutputManager output_manager;
  output_manager.scalar_output(new_p_solution);
}




