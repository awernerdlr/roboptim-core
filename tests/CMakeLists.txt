# Copyright 2010, Thomas Moulard, LAAS-CNRS
#
# This file is part of roboptim-core.
# roboptim-core is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# roboptim-core is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Lesser Public License for more details.
# You should have received a copy of the GNU Lesser General Public License
# along with roboptim-core.  If not, see <http://www.gnu.org/licenses/>.

# Declare the CHECK_PREFIX option.
# It allows the user to instrument the test suite easily to make it run
# through Valgrind for instance.
SET(CHECK_PREFIX "" CACHE STRING "Shell prefix used when running tests")

# Generate local-libdir.hh.
CONFIG_FILES(shared-tests/local-libdir.hh)

# Make sure local headers are found
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/tests)


# Add Boost path to include directories.
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# Make Boost.Test generates the main function in test cases.
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)


SET(COMMON_HEADERS
  ${HEADERS}
  shared-tests/common.hh
  shared-tests/hs071.hh
  shared-tests/hs071-nonscalar-constraints.hh
  )


# ROBOPTIM_CORE_TEST(NAME)
# ------------------------
#
# Define a test named `NAME'.
#
# This macro will create a binary from `NAME.cc', link it against
# Boost and add it to the test suite.
#
MACRO(ROBOPTIM_CORE_TEST NAME)
  ADD_EXECUTABLE(${NAME} ${NAME}.cc ${COMMON_HEADERS})
  ADD_TEST(${NAME} ${CHECK_PREFIX} ${CMAKE_CURRENT_BINARY_DIR}/${NAME})

  TARGET_LINK_LIBRARIES(${NAME} roboptim-core)
  PKG_CONFIG_USE_DEPENDENCY(${NAME} eigen3)

  # Link against Boost.
  TARGET_LINK_LIBRARIES(${NAME} ${Boost_LIBRARIES})
ENDMACRO(ROBOPTIM_CORE_TEST)


# simple
# Link the dummy solver with this test to bypass the plug-in system.
ADD_EXECUTABLE(simple
  ${COMMON_HEADERS}
  simple.cc
  ${CMAKE_SOURCE_DIR}/src/dummy.cc)
TARGET_LINK_LIBRARIES(simple roboptim-core ${Boost_LIBRARIES})

# Basic types.
ROBOPTIM_CORE_TEST(interval)
ROBOPTIM_CORE_TEST(util)
ROBOPTIM_CORE_TEST(result)
ROBOPTIM_CORE_TEST(function)
ROBOPTIM_CORE_TEST(derivable-function)
ROBOPTIM_CORE_TEST(twice-derivable-function)
ROBOPTIM_CORE_TEST(quadratic-function)
ROBOPTIM_CORE_TEST(linear-function)
ROBOPTIM_CORE_TEST(problem-cc)
ROBOPTIM_CORE_TEST(numeric-linear-function)
ROBOPTIM_CORE_TEST(numeric-quadratic-function)
ROBOPTIM_CORE_TEST(n-times-derivable-function)
ROBOPTIM_CORE_TEST(parametrized-function)
ROBOPTIM_CORE_TEST(derivable-parametrized-function)

# Dynamic loading mechanism.
ROBOPTIM_CORE_TEST(plugin)

# Dynamic loading mechanism with solver's last state
ROBOPTIM_CORE_TEST(plugin-laststate)

# Algorithm.
ROBOPTIM_CORE_TEST(finite-difference-gradient)

# Built-in mathematical functions.
ROBOPTIM_CORE_TEST(identity-function)
ROBOPTIM_CORE_TEST(constant-function)

ROBOPTIM_CORE_TEST(cached-function)
ROBOPTIM_CORE_TEST(split)

# Visualization
ROBOPTIM_CORE_TEST(visualization-gnuplot-simple)

# visualization-gnuplot-function
ROBOPTIM_CORE_TEST(visualization-gnuplot-function)
