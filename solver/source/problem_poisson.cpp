#include "problem_poisson.h"

// The default constructor
ProblemPoisson::ProblemPoisson()
:
x(CELL_NUMBER),
b(CELL_NUMBER),
A(CELL_NUMBER, CELL_NUMBER)
{
  helper_poisson_ptr = new HelperPoisson;
}

// Another constructor that passes in flags
ProblemPoisson::ProblemPoisson(int CELL_NUMBER, int AdaptFlag, double X_SOURCE, double Y_SOURCE)
:
x(CELL_NUMBER),
b(CELL_NUMBER),
A(CELL_NUMBER, CELL_NUMBER),
x_source_(X_SOURCE),
y_source_(Y_SOURCE),
AdaptFlag_(AdaptFlag)
{
  helper_poisson_ptr = new HelperPoisson;
  helper_adapt_ptr = new HelperAdapt;
}

void ProblemPoisson::run()
{
  pre_processing();
  assemble_system();
  solve();
  output_results();
  post_processing();
}

void ProblemPoisson::pre_processing()
{
  // Do we have anything to set up?
}

void ProblemPoisson::assemble_system()
{
  // Iterate over cells to first: find source
  // Should add "if given the source coordinate"
  id_source_ = helper_adapt_ptr->find_source(x_source_, y_source_);
  // Iterate over cells to assemble the system
  for (int id = 0; id < CELL_NUMBER; ++id)
  {
    // if AdaptFlag is on, use adaptive method to calculate cell coordinate
    if (AdaptFlag_ == 0)
      Cell cell(id);
    else 
      Cell cell(id, id_source_, x_source_, y_source_);
    map<DIRECTION, int>::iterator it;
    // Iterate over faces
    for (it = cell.neighbour_ids_.begin(); it != cell.neighbour_ids_.end(); it++)
    {
      // Face contribution
      if (it->second == OUTSIDE_CELL_ID)
      {
        triplet_list.push_back(T(cell.id_, cell.id_, -2));
        double x_boundary_value = helper_poisson_ptr->get_boundary_value_x(cell, it->first);
        b(cell.id_) += -2*x_boundary_value;
      }
      else
      {
        triplet_list.push_back(T(cell.id_, cell.id_, -1));
        triplet_list.push_back(T(cell.id_, it->second, 1));
      }
    }
    // Volume contribution
    double rhs_f = helper_poisson_ptr->rhs_function(cell.cell_center_);
    b(cell.id_) += -rhs_f*Area;   
    // This is a very efficient way of assembling the matrix A. The function is provided by the external library Eigen.
    A.setFromTriplets(triplet_list.begin(), triplet_list.end());
  }
}

void ProblemPoisson::solve()
{

  // SimplicialCholesky<SparseMatrix<double>> chol(A);  // performs a Cholesky factorization of A
  // VectorXd x = chol.solve(b);         // use the factorization to solve for the given right hand side

  SparseLU<SparseMatrix<double>>   solver;
  solver.analyzePattern(A); 
  solver.factorize(A); 
  x = solver.solve(b); 

  // cout << MatrixXd(A) << endl;
  // cout << x << endl;
  // cout << b << endl;
  // cout << x.maxCoeff() << endl;
}



void ProblemPoisson::output_results()
{
  OutputManager output_manager(0);
  output_manager.scalar_output(x, "u");
}



void ProblemPoisson::post_processing()
{
  double error = 0;
  for (int i = 0; i < CELL_NUMBER; ++i)
  {
    Cell cell(i);
    error += pow(helper_poisson_ptr->boundary_function_x(cell.cell_center_) - x(i), 2)*Area; 
  }
  error = sqrt(error);

  // L2 norm should convege in second order w.r.t. mesh size
  cout << "L2 norm error is " << error << endl;
}





ProblemPoisson::~ProblemPoisson()
{
  delete helper_poisson_ptr;
  delete helper_adapt_ptr;
}
