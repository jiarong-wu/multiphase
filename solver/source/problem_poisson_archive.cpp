// #include "problem_poisson.h"
// #include "helper_poisson.h"
// #include "helper_adapt.h"
// #include "cell.h"

// // The default constructor
// ProblemPoisson::ProblemPoisson()
// :
// x(CELL_NUMBER),
// b(CELL_NUMBER),
// A(CELL_NUMBER, CELL_NUMBER),
// AdaptFlag_(0),
// helper_poisson(),
// helper_adapt()
// {
//   HelperPoisson helper_poisson();
// }

// // Another constructor that passes in flags
// ProblemPoisson::ProblemPoisson(int CELL_NUMBER, int AdaptFlag, double X_SOURCE, double Y_SOURCE)
// :
// x(CELL_NUMBER),
// b(CELL_NUMBER),
// A(CELL_NUMBER, CELL_NUMBER),
// x_source_(X_SOURCE),
// y_source_(Y_SOURCE),
// AdaptFlag_(AdaptFlag)
// {
//   HelperPoisson helper_poisson();
//   HelperAdapt helper_adapt();
// }

// void ProblemPoisson::run()
// {

//   pre_processing();
//   assemble_system();
//   solve();
//   output_results();
//   post_processing();
// }

// void ProblemPoisson::pre_processing()
// {
//   // Do we have anything to set up?
// }

// void ProblemPoisson::assemble_system()
// {
//   // Later make REFINE_FACTOR a changeable parameter
//   double REFINE_FACTOR = 1.2;
//   // Iterate over cells to first: find source
//   // Should add "if given the source coordinate"
//   id_source_ = helper_adapt.find_source(x_source_, y_source_);
//   // Iterate over cells to assemble the system
//   for (int id = 0; id < CELL_NUMBER; ++id)
//   {  
//     // if AdaptFlag is on, use adaptive method to calculate cell coordinate
//     Cell* cell_ptr;
//     if (AdaptFlag_ == 0)
//       {
//         cout << "If this is run it's right!" << endl;
//         Cell cell(id);
//         cell_ptr = &cell;
//       }
//     else 
//       {
//         cout << "If this is run it's wrong!" << endl;
//         Cell cell(id, id_source_, x_source_, y_source_, REFINE_FACTOR);
//         cell_ptr = &cell;
//       }
//     map<DIRECTION, int>::iterator it;
//     // Iterate over faces
//     for (it = cell_ptr->neighbour_ids_.begin(); it != cell_ptr->neighbour_ids_.end(); it++)
//     {
//       // Face contribution
//       if (it->second == OUTSIDE_CELL_ID)
//       {
//         cout << "Breaking point 1.1!" << endl;
//         triplet_list.push_back(T(cell_ptr->id_, cell_ptr->id_, -2));
//         double x_boundary_value = helper_poisson.get_boundary_value_x(*cell_ptr, it->first);
//         b(cell_ptr->id_) += -2*x_boundary_value;
//       }
//       else
//       {
//         cout << "Breaking point 1.2!" << endl;
//         triplet_list.push_back(T(cell_ptr->id_, cell_ptr->id_, -1));
//         triplet_list.push_back(T(cell_ptr->id_, it->second, 1));
//       }
//     }
//     // Volume contribution
//     cout << "Breaking point 2!" << endl;
//     double rhs_f = helper_poisson.rhs_function(cell_ptr->cell_center_);
//     b(cell_ptr->id_) += -rhs_f*Area;   
//     // This is a very efficient way of assembling the matrix A. The function is provided by the external library Eigen.
//     cout << "Breaking point 3!" << endl;
//     A.setFromTriplets(triplet_list.begin(), triplet_list.end());
//   }
// }

// void ProblemPoisson::solve()
// {

//   // SimplicialCholesky<SparseMatrix<double>> chol(A);  // performs a Cholesky factorization of A
//   // VectorXd x = chol.solve(b);         // use the factorization to solve for the given right hand side

//   SparseLU<SparseMatrix<double>>   solver;
//   solver.analyzePattern(A); 
//   solver.factorize(A); 
//   x = solver.solve(b); 

//   // cout << MatrixXd(A) << endl;
//   // cout << x << endl;
//   // cout << b << endl;
//   // cout << x.maxCoeff() << endl;
// }



// void ProblemPoisson::output_results()
// {
//   OutputManager output_manager(0);
//   output_manager.scalar_output(x, "u");
// }



// void ProblemPoisson::post_processing()
// {
//   double error = 0;
//   for (int i = 0; i < CELL_NUMBER; ++i)
//   {
//     Cell cell(i);
//     error += pow(helper_poisson.boundary_function_x(cell.cell_center_) - x(i), 2)*Area; 
//   }
//   error = sqrt(error);

//   // L2 norm should convege in second order w.r.t. mesh size
//   cout << "L2 norm error is " << error << endl;
// }


