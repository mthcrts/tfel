/*!
 * \file   OctaveMaterialPropertyInterface.hxx
 * \brief  This file declares the OctaveMaterialPropertyInterface class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTOCTAVELAWINTERFACE_H_
#define _LIB_MFRONTOCTAVELAWINTERFACE_H_ 

#include<string>
#include<fstream>
#include<vector>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/LawFunction.hxx"
#include"MFront/InterfaceBase.hxx"
#include"MFront/VariableDescription.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/VariableBoundsDescription.hxx"
#include"MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront{

  struct OctaveMaterialPropertyInterface
    : public AbstractMaterialPropertyInterface
  {
    static std::string 
    getName(void);
    
    OctaveMaterialPropertyInterface();
    /*!
     * \param[in] k  : keyword treated
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) override;
    /*!
     * \brief generate the output files
     * \param[in] mpd : material property description
     * \param[in] fd  : mfront file description
     */
    virtual void writeOutputFiles(const MaterialPropertyDescription&,
				  const FileDescription&) override;
    /*!
     * \brief : fill the target descripton
     * \param[out] d   : target description
     * \param[in]  mpd : material property description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const MaterialPropertyDescription&) override;

    ~OctaveMaterialPropertyInterface();
    
  private:

    static std::string 
    toString(const unsigned short);

    static void
    replace(std::string&,
	    const std::string::value_type,
	    const std::string::value_type);

    std::string
    treatDescriptionString(const std::string&) const;

    void writeSrcFile(const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const VariableDescriptionContainer&,
		      const std::vector<std::string>&,
		      const StaticVariableDescriptionContainer&,
		      const std::vector<std::string>&,
		      const std::map<std::string,double>&,
		      const LawFunction&,
		      const std::vector<VariableBoundsDescription>&,
		      const std::vector<VariableBoundsDescription>&);

    std::ofstream headerFile;

    std::ofstream srcFile;

    std::string headerFileName;

    std::string srcFileName;

  }; // end of MfrontOctaveLawInterface

} // end of namespace mfront

#endif /* _LIB_MFRONTOCTAVELAWINTERFACE_H */