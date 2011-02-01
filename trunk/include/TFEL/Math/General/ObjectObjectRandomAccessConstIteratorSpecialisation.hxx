/*!
 * \file   ObjectObjectRandomAccessConstIteratorSpecialisation.hxx
 * \brief  This file declares the  class
 * \author Helfer Thomas
 * \date   17 Oct 2006
 */

#ifndef _LIB_TFEL_OBJECTOBJECTRANDOMACCESSCONSTITERATORSPECIALISATION_H_
#define _LIB_TFEL_OBJECTOBJECTRANDOMACCESSCONSTITERATORSPECIALISATION_H_ 

#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/ObjectObjectRandomAccessConstIterator.hxx"

namespace tfel{

  namespace math{

    /*
     * Partial Specialisation
     */
    template<typename A,typename B>
    class ObjectObjectRandomAccessConstIterator<A,B,OpPlus>
    {
      typedef typename A::const_iterator ConstIteratorTypeA;
      typedef typename B::const_iterator ConstIteratorTypeB;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeA>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeB>::cond));
      typedef typename std::iterator_traits<ConstIteratorTypeA>::value_type NumTypeA;
      typedef typename std::iterator_traits<ConstIteratorTypeB>::value_type NumTypeB;

      ConstIteratorTypeA pA;
      ConstIteratorTypeB pB;

      typedef typename std::iterator_traits<ConstIteratorTypeA>::difference_type Distance;

    public:

