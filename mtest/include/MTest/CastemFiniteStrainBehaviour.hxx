/*! 
 * \file   mtest/include/MTest/CastemFiniteStrainBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  18 november 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR_HXX
#define LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR_HXX 

#include"MTest/CastemStandardBehaviour.hxx"

namespace mtest
{

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL CastemFiniteStrainBehaviour
    : public CastemStandardBehaviour
  {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    CastemFiniteStrainBehaviour(const Hypothesis,
				const std::string&,
				const std::string&);
    /*!
     * \param[in] umb: behaviour description
     */
    CastemFiniteStrainBehaviour(const UmatBehaviourDescription&);
    /*!
     * \param[out] v : initial values of the driving variables
     */
    void  getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>&) const override;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out] wk    : workspace
     * \param[in]  s     : current state
     * \param[in]  dt    : time increment
     * \param[in]  ktype : type of the stiffness matrix
     */
    std::pair<bool,real>
    computePredictionOperator(BehaviourWorkSpace&,
			      const CurrentState&,
			      const StiffnessMatrixType) const override;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out/in] s     : current state
     * \param[out]    wk    : workspace
     * \param[in]     dt    : time increment
     * \param[in]     ktype : type of the stiffness matrix
     */
    std::pair<bool,real>
    integrate(CurrentState&,
	      BehaviourWorkSpace&,
	      const real,
	      const StiffnessMatrixType) const override;
    /*!
     * \brief some interfaces requires dummy material properties to be
     * declared. For example, the Cast3M finite element solver
     * requires the mass density and some extra material properties
     * describing orthotropic axes to be declared.  This method is
     * meant to automatically declare those if they are not defined by
     * the user.
     * \param[out] mp  : evolution manager where 
     * \param[in]  evm : evolution manager
     */
    void setOptionalMaterialPropertiesDefaultValues(EvolutionManager&,
						    const EvolutionManager&) const override;
    /*!
     * \return the string passed to the UMAT function through the
     * CMNAME parameter.
     */
    const char* getBehaviourNameForUMATFunctionCall() const override;
    //! destructor
    ~CastemFiniteStrainBehaviour() override;
  protected:
    /*!
     * \brief compute the elastic stiffness
     * \param[out] Kt   : tangent operator
     * \param[in]  mp   : material properties
     * \param[in]  drot : rotation matrix (Fortran convention)
     */
    virtual void
    computeElasticStiffness(tfel::math::matrix<real>&,
			    const tfel::math::vector<real>&,
			    const tfel::math::tmatrix<3u,3u,real>&) const;
    /*!
     * The umat interface can handle plane stress by calling the
     * generalised plane strain version of the behaviour.  In this
     * case, the hypothesis used by the behaviour is different than
     * the hypothesis used to perform the computation. This flag
     * distinguishes this case.
     */
    bool usesGenericPlaneStressAlgorithm = false;
  }; // end of struct Behaviour
  
} // end of namespace mtest

#endif /* LIB_MTEST_CASTEMFINITESTRAINUMATBEHAVIOUR_HXX */

