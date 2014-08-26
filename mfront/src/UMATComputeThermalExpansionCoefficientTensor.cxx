/*!
 * \file   UMATComputeThermalExpansionCoefficientTensor.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#include<algorithm>

#include"TFEL/Math/stensor.hxx"
#include"MFront/UMAT/UMATComputeThermalExpansionCoefficientTensor.hxx"

namespace umat
{

  static void
  UMATComputeIsotropicThermalExpansionCoefficientTensor2D(const UMATReal* const props,
					       tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a = props[3];
    A(0)=A(1)=A(2)=a;
    A(3)=0.;
  }

  static void
  UMATComputeOrthotropicThermalExpansionCoefficientTensor2D(const UMATReal* const props,
						 tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a0 = props[10];
    const double a1 = props[11];
    const double a2 = props[12];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
    A(3)=0.;
  } // end of struct UMATComputeThermalExpansionCoefficientTensor
 
  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				    ISOTROPIC>::exe(const UMATReal* const props,
						    tfel::config::Types<1u,UMATReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a = props[3];
    A(0)=A(1)=A(2)=a;
  } // end of struct UMATComputeThermalExpansionCoefficientTensor

  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::PLANESTRESS,
				    ISOTROPIC>::exe(const UMATReal* const,
						    tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor&)
  {
    using namespace std;
#warning "something needs to be done HERE"
    string msg("UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,"
	       "tfel::material::ModellingHypothesis::PLANESTRESS,ISOTROPIC>::exe : ");
    msg += "unimplemented feature";
    throw(runtime_error(msg));
  } // end of struct UMATComputeThermalExpansionCoefficientTensor

  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::AXISYMMETRICAL,
				    ISOTROPIC>::exe(const UMATReal* const props,
						    tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor& C)
  {
    UMATComputeIsotropicThermalExpansionCoefficientTensor2D(props,C);
  } // end of struct UMATComputeThermalExpansionCoefficientTensor

  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::PLANESTRAIN,
				    ISOTROPIC>::exe(const UMATReal* const props,
						    tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor& C)
  {
    UMATComputeIsotropicThermalExpansionCoefficientTensor2D(props,C);
  } // end of struct UMATComputeThermalExpansionCoefficientTensor

  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
				    ISOTROPIC>::exe(const UMATReal* const props,
						    tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor& C)
  {
    UMATComputeIsotropicThermalExpansionCoefficientTensor2D(props,C);
  } // end of struct UMATComputeThermalExpansionCoefficientTensor

  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
				    ISOTROPIC>::exe(const UMATReal* const props,
						    tfel::config::Types<3u,UMATReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a = props[3];
    A(0)=A(1)=A(2)=a;
    A(3)=A(4)=A(5)=0.;
  } // end of struct UMATComputeThermalExpansionCoefficientTensor

  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				    ORTHOTROPIC>::exe(const UMATReal* const props,
						      tfel::config::Types<1u,UMATReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a0 = props[7];
    const double a1 = props[8];
    const double a2 = props[9];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
  } // end of struct UMATComputeThermalExpansionCoefficientTensor

  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::PLANESTRESS,
				    ORTHOTROPIC>::exe(const UMATReal* const,
						      tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor&)
  {
    using namespace std;
#warning "something needs to be done HERE"
    string msg("UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,"
	       "tfel::material::ModellingHypothesis::PLANESTRESS,"
	       "ORTHOTROPIC>::exe : ");
    msg += "unimplemented feature";
    throw(runtime_error(msg));
  } // end of struct UMATComputeThermalExpansionCoefficientTensor

  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::AXISYMMETRICAL,
				    ORTHOTROPIC>::exe(const UMATReal* const props,
						      tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor& C)
  {
    UMATComputeOrthotropicThermalExpansionCoefficientTensor2D(props,C);
  } // end of struct UMATComputeThermalExpansionCoefficientTensor
  
  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::PLANESTRAIN,
				    ORTHOTROPIC>::exe(const UMATReal* const props,
						      tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor& C)
  {
    UMATComputeOrthotropicThermalExpansionCoefficientTensor2D(props,C);
  } // end of struct UMATComputeThermalExpansionCoefficientTensor
  
  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
				    ORTHOTROPIC>::exe(const UMATReal* const props,
						      tfel::config::Types<2u,UMATReal,false>::ThermalExpansionCoefficientTensor& C)
  {
    UMATComputeOrthotropicThermalExpansionCoefficientTensor2D(props,C);
  } // end of struct UMATComputeThermalExpansionCoefficientTensor
  
  void
  UMATComputeThermalExpansionCoefficientTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
				    ORTHOTROPIC>::exe(const UMATReal* const props,
						      tfel::config::Types<3u,UMATReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a0 = props[16];
    const double a1 = props[17];
    const double a2 = props[18];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
    A(3)=A(4)=A(5)=0.;
  } // end of struct UMATComputeThermalExpansionCoefficientTensor
  
} // end of namespace umat