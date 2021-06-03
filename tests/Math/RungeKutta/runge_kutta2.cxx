/*!
 * \file   tests/Math/RungeKutta/runge_kutta2.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20 jui 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "runge_kutta2-header.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;

  test<float>();
  test<double>();
  test<long double>();

  return EXIT_SUCCESS;
}
