/*!
 * \file   mfront/src/KinematicHardeningRuleFactory.cxx
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

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/PragerKinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRuleFactory.hxx"

namespace mfront {

  namespace bbrick {

    KinematicHardeningRuleFactory& KinematicHardeningRuleFactory::getFactory() {
      static KinematicHardeningRuleFactory i;
      return i;
    }  // end of KinematicHardeningRuleFactory::getFactory

    void KinematicHardeningRuleFactory::addGenerator(const std::string& n,
                                                 const Generator& g) {
      if (!this->generators.insert({n, g}).second) {
        tfel::raise(
            "KinematicHardeningRuleFactory::addGenerator: "
            "generator '" +
            n + "' already registred");
      }
    }  // end of KinematicHardeningRuleFactory::addGenerator

    std::shared_ptr<KinematicHardeningRule> KinematicHardeningRuleFactory::generate(
        const std::string& n) const {
      const auto p = this->generators.find(n);
      if (p == this->generators.end()) {
        tfel::raise(
            "KinematicHardeningRuleFactory::generate: "
            "no generator named '" +
            n + "'");
      }
      return p->second();
    }  // end of KinematicHardeningRuleFactory::generate

    KinematicHardeningRuleFactory::KinematicHardeningRuleFactory() {
      this->addGenerator("Prager", []() {
        return std::make_shared<bbrick::PragerKinematicHardeningRule>();
      });
    }  // end of KinematicHardeningRuleFactory::KinematicHardeningRuleFactory

    KinematicHardeningRuleFactory::~KinematicHardeningRuleFactory() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
