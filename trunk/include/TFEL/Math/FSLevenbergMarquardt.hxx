/*!
 * \file   FSLevenbergMarquardt.hxx
 * \brief  
 *
 * "Methods for non-linear least
 * squares problems"
 * 2nd Edition, April 2004
 * K. Madsen, H.B. Nielsen, O. Tingleff
 *
 * \author Helfer Thomas
 * \date   21 nov 2008
 */

#ifndef _LIB_FSLEVENBERGMARQUARDT_HXX_
#define _LIB_FSLEVENBERGMARQUARDT_HXX_ 

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/TinyMatrixSolve.hxx"

namespace tfel
{

  namespace math
  {

    /*!
     * an helper class to define the type of variable used by the
     * FSLevenbergMarquardt class.
     * \param N : number of variables
     * \param T : numerical type
     */
    template<unsigned short N, // number of variables
	     typename T>
    struct FSLevenbergMarquardtVariable
    {
      //! a simple alias
      typedef tvector<N,T> type;
    }; // end of struct Variable

    /*!
     * an helper class to define the type of variable used by the
     * FSLevenbergMarquardt class in 1D.
     * \param T : numerical type
     */
    template<typename T>
    struct FSLevenbergMarquardtVariable<1u,T>
    {
      //! a simple alias
      typedef T type;
    }; // end of struct Variable

    
    /*!
     * class implementing the Levenberg-Marquardt algorithm.
     * \param N : number of variables
     * \param N : number of parameters
     * \param T : numerical type
     */
    template<unsigned short N, // number of variables
	     unsigned short M, // number of parameters
	     typename T = double>
    struct FSLevenbergMarquardt
    {
      //! a simple alias
      typedef  typename FSLevenbergMarquardtVariable<N,T>::type Variable;
      //! a simple alias
      typedef  typename FSLevenbergMarquardtVariable<M,T>::type Parameter;
      
      //! a simple alias
      typedef std::pair<T,Parameter> (* PtrFun)(const Variable&,
						const Parameter&);

      /*!
       * Default constructor
       * \param f_ : function to be minimized
       */
      FSLevenbergMarquardt(const PtrFun);

      /*!
       * add a new data
       * \param x : abscissae
       * \param y : value
       */
      void
      addData(const Variable&,
	      const T);

      /*!
       * set the initial solution guess
       * \param p_ : initial guess
       */
      void
      setInitialGuess(const Parameter&);

      /*!
       * set the initial damping parameter
       * \param value : initial damping parameter
       */
      void
      setInitialDampingParameter(const T);

      /*!
       * set the first criterium value
       * \param value : first criterium value
       */
      void
      setFirstCriterium(const T);

      /*!
       * set the second criterium value
       * \param value : second criterium value
       */
      void
      setSecondCriterium(const T);

      /*!
       * set the multiplication factor value
       * \param value : multiplication factor value
       */
      void
      setMultiplicationFactor(const T);

      /*!
       * set the maximum number of iterations value
       * \param value : maximum number of iterations value
       */
      void
      setMaximumIteration(const T);

      /*!
       * start the resolution
       * \return the optimal set of parameters found
       * \throw MathDivergenceException if convergence was not reached
       */
      const Parameter&
      execute(void);
      
    private:

      /*!
       * set of data used for minimisation
       */
      std::vector<std::pair<Variable,double> > data;
      /*!
       * current set of parameters
       */
      Parameter p;
      /*!
       * initial damping paramater
       */
      T lambda0;
      /*!
       * multiplication factor
       */
      T factor;
      /*!
       * first criterium
       */
      T eps1;
      /*!
       * second criterium
       */
      T eps2;
      /*!
       * maximum number of iterations
       */
      unsigned short iterMax;
      /*!
       * model to be fitted
       */
      PtrFun f;

    }; // end of struct FSLevenbergMarquardt

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/LevenbergMarquardt/FSLevenbergMarquardt.ixx"

#endif /* _LIB_FSLEVENBERGMARQUARDT_HXX */

