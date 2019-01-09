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
    // ~ProblemPoisson();     
    /* Run this problem. */
    void run(); 
        
    private:
         
    VectorXd x, b;
    SparseMatrix<double> A;
    vector<T> triplet_list;
    /* The flag for adaptive grid. */
    int AdaptFlag_;
    /* The position of the source term.*/
    double x_source_;
    double y_source_;
    /* The id of the source cell. */
    int id_source_;

    HelperPoisson helper_poisson;
    HelperAdapt helper_adapt;


    void pre_processing();
    void assemble_system();
    void solve();
    void output_results();  
    void post_processing(); 


};

// class ProblemPoisson : public Problem 
// {
// 	public:
//     /* Constructor. */
//     ProblemPoisson(); 
//     ~ProblemPoisson();     
//     /* Run this problem. */
//     void run(); 
    	
// 	private:
         
//     VectorXd x, b;
//     SparseMatrix<double> A;
//     vector<T> triplet_list;

//     HelperPoisson* helper_poisson_ptr;

//     void pre_processing();
//     void assemble_system();
//     void solve();
//     void output_results();	
//     void post_processing();	


// };

#endif