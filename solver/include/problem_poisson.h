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
    ~ProblemPoisson();     
    /* Run this problem. */
    void run(); 
    	
	private:
         
    VectorXd x, b;
    SparseMatrix<double> A;
    vector<T> triplet_list;

    HelperPoisson* helper_poisson_ptr;


    void pre_processing();
    void assemble_system();
    void solve();
    void output_results();	
    void post_processing();	


};

#endif