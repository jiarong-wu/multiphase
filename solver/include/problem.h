/**
 * \class Problem
 *
 * \brief The abstract base class for all of the problems.
 *
 * This class provides only an interface so that all the specific problem classes
 * can inherit from this class.  
 *
 */

#ifndef PROBLEM_H
#define PROBLEM_H

#include "common_definitions.h"
#include "helper.h"

class Problem
{
  public:

    // Helper* helper_ptr;

    virtual void run() = 0;            

};

#endif