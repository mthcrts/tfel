/*!
 * \file   include/TFEL/Math/T2toST2/T2toST2Concept.ixx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOST2_CONCEPT_IXX
#define LIB_TFEL_MATH_T2TOST2_CONCEPT_IXX 1

#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel::math {

  template <class T>
  TFEL_MATH_INLINE numeric_type<T> T2toST2Concept<T>::operator()(
      const unsigned short i, const unsigned short j) const {
    return static_cast<const T&>(*this).operator()(i, j);
  }  // end of T2toST2Concept<T>::operator()

  template <typename T2toST2Type>
  typename std::enable_if<
      implementsT2toST2Concept<T2toST2Type>(),
      typename tfel::typetraits::AbsType<numeric_type<T2toST2Type>>::type>::type
  abs(const T2toST2Type& v) {
    using NumType = numeric_type<T2toST2Type>;
    using AbsNumType = typename tfel::typetraits::AbsType<NumType>::type;
    AbsNumType a(0);
    constexpr auto ssize =
        StensorDimeToSize<getSpaceDimension<T2toST2Type>()>::value;
    constexpr auto tsize =
        TensorDimeToSize<getSpaceDimension<T2toST2Type>()>::value;
    for (unsigned short i = 0; i < ssize; ++i) {
      for (unsigned short j = 0; j < tsize; ++j) {
        a += abs(v(i, j));
      }
    }
    return a;
  }

  template <typename T2toST2ResultType,
            typename T2toST2Type,
            typename StensorType,
            typename TensorType>
  typename std::enable_if<
      implementsT2toST2Concept<T2toST2ResultType>() &&
          implementsT2toST2Concept<T2toST2Type>() &&
          implementsStensorConcept<StensorType>() &&
          implementsTensorConcept<TensorType>() &&
          getSpaceDimension<T2toST2ResultType>() == 1u &&
          getSpaceDimension<T2toST2Type>() == 1u &&
          getSpaceDimension<StensorType>() == 1u &&
          getSpaceDimension<TensorType>() == 1u &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>(),
      void>::type
  computePushForwardDerivative(T2toST2ResultType& dTdF,
                               const T2toST2Type& dSdF,
                               const StensorType& S,
                               const TensorType& F) {
    // derivative with respect to F0
    dTdF(0, 0) = F[0] * F[0] * dSdF(0, 0) + 2 * F[0] * S[0];
    dTdF(1, 0) = F[1] * F[1] * dSdF(1, 0);
    dTdF(2, 0) = F[2] * F[2] * dSdF(2, 0);
    // derivative with respect to F1
    dTdF(0, 1) = F[0] * F[0] * dSdF(0, 1);
    dTdF(1, 1) = F[1] * F[1] * dSdF(1, 1) + 2 * F[1] * S[1];
    dTdF(2, 1) = F[2] * F[2] * dSdF(2, 1);
    // derivative with respect to F1
    dTdF(0, 2) = F[0] * F[0] * dSdF(0, 2);
    dTdF(1, 2) = F[1] * F[1] * dSdF(1, 2);
    dTdF(2, 2) = F[2] * F[2] * dSdF(2, 2) + 2 * F[2] * S[2];
  }

  template <typename T2toST2ResultType,
            typename T2toST2Type,
            typename StensorType,
            typename TensorType>
  typename std::enable_if<
      implementsT2toST2Concept<T2toST2ResultType>() &&
          implementsT2toST2Concept<T2toST2Type>() &&
          implementsStensorConcept<StensorType>() &&
          implementsTensorConcept<TensorType>() &&
          getSpaceDimension<T2toST2ResultType>() == 2u &&
          getSpaceDimension<T2toST2Type>() == 2u &&
          getSpaceDimension<StensorType>() == 2u &&
          getSpaceDimension<TensorType>() == 2u &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>(),
      void>::type
  computePushForwardDerivative(T2toST2ResultType& dTdF,
                               const T2toST2Type& dSdF,
                               const StensorType& S,
                               const TensorType& F) {
    typedef numeric_type<T2toST2Type> value_type;
    constexpr auto cste = Cste<value_type>::sqrt2;
    constexpr auto icste = Cste<value_type>::isqrt2;
    // derivative with respect to F0
    dTdF(0, 0) =
        F[0] * ((F[3] * dSdF(3, 0)) * icste + F[0] * dSdF(0, 0) + S[0]) +
        F[3] *
            ((F[0] * dSdF(3, 0)) * icste + (S[3]) * icste + F[3] * dSdF(1, 0)) +
        (F[3] * S[3]) * icste + F[0] * S[0];
    dTdF(1, 0) = F[1] * ((F[4] * dSdF(3, 0)) * icste + F[1] * dSdF(1, 0)) +
                 F[4] * ((F[1] * dSdF(3, 0)) * icste + F[4] * dSdF(0, 0));
    dTdF(2, 0) = F[2] * F[2] * dSdF(2, 0);
    dTdF(3, 0) = (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 0) + F[1] * S[3] +
                 cste * F[1] * F[3] * dSdF(1, 0) +
                 cste * F[0] * F[4] * dSdF(0, 0) + cste * F[4] * S[0];
    // derivative with respect to F1
    dTdF(0, 1) = F[0] * ((F[3] * dSdF(3, 1)) * icste + F[0] * dSdF(0, 1)) +
                 F[3] * ((F[0] * dSdF(3, 1)) * icste + F[3] * dSdF(1, 1));
    dTdF(1, 1) =
        F[1] * ((F[4] * dSdF(3, 1)) * icste + F[1] * dSdF(1, 1) + S[1]) +
        F[4] *
            ((F[1] * dSdF(3, 1)) * icste + (S[3]) * icste + F[4] * dSdF(0, 1)) +
        (F[4] * S[3]) * icste + F[1] * S[1];
    dTdF(2, 1) = F[2] * F[2] * dSdF(2, 1);
    dTdF(3, 1) = (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 1) + F[0] * S[3] +
                 cste * F[1] * F[3] * dSdF(1, 1) + cste * F[3] * S[1] +
                 cste * F[0] * F[4] * dSdF(0, 1);
    // derivative with respect to F2
    dTdF(0, 2) = F[0] * ((F[3] * dSdF(3, 2)) * icste + F[0] * dSdF(0, 2)) +
                 F[3] * ((F[0] * dSdF(3, 2)) * icste + F[3] * dSdF(1, 2));
    dTdF(1, 2) = F[1] * ((F[4] * dSdF(3, 2)) * icste + F[1] * dSdF(1, 2)) +
                 F[4] * ((F[1] * dSdF(3, 2)) * icste + F[4] * dSdF(0, 2));
    dTdF(2, 2) = F[2] * F[2] * dSdF(2, 2) + 2 * F[2] * S[2];
    dTdF(3, 2) = (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 2) +
                 cste * F[1] * F[3] * dSdF(1, 2) +
                 cste * F[0] * F[4] * dSdF(0, 2);
    // derivative with respect to F3
    dTdF(0, 3) =
        F[0] *
            ((F[3] * dSdF(3, 3)) * icste + (S[3]) * icste + F[0] * dSdF(0, 3)) +
        F[3] * ((F[0] * dSdF(3, 3)) * icste + F[3] * dSdF(1, 3) + S[1]) +
        (F[0] * S[3]) * icste + F[3] * S[1];
    dTdF(1, 3) = F[1] * ((F[4] * dSdF(3, 3)) * icste + F[1] * dSdF(1, 3)) +
                 F[4] * ((F[1] * dSdF(3, 3)) * icste + F[4] * dSdF(0, 3));
    dTdF(2, 3) = F[2] * F[2] * dSdF(2, 3);
    dTdF(3, 3) = (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 3) + F[4] * S[3] +
                 cste * F[1] * F[3] * dSdF(1, 3) + cste * F[1] * S[1] +
                 cste * F[0] * F[4] * dSdF(0, 3);
    // derivative with respect to F4
    dTdF(0, 4) = F[0] * ((F[3] * dSdF(3, 4)) * icste + F[0] * dSdF(0, 4)) +
                 F[3] * ((F[0] * dSdF(3, 4)) * icste + F[3] * dSdF(1, 4));
    dTdF(1, 4) =
        F[1] *
            ((F[4] * dSdF(3, 4)) * icste + (S[3]) * icste + F[1] * dSdF(1, 4)) +
        F[4] * ((F[1] * dSdF(3, 4)) * icste + F[4] * dSdF(0, 4) + S[0]) +
        (F[1] * S[3]) * icste + F[4] * S[0];
    dTdF(2, 4) = F[2] * F[2] * dSdF(2, 4);
    dTdF(3, 4) = (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 4) + F[3] * S[3] +
                 cste * F[1] * F[3] * dSdF(1, 4) +
                 cste * F[0] * F[4] * dSdF(0, 4) + cste * F[0] * S[0];
  }

  template <typename T2toST2ResultType,
            typename T2toST2Type,
            typename StensorType,
            typename TensorType>
  typename std::enable_if<
      implementsT2toST2Concept<T2toST2ResultType>() &&
          implementsT2toST2Concept<T2toST2Type>() &&
          implementsStensorConcept<StensorType>() &&
          implementsTensorConcept<TensorType>() &&
          getSpaceDimension<T2toST2ResultType>() == 3u &&
          getSpaceDimension<T2toST2Type>() == 3u &&
          getSpaceDimension<StensorType>() == 3u &&
          getSpaceDimension<TensorType>() == 3u &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>(),
      void>::type
  computePushForwardDerivative(T2toST2ResultType& dTdF,
                               const T2toST2Type& dSdF,
                               const StensorType& S,
                               const TensorType& F) {
    typedef numeric_type<T2toST2Type> value_type;
    constexpr auto cste = Cste<value_type>::sqrt2;
    constexpr auto icste = Cste<value_type>::isqrt2;
    // derivative with respect to F0
    dTdF(0, 0) =
        F[3] * ((F[5] * dSdF(5, 0)) * icste + (F[0] * dSdF(3, 0)) * icste +
                (S[3]) * icste + F[3] * dSdF(1, 0)) +
        F[5] * ((F[3] * dSdF(5, 0)) * icste + (F[0] * dSdF(4, 0)) * icste +
                (S[4]) * icste + F[5] * dSdF(2, 0)) +
        F[0] * ((F[5] * dSdF(4, 0)) * icste + (F[3] * dSdF(3, 0)) * icste +
                F[0] * dSdF(0, 0) + S[0]) +
        (F[5] * S[4]) * icste + (F[3] * S[3]) * icste + F[0] * S[0];
    dTdF(1, 0) = F[1] * ((F[7] * dSdF(5, 0)) * icste +
                         (F[4] * dSdF(3, 0)) * icste + F[1] * dSdF(1, 0)) +
                 F[7] * ((F[1] * dSdF(5, 0)) * icste +
                         (F[4] * dSdF(4, 0)) * icste + F[7] * dSdF(2, 0)) +
                 F[4] * ((F[7] * dSdF(4, 0)) * icste +
                         (F[1] * dSdF(3, 0)) * icste + F[4] * dSdF(0, 0));
    dTdF(2, 0) = F[2] * ((F[8] * dSdF(5, 0)) * icste +
                         (F[6] * dSdF(4, 0)) * icste + F[2] * dSdF(2, 0)) +
                 F[8] * ((F[2] * dSdF(5, 0)) * icste +
                         (F[6] * dSdF(3, 0)) * icste + F[8] * dSdF(1, 0)) +
                 F[6] * ((F[2] * dSdF(4, 0)) * icste +
                         (F[8] * dSdF(3, 0)) * icste + F[6] * dSdF(0, 0));
    dTdF(3, 0) = (F[3] * F[7] + F[1] * F[5]) * dSdF(5, 0) +
                 (F[0] * F[7] + F[4] * F[5]) * dSdF(4, 0) + F[7] * S[4] +
                 (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 0) + F[1] * S[3] +
                 cste * F[5] * F[7] * dSdF(2, 0) +
                 cste * F[1] * F[3] * dSdF(1, 0) +
                 cste * F[0] * F[4] * dSdF(0, 0) + cste * F[4] * S[0];
    dTdF(4, 0) = (F[5] * F[8] + F[2] * F[3]) * dSdF(5, 0) +
                 (F[5] * F[6] + F[0] * F[2]) * dSdF(4, 0) + F[2] * S[4] +
                 (F[0] * F[8] + F[3] * F[6]) * dSdF(3, 0) + F[8] * S[3] +
                 cste * F[2] * F[5] * dSdF(2, 0) +
                 cste * F[3] * F[8] * dSdF(1, 0) +
                 cste * F[0] * F[6] * dSdF(0, 0) + cste * F[6] * S[0];
    dTdF(5, 0) = (F[7] * F[8] + F[1] * F[2]) * dSdF(5, 0) +
                 (F[6] * F[7] + F[2] * F[4]) * dSdF(4, 0) +
                 (F[4] * F[8] + F[1] * F[6]) * dSdF(3, 0) +
                 cste * F[2] * F[7] * dSdF(2, 0) +
                 cste * F[1] * F[8] * dSdF(1, 0) +
                 cste * F[4] * F[6] * dSdF(0, 0);
    // derivative with respect to F1
    dTdF(0, 1) = F[3] * ((F[5] * dSdF(5, 1)) * icste +
                         (F[0] * dSdF(3, 1)) * icste + F[3] * dSdF(1, 1)) +
                 F[5] * ((F[3] * dSdF(5, 1)) * icste +
                         (F[0] * dSdF(4, 1)) * icste + F[5] * dSdF(2, 1)) +
                 F[0] * ((F[5] * dSdF(4, 1)) * icste +
                         (F[3] * dSdF(3, 1)) * icste + F[0] * dSdF(0, 1));
    dTdF(1, 1) =
        F[1] * ((F[7] * dSdF(5, 1)) * icste + (F[4] * dSdF(3, 1)) * icste +
                F[1] * dSdF(1, 1) + S[1]) +
        F[7] * ((F[1] * dSdF(5, 1)) * icste + (S[5]) * icste +
                (F[4] * dSdF(4, 1)) * icste + F[7] * dSdF(2, 1)) +
        (F[7] * S[5]) * icste +
        F[4] * ((F[7] * dSdF(4, 1)) * icste + (F[1] * dSdF(3, 1)) * icste +
                (S[3]) * icste + F[4] * dSdF(0, 1)) +
        (F[4] * S[3]) * icste + F[1] * S[1];
    dTdF(2, 1) = F[2] * ((F[8] * dSdF(5, 1)) * icste +
                         (F[6] * dSdF(4, 1)) * icste + F[2] * dSdF(2, 1)) +
                 F[8] * ((F[2] * dSdF(5, 1)) * icste +
                         (F[6] * dSdF(3, 1)) * icste + F[8] * dSdF(1, 1)) +
                 F[6] * ((F[2] * dSdF(4, 1)) * icste +
                         (F[8] * dSdF(3, 1)) * icste + F[6] * dSdF(0, 1));
    dTdF(3, 1) = (F[3] * F[7] + F[1] * F[5]) * dSdF(5, 1) + F[5] * S[5] +
                 (F[0] * F[7] + F[4] * F[5]) * dSdF(4, 1) +
                 (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 1) + F[0] * S[3] +
                 cste * F[5] * F[7] * dSdF(2, 1) +
                 cste * F[1] * F[3] * dSdF(1, 1) + cste * F[3] * S[1] +
                 cste * F[0] * F[4] * dSdF(0, 1);
    dTdF(4, 1) = (F[5] * F[8] + F[2] * F[3]) * dSdF(5, 1) +
                 (F[5] * F[6] + F[0] * F[2]) * dSdF(4, 1) +
                 (F[0] * F[8] + F[3] * F[6]) * dSdF(3, 1) +
                 cste * F[2] * F[5] * dSdF(2, 1) +
                 cste * F[3] * F[8] * dSdF(1, 1) +
                 cste * F[0] * F[6] * dSdF(0, 1);
    dTdF(5, 1) = (F[7] * F[8] + F[1] * F[2]) * dSdF(5, 1) + F[2] * S[5] +
                 (F[6] * F[7] + F[2] * F[4]) * dSdF(4, 1) +
                 (F[4] * F[8] + F[1] * F[6]) * dSdF(3, 1) + F[6] * S[3] +
                 cste * F[2] * F[7] * dSdF(2, 1) +
                 cste * F[1] * F[8] * dSdF(1, 1) + cste * F[8] * S[1] +
                 cste * F[4] * F[6] * dSdF(0, 1);
    // derivative with respect to F2
    dTdF(0, 2) = F[3] * ((F[5] * dSdF(5, 2)) * icste +
                         (F[0] * dSdF(3, 2)) * icste + F[3] * dSdF(1, 2)) +
                 F[5] * ((F[3] * dSdF(5, 2)) * icste +
                         (F[0] * dSdF(4, 2)) * icste + F[5] * dSdF(2, 2)) +
                 F[0] * ((F[5] * dSdF(4, 2)) * icste +
                         (F[3] * dSdF(3, 2)) * icste + F[0] * dSdF(0, 2));
    dTdF(1, 2) = F[1] * ((F[7] * dSdF(5, 2)) * icste +
                         (F[4] * dSdF(3, 2)) * icste + F[1] * dSdF(1, 2)) +
                 F[7] * ((F[1] * dSdF(5, 2)) * icste +
                         (F[4] * dSdF(4, 2)) * icste + F[7] * dSdF(2, 2)) +
                 F[4] * ((F[7] * dSdF(4, 2)) * icste +
                         (F[1] * dSdF(3, 2)) * icste + F[4] * dSdF(0, 2));
    dTdF(2, 2) =
        F[2] * ((F[8] * dSdF(5, 2)) * icste + (F[6] * dSdF(4, 2)) * icste +
                F[2] * dSdF(2, 2) + S[2]) +
        F[8] * ((F[2] * dSdF(5, 2)) * icste + (S[5]) * icste +
                (F[6] * dSdF(3, 2)) * icste + F[8] * dSdF(1, 2)) +
        (F[8] * S[5]) * icste +
        F[6] * ((F[2] * dSdF(4, 2)) * icste + (S[4]) * icste +
                (F[8] * dSdF(3, 2)) * icste + F[6] * dSdF(0, 2)) +
        (F[6] * S[4]) * icste + F[2] * S[2];
    dTdF(3, 2) = (F[3] * F[7] + F[1] * F[5]) * dSdF(5, 2) +
                 (F[0] * F[7] + F[4] * F[5]) * dSdF(4, 2) +
                 (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 2) +
                 cste * F[5] * F[7] * dSdF(2, 2) +
                 cste * F[1] * F[3] * dSdF(1, 2) +
                 cste * F[0] * F[4] * dSdF(0, 2);
    dTdF(4, 2) = (F[5] * F[8] + F[2] * F[3]) * dSdF(5, 2) + F[3] * S[5] +
                 (F[5] * F[6] + F[0] * F[2]) * dSdF(4, 2) + F[0] * S[4] +
                 (F[0] * F[8] + F[3] * F[6]) * dSdF(3, 2) +
                 cste * F[2] * F[5] * dSdF(2, 2) + cste * F[5] * S[2] +
                 cste * F[3] * F[8] * dSdF(1, 2) +
                 cste * F[0] * F[6] * dSdF(0, 2);
    dTdF(5, 2) = (F[7] * F[8] + F[1] * F[2]) * dSdF(5, 2) + F[1] * S[5] +
                 (F[6] * F[7] + F[2] * F[4]) * dSdF(4, 2) + F[4] * S[4] +
                 (F[4] * F[8] + F[1] * F[6]) * dSdF(3, 2) +
                 cste * F[2] * F[7] * dSdF(2, 2) + cste * F[7] * S[2] +
                 cste * F[1] * F[8] * dSdF(1, 2) +
                 cste * F[4] * F[6] * dSdF(0, 2);
    // derivative with respect to F3
    dTdF(0, 3) =
        F[3] * ((F[5] * dSdF(5, 3)) * icste + (F[0] * dSdF(3, 3)) * icste +
                F[3] * dSdF(1, 3) + S[1]) +
        F[5] * ((F[3] * dSdF(5, 3)) * icste + (S[5]) * icste +
                (F[0] * dSdF(4, 3)) * icste + F[5] * dSdF(2, 3)) +
        (F[5] * S[5]) * icste +
        F[0] * ((F[5] * dSdF(4, 3)) * icste + (F[3] * dSdF(3, 3)) * icste +
                (S[3]) * icste + F[0] * dSdF(0, 3)) +
        (F[0] * S[3]) * icste + F[3] * S[1];
    dTdF(1, 3) = F[1] * ((F[7] * dSdF(5, 3)) * icste +
                         (F[4] * dSdF(3, 3)) * icste + F[1] * dSdF(1, 3)) +
                 F[7] * ((F[1] * dSdF(5, 3)) * icste +
                         (F[4] * dSdF(4, 3)) * icste + F[7] * dSdF(2, 3)) +
                 F[4] * ((F[7] * dSdF(4, 3)) * icste +
                         (F[1] * dSdF(3, 3)) * icste + F[4] * dSdF(0, 3));
    dTdF(2, 3) = F[2] * ((F[8] * dSdF(5, 3)) * icste +
                         (F[6] * dSdF(4, 3)) * icste + F[2] * dSdF(2, 3)) +
                 F[8] * ((F[2] * dSdF(5, 3)) * icste +
                         (F[6] * dSdF(3, 3)) * icste + F[8] * dSdF(1, 3)) +
                 F[6] * ((F[2] * dSdF(4, 3)) * icste +
                         (F[8] * dSdF(3, 3)) * icste + F[6] * dSdF(0, 3));
    dTdF(3, 3) = (F[3] * F[7] + F[1] * F[5]) * dSdF(5, 3) + F[7] * S[5] +
                 (F[0] * F[7] + F[4] * F[5]) * dSdF(4, 3) +
                 (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 3) + F[4] * S[3] +
                 cste * F[5] * F[7] * dSdF(2, 3) +
                 cste * F[1] * F[3] * dSdF(1, 3) + cste * F[1] * S[1] +
                 cste * F[0] * F[4] * dSdF(0, 3);
    dTdF(4, 3) = (F[5] * F[8] + F[2] * F[3]) * dSdF(5, 3) + F[2] * S[5] +
                 (F[5] * F[6] + F[0] * F[2]) * dSdF(4, 3) +
                 (F[0] * F[8] + F[3] * F[6]) * dSdF(3, 3) + F[6] * S[3] +
                 cste * F[2] * F[5] * dSdF(2, 3) +
                 cste * F[3] * F[8] * dSdF(1, 3) + cste * F[8] * S[1] +
                 cste * F[0] * F[6] * dSdF(0, 3);
    dTdF(5, 3) = (F[7] * F[8] + F[1] * F[2]) * dSdF(5, 3) +
                 (F[6] * F[7] + F[2] * F[4]) * dSdF(4, 3) +
                 (F[4] * F[8] + F[1] * F[6]) * dSdF(3, 3) +
                 cste * F[2] * F[7] * dSdF(2, 3) +
                 cste * F[1] * F[8] * dSdF(1, 3) +
                 cste * F[4] * F[6] * dSdF(0, 3);
    // derivative with respect to F4
    dTdF(0, 4) = F[3] * ((F[5] * dSdF(5, 4)) * icste +
                         (F[0] * dSdF(3, 4)) * icste + F[3] * dSdF(1, 4)) +
                 F[5] * ((F[3] * dSdF(5, 4)) * icste +
                         (F[0] * dSdF(4, 4)) * icste + F[5] * dSdF(2, 4)) +
                 F[0] * ((F[5] * dSdF(4, 4)) * icste +
                         (F[3] * dSdF(3, 4)) * icste + F[0] * dSdF(0, 4));
    dTdF(1, 4) =
        F[1] * ((F[7] * dSdF(5, 4)) * icste + (F[4] * dSdF(3, 4)) * icste +
                (S[3]) * icste + F[1] * dSdF(1, 4)) +
        F[7] * ((F[1] * dSdF(5, 4)) * icste + (F[4] * dSdF(4, 4)) * icste +
                (S[4]) * icste + F[7] * dSdF(2, 4)) +
        F[4] * ((F[7] * dSdF(4, 4)) * icste + (F[1] * dSdF(3, 4)) * icste +
                F[4] * dSdF(0, 4) + S[0]) +
        (F[7] * S[4]) * icste + (F[1] * S[3]) * icste + F[4] * S[0];
    dTdF(2, 4) = F[2] * ((F[8] * dSdF(5, 4)) * icste +
                         (F[6] * dSdF(4, 4)) * icste + F[2] * dSdF(2, 4)) +
                 F[8] * ((F[2] * dSdF(5, 4)) * icste +
                         (F[6] * dSdF(3, 4)) * icste + F[8] * dSdF(1, 4)) +
                 F[6] * ((F[2] * dSdF(4, 4)) * icste +
                         (F[8] * dSdF(3, 4)) * icste + F[6] * dSdF(0, 4));
    dTdF(3, 4) = (F[3] * F[7] + F[1] * F[5]) * dSdF(5, 4) +
                 (F[0] * F[7] + F[4] * F[5]) * dSdF(4, 4) + F[5] * S[4] +
                 (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 4) + F[3] * S[3] +
                 cste * F[5] * F[7] * dSdF(2, 4) +
                 cste * F[1] * F[3] * dSdF(1, 4) +
                 cste * F[0] * F[4] * dSdF(0, 4) + cste * F[0] * S[0];
    dTdF(4, 4) = (F[5] * F[8] + F[2] * F[3]) * dSdF(5, 4) +
                 (F[5] * F[6] + F[0] * F[2]) * dSdF(4, 4) +
                 (F[0] * F[8] + F[3] * F[6]) * dSdF(3, 4) +
                 cste * F[2] * F[5] * dSdF(2, 4) +
                 cste * F[3] * F[8] * dSdF(1, 4) +
                 cste * F[0] * F[6] * dSdF(0, 4);
    dTdF(5, 4) = (F[7] * F[8] + F[1] * F[2]) * dSdF(5, 4) +
                 (F[6] * F[7] + F[2] * F[4]) * dSdF(4, 4) + F[2] * S[4] +
                 (F[4] * F[8] + F[1] * F[6]) * dSdF(3, 4) + F[8] * S[3] +
                 cste * F[2] * F[7] * dSdF(2, 4) +
                 cste * F[1] * F[8] * dSdF(1, 4) +
                 cste * F[4] * F[6] * dSdF(0, 4) + cste * F[6] * S[0];
    // derivative with respect to F5
    dTdF(0, 5) =
        F[3] * ((F[5] * dSdF(5, 5)) * icste + (S[5]) * icste +
                (F[0] * dSdF(3, 5)) * icste + F[3] * dSdF(1, 5)) +
        F[5] * ((F[3] * dSdF(5, 5)) * icste + (F[0] * dSdF(4, 5)) * icste +
                F[5] * dSdF(2, 5) + S[2]) +
        (F[3] * S[5]) * icste +
        F[0] * ((F[5] * dSdF(4, 5)) * icste + (S[4]) * icste +
                (F[3] * dSdF(3, 5)) * icste + F[0] * dSdF(0, 5)) +
        (F[0] * S[4]) * icste + F[5] * S[2];
    dTdF(1, 5) = F[1] * ((F[7] * dSdF(5, 5)) * icste +
                         (F[4] * dSdF(3, 5)) * icste + F[1] * dSdF(1, 5)) +
                 F[7] * ((F[1] * dSdF(5, 5)) * icste +
                         (F[4] * dSdF(4, 5)) * icste + F[7] * dSdF(2, 5)) +
                 F[4] * ((F[7] * dSdF(4, 5)) * icste +
                         (F[1] * dSdF(3, 5)) * icste + F[4] * dSdF(0, 5));
    dTdF(2, 5) = F[2] * ((F[8] * dSdF(5, 5)) * icste +
                         (F[6] * dSdF(4, 5)) * icste + F[2] * dSdF(2, 5)) +
                 F[8] * ((F[2] * dSdF(5, 5)) * icste +
                         (F[6] * dSdF(3, 5)) * icste + F[8] * dSdF(1, 5)) +
                 F[6] * ((F[2] * dSdF(4, 5)) * icste +
                         (F[8] * dSdF(3, 5)) * icste + F[6] * dSdF(0, 5));
    dTdF(3, 5) = (F[3] * F[7] + F[1] * F[5]) * dSdF(5, 5) + F[1] * S[5] +
                 (F[0] * F[7] + F[4] * F[5]) * dSdF(4, 5) + F[4] * S[4] +
                 (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 5) +
                 cste * F[5] * F[7] * dSdF(2, 5) + cste * F[7] * S[2] +
                 cste * F[1] * F[3] * dSdF(1, 5) +
                 cste * F[0] * F[4] * dSdF(0, 5);
    dTdF(4, 5) = (F[5] * F[8] + F[2] * F[3]) * dSdF(5, 5) + F[8] * S[5] +
                 (F[5] * F[6] + F[0] * F[2]) * dSdF(4, 5) + F[6] * S[4] +
                 (F[0] * F[8] + F[3] * F[6]) * dSdF(3, 5) +
                 cste * F[2] * F[5] * dSdF(2, 5) + cste * F[2] * S[2] +
                 cste * F[3] * F[8] * dSdF(1, 5) +
                 cste * F[0] * F[6] * dSdF(0, 5);
    dTdF(5, 5) = (F[7] * F[8] + F[1] * F[2]) * dSdF(5, 5) +
                 (F[6] * F[7] + F[2] * F[4]) * dSdF(4, 5) +
                 (F[4] * F[8] + F[1] * F[6]) * dSdF(3, 5) +
                 cste * F[2] * F[7] * dSdF(2, 5) +
                 cste * F[1] * F[8] * dSdF(1, 5) +
                 cste * F[4] * F[6] * dSdF(0, 5);
    // derivative with respect to F6
    dTdF(0, 6) = F[3] * ((F[5] * dSdF(5, 6)) * icste +
                         (F[0] * dSdF(3, 6)) * icste + F[3] * dSdF(1, 6)) +
                 F[5] * ((F[3] * dSdF(5, 6)) * icste +
                         (F[0] * dSdF(4, 6)) * icste + F[5] * dSdF(2, 6)) +
                 F[0] * ((F[5] * dSdF(4, 6)) * icste +
                         (F[3] * dSdF(3, 6)) * icste + F[0] * dSdF(0, 6));
    dTdF(1, 6) = F[1] * ((F[7] * dSdF(5, 6)) * icste +
                         (F[4] * dSdF(3, 6)) * icste + F[1] * dSdF(1, 6)) +
                 F[7] * ((F[1] * dSdF(5, 6)) * icste +
                         (F[4] * dSdF(4, 6)) * icste + F[7] * dSdF(2, 6)) +
                 F[4] * ((F[7] * dSdF(4, 6)) * icste +
                         (F[1] * dSdF(3, 6)) * icste + F[4] * dSdF(0, 6));
    dTdF(2, 6) =
        F[2] * ((F[8] * dSdF(5, 6)) * icste + (F[6] * dSdF(4, 6)) * icste +
                (S[4]) * icste + F[2] * dSdF(2, 6)) +
        F[8] * ((F[2] * dSdF(5, 6)) * icste + (F[6] * dSdF(3, 6)) * icste +
                (S[3]) * icste + F[8] * dSdF(1, 6)) +
        F[6] * ((F[2] * dSdF(4, 6)) * icste + (F[8] * dSdF(3, 6)) * icste +
                F[6] * dSdF(0, 6) + S[0]) +
        (F[2] * S[4]) * icste + (F[8] * S[3]) * icste + F[6] * S[0];
    dTdF(3, 6) = (F[3] * F[7] + F[1] * F[5]) * dSdF(5, 6) +
                 (F[0] * F[7] + F[4] * F[5]) * dSdF(4, 6) +
                 (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 6) +
                 cste * F[5] * F[7] * dSdF(2, 6) +
                 cste * F[1] * F[3] * dSdF(1, 6) +
                 cste * F[0] * F[4] * dSdF(0, 6);
    dTdF(4, 6) = (F[5] * F[8] + F[2] * F[3]) * dSdF(5, 6) +
                 (F[5] * F[6] + F[0] * F[2]) * dSdF(4, 6) + F[5] * S[4] +
                 (F[0] * F[8] + F[3] * F[6]) * dSdF(3, 6) + F[3] * S[3] +
                 cste * F[2] * F[5] * dSdF(2, 6) +
                 cste * F[3] * F[8] * dSdF(1, 6) +
                 cste * F[0] * F[6] * dSdF(0, 6) + cste * F[0] * S[0];
    dTdF(5, 6) = (F[7] * F[8] + F[1] * F[2]) * dSdF(5, 6) +
                 (F[6] * F[7] + F[2] * F[4]) * dSdF(4, 6) + F[7] * S[4] +
                 (F[4] * F[8] + F[1] * F[6]) * dSdF(3, 6) + F[1] * S[3] +
                 cste * F[2] * F[7] * dSdF(2, 6) +
                 cste * F[1] * F[8] * dSdF(1, 6) +
                 cste * F[4] * F[6] * dSdF(0, 6) + cste * F[4] * S[0];
    // derivative with respect to F7
    dTdF(0, 7) = F[3] * ((F[5] * dSdF(5, 7)) * icste +
                         (F[0] * dSdF(3, 7)) * icste + F[3] * dSdF(1, 7)) +
                 F[5] * ((F[3] * dSdF(5, 7)) * icste +
                         (F[0] * dSdF(4, 7)) * icste + F[5] * dSdF(2, 7)) +
                 F[0] * ((F[5] * dSdF(4, 7)) * icste +
                         (F[3] * dSdF(3, 7)) * icste + F[0] * dSdF(0, 7));
    dTdF(1, 7) =
        F[1] * ((F[7] * dSdF(5, 7)) * icste + (S[5]) * icste +
                (F[4] * dSdF(3, 7)) * icste + F[1] * dSdF(1, 7)) +
        F[7] * ((F[1] * dSdF(5, 7)) * icste + (F[4] * dSdF(4, 7)) * icste +
                F[7] * dSdF(2, 7) + S[2]) +
        (F[1] * S[5]) * icste +
        F[4] * ((F[7] * dSdF(4, 7)) * icste + (S[4]) * icste +
                (F[1] * dSdF(3, 7)) * icste + F[4] * dSdF(0, 7)) +
        (F[4] * S[4]) * icste + F[7] * S[2];
    dTdF(2, 7) = F[2] * ((F[8] * dSdF(5, 7)) * icste +
                         (F[6] * dSdF(4, 7)) * icste + F[2] * dSdF(2, 7)) +
                 F[8] * ((F[2] * dSdF(5, 7)) * icste +
                         (F[6] * dSdF(3, 7)) * icste + F[8] * dSdF(1, 7)) +
                 F[6] * ((F[2] * dSdF(4, 7)) * icste +
                         (F[8] * dSdF(3, 7)) * icste + F[6] * dSdF(0, 7));
    dTdF(3, 7) = (F[3] * F[7] + F[1] * F[5]) * dSdF(5, 7) + F[3] * S[5] +
                 (F[0] * F[7] + F[4] * F[5]) * dSdF(4, 7) + F[0] * S[4] +
                 (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 7) +
                 cste * F[5] * F[7] * dSdF(2, 7) + cste * F[5] * S[2] +
                 cste * F[1] * F[3] * dSdF(1, 7) +
                 cste * F[0] * F[4] * dSdF(0, 7);
    dTdF(4, 7) = (F[5] * F[8] + F[2] * F[3]) * dSdF(5, 7) +
                 (F[5] * F[6] + F[0] * F[2]) * dSdF(4, 7) +
                 (F[0] * F[8] + F[3] * F[6]) * dSdF(3, 7) +
                 cste * F[2] * F[5] * dSdF(2, 7) +
                 cste * F[3] * F[8] * dSdF(1, 7) +
                 cste * F[0] * F[6] * dSdF(0, 7);
    dTdF(5, 7) = (F[7] * F[8] + F[1] * F[2]) * dSdF(5, 7) + F[8] * S[5] +
                 (F[6] * F[7] + F[2] * F[4]) * dSdF(4, 7) + F[6] * S[4] +
                 (F[4] * F[8] + F[1] * F[6]) * dSdF(3, 7) +
                 cste * F[2] * F[7] * dSdF(2, 7) + cste * F[2] * S[2] +
                 cste * F[1] * F[8] * dSdF(1, 7) +
                 cste * F[4] * F[6] * dSdF(0, 7);
    // derivative with respect to F8
    dTdF(0, 8) = F[3] * ((F[5] * dSdF(5, 8)) * icste +
                         (F[0] * dSdF(3, 8)) * icste + F[3] * dSdF(1, 8)) +
                 F[5] * ((F[3] * dSdF(5, 8)) * icste +
                         (F[0] * dSdF(4, 8)) * icste + F[5] * dSdF(2, 8)) +
                 F[0] * ((F[5] * dSdF(4, 8)) * icste +
                         (F[3] * dSdF(3, 8)) * icste + F[0] * dSdF(0, 8));
    dTdF(1, 8) = F[1] * ((F[7] * dSdF(5, 8)) * icste +
                         (F[4] * dSdF(3, 8)) * icste + F[1] * dSdF(1, 8)) +
                 F[7] * ((F[1] * dSdF(5, 8)) * icste +
                         (F[4] * dSdF(4, 8)) * icste + F[7] * dSdF(2, 8)) +
                 F[4] * ((F[7] * dSdF(4, 8)) * icste +
                         (F[1] * dSdF(3, 8)) * icste + F[4] * dSdF(0, 8));
    dTdF(2, 8) =
        F[2] * ((F[8] * dSdF(5, 8)) * icste + (S[5]) * icste +
                (F[6] * dSdF(4, 8)) * icste + F[2] * dSdF(2, 8)) +
        F[8] * ((F[2] * dSdF(5, 8)) * icste + (F[6] * dSdF(3, 8)) * icste +
                F[8] * dSdF(1, 8) + S[1]) +
        (F[2] * S[5]) * icste +
        F[6] * ((F[2] * dSdF(4, 8)) * icste + (F[8] * dSdF(3, 8)) * icste +
                (S[3]) * icste + F[6] * dSdF(0, 8)) +
        (F[6] * S[3]) * icste + F[8] * S[1];
    dTdF(3, 8) = (F[3] * F[7] + F[1] * F[5]) * dSdF(5, 8) +
                 (F[0] * F[7] + F[4] * F[5]) * dSdF(4, 8) +
                 (F[3] * F[4] + F[0] * F[1]) * dSdF(3, 8) +
                 cste * F[5] * F[7] * dSdF(2, 8) +
                 cste * F[1] * F[3] * dSdF(1, 8) +
                 cste * F[0] * F[4] * dSdF(0, 8);
    dTdF(4, 8) = (F[5] * F[8] + F[2] * F[3]) * dSdF(5, 8) + F[5] * S[5] +
                 (F[5] * F[6] + F[0] * F[2]) * dSdF(4, 8) +
                 (F[0] * F[8] + F[3] * F[6]) * dSdF(3, 8) + F[0] * S[3] +
                 cste * F[2] * F[5] * dSdF(2, 8) +
                 cste * F[3] * F[8] * dSdF(1, 8) + cste * F[3] * S[1] +
                 cste * F[0] * F[6] * dSdF(0, 8);
    dTdF(5, 8) = (F[7] * F[8] + F[1] * F[2]) * dSdF(5, 8) + F[7] * S[5] +
                 (F[6] * F[7] + F[2] * F[4]) * dSdF(4, 8) +
                 (F[4] * F[8] + F[1] * F[6]) * dSdF(3, 8) + F[4] * S[3] +
                 cste * F[2] * F[7] * dSdF(2, 8) +
                 cste * F[1] * F[8] * dSdF(1, 8) + cste * F[1] * S[1] +
                 cste * F[4] * F[6] * dSdF(0, 8);
  }

  template <typename T2toST2ResultType,
            typename T2toST2Type,
            typename StensorType,
            typename TensorType>
  typename std::enable_if<
      implementsT2toST2Concept<T2toST2ResultType>() &&
          implementsT2toST2Concept<T2toST2Type>() &&
          implementsStensorConcept<StensorType>() &&
          implementsTensorConcept<TensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<T2toST2Type>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<StensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<TensorType>() &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>(),
      void>::type
  computeCauchyStressDerivativeFromKirchhoffStressDerivative(
      T2toST2ResultType& ds,
      const T2toST2Type& dt_K,
      const StensorType& s,
      const TensorType& F) {
    const auto iJ = 1 / det(F);
    const auto dJ = computeDeterminantDerivative(F);
    ds = iJ * (dt_K - (s ^ dJ));
  }

  template <typename T2toST2ResultType,
            typename T2toST2Type,
            typename StensorType,
            typename TensorType>
  typename std::enable_if<
      implementsT2toST2Concept<T2toST2ResultType>() &&
          implementsT2toST2Concept<T2toST2Type>() &&
          implementsStensorConcept<StensorType>() &&
          implementsTensorConcept<TensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<T2toST2Type>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<StensorType>() &&
          getSpaceDimension<T2toST2ResultType>() ==
              getSpaceDimension<TensorType>() &&
          tfel::typetraits::IsFundamentalNumericType<
              numeric_type<TensorType>>::cond &&
          isAssignableTo<typename ComputeBinaryResult<numeric_type<T2toST2Type>,
                                                      numeric_type<StensorType>,
                                                      OpPlus>::Result,
                         numeric_type<T2toST2ResultType>>(),
      void>::type
  computeKirchhoffStressDerivativeFromCauchyStressDerivative(
      T2toST2ResultType& dt_K,
      const T2toST2Type& ds,
      const StensorType& s,
      const TensorType& F) {
    const auto J = det(F);
    const auto dJ = computeDeterminantDerivative(F);
    dt_K = J * ds + (s ^ dJ);
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_T2TOST2_CONCEPT_IXX */
