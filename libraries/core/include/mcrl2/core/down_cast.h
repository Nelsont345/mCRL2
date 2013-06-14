// Author(s): Jeroen Keiren
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file mcrl2/core/down_cast.h
/// \brief Function to explicitly down cast expressions in a hierarchy.

#ifndef MCRL2_CORE_DOWN_CAST_H
#define MCRL2_CORE_DOWN_CAST_H

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "mcrl2/atermpp/aterm.h"

namespace mcrl2
{

namespace core
{

template <class Derived, class Base>
Derived down_cast(Base t,
            typename boost::enable_if< boost::is_base_of<Base, Derived> >::type* = 0,
            typename boost::enable_if< boost::is_base_of<atermpp::aterm, Base> >::type* = 0)
{
  static_assert(sizeof(Derived)==sizeof(atermpp::aterm),
                "Size of types should be equal to the size of an atermpp::aterm");
  return (Derived)t;
}

template <class Derived, class Base>
Derived down_cast(Base t,
            typename boost::enable_if< boost::is_base_of<Base, Derived> >::type* = 0,
            typename boost::disable_if< boost::is_base_of<atermpp::aterm, Base> >::type* = 0,
            typename boost::enable_if< boost::is_convertible<Base, Derived> >::type* = 0)
{
  return (Derived)t;
}


template <class Derived, class Base>
Derived down_cast(Base t,
            typename boost::enable_if< boost::is_base_of<Base, Derived> >::type* = 0,
            typename boost::disable_if< boost::is_base_of<atermpp::aterm, Base> >::type* = 0,
            typename boost::disable_if< boost::is_convertible<Base, Derived> >::type* = 0)
{
  return Derived(t);
}

template <class Derived, class Base>
const Derived& static_down_cast(const Base& x)
{
  return static_cast<const Derived&>(x);
}

} // namespace core

} // namespace mcrl2

#endif // MCRL2_BASIC_IDENTIFIER_STRING_H