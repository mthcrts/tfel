/*!
 * \file   mfront/include/MFront/IsotropicMisesPlasticFlowDSL.hxx
 * \brief  This file declares the IsotropicMisesPlasticFlowDSL class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTISOTROPICPLASTICFLOWPARSER_HXX
#define LIB_MFRONTISOTROPICPLASTICFLOWPARSER_HXX 

#include<string>
#include"MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront{

  /*!
   * \brief a dsl dedicated to isotropic mises plastic flows
   */
  struct IsotropicMisesPlasticFlowDSL
    : public IsotropicBehaviourDSLBase
  {
    //! \return the name of the dsl
    static std::string getName();
    //! \return a description of the dsl
    static std::string getDescription();
    //! constructor
    IsotropicMisesPlasticFlowDSL();

    void endsInputFileProcessing() override;
    //! desctructor
    ~IsotropicMisesPlasticFlowDSL() override;

  protected:

    double getDefaultThetaValue() const override;
    
    void writeBehaviourParserSpecificInitializeMethodPart(std::ostream&,
							  const Hypothesis) const override;

    void writeBehaviourParserSpecificMembers(std::ostream&,
					     const Hypothesis) const override;

    void writeBehaviourIntegrator(std::ostream&,
				  const Hypothesis) const override;

    void writeBehaviourComputeTangentOperator(std::ostream&,
					      const Hypothesis) const override;
  };

} // end of namespace mfront  

#endif /* LIB_MFRONTISOTROPICPLASTICFLOWPARSER_HXX */

