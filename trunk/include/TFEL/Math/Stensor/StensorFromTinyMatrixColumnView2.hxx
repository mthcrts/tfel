/*!
 * \file   StensorFromTinyMatrixColumnView2.hxx
 * \brief This class provides a view of an stensor provided a given
 * runtime offset. This class is used by the behaviours class
 * generated by the MFrontImplicitParser.
 * Stensor From Matric Column
 * View \author Helfer Thomas \date 16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_STENSORFROMTINYMATRIXCOLUMNVIEW2_HXX_
#define _LIB_TFEL_MATH_STENSORFROMTINYMATRIXCOLUMNVIEW2_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"
#include"TFEL/Math/Stensor/StensorExpr.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tmatrix.hxx"

namespace tfel
{
  
  namespace math
  {

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T>
    struct StensorFromTinyMatrixColumnView2Expr
    {}; // end of struct StensorFromTinyMatrixColumnView2Expr

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T>
    struct StensorExpr<stensor<N,T>,StensorFromTinyMatrixColumnView2Expr<N,Mn,Mm,In,Im,T> >
      : public StensorConcept<StensorExpr<stensor<N,T>,StensorFromTinyMatrixColumnView2Expr<N,Mn,Mm,In,Im,T> > >,
	public stensor_base<StensorExpr<stensor<N,T>,StensorFromTinyMatrixColumnView2Expr<N,Mn,Mm,In,Im,T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      StensorExpr(tmatrix<Mn,Mm,T>& m_,
		  const unsigned short i_,
		  const unsigned short j_)
	: m(m_),
	  oi(i_),
	  oj(j_)
      {} // end of StensorExpr

      const T&
      operator()(const unsigned short i) const
      {
	return this->m(In+(this->oi)*StensorDimeToSize<N>::value+i,Im+this->oj);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->m(In+(this->oi)*StensorDimeToSize<N>::value+i,Im+this->oj);
      } // end of operator()

      const T&
      operator[](const unsigned short i) const
      {
	return this->m(In+(this->oi)*StensorDimeToSize<N>::value+i,Im+this->oj);
      } // end of operator[] const

      T&
      operator[](const unsigned short i)
      {
	return this->m(In+(this->oi)*StensorDimeToSize<N>::value+i,Im+this->oj);
      } // end of operator[]

      using stensor_base<StensorExpr>::operator =;
    
    protected:

      tmatrix<Mn,Mm,T>& m;
      const unsigned short oi;
      const unsigned short oj;
      
    private:
      
      //! simple checks
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      //! simple checks
      TFEL_STATIC_ASSERT((Im<Mm));
      //! simple checks
      TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value<=Mn-In));

    }; // end of struct StensorExpr

    template<unsigned short N, unsigned short Mn,
	     unsigned short Mm,unsigned short In,
	     unsigned short Im,typename T = double>
    struct StensorFromTinyMatrixColumnView2
    {
      typedef StensorExpr<stensor<N,T>,StensorFromTinyMatrixColumnView2Expr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct StensorFromTinyMatrixColumnView2
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_STENSORFROMTINYMATRIXCOLUMNVIEW2_HXX */

