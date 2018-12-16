#ifndef PROBLEM_FLOW_H
#define PROBLEM_FLOW_H

#include "common_definitions.h"
#include "cell.h"
#include "output_manager.h"
#include "problem.h"
#include "helper_flow.h"

class ProblemFlow : public Problem
{
	public:
    /* Constructor. */
    ProblemFlow(); 
    ~ProblemFlow();     
    /* Run this problem. */
    void run(); 
    	
	private:
         
    VectorXd new_p_solution, new_S_solution, old_p_solution, old_S_solution, system_rhs, flag_vector;
    SparseMatrix<double> system_matrix;
    vector<T> triplet_list;

    HelperFlow* helper_flow_ptr;

    double time;

    void run_one_step();

    void assemble_system_p();
    void solve_p();
    void compute_S();
    void output_results(int cycle);	
    void post_processing();


};

#endif