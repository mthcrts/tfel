/*!
 * \file   include/MFront/BehaviourBrick/KinematicHardeningRule.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULE_HXX

#include <string>
#include <memory>
#include <vector>
#include "MFront/BehaviourDescription.hxx"

namespace tfel {
  namespace utilities {
    // forward declaration
    struct Data;
  }  // end of namespace utilities
}  // end of namespace tfel

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;

  namespace bbrick {

    // forward declaration
    struct OptionDescription;
    // forward declaration
    struct StressPotential;

    /*!
     * \brief class describing an kinematic hardening rule
     */
    struct KinematicHardeningRule {
      //! a simple alias
      using Data = tfel::utilities::Data;
      //! a simple alias
      using DataMap = std::map<std::string, Data>;
      //! a simple alias
      using ModellingHypothesis = tfel::material::ModellingHypothesis;
      //! a simple alias
      using Hypothesis = ModellingHypothesis::Hypothesis;
      //! a simple alias
      using MaterialProperty = BehaviourDescription::MaterialProperty;
      //! a simple alias
      using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
      /*!
       * \return the name of a variable from a base name, the flow id and the
       * kinematic hardening rule id.
       * \param[in] n: base name
       * \param[in] fid: flow id
       * \param[in] kid: kinematic hardening rule id
       */
      static std::string getVariableId(const std::string&,
                                       const std::string&,
                                       const std::string&);
      /*!
       * \param[in,out] bd: behaviour description
       * \param[in,out] dsl: abstract behaviour dsl
       * \param[in] fid: flow id
       * \param[in] kid: kinematic hardening rule id
       * \param[in] d: options
       */
      virtual void initialize(BehaviourDescription&,
                              AbstractBehaviourDSL&,
                              const std::string&,
                              const std::string&,
                              const DataMap&) = 0;
      /*!
       * \brief method called at the end of the input file processing
       * \param[in] dsl: abstract behaviour dsl
       * \param[in] bd: behaviour description
       * \param[in] fid: flow id
       * \param[in] kid: kinematic hardening rule id
       */
      virtual void endTreatment(BehaviourDescription&,
                                const AbstractBehaviourDSL&,
                                const std::string&,
                                const std::string&) const = 0;
      //! \return the flow options
      virtual std::vector<OptionDescription> getOptions() const = 0;
      /*!
       * \return the list of kinematic hardening variables.
       * \param[in] fid: flow id
       * \param[in] kid: kinematic hardening rule id
       */
      virtual std::vector<std::string> getKinematicHardeningsVariables(
          const std::string&, const std::string&) const = 0;
      /*!
       * \brief compute the initial values of the kinematic hardening variables.
       * Thoses initial values are used to compute an elastic prediction of the
       * criterion.
       * \param[in] fid: flow id
       * \param[in] kid: kinematic hardening rule id
       */
      virtual std::string computeKinematicHardeningsInitialValues(
          const std::string&, const std::string&) const = 0;
      /*!
       * \brief compute the values of the kinematic hardening variables
       * at \f$t+\theta\,dt\f$.
       * \param[in] fid: flow id
       * \param[in] kid: kinematic hardening rule id
       */
      virtual std::string computeKinematicHardenings(
          const std::string&, const std::string&) const = 0;
      /*!
       * \brief compute the derivatives of a variable \f$v\f$ knowing the
       * derivative of \f$\frac{d\,v}{d\underline{s}}\f$.
       * \param[in] v: variable name
       * \param[in] dv_ds: derivative of v with respect to the effective stress
       * \param[in] fid: flow id
       * \param[in] kid: kinematic hardening rule id
       */
      virtual std::string computeDerivatives(
          const std::string&,
          const std::string&,
          const std::string&,
          const std::string&) const = 0;
      /*!
       * \return the code computing the implicit equations associated with the
       * back-strains and the derivatives of those equations (if requested).
       * \param[in] bd: behaviour description
       * \param[in] sp: stress potential
       * \param[in] fid: flow id
       * \param[in] kid: kinematic hardening rule id
       * \param[in] b: compute derivatives
       */
      virtual std::string buildBackStrainImplicitEquations(
          const BehaviourDescription&,
          const StressPotential&,
          const std::string&,
          const std::string&,
          const bool) const = 0;
      //! destructor
      virtual ~KinematicHardeningRule();
    };  // end of struct KinematicHardeningRule

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULE_HXX */
