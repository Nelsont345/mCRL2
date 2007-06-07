///////////////////////////////////////////////////////////////////////////////
/// \file mcrl2/lps/function.h
/// Add your file description here.
//
//  Copyright 2007 Wieger Wesselink. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LPS_FUNCTION_H
#define LPS_FUNCTION_H

#include "mcrl2/lps/data.h"

namespace lps {

/// \brief Note that data_operation and function are the same.
typedef data_operation function;

/// \brief Note that data_operation and function are the same.
typedef data_operation_list function_list;

/// \brief Returns true if the term t is a function
inline
bool is_function(aterm_appl t)
{
  return is_data_operation(t);
}

} // namespace lps

#endif // LPS_FUNCTION_H
