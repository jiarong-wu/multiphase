#ifndef HELPER_ADAPT
#define HELPER_ADAPT

#include "common_definitions.h"
#include "helper.h"

class HelperAdapt: public Helper
{
  public: 	
    int find_source(double x_position, double y_position);
	vec compute_step(int index, int index_source, double distance, double REFINE_FACTOR);

};

#endif