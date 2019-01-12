

#ifndef PROBLEM_VERTICAL_H
#define PROBLEM_VERTICAL_H

#include "common_definitions.h"
#include "cell.h"
#include "output_manager.h"
#include "problem.h"
#include "helper_vertical.h"

using namespace VerticalModel;

class ProblemVertical : public Problem
{
	public:
    /* Constructor. */
    ProblemVertical(); 
    ~ProblemVertical();     
    /* Run this problem. */
    void run(); 
    	
	private:
         

    VectorXd new_Pc_solution, new_Sc_solution, old_Pc_solution, old_Sc_solution, 
             new_Pb_solution, new_Sb_solution, old_Pb_solution, old_Sb_solution, 
             old_Pcap_solution, new_Pcap_solution, system_rhs, flag_vector;

    SparseMatrix<double> system_matrix;
    vector<T> triplet_list;


    HelperVertical* helper_vertical_ptr;

    double time;



    int InjWell; //cell ids of the injection and extraction wells. 
    int ProWell;

    // injection term; unit is m3/s/m2 = m/s. 
    double Qc_int;
    double Qb_int;

    // extraction volume; unit is m3/s/m2 = m/s. 
    double Qc_ext;
    double Qb_ext; 

    void run_one_step();

    void assemble_system_Pb();
    void solve_Pb();
    void compute_Pcap();     
    void compute_Pc();    
    void compute_Sc();    
    void compute_Sb();
    void update();
    void output_results(int cycle);	
    void post_processing();
    void compare_with_analytical_solution();


};

#endif