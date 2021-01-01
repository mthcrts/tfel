/*!
 * \file   include/TFEL/Math/st2tot2.hxx
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

#ifndef LIB_TFEL_MATH_ST2TOT2_HXX
#define LIB_TFEL_MATH_ST2TOT2_HXX

#include <cstddef>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsInvalid.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/fsarray.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Forward/st2tot2.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Math/ST2toT2/ST2toT2Concept.hxx"
#include "TFEL/Math/ST2toT2/ST2toT2ConceptOperations.hxx"
#include "TFEL/Math/ST2toT2/StensorProductLeftDerivativeExpr.hxx"
#include "TFEL/Math/ST2toT2/StensorProductRightDerivativeExpr.hxx"

namespace tfel::math {

  /*!
   * \brief partial specialisation of the `DerivativeTypeDispatcher`
   * metafunction.
   */
  template <typename TensorType1, typename StensorType2>
  struct DerivativeTypeDispatcher<TensorTag,
                                  StensorTag,
                                  TensorType1,
                                  StensorType2> {
    static_assert(implementsTensorConcept<TensorType1>(),
                  "template argument TensorType1 is not a tensor");
    static_assert(implementsStensorConcept<StensorType2>(),
                  "template argument StensorType2 is not a symmetric tensor");
    static_assert(getSpaceDimension<TensorType1>() ==
                      getSpaceDimension<StensorType2>(),
                  "symmetric tensor types don't have the same dimension");
    static_assert(tfel::typetraits::IsScalar<numeric_type<TensorType1>>::cond,
                  "the first tensor type does not hold a scalar");
    static_assert(
        tfel::typetraits::IsScalar<numeric_type<StensorType2>>::cond,
        "the second symmetric tensor type does not hold a scalar");
    //! \brief result
    using type = st2tot2<getSpaceDimension<TensorType1>(),
                         derivative_type<numeric_type<TensorType1>,
                                         numeric_type<StensorType2>>>;
  };  // end of struct DerivativeTypeDispatcher

  /*!
   * \brief a base for stensor or classes acting like stensor.
   * \param Child : child class
   * \param N     : spatial dimension
   * \param T     : numerical type
   */
  template <typename Child>
  struct st2tot2_base {
    /*!
     * Assignement operator
     */
    template <typename St2tot2Type>
    TFEL_MATH_INLINE std::enable_if_t<
        implementsST2toT2Concept<St2tot2Type>() &&
            getSpaceDimension<Child>() == getSpaceDimension<St2tot2Type>() &&
            tfel::typetraits::IsAssignableTo<
                numeric_type<St2tot2Type>,
                numeric_type<Child>>::cond,
        Child&>
    operator=(const St2tot2Type&);
    //! Assignement operator
    template <typename St2tot2Type>
    TFEL_MATH_INLINE std::enable_if_t<
        implementsST2toT2Concept<St2tot2Type>() &&
            getSpaceDimension<Child>() == getSpaceDimension<St2tot2Type>() &&
            tfel::typetraits::IsAssignableTo<
                numeric_type<St2tot2Type>,
                numeric_type<Child>>::cond,
        Child&>
    operator+=(const St2tot2Type&);
    //! Assignement operator
    template <typename St2tot2Type>
    TFEL_MATH_INLINE std::enable_if_t<
        implementsST2toT2Concept<St2tot2Type>() &&
            getSpaceDimension<Child>() == getSpaceDimension<St2tot2Type>() &&
            tfel::typetraits::IsAssignableTo<
                numeric_type<St2tot2Type>,
                numeric_type<Child>>::cond,
        Child&>
    operator-=(const St2tot2Type&);
    /*!
     * operator*=
     */
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<numeric_type<Child>,
                                    T2,
                                    OpMult>::type,
                numeric_type<Child>>::value,
        Child&>
    operator*=(const T2);
    /*!
     * operator/=
     */
    template <typename T2>
    TFEL_MATH_INLINE std::enable_if_t<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<numeric_type<Child>,
                                    T2,
                                    OpDiv>::type,
                numeric_type<Child>>::value,
        Child&>
    operator/=(const T2);
  };  // end of struct st2tot2_base

  template <unsigned short N, typename T>
  struct st2tot2
      : public ST2toT2Concept<st2tot2<N, T>>,
        public fsarray<StensorDimeToSize<N>::value * TensorDimeToSize<N>::value,
                       T>,
        public st2tot2_base<st2tot2<N, T>> {
    /*!
     * \param[in] B : second tensor of the product
     * \return the left part of the derivative of a tensor product
     */
    template <typename StensorType>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsStensorConcept<StensorType>() &&
            getSpaceDimension<StensorType>() == N &&
            tfel::typetraits::IsAssignableTo<
                numeric_type<StensorType>,
                T>::cond,
        Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>>
    tpld(const StensorType&);
    /*!
     * \param[in] B : second tensor of the product
     * \param[in] C : derivative of the first tensor
     * \return the left part of the derivative of a tensor product
     */
    template <typename StensorType, typename ST2toST2Type>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsStensorConcept<StensorType>() &&
            implementsST2toST2Concept<ST2toST2Type>() &&
            getSpaceDimension<StensorType>() == N &&
            getSpaceDimension<ST2toST2Type>() == N &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    numeric_type<StensorType>,
                    numeric_type<ST2toST2Type>,
                    OpMult>::Result,
                T>::cond,
        Expr<st2tot2<N, T>, StensorProductLeftDerivativeExpr<N>>>
    tpld(const StensorType&, const ST2toST2Type&);
    /*!
     * \param[in] A : first tensor of the product
     * \return the right part of the derivative of a tensor product
     */
    template <typename StensorType>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsStensorConcept<StensorType>() &&
            getSpaceDimension<StensorType>() == N &&
            tfel::typetraits::IsAssignableTo<
                numeric_type<StensorType>,
                T>::cond,
        Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>>
    tprd(const StensorType&);
    /*!
     * \param[in] A : first tensor of the product
     * \param[in] C : derivative of the first tensor
     * \return the right part of the derivative of a tensor product
     */
    template <typename StensorType, typename ST2toST2Type>
    static TFEL_MATH_INLINE std::enable_if_t<
        implementsStensorConcept<StensorType>() &&
            implementsST2toST2Concept<ST2toST2Type>() &&
            getSpaceDimension<StensorType>() == N &&
            getSpaceDimension<ST2toST2Type>() == N &&
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<
                    numeric_type<StensorType>,
                    numeric_type<ST2toST2Type>,
                    OpMult>::Result,
                T>::cond,
        Expr<st2tot2<N, T>, StensorProductRightDerivativeExpr<N>>>
    tprd(const StensorType&, const ST2toST2Type&);
    /*!
     * This is a StensorConcept requirement.
     */
    typedef EmptyRunTimeProperties RunTimeProperties;
    /*!
     * \brief Default Constructor
     */
    TFEL_MATH_INLINE explicit constexpr st2tot2();
    /*!
     * \brief default Constructor
     * \param[in] init: value used to initialise the components of the st2tot2
     */
    template <typename T2,
              std::enable_if_t<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                               bool> = true>
    TFEL_MATH_INLINE explicit constexpr st2tot2(const T2&);
    /*!
     * \brief Copy Constructor
     */
    TFEL_MATH_INLINE constexpr st2tot2(const st2tot2<N, T>& src);
    // Copy Constructor
    template <typename T2, typename Op>
    TFEL_MATH_INLINE st2tot2(const Expr<st2tot2<N, T2>, Op>& x);
    //! assignement operator
    TFEL_MATH_INLINE st2tot2& operator=(const st2tot2<N, T>&);
    /*!
     * Import values
     */
    template <typename T2>
    TFEL_MATH_INLINE2 std::enable_if_t<
        tfel::typetraits::IsSafelyReinterpretCastableTo<T2, base_type<T>>::cond,
        void>
    import(const T2* const);
    /*!
     * Assignement operator
     */
    using st2tot2_base<st2tot2>::operator=;

    //! access operator
    TFEL_MATH_INLINE constexpr const T& operator()(const unsigned short,
                                                   const unsigned short) const;
    //! access operator
    TFEL_MATH_INLINE T& operator()(const unsigned short, const unsigned short);
    /*!
     * Return the RunTimeProperties of the tvector
     * \return tvector::RunTimeProperties
     */
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const;

    template <typename InputIterator>
    TFEL_MATH_INLINE2 void copy(const InputIterator src);

  };  // end of struct st2tot2

}  // end of namespace tfel::math

namespace tfel::typetraits {

  template <unsigned short N, typename T2, typename T>
  struct IsAssignableTo<tfel::math::st2tot2<N, T2>, tfel::math::st2tot2<N, T>> {
    /*!
     *  Result
     */
    static constexpr bool cond = IsAssignableTo<T2, T>::cond;
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/ST2toT2/st2tot2.ixx"
#include "TFEL/Math/ST2toT2/st2tot2ResultType.hxx"

#endif /* LIB_TFEL_MATH_ST2TOT2_HXX */
