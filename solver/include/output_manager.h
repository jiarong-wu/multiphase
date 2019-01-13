#ifndef OUTPUT_MANAGER_H
#define OUTPUT_MANAGER_H

#include "common_definitions.h"
#include "cell.h"

class OutputManager
{	
  public:
  	// We might specify output file name and output file path
  	string solution_name_;
  	string output_path_;

    int cycle_;

    /* Output manager needs to know information about the adaptive mesh. */
    /* This is a design fault. */
    int AdaptFlag_; 
    double REFINE_RATIO_;
    double x_source_;
    double y_source_;

		/* Constructor */
        OutputManager(int cycle);
		OutputManager(int cycle, double REFINE_RATIO, double x_source, double y_source);

		void scalar_output(VectorXd &x, const string &name);
		void vector_output();

};


#endif