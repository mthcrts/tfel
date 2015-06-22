/*!
 * \file   tests/Tests/testfunctionwrapper.cxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief a simple test of the TestFunctionWrapper class
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<cassert>
#include<memory>

#include"TFEL/Tests/TestFunctionWrapper.hxx"

#if !( defined _MSC_VER )
#define TFEL_TESTS_STATIC static
#else
#define TFEL_TESTS_STATIC
#endif

TFEL_TESTS_STATIC bool test1(void)
{
  return true;
}

TFEL_TESTS_STATIC bool test2(void)
{
  return false;
}

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  typedef TestFunctionWrapper<test1> Wrapper1;
  typedef TestFunctionWrapper<test2> Wrapper2;
  auto_ptr<Test> a(new Wrapper1("test1"));
  auto_ptr<Test> b(new Wrapper2("test2"));
  // first test
  assert(a->name()=="test1");
  assert(a->execute().success());
  // second test
  assert(b->name()=="test2");
  assert(!b->execute().success());
  return EXIT_SUCCESS;
} // end of main
