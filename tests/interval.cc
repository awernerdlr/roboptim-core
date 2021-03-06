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

#include <iostream>

#include <roboptim/core/function.hh>
#include <roboptim/core/io.hh>
#include <roboptim/core/util.hh>

using namespace roboptim;

boost::shared_ptr<boost::test_tools::output_test_stream>
output;

struct MyFunctor
{
  void operator () (double t)
  {
    (*output) << t << std::endl;
  }
};

BOOST_AUTO_TEST_CASE (interval)
{
  output = retrievePattern ("interval");

  typedef Function::interval_t interval_t;
  typedef Function::discreteInterval_t discreteInterval_t;

  {
    interval_t i1 = Function::makeInterval (0., 5.);
    BOOST_CHECK_EQUAL (Function::getLowerBound (i1), 0.);
    BOOST_CHECK_EQUAL (Function::getUpperBound (i1), 5.);

    interval_t i2 = Function::makeInfiniteInterval ();
    double inf = Function::infinity ();
    // Just make sure inf != NaN.
    BOOST_CHECK_EQUAL (inf, inf);
    BOOST_CHECK_EQUAL (Function::getLowerBound (i2), -inf);
    BOOST_CHECK_EQUAL (Function::getUpperBound (i2), inf);

    interval_t i3 = Function::makeLowerInterval (42.);
    BOOST_CHECK_EQUAL (Function::getLowerBound (i3), 42.);
    BOOST_CHECK_EQUAL (Function::getUpperBound (i3), inf);

    interval_t i4 = Function::makeUpperInterval (42.);
    BOOST_CHECK_EQUAL (Function::getLowerBound (i4), -inf);
    BOOST_CHECK_EQUAL (Function::getUpperBound (i4), 42.);

    (*output) << i1 << std::endl
	      << i2 << std::endl
	      << i3 << std::endl
	      << i4 << std::endl;
  }
  {
    discreteInterval_t i1 = Function::makeDiscreteInterval (0., 5., 1.);
    BOOST_CHECK_EQUAL (Function::getLowerBound (i1), 0.);
    BOOST_CHECK_EQUAL (Function::getUpperBound (i1), 5.);
    BOOST_CHECK_EQUAL (Function::getStep (i1), 1.);

    double inf = Function::infinity ();
    discreteInterval_t i2 = Function::makeDiscreteInterval (-inf, inf, .1);
    BOOST_CHECK_EQUAL (Function::getLowerBound (i2), -inf);
    BOOST_CHECK_EQUAL (Function::getUpperBound (i2), inf);
    BOOST_CHECK_EQUAL (Function::getStep (i2), .1);

    discreteInterval_t i3 = Function::makeDiscreteInterval
      (Function::makeInterval (1., 4.), .1);

    (*output) << i1 << std::endl
	      << i2 << std::endl
	      << i3 << std::endl;

    Function::foreach (i1, MyFunctor ());
  }

  std::cout << output->str () << std::endl;
  BOOST_CHECK (output->match_pattern ());
}
