/*!
 * \file   MFrontImplicitParser.hxx
 * \brief  This file declares the MFrontImplicitParser class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTIMPLICITPARSER_H_
#define _LIB_MFRONTIMPLICITPARSER_H_ 

#include<string>
#include"MFront/MFrontBehaviourParserBase.hxx"
#include"MFront/MFrontVirtualParser.hxx"

namespace mfront{

  struct MFrontImplicitParser
    : public MFrontVirtualParser,
      public MFrontBehaviourParserBase<MFrontImplicitParser>
  {
    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    MFrontImplicitParser();

    void setVerboseMode();

    void setDebugMode();

    void setWarningMode();

    void treatFile(const std::string&);

    void writeOutputFiles();

    void
    setInterfaces(const std::set<std::string>&);

    std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(void);

    std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(void);

    std::map<std::string,std::vector<std::string> >
    getGeneratedSources(void);

    std::vector<std::string>
    getGeneratedIncludes(void);

    std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(void);

    std::map<std::string,
	     std::pair<std::vector<std::string>,
		       std::vector<std::string> > >
    getSpecificTargets(void);

    ~MFrontImplicitParser();

  private:

    enum Algorithm{
      NEWTONRAPHSON,
      NEWTONRAPHSON_NR,
      BROYDEN,
      BROYDEN2,
      DEFAULT
    };

    bool
    isJacobianPart(const std::string&);

    virtual void
    predictorAnalyser(const std::string&);
    
    virtual void
    integratorAnalyser(const std::string&);

    virtual std::string
    tangentOperatorVariableModifier(const std::string&,const bool);

    virtual std::string
    integratorVariableModifier(const std::string&,const bool);

    virtual std::string
    computeStressVariableModifier1(const std::string&,const bool);

    virtual std::string
    computeStressVariableModifier2(const std::string&,const bool);

    virtual void
    treatUnknownVariableMethod(const std::string&);

    virtual void
    treatIntegrator(void);

    virtual void endsInputFileProcessing(void);

    virtual void writeBehaviourIntegrator(void);

    virtual void writeBehaviourParserSpecificIncludes(void);

    virtual void writeBehaviourParserSpecificTypedefs(void);

    virtual void writeBehaviourParserSpecificMembers(void);

    virtual void writeBehaviourStateVarsIncrements(void);
    
    virtual void writeBehaviourConstructors(void);

    virtual void writeBehaviourParserSpecificConstructorPart(void);

    virtual void writeBehaviourStaticVars(void);

    virtual void writeComputeNumericalJacobian(void);

    virtual void writeBehaviourComputeTangentOperator(void);

    virtual void writeGetPartialJacobianInvert(void);

    virtual void treatTheta(void);

    virtual void treatIterMax(void);

    virtual void treatEpsilon(void);

    virtual void treatAlgorithm(void);

    virtual void treatPredictor(void);

    virtual void treatComputeStress(void);

    virtual void treatStateVariables(void);

    virtual void treatAuxiliaryStateVariables(void);

    virtual void treatCompareToNumericalJacobian(void);

    virtual void treatJacobianComparisonCriterium(void);

    virtual void generateOutputFiles(void);

    virtual void treatUseAcceleration(void);

    virtual void treatAccelerationTrigger(void);

    virtual void treatAccelerationPeriod(void);

    virtual void treatUseRelaxation(void);

    virtual void treatRelaxationTrigger(void);
    
    virtual void treatRelaxationCoefficient(void);

    virtual void treatInitJacobian(void);

    virtual void treatTangentOperator(void);

    virtual void treatIsTangentOperatorSymmetric(void);

    virtual void
    treatUnknownKeyword(void);

    std::string
    getJacobianPart(const VarHandler&,
		    const VarHandler&,
		    const SupportedTypes::TypeSize&,
		    const SupportedTypes::TypeSize&,
		    const SupportedTypes::TypeSize&,
		    const std::string& = "this->jacobian",
		    const std::string& = "");

    friend class MFrontBehaviourParserBase<MFrontImplicitParser>;

    std::set<std::string> jacobianPartsUsedInIntegrator;

    std::set<std::string> internalStateVariableIncrementsUsedInPredictor;

    // normalisation factors
    std::map<std::string,std::string> nf;

    std::string computeStress;

    std::string computeFinalStress;

    std::string predictor;

    std::string initJacobian;

    std::string tangentOperator;

    Algorithm algorithm;

    bool compareToNumericalJacobian;

    bool isConsistantTangentOperatorSymmetricDefined;

    bool useRelaxation;

    bool useAcceleration;

  }; // end of struct MFrontImplicitParser

} // end of namespace mfront  

#endif /* _LIB_MFRONTIMPLICITPARSER_H */

