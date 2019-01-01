#ifndef PROBLEM_POISSON_H
#define PROBLEM_POISSON_H

#include "common_definitions.h"
#include "cell.h"
#include "output_manager.h"
#include "problem.h"
#include "helper_poisson.h"

class ProblemPoisson : public Problem 
{
	public:

    /* Constructor. */
    ProblemPoisson(); 
    ProblemPoisson(int CELL_NUMBER, int AdaptFlag, double X_SOURCE, double Y_SOURCE);
    ~ProblemPoisson();     
    /* Run this problem. */
    void run(); 
    	
	private:
         
    VectorXd x, b;
    SparseMatrix<double> A;
    vector<T> triplet_list;
    /* The flag for adaptive grid. */
    int AdaptFlag_ = 0;
    /* The position of the source term.*/
    double x_source_ = 0;
    double y_source_ = 0;
    /* The id of the source cell. */
    int id_source_ = -1;

    HelperPoisson* helper_poisson_ptr;

    void pre_processing();
    void assemble_system();
    void solve();
    void output_results();	
    void post_processing();	


};

#endif