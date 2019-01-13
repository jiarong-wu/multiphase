/**
 * \class ProblemPoisson
 * 
 * This is the poisson problem class. In this class we set up a poisson problem
 * with analytical solution and test the code function.
 *
 */

#ifndef PROBLEM_POISSON_H
#define PROBLEM_POISSON_H

#include "common_definitions.h"
#include "cell.h"
#include "output_manager.h"
#include "problem.h"
#include "helper_poisson.h"
#include "helper_adapt.h"

class ProblemPoisson : public Problem 
{
  public:
    /* Constructor. */
    ProblemPoisson(); 
    ProblemPoisson(int CELL_NUMBER, int AdaptFlag, double X_SOURCE, double Y_SOURCE);   
    /* Run this problem. */
    void run(); 
        
  protected:
         
    VectorXd x, b;
    SparseMatrix<double> A;
    vector<T> triplet_list;

    /** The flag for adaptive grid. */ 
    int AdaptFlag_; 
    /** The x position of the source term. */ 
    double x_source_; 
    /** The y position of the source term. */
    double y_source_; 
    /** The id of the source cell. */
    int id_source_; 
    
    /** The helper for problem related information */
    HelperPoisson helper_poisson;

    /** The helper for adaptive mesh related information */
    HelperAdapt helper_adapt;


    void pre_processing();
    void assemble_system();
    void solve();
    void output_results();  
    void post_processing(); 
};

#endif