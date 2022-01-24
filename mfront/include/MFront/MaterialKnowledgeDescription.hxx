/*!
 * \file  mfront/include/MFront/MaterialKnowledgeDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 mars 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_HXX
#define LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_HXX

#include <map>
#include <string>
#include <vector>
#include "MFront/MFrontConfig.hxx"
#include "MFront/MaterialKnowledgeAttribute.hxx"

namespace mfront {

  /*!
   * \brief base class for the description of material knowledge
   */
  struct MFRONT_VISIBILITY_EXPORT MaterialKnowledgeDescription {
    //! \brief attribute name
    static const char* const parametersAsStaticVariables;
    //! \brief attribute name
    static const char* const buildIdentifier;
    /*!
     * \brief throw an exception saying that no attribute with the given name
     * exists
     */
    [[noreturn]] static void throwUndefinedAttribute(const std::string&);
    /*!
     * \brief insert a new attribute
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void setAttribute(const std::string&,
                      const MaterialKnowledgeAttribute&,
                      const bool);
    /*!
     * \brief update an existing new attribute
     * \param[in] n: name
     * \param[in] a: attribute
     */
    void updateAttribute(const std::string&, const MaterialKnowledgeAttribute&);
    /*!
     * \return true if an attribute with the given name as been registred
     * \param[in] n : name
     */
    bool hasAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    std::enable_if_t<isMaterialKnowledgeAttributeType<T>(), T&> getAttribute(
        const std::string&);
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    std::enable_if_t<isMaterialKnowledgeAttributeType<T>(), const T&>
    getAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n: name
     */
    template <typename T>
    std::enable_if_t<isMaterialKnowledgeAttributeType<T>(), T>
    getAttribute(const std::string&, const T&) const;
    /*!
     * \return all the attribute registred
     * \param[in] n : name
     */
    const std::map<std::string, MaterialKnowledgeAttribute>& getAttributes()
        const;

   protected:
    //! \brief behaviour attributes
    std::map<std::string, MaterialKnowledgeAttribute> attributes;
  };  // end of struct MaterialKnowledgeDescription

  /*!
   * \brief this function returns the value of the
   * `MaterialKnowledgeDescription::parametersAsStaticVariables`
   * attribute if it is defined, `false` otherwise.
   * \return if the parameters are treated as static variables.
   * \param[in] h: attribute handler
   */
  MFRONT_VISIBILITY_EXPORT bool areParametersTreatedAsStaticVariables(
      const MaterialKnowledgeDescription&);

}  // end of namespace mfront

#include "MFront/MaterialKnowledgeDescription.ixx"

#endif /* LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_HXX */