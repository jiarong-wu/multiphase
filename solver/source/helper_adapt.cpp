#include "helper_adapt.h"
#include "common_definitions.h"
#include "cell.h"
/* The helper function that returns the source cell id. */
int HelperAdapt::find_source(double x_position, double y_position)
{
    for (int id = 0; id < CELL_NUMBER; ++id)
    {
      Cell cell(id);
      // Now we are still using uniform H to first initialize cells
      // Note: if the position happen to be at the surface of two cells,
      // The current function will return the one to the left or below
      if ((fabs(cell.cell_center_[0]-x_position) <= H/2.) && 
        (fabs(cell.cell_center_[1]-y_position) <= H/2.))
      {
        return cell.id_;
      }  
    }
}

vec HelperAdapt::compute_step(int index, int index_source, double distance, double REFINE_FACTOR)
{
    double denominator = 0;
    // step_source is the source cell step
    double step_source;
    vec step;
    // If index_1_source is an even number
    if (index_source%2 == 0)
    {
      for (int i=0; i < index_source/2; ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5 + pow(REFINE_FACTOR, index_source/2);
      // cout << denominator <<endl;
      step_source = distance/denominator;
      // Calculating each cell width to the left of the targeted cell, put them in an array
      for (int i=0; i <= index; ++i)
      {
        int power = index_source/2 - abs(i - index_source/2);
        step.push_back(pow(REFINE_FACTOR, power)*step_source);
      }
    }
    // If index_1_source_ is an odd number 
    else
    {
      for (int i=0; i < (index_source/2 + 1); ++i)
        denominator += 2*pow(REFINE_FACTOR, i);
      denominator += -0.5;
      // cout << denominator <<endl;
      step_source = distance/denominator;
      for (int i=0; i <= index; ++i)
      {
        int power = (index_source/2 - abs(2*i - index_source)/2);
        step.push_back(pow(REFINE_FACTOR, power)*step_source);
      }
    }
    return step;
}

