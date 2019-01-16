/**
 * \author Tianju Xue
 * \author Yiheng Tao
 * \author Jiarong Wu
 * \bug No bug report currently. 
 * \note The code is still under development.
 * \brief This is a Finite Volume solver for generic problems. The goal is to solve the 
 * multi-phase flow problems with potential applications in carbon capture and storage.
 * However, it is also possible to use the solve to solve other types of equations. A demo
 * example could the Poisson's equaiont \f$-\Delta u = f\f$. 
 * \mainpage The APC524 project
 */

 // \section sec1 Background
 

#include "problem.h"
#include "problem_poisson.h"
#include "problem_flow.h"
#include "problem_vertical.h"
#include "cell.h"

int main(int argc, char *argv[])
{
  if (argc != 3) {
    printf("USAGE: %s <problem type> <adaptive option>\n", argv[0]);
    exit(1);
  }
  const int FLAG = atoi(argv[1]);
  const int ADAPT = atoi(argv[2]);

  if (FLAG == 0)
  {
    // We now have to manually choose modes, and the refine factor is hardcoded into 
    // problem_poisson.cpp Remember there is also a part in output manager that needs 
    // to be changed if changing REFINE_FACTOR or the source position
    ProblemPoisson *poisson;
    if (ADAPT == 0)
      poisson = new ProblemPoisson;
    else if (ADAPT == 1)
      poisson = new ProblemPoisson(CELL_NUMBER, 1, 0.5, 0.5);
    poisson->run();
    delete poisson; 
  }
  else if (FLAG == 1)
  {
    ProblemFlow two_phase_flow;
    two_phase_flow.run();
  }
  else
  {
    ProblemVertical VE_model;
    VE_model.run();
  }

  
  return 0;

}