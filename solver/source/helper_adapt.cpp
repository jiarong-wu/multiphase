#include "helper_adapt.h"
#include "common_definitions.h"
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

