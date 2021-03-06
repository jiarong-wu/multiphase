#ifndef COMMON_DEFINITIONS_H
#define COMMON_DEFINITIONS_H

#include <iostream>
#include <map>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <time.h>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>

using namespace Eigen;
using namespace std;

/* Pre-define some data type. Just some aliases. */
typedef Triplet<double> T;
typedef vector<double> vec;
typedef vector<vec> matrix;
typedef vector<int> int_vec;

/* Some common constant variables are defined in this file. */

/* To do: Try to avoid global variables as much as possible. */
/* Some of the constants should be made into the Cell class static member variables */
/* Or we can try to establish a namespace to store those variables */


const int FLAG = 0;

/* How many times we want to refine the mesh.
   For each time, one square cell is divided into four. */
const int DIVISION = 5;

/* The dimention of the problem 
   The code is not written in a dimensional independent way so keep this unchanged.
   A dimensional independent code can be finished by the help of C++ function template */
const int DIM = 2;

/* Total number of elements*/
const int CELL_NUMBER = pow(4, DIVISION);


/* How many elements along one line */
const int LINE_CELL_NUMBER = pow(2, DIVISION);

/* Total number of dof */
const int TOTAL_DOF = CELL_NUMBER;


//  {X_1} is the lower left point while {X_2} is the upper right point of the physical domain  
const vec X_1{0, 0};
const vec X_2{1, 1};
/* Cell size */
const double H = (X_2[0] - X_1[0])/LINE_CELL_NUMBER;
/* Cell area */
const double Area = H*H;  



namespace CellModel
{

  //  A square cell has 4 faces 
  // const int FACE_PER_ELE = 4;

  /* A square cell has 4 vertices */
  const int VERTICES_PER_CELL = 4;

  /* Along one line, there are 2 vertices for a square cell  */
  const int VERTICES_PER_LINE = 2;

  /* Use -1 to represent a ``cell'' that is out of the boundary */
  const int OUTSIDE_CELL_ID = -1;

  /* Used in outputing VTK files */
  const int CELL_TYPE = 9;

  /* {WEST} = 0, {EAST} = 1, {SOUTH} = 2, {NORTH} = 3, {NUMBER_DIRECTIONS} = 4 */
  enum DIRECTION {WEST, EAST, SOUTH, NORTH, NUMBER_DIRECTIONS};

}



namespace PoissonModel
{

}


namespace FlowModel
{
  const double DELTA_T = 0.001;
  const double mu = 0.2;  
}



namespace VerticalModel
{


  const double Lx = (X_2[0] - X_1[0]);

  const double mu_c = 4.25e-5;

  const double mu_b = 3e-4;

  const double grav = 9.8;

  const double rho_c = 710; 

  const double rho_b = 1000;

  const double rg_c = rho_c * grav;

  const double rg_b = rho_b * grav;

  const double sb_res = 0;

  const double krc_star = 1;

  const double formation_thickness = 50; 

  const double porosity = 0.03; 

  const double coarse_porosity = porosity*formation_thickness;

  const double Pcap_entry = 0; 

  const double Lz = 50.;

  const double k = 1e-12; 

  const double K = k*Lz;

  const double Nz = 100.;

  const double dz = Lz/Nz;

  /* For generating random permeability tensor */
  const int RONDOM_N = 1000;

  const double DELTA_T = 8640;

}




#endif
