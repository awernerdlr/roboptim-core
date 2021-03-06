// Copyright (C) 2009 by Thomas Moulard, AIST, CNRS, INRIA.
//
// This file is part of the roboptim.
//
// roboptim is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// roboptim is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with roboptim.  If not, see <http://www.gnu.org/licenses/>.

#include "debug.hh"

#include "roboptim/core/differentiable-function.hh"
#include "roboptim/core/indent.hh"
#include "roboptim/core/util.hh"

namespace roboptim
{
  DifferentiableFunction::DifferentiableFunction (size_type inputSize,
						  size_type outputSize,
						  std::string name)
    throw ()
    : Function (inputSize, outputSize, name)
  {
  }

  void
  DifferentiableFunction::impl_jacobian (jacobian_t& jacobian,
					 const argument_t& argument)
    const throw ()
  {
#ifndef ROBOPTIM_DO_NOT_CHECK_ALLOCATION
      Eigen::internal::set_is_malloc_allowed (true);
#endif //! ROBOPTIM_DO_NOT_CHECK_ALLOCATION
    for (jacobian_t::Index i = 0; i < outputSize (); ++i)
      jacobian.row (i) = gradient (argument, i);
  }

  std::ostream&
  DifferentiableFunction::print (std::ostream& o) const throw ()
  {
    if (getName ().empty ())
      return o << "Differentiable function";
    else
      return o << getName () << " (differentiable function)";
  }

} // end of namespace roboptim
