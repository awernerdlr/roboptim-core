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

#undef NDEBUG

#include "shared-tests/common.hh"

#include <roboptim/core/io.hh>
#include <roboptim/core/n-times-derivable-function.hh>

using namespace roboptim;

// Define a 10 times derivable null function.
struct F : public NTimesDerivableFunction<10>
{
  using NTimesDerivableFunction<10>::impl_compute;

  F () : NTimesDerivableFunction<10> (4, "0")
  {}

  virtual void impl_compute (result_t& result, double) const throw ()
  {
    result.setZero ();
  }

  virtual void impl_derivative (gradient_t& derivative,
				double,
				size_type order = 1) const throw ()
  {
    assert (order <= derivabilityOrder);
    derivative.setZero ();
  }
};

BOOST_AUTO_TEST_CASE (n_times_derivable_function)
{
  boost::shared_ptr<boost::test_tools::output_test_stream>
    output = retrievePattern ("n-times-derivable-function");

  F f;
  (*output) << f << std::endl;

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}
