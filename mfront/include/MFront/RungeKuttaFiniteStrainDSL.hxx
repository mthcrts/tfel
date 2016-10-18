/*! 
 * \file  mfront/include/MFront/RungeKuttaFiniteStrainDSL.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  17/10/2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_RUNGEKUTTAFINITESTRAINPARSER_H_
#define LIB_MFRONT_RUNGEKUTTAFINITESTRAINPARSER_H_ 

#include"MFront/RungeKuttaDSLBase.hxx"

namespace mfront
{

  /*!
   *
   */
  struct RungeKuttaFiniteStrainDSL
    : public RungeKuttaDSLBase
  {

    RungeKuttaFiniteStrainDSL();

    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    virtual ~RungeKuttaFiniteStrainDSL() noexcept;

  }; // end of struct RungeKuttaFiniteStrainDSL

} // end of namespace mfront

#endif /* LIB_MFRONT_RUNGEKUTTAFINITESTRAINPARSER_H_ */

