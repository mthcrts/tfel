/*!
 * \file   mfront/mtest/CastemSmallStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<limits>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Castem/Castem.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/Castem/CastemComputeStiffnessTensor.hxx"

#include"MTest/Evolution.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/CastemSmallStrainBehaviour.hxx"
#include"MTest/UmatNormaliseTangentOperator.hxx"

namespace mtest
{

  static tfel::material::ModellingHypothesis::Hypothesis
  getEffectiveModellingHypothesis(const tfel::material::ModellingHypothesis::Hypothesis& h,
				  const std::string& l,
				  const std::string& b)
  {
    using tfel::material::ModellingHypothesis;
    if(h==ModellingHypothesis::PLANESTRESS){
      auto& elm = tfel::system::ExternalLibraryManager::getExternalLibraryManager();
      if(elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
	return ModellingHypothesis::GENERALISEDPLANESTRAIN;
      }
    }
    return h;
  } // end of getEffectiveModellingHypothesis

  CastemSmallStrainBehaviour::CastemSmallStrainBehaviour(const Hypothesis h,
							 const std::string& l,
							 const std::string& b)
    : CastemStandardBehaviour(getEffectiveModellingHypothesis(h,l,b),l,b)
  {
    if(h==ModellingHypothesis::PLANESTRESS){
      auto& elm = tfel::system::ExternalLibraryManager::getExternalLibraryManager();
      if(elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
	this->usesGenericPlaneStressAlgorithm = true;
	this->ivnames.push_back("AxialStrain");
	this->ivtypes.push_back(0);
      }
    }
  }
  
  void
  CastemSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    std::fill(v.begin(),v.end(),real(0));
  } // end of CastemSmallStrainBehaviour::setDrivingVariablesDefaultInitialValue  

  std::pair<bool,real>
  CastemSmallStrainBehaviour::computePredictionOperator(BehaviourWorkSpace& wk,
							const CurrentState& s,
							const StiffnessMatrixType ktype) const
  {
    if(ktype!=StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      wk.cs=s;
      return this->call_behaviour(wk.kt,wk.cs,wk,real(1),ktype,false);
    }
    // compute the stiffness operator from material properties
    this->computeElasticStiffness(wk.kt,s.mprops1,transpose(s.r));
    return {true,1};
  } // end of CastemSmallStrainBehaviour::computePredictionOperator

  std::pair<bool,real>
  CastemSmallStrainBehaviour::integrate(CurrentState& s,
					BehaviourWorkSpace& wk,
					const real dt,
					const StiffnessMatrixType ktype) const
  {
    return this->call_behaviour(wk.k,s,wk,dt,ktype,true);
  } // end of CastemSmallStrainBehaviour::integrate

  std::pair<bool,real>
  CastemSmallStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
					     CurrentState& s,
					     BehaviourWorkSpace& wk,
					     const real dt,
					     const StiffnessMatrixType ktype,
					     const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace castem;
    using tfel::math::vector;
    using castem::CastemComputeStiffnessTensor;
    static const real sqrt2 = sqrt(real(2));
    const auto h = this->usesGenericPlaneStressAlgorithm ?
                   ModellingHypothesis::PLANESTRESS :this->getHypothesis();
    const auto nprops = static_cast<CastemInt>(s.mprops1.size());
    CastemInt ntens,ndi,nstatv;
    unsigned short dimension;
    if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ndi   = 14;
      ntens = 3;
      dimension = 1u;
    } else if (h==ModellingHypothesis::AXISYMMETRICAL){
      ndi = 0;
      ntens = 4;
      dimension = 2u;
    } else if (h==ModellingHypothesis::PLANESTRESS){
      ndi = -2;
      ntens = 4;
      dimension = 2u;
    } else if (h==ModellingHypothesis::PLANESTRAIN){
      ndi = -1;
      ntens = 4;
      dimension = 2u;
    } else if (h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      ndi = -3;
      ntens = 4;
      dimension = 2u;
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      ndi = 2;
      ntens = 6;
      dimension = 3u;
    } else {
      throw(runtime_error("CastemSmallStrainBehaviour::integrate: "
			  "unsupported hypothesis"));
    }
    if((wk.D.getNbRows()!=Kt.getNbRows())||
       (wk.D.getNbCols()!=Kt.getNbCols())){
      throw(runtime_error("CastemSmallStrainBehaviour::integrate: "
			  "the memory has not been allocated correctly"));
    }
    if(((s.iv0.size()==0)&&(wk.ivs.size()!=1u))||
       ((s.iv0.size()!=0)&&(s.iv0.size()!=wk.ivs.size()))){
      throw(runtime_error("CastemSmallStrainBehaviour::integrate: "
			  "the memory has not been allocated correctly (2)"));
    }
    fill(wk.D.begin(),wk.D.end(),0.);
    // choosing the type of stiffness matrix
    UmatBehaviourBase::initializeTangentOperator(wk.D,ktype,b);
    // state variable initial values
    if(s.iv0.size()!=0){
      copy(s.iv0.begin(),s.iv0.end(),wk.ivs.begin());
    }
    nstatv = static_cast<CastemInt>(wk.ivs.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot = transpose(s.r);
    tmatrix<3u,3u,real>::size_type i;
    CastemInt kinc(1);
    stensor<3u,real> ue0(real(0));
    stensor<3u,real> ude(real(0));
    copy(s.e0.begin(),s.e0.end(),ue0.begin());
    for(i=0;i!=s.e1.size();++i){
      ude(i) = s.e1(i)-s.e0(i);
    }
    copy(s.s0.begin(),s.s0.end(),s.s1.begin());
    // thermal strain
    for(i=0;i!=s.e1.size();++i){
      ue0(i) -= s.e_th0(i);
      ude(i) -= s.e_th1(i)-s.e_th0(i);
    }
    // castem conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s.s1(i) /= sqrt2;
      ue0(i)  *= sqrt2;
      ude(i)  *= sqrt2;
    }
    auto ndt = std::numeric_limits<CastemReal>::max();
    (this->fct)(&(s.s1(0)),&wk.ivs(0),&(wk.D(0,0)),
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,
		&ue0(0),&ude(0),nullptr,&dt,
		&(s.esv0(0))  ,&(s.desv(0)),
		&(s.esv0(0))+1,&(s.desv(0))+1,
		nullptr,&ndi,nullptr,&ntens,&nstatv,
		&(s.mprops1(0)),
		&nprops,nullptr,&drot(0,0),&ndt,
		nullptr,nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,&kinc,0);
    if(kinc!=1){
      return {false,ndt};
    }
    // tangent operator (...)
    if(ktype!=StiffnessMatrixType::NOSTIFFNESS){ 
      if(ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
	this->computeElasticStiffness(Kt,s.mprops1,drot);
      } else {
	UmatNormaliseTangentOperator::exe(&Kt(0,0),wk.D,dimension);
      }
    }
    if(!s.iv1.empty()){
      std::copy_n(wk.ivs.begin(),s.iv1.size(),s.iv1.begin());
    }
    // turning things in standard conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s.s1(i) *= sqrt2;
    }
    return {true,ndt};
  } // end of CastemSmallStrainBehaviour::integrate

  void
  CastemSmallStrainBehaviour::computeElasticStiffness(tfel::math::matrix<real>& Kt,
						      const tfel::math::vector<real>& mp,
						      const tfel::math::tmatrix<3u,3u,real>& drot) const
  {
    using namespace std;
    using namespace tfel::math;
    using castem::CastemComputeStiffnessTensor;
    const auto h = this->getHypothesis();
    if(this->stype==0u){
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<1u,real>(&Kt(0,0)) = De;
      } else if (h==ModellingHypothesis::AXISYMMETRICAL){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::AXISYMMETRICAL,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<2u,real>(&Kt(0,0)) = De;
      } else if (h==ModellingHypothesis::PLANESTRESS){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::PLANESTRESS,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<2u,real>(&Kt(0,0)) = De;
      } else if (h==ModellingHypothesis::PLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::PLANESTRAIN,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<2u,real>(&Kt(0,0)) = De;
      } else if (h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::GENERALISEDPLANESTRAIN,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<2u,real>(&Kt(0,0)) = De;
      } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	st2tost2<3u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::TRIDIMENSIONAL,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<3u,real>(&Kt(0,0)) = De;
      } else {
	throw(runtime_error("CastemSmallStrainBehaviour::integrate: "
			    "unsupported hypothesis"));
      }
    } else if(this->stype==1u){
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<1u,real>(&Kt(0,0)) = De;
      } else if (h==ModellingHypothesis::AXISYMMETRICAL){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				     ModellingHypothesis::AXISYMMETRICAL,
				     castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<2u,real>(&Kt(0,0)) = change_basis(De,drot);
      } else if (h==ModellingHypothesis::PLANESTRESS){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::PLANESTRESS,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<2u,real>(&Kt(0,0)) = change_basis(De,drot);
      } else if (h==ModellingHypothesis::PLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::PLANESTRAIN,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<2u,real>(&Kt(0,0)) = change_basis(De,drot);
      } else if (h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::GENERALISEDPLANESTRAIN,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<2u,real>(&Kt(0,0)) = change_basis(De,drot);
      } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	st2tost2<3u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::TRIDIMENSIONAL,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	ST2toST2View<3u,real>(&Kt(0,0)) = change_basis(De,drot);
      } else {
	throw(std::runtime_error("CastemSmallStrainBehaviour::integrate : "
				 "unsupported hypothesis"));
      }
    } else {
      throw(std::runtime_error("CastemSmallStrainBehaviour::integrate : "
			       "invalid behaviour type (neither "
			       "isotropic or orthotropic)"));
    }
  }

  void
  CastemSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(EvolutionManager& mp,
									    const EvolutionManager& evm) const
  {
    const auto h = this->getHypothesis();
    CastemStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(mp,evm);
    if(this->stype==0){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion",0.);
    } else if(this->stype==1){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion1",0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion2",0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion3",0.);
    } else {
      throw(std::runtime_error("CastemSmallStrainBehaviour::"
			       "setOptionalMaterialPropertiesDefaultValues: "
			       "unsupported symmetry type"));
    }
    if(h==ModellingHypothesis::PLANESTRESS){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"PlateWidth",0.);
    }
  } // end of CastemSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues
      
  CastemSmallStrainBehaviour::~CastemSmallStrainBehaviour()
  {}
  
} // end of namespace mtest

