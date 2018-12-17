#ifndef HELPER_VERTICAL_H
#define HELPER_VERTICAL_H

#include "common_definitions.h"
#include "helper.h"

class HelperVertical: public Helper
{
  public:
    

    void initial_condition_Sc(VectorXd &old_Sc_solution);

    void initial_condition_Sb(VectorXd &old_Sb_solution, VectorXd &old_Sc_solution);

    void initial_condition_Pb(VectorXd &old_Pb_solution);

    void initial_condition_Pcap(VectorXd &Pcap_solution,  VectorXd &old_Sc_solution);

    void initial_condition_Pc(VectorXd &old_Pc_solution, VectorXd &old_Pb_solution, VectorXd &Pcap_solution);

    void get_Pcap(VectorXd &Pcap_solution, VectorXd &old_Sc_solution);

    void get_Pc(VectorXd &new_Pc_solution, VectorXd &new_Pb_solution, VectorXd &Pcap_solution);

    void get_Sb(VectorXd &old_Sb_solution, VectorXd &old_Sc_solution);

    double compute_K(Cell &cell, DIRECTION direction);

    bool determine_upwind(Cell &cell, DIRECTION direction, VectorXd &Pb_solution, VectorXd &Pc_solution, VectorXd &Pcap_solution);

    double get_saturation(Cell &cell, DIRECTION direction, VectorXd &Sc_solution, 
                          VectorXd &Pb_solution, VectorXd &Pc_solution, VectorXd &Pcap_solution);
  
    double get_coeff_b(double Sc);
 
    double get_coeff_c(double Sc);

    double boundary_function_Pb(vec &point);

    double get_ghost_value_Pb(Cell &cell, DIRECTION direction, VectorXd &Pb_solution);

    double get_ghost_value_Pcap(Cell &cell, DIRECTION direction, VectorXd &Pcap_solution);

    double get_ghost_value_Pc(Cell &cell, DIRECTION direction, VectorXd &Pb_solution, VectorXd &Pcap_solution);

    double get_boundary_value_Pb(Cell &cell, DIRECTION direction);

};

#endif