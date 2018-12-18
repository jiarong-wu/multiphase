#include "problem_vertical.h"


ProblemVertical::ProblemVertical()
:
new_Pc_solution(CELL_NUMBER), 
new_Sc_solution(CELL_NUMBER), 
old_Pc_solution(CELL_NUMBER), 
old_Sc_solution(CELL_NUMBER), 

new_Pb_solution(CELL_NUMBER), 
new_Sb_solution(CELL_NUMBER), 
old_Pb_solution(CELL_NUMBER), 
old_Sb_solution(CELL_NUMBER), 

new_Pcap_solution(CELL_NUMBER),
old_Pcap_solution(CELL_NUMBER),

system_rhs(CELL_NUMBER),
flag_vector(CELL_NUMBER),
system_matrix(CELL_NUMBER, CELL_NUMBER),
time(0)
{
  helper_vertical_ptr = new HelperVertical;
}


void ProblemVertical::run()
{

  helper_vertical_ptr->initial_condition_Sc(old_Sc_solution);
  helper_vertical_ptr->initial_condition_Sb(old_Sb_solution, old_Sc_solution);  
  helper_vertical_ptr->initial_condition_Pb(old_Pb_solution);
  helper_vertical_ptr->initial_condition_Pcap(old_Pcap_solution, old_Sc_solution);
  helper_vertical_ptr->initial_condition_Pc(old_Pc_solution, old_Pb_solution, old_Pcap_solution);

  cout << "Total number of dofs is " << CELL_NUMBER << endl;

  for (int i = 0; i < 50; ++i)
  {

    time += DELTA_T;

    triplet_list.clear();
    system_rhs.setZero();
    system_matrix.setZero();

    cout << "This is time step " << i << endl;
    run_one_step();
    if (i%10 == 0)
    {
      output_results(i/10);
    }

    // assert(("Numerical issue! Saturation is out of bound!",  new_p_solution.maxCoeff() < 2 &&  new_p_solution.maxCoeff() >  -1));
  }

  post_processing();
}


void ProblemVertical::run_one_step()
{
  assemble_system_Pb();
  solve_Pb();
  compute_Pc();
  compute_Sc();
  compute_Sb();
  compute_Pcap(); 
  update();  
}


void ProblemVertical::assemble_system_Pb()
{

  int counter = 0;
  for (int id = 0; id < CELL_NUMBER; ++id)
  {
    Cell cell(id);

    map<DIRECTION, int>::iterator it;
    for (it = cell.neighbour_ids_.begin(); it != cell.neighbour_ids_.end(); it++)
    {

      double saturation = helper_vertical_ptr->get_saturation(cell, it->first, old_Sc_solution, old_Pb_solution, old_Pc_solution, old_Pcap_solution);

      // if (saturation > 1e-5)
      // {
      //   cout << "cell id is " << cell.id_ << endl;        
      //   cout << "saturation is " << saturation << endl;
      // }


      double coeff_b = helper_vertical_ptr->get_coeff_b(saturation);
      double coeff_c = helper_vertical_ptr->get_coeff_c(saturation);

      // if (saturation > 1e-5)
      // {
      //   cout << "saturation is " << saturation << " and coeff_b is " << coeff_b << " and coeff_c is " << coeff_c << endl;
      // }


      double K_component = helper_vertical_ptr->compute_K(cell, it->first);

      // if (saturation > 0.9)
      // {
      //   // We can have a test here. When will saturation be greater than 0.9? Must be left boundary.
      //   cout << "(" << cell.face_centers_[it->first][0] << " , " << cell.face_centers_[it->first][1] << ")" << endl << endl; 
      // }


      // Face contribution
      if (it->second == OUTSIDE_CELL_ID)
      {
        double Pb_boundary_value   = helper_vertical_ptr->get_boundary_value_Pb(cell, it->first);

        double Pcap_ghost_value = helper_vertical_ptr->get_ghost_value_Pcap(cell, it->first, old_Pcap_solution);
        double Pcap_self_value = old_Pcap_solution(cell.id_);


        assert(("Bug! Ghost value of Pb should be the same as inside", fabs(Pcap_ghost_value - Pcap_self_value) < 1e-10));

        system_rhs(cell.id_) += -2*(coeff_b + coeff_c)*Pb_boundary_value;
        triplet_list.push_back(T(cell.id_, cell.id_, -2*(coeff_b + coeff_c)));

        system_rhs(cell.id_) += -coeff_c*Pcap_ghost_value;
        system_rhs(cell.id_) +=  coeff_c*Pcap_self_value;

      }
      else
      {
     
        triplet_list.push_back(T(cell.id_, cell.id_,  -(coeff_b + coeff_c)));
        triplet_list.push_back(T(cell.id_, it->second, (coeff_b + coeff_c)));

        system_rhs(cell.id_) +=  coeff_c*old_Pcap_solution(cell.id_);            
        system_rhs(cell.id_) += -coeff_c*old_Pcap_solution(it->second);            

      }

    }

    // Volume contribution
    system_rhs(cell.id_) += 0*Area;


    // if (cell.id_ == 1*LINE_CELL_NUMBER + 1)
    // {
    //   double Q = 8e-7;
    //   system_rhs(cell.id_) += -Q*Area;
    // }


    if (cell.id_ == 685)
    {
      double Q = 8e-7;
      system_rhs(cell.id_) += -Q*Area;
    }

  }

  // This is a very efficient way of assembling the matrix A. The function is provided by the external library Eigen.
  system_matrix.setFromTriplets(triplet_list.begin(), triplet_list.end());

}


