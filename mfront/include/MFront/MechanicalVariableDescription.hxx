/*! 
 * \file  MechanicalVariableDescription.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 févr. 2014
 */

#ifndef _LIB_MFRONT_MECHANICALVARIABLEDESCRIPTION_H_
#define _LIB_MFRONT_MECHANICALVARIABLEDESCRIPTION_H_ 

#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"MFront/VariableDescription.hxx"

namespace mfront
{

  /*!
   * A structure describing a mechanical variable. This structure add
   * a modelling hypothesis specification to the the data provided
   * within the VariableDescription structure.
   *
   * \note : if the modelling hypothesis is UNDEFINEDHYPOTHESIS, one
   * shall consider that this variable is valid for all hypothesis.
   */
  struct TFEL_VISIBILITY_EXPORT MechanicalVariableDescription
    : public VariableDescription
  {
    /*!
     * Constructor
     * \param[in] type_      : variable type
     * \param[in] name_      : variable name
     * \param[in] arraySize_ : if greater than 1, this variable will
     * be considered as an array
     * \param[in] lineNumber_ : if greater than 1, will be considered as an array
     * \param[in] h          : modelling hypothesis
     */
    MechanicalVariableDescription(const std::string&,
				  const std::string&,
				  const unsigned short,
				  const unsigned short,
				  const tfel::material::ModellingHypothesis::Hypothesis =
				  tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS);
  protected:
    /*!
     * modelling hypothesis for which this variable is defined
     */
    tfel::material::ModellingHypothesis::Hypothesis hypothesis;
  }; // end of struct MechanicalVariableDescription

} // end of namespace mfront


#endif /* _LIB_MFRONT_MECHANICALVARIABLEDESCRIPTION_H */
