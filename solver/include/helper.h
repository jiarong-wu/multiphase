/**
 * \class Helper
 *
 * \brief The abstract base class for all of the helpers.
 *
 * This class provides only an interface so that all the specific helpers classes
 * can inherit from this class. It is not useful by itself rather its usefulness 
 * is only a function of how much it helps the reader. It is in a sense defined 
 * by the person who reads it and otherwise does not exist in any real form.
 *
 */

#ifndef HELPER_H
#define HELPER_H

#include "common_definitions.h"

// No need to include cell.h. It will only cause circular
// #include "cell.h"


class Helper
{
  public:
};

#endif