void ProblemVertical::solve_Pb()
{

  // SimplicialCholesky<SparseMatrix<double>> chol(A);  // performs a Cholesky factorization of A
  // VectorXd x = chol.solve(b);         // use the factorization to solve for the given right hand side

  SparseLU<SparseMatrix<double>>   solver;
  solver.analyzePattern(system_matrix); 
  solver.factorize(system_matrix); 
  new_Pb_solution = solver.solve(system_rhs); 

  // cout << setprecision(8) << setfill(' ') << MatrixXd(system_matrix) << endl << endl;
  // cout << new_p_solution << endl;
  // cout << "rhs is " << endl << setprecision(4) << system_rhs << endl << endl;
  // cout << new_p_solution.maxCoeff() << endl;



}




void ProblemVertical::compute_Pc()
{
  helper_vertical_ptr->get_Pc(new_Pc_solution, new_Pb_solution, old_Pcap_solution);


  // cout << "Pb is " <<  endl << setprecision(10) << new_Pb_solution   << endl << endl;
  // cout << "Pcap is" << endl << setprecision(10) << new_Pcap_solution << endl << endl;
  // cout << "Pc is" <<   endl << setprecision(10) << new_Pc_solution   << endl << endl;

}




void ProblemVertical::compute_Sc()
{

  new_Sc_solution.setZero();

  for (int id = 0; id < CELL_NUMBER; ++id)
  {
    Cell cell(id);
    map<DIRECTION, int>::iterator it;
    for (it = cell.neighbour_ids_.begin(); it != cell.neighbour_ids_.end(); it++)
    {

      double K_component = helper_vertical_ptr->compute_K(cell, it->first);

      double saturation = helper_vertical_ptr->get_saturation(cell, it->first, old_Sc_solution, old_Pb_solution, old_Pc_solution, old_Pcap_solution);

      double coeff_c = helper_vertical_ptr->get_coeff_c(saturation);

      double wind;

      // Face contribution
      if (it->second == OUTSIDE_CELL_ID)
      {
        double Pc_ghost_value = helper_vertical_ptr->get_ghost_value_Pc(cell, it->first, new_Pb_solution, new_Pcap_solution);
        double Pc_self_value = new_Pc_solution(cell.id_);
        wind = (Pc_ghost_value - Pc_self_value)/H;
      }
      else
      {        
        double Pc_neighbor_value = new_Pc_solution(it->second);
        double Pc_self_value = new_Pc_solution(cell.id_);
        wind = (Pc_neighbor_value - Pc_self_value)/H;
      }

      new_Sc_solution(cell.id_) += coeff_c*wind*H*DELTA_T/Area/coarse_porosity; 

    }

    // Volume contribution
    new_Sc_solution(cell.id_) += old_Sc_solution(cell.id_);

    // if (cell.id_ == 1*LINE_CELL_NUMBER + 1)
    // {
    //   double Q = 8e-7;
    //   new_Sc_solution(cell.id_) += DELTA_T*Q/coarse_porosity;
    // }

    if (cell.id_ == 685)
    {
      double Q = 8e-7;
      new_Sc_solution(cell.id_) += DELTA_T*Q/coarse_porosity;
    }


  }


  // cout << setprecision(10) << new_Sc_solution  << endl << endl;

}



void ProblemVertical::compute_Sb()
{
  helper_vertical_ptr->get_Sb(new_Sb_solution, new_Sc_solution);
}


void ProblemVertical::compute_Pcap()
{
  helper_vertical_ptr->get_Pcap(new_Pcap_solution, new_Sc_solution);
}


void ProblemVertical::update()
{
  old_Sb_solution   = new_Sb_solution;
  old_Sc_solution   = new_Sc_solution;
  old_Pb_solution   = new_Pb_solution;  
  old_Pc_solution   = new_Pc_solution;
  old_Pcap_solution = new_Pcap_solution; 
}


void ProblemVertical::output_results(int cycle)
{

  string Pc_name = "Pc";
  string Sc_name = "Sc";
  string Pb_name = "Pb";
  string Sb_name = "Sb";

  OutputManager output_manager(cycle);

  output_manager.scalar_output(new_Pc_solution, Pc_name);
  output_manager.scalar_output(new_Sc_solution, Sc_name);  
  output_manager.scalar_output(new_Pb_solution, Pb_name);
  output_manager.scalar_output(new_Sb_solution, Sb_name);  

 
}



void ProblemVertical::post_processing()
{

  for (int i = 0; i < CELL_NUMBER; ++i)
  {
    assert(("Bug! Saturation is not in a correct range.", old_Sc_solution(i) < 2 && old_Sc_solution(i) > -1));
  }

}


ProblemVertical::~ProblemVertical()
{
  delete helper_vertical_ptr;
}




