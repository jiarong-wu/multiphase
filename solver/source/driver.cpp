/**
 * \author Tianju Xue
 * \author Yiheng Tao
 * \author Jiarong Wu
 * \bug Test bug  
 * \mainpage The APC524 project
 * \section sec1 Introduction
 * \section sec2 Workflow
 */

#include "problem.h"
#include "problem_poisson.h"
#include "problem_flow.h"
#include "problem_vertical.h"
#include "cell.h"

int main()
{
   
  if (FLAG == 0)
  {
    // We now have to manually choose modes, and the refine factor is hardcoded into 
    // problem_poisson.cpp Remember there is also a part in output manager that needs 
    // to be changed if changing REFINE_FACTOR or the source position
    ProblemPoisson poisson;
    // ProblemPoisson poisson(CELL_NUMBER, 1, 0.5, 0.5);
    poisson.run(); 
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