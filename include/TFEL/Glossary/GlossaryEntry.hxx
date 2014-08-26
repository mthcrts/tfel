/*! 
 * \file  GlossaryEntry.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 mars 2014
 */

#ifndef _LIB_TFEL_GLOSSARY_GLOSSARYENTRY_H_
#define _LIB_TFEL_GLOSSARY_GLOSSARYENTRY_H_ 

#include<set>
#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace glossary
  {
  
    /*!
     * structure describing an entry of the glossary
     */
    struct TFELGLOSSARY_VISIBILITY_EXPORT GlossaryEntry
    {
      /*!
       * constructor
       * \param[in] k  : key used to store the entry
       * \param[in] n  : name
       * \param[in] u  : unit
       * \param[in] t  : type
       * \param[in] sd : short description
       * \param[in] d  : description
       * \param[in] no : note
       */
      GlossaryEntry(const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::vector<std::string>&,
		    const std::vector<std::string>&);
      /*!
       * constructor
       * \param[in] k  : key used to store
       * \param[in] n  : name
       * \param[in] u  : unit
       * \param[in] t  : type
       * \param[in] sd : short description
       * \param[in] d  : description
       * \param[in] no : note
       */
      GlossaryEntry(const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&);
      /*!
       * constructor
       * \param[in] k  : key used to store the entry
       * \param[in] n  : names
       * \param[in] u  : unit
       * \param[in] t  : type
       * \param[in] sd : short description
       * \param[in] d  : description
       * \param[in] no : note
       */
      GlossaryEntry(const std::string&,
		    const std::vector<std::string>&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::vector<std::string>&,
		    const std::vector<std::string>&);
      /*!
       * constructor
       * \param[in] k  : key used to store the entry
       * \param[in] n  : names
       * \param[in] u  : unit
       * \param[in] t  : type
       * \param[in] sd : short description
       * \param[in] d  : description
       * \param[in] no : note
       */
      GlossaryEntry(const std::string&,
		    const std::vector<std::string>&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&);
      /*!
       * constructor
       * \param[in] k  : key used to store the entry 
       * \param[in] b  : iterator to the first name
       * \param[in] e  : iterator to the last name
       * \param[in] u  : unit
       * \param[in] t  : type
       * \param[in] sd : short description
       * \param[in] d  : description
       * \param[in] no : note
       */
      GlossaryEntry(const std::string&,
		    const char* const * const,
		    const char* const * const,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::vector<std::string>&,
		    const std::vector<std::string>&);
      /*!
       * constructor
       * \param[in] k  : key used to store the entry
       * \param[in] b  : iterator to the first name
       * \param[in] e  : iterator to the last name
       * \param[in] u  : unit
       * \param[in] t  : type
       * \param[in] sd : short description
       * \param[in] d  : description
       * \param[in] no : note
       */
      GlossaryEntry(const std::string&,
		    const char* const * const,
		    const char* const * const,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&,
		    const std::string&);
      /*!
       * \return the key
       */
      const std::string&
	getKey(void) const;
      /*!
       * \return names asssociated with this key
       */
      const std::vector<std::string>&
	getNames(void) const;
      /*!
       * \return the unit
       */
      const std::string&
	getUnit(void) const;
      /*!
       * \return the type
       */
      const std::string&
	getType(void) const;
      /*!
       * \return the short description
       */
      const std::string&
	getShortDescription(void) const;
      /*!
       * \return the description
       */
      const std::vector<std::string>&
	getDescription(void) const;
      /*!
       * \return the notes
       */
      const std::vector<std::string>&
	getNotes(void) const;
      /*!
       * cast operator to string. Equivalent to getKey()
       */
      operator const std::string& () const;
      /*!
       * comparison operator
       */
      TFELGLOSSARY_VISIBILITY_EXPORT
	friend bool
	operator<(const GlossaryEntry&,
		  const GlossaryEntry&);
      /*!
       * comparison operator
       */
      TFELGLOSSARY_VISIBILITY_EXPORT
	friend bool
	operator != (const std::string&,
		     const GlossaryEntry&);
      /*!
       * comparison operator
       */
      TFELGLOSSARY_VISIBILITY_EXPORT
	friend bool
	operator != (const GlossaryEntry&,
		     const std::string&);
      /*!
       * comparison operator
       */
      TFELGLOSSARY_VISIBILITY_EXPORT
	friend bool
	operator == (const std::string&,
		     const GlossaryEntry&);
      /*!
       * comparison operator
       */
      TFELGLOSSARY_VISIBILITY_EXPORT
	friend bool
	operator == (const GlossaryEntry&,
		     const std::string&);
    private:
      /*!
       * \brief check if this entry is valid.
       * Called by constructors
       */
      void check(void) const;
      /*!
       * key used to store this entry in the glossary
       */
      const std::string key;
      /*!
       * names used in input file
       */
      const std::vector<std::string> names;
      /*!
       * unit of the entry
       */
      const std::string unit;
      /*!
       * type of the entry
       */
      const std::string type;
      /*!
       * short description of the entry
       */
      const std::string short_description;
      /*!
       * description
       */
      const std::vector<std::string> description;
      /*!
       * note
       */
      const std::vector<std::string> notes;
    }; // end of struct GlossaryEntry

  } // end of namespace glossary

} // end of namespace tfel

#endif /* _LIB_TFEL_GLOSSARY_GLOSSARYENTRY_H */