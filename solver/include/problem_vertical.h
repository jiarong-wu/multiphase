#ifndef PROBLEM_VERTICAL_H
#define PROBLEM_VERTICAL_H

#include "common_definitions.h"
#include "cell.h"
#include "output_manager.h"
#include "problem.h"
#include "helper_vertical.h"

class ProblemVertical : public Problem
{
	public:
    /* Constructor. */
    ProblemVertical(); 
    ~ProblemVertical();     
    /* Run this problem. */
    void run(); 
    	
	private:
         
    VectorXd new_p_solution, new_S_solution,  old_p_solution, old_S_solution, system_rhs, flag_vector;
    SparseMatrix<double> system_matrix;
    vector<T> triplet_list;

    HelperVertical* helper_vertical_ptr;

    double time;

    void run_one_step();

    void assemble_system_Pb();
    void solve_Pb();
    void compute_Pc();    
    void compute_Sc();    
    void compute_Sb();
    void output_results(int cycle);	
    void post_processing();


};

#endif