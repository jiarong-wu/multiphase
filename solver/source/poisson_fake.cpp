#include "problem_poisson.h"
#include "helper_poisson.h"
#include "helper_adapt.h"
#include "cell.h"

// The default constructor
ProblemPoisson::ProblemPoisson()
:
x(CELL_NUMBER),
b(CELL_NUMBER),
A(CELL_NUMBER, CELL_NUMBER),
AdaptFlag_(0),
helper_poisson(),
helper_adapt()
{
  HelperPoisson helper_poisson();
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
  HelperPoisson helper_poisson();
  HelperAdapt helper_adapt();
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
    // if (AdaptFlag_ == 0)
    //   {
    //     cout << "If this is run it's right!" << endl;
    //     Cell cell(id);
    //     cell_ptr = &cell;
    //   }
    // else 
    //   {
    //     cout << "If this is run it's wrong!" << endl;
    //     Cell cell(id, id_source_, x_source_, y_source_, REFINE_FACTOR);
    //     cell_ptr = &cell;
    //   }
}

void ProblemPoisson::solve()
{
  for (int id = 0; id < CELL_NUMBER; ++id)
  {
    x[id] = 0;
  }
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
    error += pow(helper_poisson.boundary_function_x(cell.cell_center_) - x(i), 2)*Area; 
  }
  error = sqrt(error);

  // L2 norm should convege in second order w.r.t. mesh size
  cout << "L2 norm error is " << error << endl;
}