      /*!
       * brief  Return the name of the class.						    
       * param  void.									    
       * return std::string, the name of the class.					    
       * see    Name.									    
       */										    
      static const std::string getName(void){							    
	using namespace std;								    
	using namespace tfel::utilities;						    
	return string("ObjectObjectRandomAccessConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpPlus>::getName()+string(">");        
      }

      typedef std::random_access_iterator_tag iterator_category;
      typedef typename  ComputeBinaryResult<NumTypeA,NumTypeB,OpPlus>::Handle NumType;
      typedef const NumType   value_type;
      typedef Distance difference_type;
      typedef const NumType*  pointer;
      typedef const NumType&  reference;

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator()
      {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(const ConstIteratorTypeA pA_,
						 const ConstIteratorTypeB pB_)
	: pA(pA_), pB(pB_)
      {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(const ObjectObjectRandomAccessConstIterator& src)
	: pA(src.pA), pB(src.pB)
      {}
      
      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator=(const ObjectObjectRandomAccessConstIterator& src)
      {
	// self assignement does not have to be checked.
	pA = src.pA;
	pB = src.pB;
	return *this;
      }

      TFEL_MATH_INLINE value_type operator*(void) const 
      {
	return (*pA)+(*pB);
      }
      
      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(void)
      {
	pA++;
	pB++;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(int)
      {
	++pA;
	++pB;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+=(const Distance n)
      {
	pA+=n;
	pB+=n;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+(const Distance n)
      {
	return ObjectObjectRandomAccessConstIterator(pA+n,pB+n);
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-=(const Distance n)
      {
	pA-=n;
	pB-=n;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-(const Distance n)
      {
	return ObjectObjectRandomAccessConstIterator(pA-n,pB-n);
      }

      TFEL_MATH_INLINE value_type operator[](const Distance n) const 
      {
	return (pA[n])+(pB[n]);
      }

      TFEL_MATH_INLINE const Distance 
      operator-(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return pA-src.pA;
      }

      TFEL_MATH_INLINE const bool 
      operator==(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return (pA==src.pA)&&(pB==src.pB);
      }

      TFEL_MATH_INLINE const bool 
      operator!=(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return (pA!=src.pA)||(pB!=src.pB);
      }

    };

    /*
     * Partial Specialisation
     */
    template<typename A,typename B>
    class ObjectObjectRandomAccessConstIterator<A,B,OpMinus>
    {
      typedef typename A::const_iterator ConstIteratorTypeA;
      typedef typename B::const_iterator ConstIteratorTypeB;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeA>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeB>::cond));
      typedef typename std::iterator_traits<ConstIteratorTypeA>::value_type NumTypeA;
      typedef typename std::iterator_traits<ConstIteratorTypeB>::value_type NumTypeB;

      ConstIteratorTypeA pA;
      ConstIteratorTypeB pB;

      typedef typename std::iterator_traits<ConstIteratorTypeA>::difference_type Distance;

    public:

      /*!										    
       * brief  Return the name of the class.						    
       * param  void.									    
       * return std::string, the name of the class.					    
       * see    Name.									    
       */										    
      static const std::string getName(void){							    
	using namespace std;								    
	using namespace tfel::utilities;						    
	return string("ObjectObjectRandomAccessConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpMinus>::getName()+string(">");        
      }

      typedef std::random_access_iterator_tag iterator_category;
      typedef typename  ComputeBinaryResult<NumTypeA,NumTypeB,OpMinus>::Handle NumType;
      typedef const NumType   value_type;
      typedef Distance difference_type;
      typedef const NumType*  pointer;
      typedef const NumType&  reference;

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator()
      {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(const ConstIteratorTypeA pA_,
						 const ConstIteratorTypeB pB_)
	: pA(pA_), pB(pB_)
      {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(const ObjectObjectRandomAccessConstIterator& src)
	: pA(src.pA), pB(src.pB)
      {}
      
      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator=(const ObjectObjectRandomAccessConstIterator& src)
      {
	// self assignement does not have to be checked.
	pA = src.pA;
	pB = src.pB;
	return *this;
      }

      TFEL_MATH_INLINE value_type operator*(void) const 
      {
	return (*pA)-(*pB);
      }
      
      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(void)
      {
	pA++;
	pB++;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(int)
      {
	++pA;
	++pB;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+=(const Distance n)
      {
	pA+=n;
	pB+=n;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+(const Distance n)
      {
	return ObjectObjectRandomAccessConstIterator(pA+n,pB+n);
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-=(const Distance n)
      {
	pA-=n;
	pB-=n;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-(const Distance n)
      {
	return ObjectObjectRandomAccessConstIterator(pA-n,pB-n);
      }

      TFEL_MATH_INLINE value_type operator[](const Distance n) const 
      {
	return (pA[n])-(pB[n]);
      }

      TFEL_MATH_INLINE const Distance 
      operator-(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return pA-src.pA;
      }

      TFEL_MATH_INLINE const bool 
      operator==(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return (pA==src.pA)&&(pB==src.pB);
      }

      TFEL_MATH_INLINE const bool 
      operator!=(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return (pA!=src.pA)||(pB!=src.pB);
      }

    };

    /*
     * Partial Specialisation
     */
    template<typename A,typename B>
    class ObjectObjectRandomAccessConstIterator<A,B,OpMult>
    {
      typedef typename A::const_iterator ConstIteratorTypeA;
      typedef typename B::const_iterator ConstIteratorTypeB;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeA>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeB>::cond));
      typedef typename std::iterator_traits<ConstIteratorTypeA>::value_type NumTypeA;
      typedef typename std::iterator_traits<ConstIteratorTypeB>::value_type NumTypeB;

      ConstIteratorTypeA pA;
      ConstIteratorTypeB pB;

      typedef typename std::iterator_traits<ConstIteratorTypeA>::difference_type Distance;

    public:

      /*!										    
       * brief  Return the name of the class.						    
       * param  void.									    
       * return std::string, the name of the class.					    
       * see    Name.									    
       */										    
      static const std::string getName(void){							    
	using namespace std;								    
	using namespace tfel::utilities;						    
	return string("ObjectObjectRandomAccessConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpMult>::getName()+string(">");        
      }

      typedef std::random_access_iterator_tag iterator_category;
      typedef typename  ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;
      typedef const NumType   value_type;
      typedef Distance difference_type;
      typedef const NumType*  pointer;
      typedef const NumType&  reference;

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator()
      {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(const ConstIteratorTypeA pA_,
						 const ConstIteratorTypeB pB_)
	: pA(pA_), pB(pB_)
      {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(const ObjectObjectRandomAccessConstIterator& src)
	: pA(src.pA), pB(src.pB)
      {}
      
      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator=(const ObjectObjectRandomAccessConstIterator& src)
      {
	// self assignement does not have to be checked.
	pA = src.pA;
	pB = src.pB;
	return *this;
      }

      TFEL_MATH_INLINE value_type operator*(void) const 
      {
	return (*pA)*(*pB);
      }
      
      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(void)
      {
	pA++;
	pB++;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(int)
      {
	++pA;
	++pB;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+=(const Distance n)
      {
	pA+=n;
	pB+=n;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+(const Distance n)
      {
	return ObjectObjectRandomAccessConstIterator(pA+n,pB+n);
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-=(const Distance n)
      {
	pA-=n;
	pB-=n;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-(const Distance n)
      {
	return ObjectObjectRandomAccessConstIterator(pA-n,pB-n);
      }

      TFEL_MATH_INLINE value_type operator[](const Distance n) const 
      {
	return (pA[n])*(pB[n]);
      }

      TFEL_MATH_INLINE const Distance 
      operator-(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return pA-src.pA;
      }

      TFEL_MATH_INLINE const bool 
      operator==(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return (pA==src.pA)&&(pB==src.pB);
      }

      TFEL_MATH_INLINE const bool 
      operator!=(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return (pA!=src.pA)||(pB!=src.pB);
      }

    };

    /*
     * Partial Specialisation
     */
    template<typename A,typename B>
    class ObjectObjectRandomAccessConstIterator<A,B,OpDiv>
    {
      typedef typename A::const_iterator ConstIteratorTypeA;
      typedef typename B::const_iterator ConstIteratorTypeB;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeA>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsRandomAccessIterator<ConstIteratorTypeB>::cond));
      typedef typename std::iterator_traits<ConstIteratorTypeA>::value_type NumTypeA;
      typedef typename std::iterator_traits<ConstIteratorTypeB>::value_type NumTypeB;

      ConstIteratorTypeA pA;
      ConstIteratorTypeB pB;

      typedef typename std::iterator_traits<ConstIteratorTypeA>::difference_type Distance;

    public:

      /*!										    
       * brief  Return the name of the class.						    
       * param  void.									    
       * return std::string, the name of the class.					    
       * see    Name.									    
       */										    
      static const std::string getName(void){							    
	using namespace std;								    
	using namespace tfel::utilities;						    
	return string("ObjectObjectRandomAccessConstIterator<")+Name<A>::getName()+string(",")+Name<B>::getName()+string(",")+Name<OpDiv>::getName()+string(">");        
      }

      typedef std::random_access_iterator_tag iterator_category;
      typedef typename  ComputeBinaryResult<NumTypeA,NumTypeB,OpDiv>::Handle NumType;
      typedef const NumType   value_type;
      typedef Distance difference_type;
      typedef const NumType*  pointer;
      typedef const NumType&  reference;

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator()
      {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(const ConstIteratorTypeA pA_,
						 const ConstIteratorTypeB pB_)
	: pA(pA_), pB(pB_)
      {}

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator(const ObjectObjectRandomAccessConstIterator& src)
	: pA(src.pA), pB(src.pB)
      {}
      
      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator=(const ObjectObjectRandomAccessConstIterator& src)
      {
	// self assignement does not have to be checked.
	pA = src.pA;
	pB = src.pB;
	return *this;
      }

      TFEL_MATH_INLINE value_type operator*(void) const 
      {
	return (*pA)/(*pB);
      }
      
      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(void)
      {
	pA++;
	pB++;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator++(int)
      {
	++pA;
	++pB;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+=(const Distance n)
      {
	pA+=n;
	pB+=n;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator+(const Distance n)
      {
	return ObjectObjectRandomAccessConstIterator(pA+n,pB+n);
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-=(const Distance n)
      {
	pA-=n;
	pB-=n;
	return *this;
      }

      TFEL_MATH_INLINE ObjectObjectRandomAccessConstIterator& operator-(const Distance n)
      {
	return ObjectObjectRandomAccessConstIterator(pA-n,pB-n);
      }

      TFEL_MATH_INLINE value_type operator[](const Distance n) const 
      {
	return (pA[n])/(pB[n]);
      }

      TFEL_MATH_INLINE const Distance 
      operator-(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return pA-src.pA;
      }

      TFEL_MATH_INLINE const bool 
      operator==(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return (pA==src.pA)&&(pB==src.pB);
      }

      TFEL_MATH_INLINE const bool 
      operator!=(const ObjectObjectRandomAccessConstIterator& src) const
      {
	return (pA!=src.pA)||(pB!=src.pB);
      }

    };

  }// end of namespace math

}// end of namespace tfel

#endif /* _LIB_TFEL_OBJECTOBJECTRANDOMACCESSCONSTITERATORSPECIALISATION_H */

