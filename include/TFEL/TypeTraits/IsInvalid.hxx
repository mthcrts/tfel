/*!
 * \file   include/TFEL/TypeTraits/IsInvalid.hxx
 * \brief  This file declares the IsInvalid traits class.
 * \author Thomas Helfer
 * \date   14 oct 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_ISINVALID_HXX
#define LIB_TFEL_ISINVALID_HXX 

#include"TFEL/Metaprogramming/InvalidType.hxx"

namespace tfel{

  namespace typetraits{

    /*!
     * \brief  Traits class which says if its argument is invalid.
     *
     * \param  T, type to be tested.
     * \return bool cond, true if T is invalid, false otherwise.
     *
     * \author Thomas Helfer
     * \date   Apr 2006
     */
    template<typename T>
    struct IsInvalid{
      /*!
       *  Result
       */
      static constexpr bool cond = false;
    };

    /*
     * Partial Specialisation
     */
    template<>
    struct IsInvalid<tfel::meta::InvalidType>{
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };

  } // end of namespace typetraits

} // end of namespace tfel  

#endif /* LIB_TFEL_ISINVALID_HXX */

