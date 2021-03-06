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

#ifndef ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HH
# define ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HH
# include <stdexcept>

# include <roboptim/core/fwd.hh>
# include <roboptim/core/differentiable-function.hh>
# include <roboptim/core/portability.hh>

namespace roboptim
{
  /// \brief Default threshold for checkGradient.
  static const double finiteDifferenceThreshold = 1e-4;
  /// \brief Default epsilon for finite difference class.
  static const double finiteDifferenceEpsilon = 1e-8;

  /// \brief Exception thrown when a gradient check fail.
  class ROBOPTIM_DLLAPI BadGradient : public std::runtime_error
  {
  public:
    /// \brief Import vector.
    typedef DifferentiableFunction::vector_t vector_t;
    /// \brief Import gradient.
    typedef DifferentiableFunction::gradient_t gradient_t;
    /// \brief Import value_type.
    typedef DifferentiableFunction::value_type value_type;
    /// \brief Import size_type.
    typedef DifferentiableFunction::size_type size_type;

    /// \brief Default constructor.
    BadGradient (const vector_t& x,
		 const gradient_t& analyticalGradient,
		 const gradient_t& finiteDifferenceGradient,
		 const value_type& threshold);

    virtual ~BadGradient () throw ();

    /// \brief Display the exception on the specified output stream.
    ///
    /// \param o output stream used for display
    /// \return output stream
    virtual std::ostream& print (std::ostream& o) const throw ();


    /// \brief Gradient has been computed for this point.
    vector_t x_;

    /// \brief Analytical gradient.
    gradient_t analyticalGradient_;

    /// \brief Gradient computed through finite differences.
    gradient_t finiteDifferenceGradient_;

    /// \brief Maximum error.
    value_type maxDelta_;

    /// \brief Component containing the maximum error.
    size_type maxDeltaComponent_;

    /// \brief Allowed threshold.
    value_type threshold_;
  };

  /// \brief Override operator<< to handle exception display.
  ///
  /// \param o output stream used for display
  /// \param f function to be displayed
  /// \return output stream
  ROBOPTIM_DLLAPI std::ostream& operator<< (std::ostream& o,
					    const BadGradient& f);

  /// \brief Contains finite difference gradients policies.
  ///
  /// Each class of this algorithm implements a finite difference
  /// gradient computation algorithm.
  namespace finiteDifferenceGradientPolicies
  {
    /// \brief Fast finite difference gradient computation.
    ///
    /// Finite difference is computed using forward difference.
    class ROBOPTIM_DLLAPI Simple
    {
    public:
      typedef Function::size_type size_type;
      void computeGradient
      (const Function& adaptee,
       Function::value_type epsilon,
       Function::result_t& gradient,
       const Function::argument_t& argument,
       Function::size_type idFunction,
       Function::argument_t& xEps) const throw ();
    };

    /// \brief Precise finite difference gradient computation.
    ///
    /// Finite difference is computed using five-points stencil
    /// (i.e. \f$\{x-2h, x-h, x, x+h, x+2h\}\f$).
    class ROBOPTIM_DLLAPI FivePointsRule
    {
    public:
      typedef Function::size_type size_type;
      void computeGradient
      (const Function& adaptee,
       Function::value_type epsilon,
       Function::result_t& gradient,
       const Function::argument_t& argument,
       Function::size_type idFunction,
       Function::argument_t& xEps) const throw ();
    };
  } // end of namespace policy.


  /// \addtogroup roboptim_function
  /// @{

  /// \brief Compute automatically a gradient with finite differences.
  ///
  /// Finite difference gradient is a method to approximate a function's
  /// gradient. It is particularly useful in RobOptim to avoid the need
  /// to compute the analytical gradient manually.
  ///
  /// This class takes a Function as its input and wraps it into a derivable
  /// function.
  ///
  /// The one dimensional formula is:
  /// \f[f'(x)\approx {f(x+\epsilon)-f(x)\over \epsilon}\f]
  /// where \f$\epsilon\f$ is a constant given when calling the class
  /// constructor.
  template <typename FdgPolicy>
  class FiniteDifferenceGradient
    : public DifferentiableFunction,
      private FdgPolicy
  {
  public:
    /// \brief Instantiate a finite differences gradient.
    ///
    /// Instantiate a derivable function that will wraps a non
    /// derivable function and compute automatically its gradient
    /// using finite differences.
    /// \param f function that will e wrapped
    /// \param e epsilon used in finite difference computation
    FiniteDifferenceGradient (const Function& f,
			      value_type e = finiteDifferenceEpsilon)
      throw ();
    ~FiniteDifferenceGradient () throw ();

  protected:
    void impl_compute (result_t&, const argument_t&) const throw ();
    void impl_gradient (gradient_t&, const argument_t& argument, size_type = 0)
      const throw ();

    /// \brief Reference to the wrapped function.
    const Function& adaptee_;

    //// \brief Epsilon used in finite differences computation.
    const value_type epsilon_;

    mutable argument_t xEps_;
  };

  /// \brief Check if a gradient is valid.
  ///
  /// Check if a gradient is valid by comparing the distance between its
  /// gradient and an automatically computed finite differences gradient.
  /// \param function function that will be checked
  /// \param functionId function id in split representation
  /// \param x point where the gradient will be evaluated
  /// \param threshold maximum tolerated error
  /// \return true if valid, false if not
  ROBOPTIM_DLLAPI bool checkGradient
  (const DifferentiableFunction& function,
   Function::size_type functionId,
   const Function::vector_t& x,
   Function::value_type threshold = finiteDifferenceThreshold)
    throw ();

  ROBOPTIM_DLLAPI void checkGradientAndThrow
  (const DifferentiableFunction& function,
   Function::size_type functionId,
   const Function::vector_t& x,
   Function::value_type threshold = finiteDifferenceThreshold)
    throw (BadGradient);

  /// Example shows finite differences gradient use.
  /// \example finite-difference-gradient.cc

  /// @}

} // end of namespace roboptim

# include <roboptim/core/finite-difference-gradient.hxx>
#endif //! ROBOPTIM_CORE_FINITE_DIFFERENCE_GRADIENT_HH
