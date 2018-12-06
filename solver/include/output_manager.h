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

		/* Constructor */
		OutputManager();

		void scalar_output(VectorXd &x);
		void vector_output();

};


#endif