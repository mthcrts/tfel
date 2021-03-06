/*! 
 * \file  ConceptRebind.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12 févr. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_CONCEPTREBIND_HXX
#define LIB_TFEL_MATH_CONCEPTREBIND_HXX 

namespace tfel{
  
  namespace math{
    
    /*!
     * The ConceptRebind metafunction use the given tag to define a
     * base class for the Type class.0
     */
    template<typename Tag,typename Type>
    struct ConceptRebind;

  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_CONCEPTREBIND_HXX */

