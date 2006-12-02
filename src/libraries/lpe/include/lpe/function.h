///////////////////////////////////////////////////////////////////////////////
/// \file function.h
/// Contains function data structures for the LPE Library.

#ifndef LPE_FUNCTION_H
#define LPE_FUNCTION_H

#include <iostream>
#include <cassert>
#include "atermpp/aterm_list.h"
#include "atermpp/aterm_appl.h"
#include "lpe/sort.h"

namespace lpe {

using atermpp::term_appl;
using atermpp::term_list;

///////////////////////////////////////////////////////////////////////////////
// function
/// \brief Represents mappings and constructors of a mCRL2 specification.
///
class function: public term_appl<lpe::sort>
{
  public:
    function()
    {}

    function(term_appl<lpe::sort> t)
      : term_appl<lpe::sort>(t)
    {
      assert(gsIsOpId(t));
    }
       
    // Added 16-11-2005 by Frank S.
    // 
    sort_list input_types() const
    {
      sort_list result;
      term_appl<lpe::sort> t = *this;
      while (t(1).is_arrow()) {
        t = t(1);
        result = push_front(result, t(0));
      }
      return reverse(result);
    }

    // Added 16-11-2005 by Frank S.
    //     
    sort result_type() const
    {
      term_appl<lpe::sort> t = *this;
      while (t(1).is_arrow()) {
        t = t(1);
      }
      return t(1);
    }

    /// Applies a substitution to this function and returns the result.
    /// The Substitution object must supply the method aterm operator()(aterm).
    ///
    template <typename Substitution>
    function substitute(Substitution f)
    {
      return function(f(*this));
    }     

    /// Returns a pretty print representation of the term.
    ///                                                   
    std::string pp() const                                
    {                                                     
      return pretty_print(term());                        
    }
};

typedef term_list<function> function_list;

} // namespace mcrl

#endif // LPE_FUNCTION_H
