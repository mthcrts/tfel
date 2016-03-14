/*! 
 * \file   test-c
 * \author Helfer Thomas
 * \date   16 jan 2010
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cstdlib>
#include<cassert>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"ErrnoHandlingCheck-cxx.hxx"
#include"VanadiumAlloy_YoungModulus_SRMA-cxx.hxx"
#include"VanadiumAlloy_PoissonRatio_SRMA-cxx.hxx"

struct CxxMaterialPropertyInterfaceTest final
  : public tfel::tests::TestCase
{
  CxxMaterialPropertyInterfaceTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "CxxMaterialPropertyInterfaceTest")
  {} // end of CxxMaterialPropertyInterfaceTest

  virtual tfel::tests::TestResult
  execute() override
  {
    const auto y = [](const double T){
      return 127.8e9 * (1.-7.825e-5*(T-293.15));
    };
    const auto n = [](const double T){
      return 0.3272  * (1.-3.056e-5*(T-293.15));
    };
    const auto e_c  = mfront::ErrnoHandlingCheck();
    const auto mp_y = mfront::VanadiumAlloy_YoungModulus_SRMA();
    const auto mp_n = mfront::VanadiumAlloy_PoissonRatio_SRMA();
    TFEL_TESTS_ASSERT(std::abs(mp_y(900)-y(900))<1.e-14*y(900));
    TFEL_TESTS_ASSERT(std::abs(mp_n(900)-n(900))<1.e-14*n(900));
    TFEL_TESTS_CHECK_THROW(mp_y(-900),std::range_error);
    TFEL_TESTS_CHECK_THROW(mp_n(-900),std::range_error);
    unsetenv("OUT_OF_BOUNDS_POLICY");
    TFEL_TESTS_ASSERT(std::abs(mp_y(50)-y(50))<1.e-14*y(50));
    setenv("OUT_OF_BOUNDS_POLICY","NONE",1);
    TFEL_TESTS_ASSERT(std::abs(mp_y(50)-y(50))<1.e-14*y(50));
    setenv("OUT_OF_BOUNDS_POLICY","WARNING",1);
    TFEL_TESTS_ASSERT(std::abs(mp_y(50)-y(50))<1.e-14*y(50));
    setenv("OUT_OF_BOUNDS_POLICY","STRICT",1);
    TFEL_TESTS_CHECK_THROW(mp_y(50),std::range_error);
    TFEL_TESTS_CHECK_THROW(mp_n(50),std::range_error);
    TFEL_TESTS_CHECK_THROW(e_c(-2),std::runtime_error);
    TFEL_TESTS_CHECK_THROW(e_c(2),std::runtime_error);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(CxxMaterialPropertyInterfaceTest,
			  "CxxMaterialPropertyInterfaceTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main(void)
{
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("CxxMaterialPropertyInterface.xml");
  const auto r = manager.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
} // end of main
