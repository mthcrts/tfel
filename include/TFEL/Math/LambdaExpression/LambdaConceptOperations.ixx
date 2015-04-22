/*!
 * \file   include/TFEL/Math/LambdaExpression/LambdaConceptOperations.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 d�c 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_LAMBDACONCEPTOPERATIONS_IXX_
#define _LIB_TFEL_LAMBDACONCEPTOPERATIONS_IXX_ 

namespace tfel{

  namespace math{

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      LambdaLambdaExpr<T1,T2,OpPlus>
      >::type
    operator +(const T1 a, const T2 b)
    {
      return LambdaLambdaExpr<T1,T2,OpPlus>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      LambdaLambdaExpr<T1,T2,OpMinus>
      >::type
    operator -(const T1 a, const T2 b)
    {
      return LambdaLambdaExpr<T1,T2,OpMinus>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      LambdaLambdaExpr<T1,T2,OpMult>
      >::type
    operator *(const T1 a, const T2 b)
    {
      return LambdaLambdaExpr<T1,T2,OpMult>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      LambdaLambdaExpr<T1,T2,OpDiv>
      >::type
    operator /(const T1 a, const T2 b)
    {
      return LambdaLambdaExpr<T1,T2,OpDiv>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      ScalarLambdaExpr<T1,T2,OpPlus>
      >::type
    operator +(const T1 a, const T2 b)
    {
      return ScalarLambdaExpr<T1,T2,OpPlus>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      ScalarLambdaExpr<T1,T2,OpMinus>
      >::type
    operator -(const T1 a, const T2 b)
    {
      return ScalarLambdaExpr<T1,T2,OpMinus>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      ScalarLambdaExpr<T1,T2,OpMult>
      >::type
    operator *(const T1 a, const T2 b)
    {
      return ScalarLambdaExpr<T1,T2,OpMult>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      ScalarLambdaExpr<T1,T2,OpDiv>
      >::type
    operator /(const T1 a, const T2 b)
    {
      return ScalarLambdaExpr<T1,T2,OpDiv>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::typetraits::IsScalar<T2>::cond,
      LambdaScalarExpr<T1,T2,OpPlus>
      >::type
    operator +(const T1 a, const T2 b)
    {
      return LambdaScalarExpr<T1,T2,OpPlus>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::typetraits::IsScalar<T2>::cond,
      LambdaScalarExpr<T1,T2,OpMinus>
      >::type
    operator -(const T1 a, const T2 b)
    {
      return LambdaScalarExpr<T1,T2,OpMinus>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::typetraits::IsScalar<T2>::cond,
      LambdaScalarExpr<T1,T2,OpMult>
      >::type
    operator *(const T1 a, const T2 b)
    {
      return LambdaScalarExpr<T1,T2,OpMult>(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::typetraits::IsScalar<T2>::cond,
      LambdaScalarExpr<T1,T2,OpDiv>
      >::type
    operator /(const T1 a, const T2 b)
    {
      return LambdaScalarExpr<T1,T2,OpDiv>(a,b);
    }

  } // end of namespace math

} // end of namespace tfel 

#endif /* _LIB_TFEL_LAMBDACONCEPTOPERATIONS_IXX */
