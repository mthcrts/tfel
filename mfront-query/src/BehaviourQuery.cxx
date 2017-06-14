/*! 
 * \file  BehaviourQuery.cxx
 * \brief
 * \author Helfer Thomas
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#include<cstdlib>
#endif
#include<algorithm>
#include<iostream>
#include<sstream>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/AbstractBehaviourDSL.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/BehaviourDescription.hxx"
#include"MFront/BehaviourQuery.hxx"

namespace mfront{

  static const BehaviourAttribute&
  getAttribute(const std::string& n,
	       const BehaviourDescription& d,
	       const tfel::material::ModellingHypothesis::Hypothesis h){
    const auto& as1 = d.getAttributes();
    const auto& as2 = d.getBehaviourData(h).getAttributes();
    auto p = as2.find(n);
    if(p!=as2.end()){
      return p->second;
    }
    p = as1.find(n);
    if(p!=as2.end()){
      return p->second;
    }
    return p->second;
  }

  static std::string to_string(const BehaviourDescription::SlipSystem& gs){
    using SlipSystemsDescription =
      BehaviourDescription::SlipSystemsDescription;
    using std::to_string;
    auto r = std::string{};
    if(gs.is<SlipSystemsDescription::system3d>()){
      const auto& p = gs.get<SlipSystemsDescription::system3d>().plane;
      const auto& b = gs.get<SlipSystemsDescription::system3d>().burgers;
      r += '('+to_string(p[0])+','+to_string(p[1])+','+to_string(p[2])+')';
      r += '['+to_string(b[0])+','+to_string(b[1])+','+to_string(b[2])+']';
    } else {
      if(!gs.is<SlipSystemsDescription::system4d>()){
	throw(std::runtime_error("to_string: internal error "
				 "(unsupported slip system type)"));
      }
      const auto& p = gs.get<SlipSystemsDescription::system3d>().plane;
      const auto& b = gs.get<SlipSystemsDescription::system3d>().burgers;
      r += '('+to_string(p[0])+','+to_string(p[1])+','+
	to_string(p[2])+','+to_string(p[3])+')';
      r += '['+to_string(b[0])+','+to_string(b[1])+','+
	to_string(b[2])+','+to_string(b[3])+']';
    }
    return r;
  } // end of to_string
  
  BehaviourQuery::BehaviourQuery(const int argc,
				 const char *const *const argv,
				 std::shared_ptr<AbstractBehaviourDSL> d,
				 const std::string& f)
    : tfel::utilities::ArgumentParserBase<BehaviourQuery>(argc,argv),
      dsl(d),
      file(f)
  {
    this->registerCommandLineCallBacks();
    this->parseArguments();
    // registring interfaces
    if(!this->interfaces.empty()){
      dsl->setInterfaces(this->interfaces);
    }
  } // end of BehaviourQuery::BehaviourQuery

  void BehaviourQuery::registerCommandLineCallBacks(){
    using namespace std;
    using Parser = tfel::utilities::ArgumentParserBase<BehaviourQuery>;
    Parser::registerNewCallBack("--verbose",&BehaviourQuery::treatVerbose,
				"set verbose output",true);
    Parser::registerNewCallBack("--include","-I",&BehaviourQuery::treatSearchPath,
				"add a new path at the beginning of the search paths",true);
    Parser::registerNewCallBack("--search-path",&BehaviourQuery::treatSearchPath,
				"add a new path at the beginning of the search paths",true);
    Parser::registerNewCallBack("--debug",&BehaviourQuery::treatDebug,
				"set debug mode");
    Parser::registerNewCallBack("--warning","-W",&BehaviourQuery::treatWarning,"print warnings");
    Parser::registerNewCallBack("--pedantic",&BehaviourQuery::treatPedantic,
				"print pedantic warning message");
    Parser::registerNewCallBack("--interface",&BehaviourQuery::treatInterface,
				"define an interface",true);
    Parser::registerNewCallBack("--modelling-hypothesis",&BehaviourQuery::treatModellingHypothesis,
				"select a modelling hypothesis",true);
    // standard queries
    const vector<pair<const char*,const char*>> sq = {
      {"--author","show the author name"},
      {"--description","show the file description"},
      {"--date","show the file implementation date"},
      {"--material","show the material name"},
      {"--library","show the library name"},
      {"--type","return the behaviour type\n"
       "The value returned has the following meaning:\n"
       "- 0: general behaviour\n"
       "- 1: small strain behaviour\n"
       "- 2: finite strain behaviour\n"
       "- 3: cohesive zone model\n"},
      // {"--kinematic","return the behaviour kinematic\n"
      //  "The value returned has the following meaning:\n"
      //  "- 0: undefined kinematic\n"
      //  "- 1: standard small strain behaviour kinematic\n"
      //  "- 2: cohesive zone model kinematic\n"
      //  "- 3: standard finite strain kinematic (F-Cauchy)\n"
      //  "- 4: ptest finite strain kinematic (eto-pk1)\n"},
      {"--symmetry","return the behaviour symmetry. If the returned value is 0, "
       "the behaviour is isotropic. If the returned value is 1, "
       "the behaviour is orthotropic."},
      {"--elastic-symmetry","return the symmetry of the elastic part of the behaviour. "
       "If the returned value is 0, this part of the behaviour is isotropic. "
       "If the returned value is 1, this part of the behaviour is orthotropic."
       "the behaviour is orthotropic."},
      {"--has-crystal-structure","return `true` if a crystal structure has been defined"},
      {"--crystal-structure","return the crystal structure"},
      {"--slip-systems","list all the slip systems, sorted by family"},
      {"--slip-systems-by-index","list all the slip systems"},
      {"--interaction-matrix","show the interaction matrix"},
      {"--interaction-matrix-structure","show the structure of the interaction matrix"},
      {"--supported-modelling-hypotheses","show the list of supported modelling hypothesis"},
      {"--material-properties","show the list of material properties for the selected modelling hypothesis"},
      {"--state-variables","show the list of state variables for the selected modelling hypothesis"},
      {"--auxiliary-state-variables","show the auxiliary state variables properties for the selected modelling hypothesis"},
      {"--external-state-variables","show the list of external state variables for the selected modelling hypothesis"},
      {"--integration-variables","show the list of integration variables for the selected modelling hypothesis"},
      {"--persistent-variables","show the list of persistent variables for the selected modelling hypothesis"},
      {"--local-variables","show the list of local variables for the selected modelling hypothesis"},
      {"--parameters","show the list of parameters for the selected modelling hypothesis"},
      {"--static-variables","show the list of static variables for the selected modelling hypothesis"},
      {"--attributes","show the list of attributes of the behaviour description"},
      {"--code-blocks","show the list of code-blocks of the behaviour description for "
	  " the selected modelling hypothesis"}};
    for(const auto& q : sq){
      Parser::registerNewCallBack(q.first,&BehaviourQuery::treatStandardQuery,q.second);
    }
    const vector<pair<const char*,const char*>> sq2 = {
      {"--attribute-type","display an attribute type"},
      {"--attribute-value","display an attribute value"},
      {"--parameter-type","display a parameter type"},
      {"--parameter-default-value","display a parameter default value"},
      {"--static-variable-value","display the value of a static variable"},
      {"--has-bounds","return `true` if a variable has bounds, `false` otherwise"},
      {"--bounds-type","return the bounds type associated to a variable.\n"
       "The returned value has the follwing meaning:\n"
       "- `None`\n"
       "- `Lower`\n"
       "- `Upper`\n"
       "- `LowerAndUpper`"},
      {"--bounds-value","show the bounds value associated as a range"},
      {"--has-physical-bounds","return `true` if a variable has physical bounds, `false` otherwise"},
      {"--physical-bounds-type","return the physical bounds type associated to a variable.\n"
       "The returned value has the follwing meaning:\n"
       "- `None`\n"
       "- `Lower`\n"
       "- `Upper`\n"
       "- `LowerAndUpper`"},
      {"--physical-bounds-value","show the bounds value associated as a range"}};
    for(const auto& q : sq2){
      Parser::registerNewCallBack(q.first,&BehaviourQuery::treatStandardQuery2,q.second,true);
    }
    Parser::registerNewCallBack("--generated-sources",&BehaviourQuery::treatGeneratedSources,
				"show all the generated sources");
    Parser::registerNewCallBack("--generated-headers",&BehaviourQuery::treatGeneratedHeaders,
				"show all the generated headers");
    Parser::registerNewCallBack("--cppflags",&BehaviourQuery::treatCppFlags,
				"show all the global headers");
    Parser::registerNewCallBack("--libraries-dependencies",
				&BehaviourQuery::treatLibrariesDependencies,
				"show all the libraries dependencies");
    Parser::registerNewCallBack("--specific-targets",&BehaviourQuery::treatSpecificTargets,
				"show all the specific targets");
  } // end of BehaviourQuery::registerCommandLineCallBacks

  void BehaviourQuery::treatStandardQuery(){
    using namespace tfel::utilities;
    using tfel::material::ModellingHypothesis;
    using std::cout;
    const auto& q = this->getCurrentCommandLineArgument();
    const auto& qn = q.as_string();
    if(qn=="--author"){
      this->queries.push_back({"author",[](const FileDescription& fd,
					   const BehaviourDescription&,
					   const Hypothesis){
	    const auto& a = fd.authorName;
	    cout << (!a.empty() ? a : "(undefined)") << '\n';
	  }});
    } else if(qn=="--description"){
      this->queries.push_back({"description",[](const FileDescription& fd,
						const BehaviourDescription&,
						const Hypothesis){
	    if(!fd.description.empty()){
	      const auto d = tokenize(fd.description,'\n');
	      for(const auto& l : d){
		if((l.size()>=2)&&((l)[0]=='*')&&((l)[1]==' ')){
		  cout << l.substr(2) << '\n';
		} else {
		  cout << l << '\n';
		}
	      }
	    } else {
	      cout << "(undefined)\n";
	    }
	  }});
    } else if(qn=="--date"){
      this->queries.push_back({"date",[](const FileDescription& fd,
					 const BehaviourDescription&,
					 const Hypothesis){
	    const auto& d = fd.date;
	    cout << (!d.empty() ? d : "(undefined)") << '\n';
	  }});
    } else if(qn=="--material"){
      this->queries.push_back({"material",[](const FileDescription&,
					     const BehaviourDescription& d,
					     const Hypothesis){
	    const auto& m = d.getMaterialName();
	    cout << (!m.empty() ? m : "(undefined)") << '\n';
	  }});
    } else if(qn=="--library"){
      this->queries.push_back({"library",[](const FileDescription&,
					    const BehaviourDescription& d,
					    const Hypothesis){
	    const auto& l = d.getLibrary();
	    cout << (!l.empty() ? l : "(undefined)") << '\n';
	  }});
    } else if(qn=="--type"){
      this->queries.push_back({"type",[](const FileDescription&,
					 const BehaviourDescription& d,
					 const Hypothesis){
	    const auto t = d.getBehaviourType();
	    if(t==BehaviourDescription::GENERALBEHAVIOUR){
	      std::cout << 0 << std::endl;
	    } else if(t==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	      std::cout << 1 << std::endl;
	    } else if(t==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	      std::cout << 2 << std::endl;
	    } else if(t==BehaviourDescription::COHESIVEZONEMODEL){
	      std::cout << 3 << std::endl;
	    } else {
	      throw(std::runtime_error("unsupported behaviour type"));
	    }}});
    // } else if(qn=="--kinematic"){
    //   this->queries.push_back({"kinematic",[](const FileDescription&,
    // 					 const BehaviourDescription& d,
    // 					 const Hypothesis){
    // 	    cout << d.getBehaviourKinematic() << '\n';
    // }});
    } else if(qn=="--symmetry"){
      this->queries.push_back({"symmetry",[](const FileDescription&,
					 const BehaviourDescription& d,
					 const Hypothesis){
	    const auto s = d.getSymmetryType();
	    if(s==mfront::ISOTROPIC){
	      cout << 0 << '\n';
	    } else if(s==mfront::ORTHOTROPIC){
	      cout << 1 << '\n';
	    } else {
	      throw(std::runtime_error("unsupported symmetry"));
	    }
	  }});
    } else if(qn=="--has-crystal-structure"){
      this->queries.push_back({"has-crystal-structure",[](const FileDescription&,
							  const BehaviourDescription& d,
							  const Hypothesis){
	    if(d.hasCrystalStructure()){
	      cout << "true\n";
	    } else {
	      cout << "false\n";
	    }
	  }});
    } else if(qn=="--crystal-structure"){
      this->queries.push_back({"crystal-structure",[](const FileDescription&,
						      const BehaviourDescription& d,
						      const Hypothesis){
	    using tfel::material::CrystalStructure;
	    const auto s = d.getCrystalStructure();
	    if(s==CrystalStructure::Cubic){
	      cout << "Cubic\n";
	    } else if(s==CrystalStructure::HCP){
	      cout << "Hexagonal closed-packed\n";
	    } else if(s==CrystalStructure::FCC){
	      cout << "Face-centered cubic\n";
	    } else {
	      throw(std::runtime_error("unsupported crystal structure"));
	    }
	  }});
    } else if(qn=="--slip-systems"){
      this->queries.push_back({"slip-systems",[](const FileDescription&,
						 const BehaviourDescription& d,
						 const Hypothesis){
	    if(!d.areSlipSystemsDefined()){
	      throw(std::runtime_error("no slip system defined"));
	    }
	    const auto& ssd = d.getSlipSystems();
	    const auto nss = ssd.getNumberOfSlipSystemsFamilies();
	    for(size_t i=0;i!=nss;++i){
	      const auto gss = ssd.getSlipSystems(i);
	      cout << "- " << to_string(ssd.getSlipSystemFamily(i)) << ":";
	      for(const auto& gs : gss){
		cout << " " << to_string(gs);
	      }
	      cout << '\n';
	    }
	  }});
    } else if(qn=="--slip-systems-by-index"){
      this->queries.push_back({"slip-systems-by-index",
	    [](const FileDescription&,
	       const BehaviourDescription& d,
	       const Hypothesis){
	    if(!d.areSlipSystemsDefined()){
	      throw(std::runtime_error("no slip system defined"));
	    }
	    const auto& ssd = d.getSlipSystems();
	    const auto nss = ssd.getNumberOfSlipSystemsFamilies();
	    auto r = size_t{};
	    for(size_t i=0;i!=nss;++i){
	      const auto gss = ssd.getSlipSystems(i);
	      for(const auto& gs : gss){
		cout << "- " << r << ": " << to_string(gs) << '\n';
		++r;
	      }
	    }
	  }});
    } else if(qn=="--interaction-matrix-structure"){
      this->queries.push_back({"interaction-matrix-structure",
	    [](const FileDescription&,
	       const BehaviourDescription& d,
	       const Hypothesis){
	    if(!d.areSlipSystemsDefined()){
	      throw(std::runtime_error("no slip system defined"));
	    }
	    const auto& im     = d.getInteractionMatrix();
	    const auto  r      = im.rank();
	    const auto& ssis_r = im.getSlidingSystemsInteraction();
	    cout << "- number of independent coefficients: " << r << '\n';
	    auto i = size_t{};
	    for(const auto& ssis : ssis_r){
	      cout << "- rank " << i << ':';
	      for(const auto ssi: ssis){
		cout << " (" << to_string(ssi.g1)
		     << ':'  << to_string(ssi.g2) << ')';
	      }
	      cout << '\n';
	      ++i;
	    }
	  }});
    } else if(qn=="--interaction-matrix"){
      this->queries.push_back({"interaction-matrix",
	    [](const FileDescription&,
	       const BehaviourDescription& d,
	       const Hypothesis){
	    if(!d.areSlipSystemsDefined()){
	      throw(std::runtime_error("no slip system defined"));
	    }
	    const auto& ssd = d.getSlipSystems();
	    const auto& im  = d.getInteractionMatrix();
	    const auto nss  = ssd.getNumberOfSlipSystemsFamilies();
	    for(size_t i=0;i!=nss;++i){
	      for(const auto& gs1 : ssd.getSlipSystems(i)){
		cout << '|';
		for(size_t j=0;j!=nss;++j){
		  for(const auto& gs2 : ssd.getSlipSystems(j)){
		    cout << " " << im.getRank(gs1,gs2);
		  }
		}
		cout << " |\n";
	      }
	    }
	  }});
    } else if(qn=="--elastic-symmetry"){
      this->queries.push_back({"elastic-symmetry",[](const FileDescription&,
						     const BehaviourDescription& d,
						     const Hypothesis){
	    const auto s = d.getElasticSymmetryType();
	    if(s==mfront::ISOTROPIC){
	      cout << 0 << '\n';
	    } else if(s==mfront::ORTHOTROPIC){
	      cout << 1 << '\n';
	    } else {
	      throw(std::runtime_error("unsupported elastic symmetry"));
	    }
	  }});
    } else if(qn=="--supported-modelling-hypotheses"){
      this->queries.push_back({"supported-modelling-hypotheses",[](const FileDescription&,
								   const BehaviourDescription& d,
								   const Hypothesis){
	    const auto& mh = d.getModellingHypotheses();
	    for(const auto& h : mh){
	      cout << ModellingHypothesis::toString(h) << " ";
	    }
	    cout << '\n';
	  }});
    } else if (qn=="--material-properties"){
      this->queries.emplace_back("material-properties",
				 this->generateVariablesListQuery<&BehaviourData::getMaterialProperties>());
    } else if (qn=="--state-variables"){
      this->queries.emplace_back("state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getStateVariables>());
    } else if (qn=="--static-variables"){
      auto nq = [](const FileDescription&,
		   const BehaviourDescription& bd,
		   const Hypothesis h){
	using namespace std;
	using namespace tfel::glossary;
	const auto& d        = bd.getBehaviourData(h);
	const auto& vars     = d.getStaticVariables();
	for(const auto& v:vars){
	  const auto& n = d.getExternalName(v.name);
	  cout << "- " << n;
	  if(v.arraySize!=1u){
	    cout << '[' << v.arraySize << ']';
	  }
	  if(n!=v.name){
	    cout << " (" << v.name << ")";
	  }
	  if(!v.description.empty()){
	    cout << ": " << v.description;
	  } else {
	    const auto& glossary = Glossary::getGlossary();
	    if(glossary.contains(n)){
	      cout << ": " << glossary.getGlossaryEntry(n).getShortDescription();
	    }
	  }
	  cout << endl;
	}
      };
      this->queries.emplace_back("static-variables",nq);
    } else if (qn=="--auxiliary-state-variables"){
      this->queries.emplace_back("auxiliary-state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getAuxiliaryStateVariables>());
    } else if (qn=="--external-state-variables"){
      this->queries.emplace_back("external-state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getExternalStateVariables>());
    } else if (qn=="--integration-variables"){
      this->queries.emplace_back("integration-state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getIntegrationVariables>());
    } else if (qn=="--persistent-variables"){
      this->queries.emplace_back("persistent-state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getPersistentVariables>());
    } else if (qn=="--local-variables"){
      this->queries.emplace_back("local-state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getLocalVariables>());
    } else if (qn=="--parameters"){
      this->queries.emplace_back("parameters",
				 this->generateVariablesListQuery<&BehaviourData::getParameters>());
    } else if (qn=="--attributes"){
      this->queries.push_back({"attributes",[](const FileDescription&,
					       const BehaviourDescription& d,
					       const Hypothesis h){
	    const auto& as1 = d.getAttributes();
	    if(!as1.empty()){
	      for(const auto& a:as1){
		cout << a.first << " ";
	      }
	    }
	    const auto& as2 = d.getBehaviourData(h).getAttributes();
	    for(const auto& a:as2){
	      cout << a.first << " ";
	    }
	    cout << '\n';
	  }});
    } else if (qn=="--code-blocks"){
      this->queries.push_back({"attributes",[](const FileDescription&,
					       const BehaviourDescription& d,
					       const Hypothesis h){
	    const auto& names = d.getBehaviourData(h).getCodeBlockNames();
	    for(const auto& n:names){
	      cout << "- "  << n << '\n';
	    }
	  }});
    } else {
      throw(std::runtime_error("Behaviour::treatStandardQuery : "
			       "unsupported query '"+qn+"'"));
    }
  }

  void BehaviourQuery::treatStandardQuery2(){
    using namespace std;
    using namespace tfel::utilities;
    using tfel::material::ModellingHypothesis;
    const auto& q  = this->getCurrentCommandLineArgument();
    const auto& qn = q.as_string();
    const auto& o  = q.getOption();
    if(o.empty()){
      throw(runtime_error("Behaviour::treatStandardQuery2 : "
			  "no option given to the '"+qn+"' query"));
    } else if (qn=="--attribute-type"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& a = getAttribute(o,d,h);
	if(a.is<bool>()){
	  cout << "bool\n";
	} else if(a.is<unsigned short>()){
	  cout << "unsigned short\n";
	} else if(a.is<string>()){
	  cout << "string\n";
	} else {
	  throw(runtime_error("Behaviour::treatStandardQuery2 : "
			      "unsupported attribute type"));
	}
      };
      this->queries.push_back({"attribute-type",l});
    } else if (qn=="--attribute-value"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& a = getAttribute(o,d,h);
	if(a.is<bool>()){
	  if(a.get<bool>()){
	    cout << "true\n";
	  } else {
	    cout << "false\n";
	  }
	} else if(a.is<unsigned short>()){
	  cout << a.get<unsigned short>() << '\n';
	} else if(a.is<string>()){
	  cout << a.get<string>() << '\n';
	} else {
	  throw(runtime_error("Behaviour::treatStandardQuery2 : "
			      "unsupported attribute type"));
	}
      };
      this->queries.push_back({"attributes-value",l});
    } else if (qn=="--has-bounds"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto  vn = extracVariableNameAndArrayPosition(o);
	const auto& n  = std::get<0>(vn);
	const auto& bd = d.getBehaviourData(h);
	const auto& p  = bd.getVariableDescriptionByExternalName(n);
	if(std::get<1>(vn)){
	  cout << (p.hasBounds(std::get<2>(vn)) ? "true" : "false") << '\n';
	} else {
	  cout << (p.hasBounds() ? "true" : "false") << '\n';
	}
      };
      this->queries.push_back({"has-bounds",l});
    } else if (qn=="--bounds-type"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& bd = d.getBehaviourData(h);
	const auto  vn = extracVariableNameAndArrayPosition(o);
	const auto& n  = std::get<0>(vn);
	const auto& p  = bd.getVariableDescriptionByExternalName(n);
	const auto& b  = std::get<1>(vn) ? p.getBounds(std::get<2>(vn)) : p.getBounds();
	if(b.boundsType==VariableBoundsDescription::LOWER){
	  cout << "Lower\n";
	} else if(b.boundsType==VariableBoundsDescription::UPPER){
	  cout << "Upper\n";
	} else if(b.boundsType==VariableBoundsDescription::LOWERANDUPPER){
	  cout << "LowerAndUpper\n";
	} else {
	  throw(std::runtime_error("unsupported bounds type"));
	}
      };
      this->queries.push_back({"bounds-type",l});
    } else if (qn=="--bounds-value"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& bd = d.getBehaviourData(h);
	const auto  vn = extracVariableNameAndArrayPosition(o);
	const auto& n  = std::get<0>(vn);
	const auto& p  = bd.getVariableDescriptionByExternalName(n);
	const auto& b  = std::get<1>(vn) ? p.getBounds(std::get<2>(vn)) : p.getBounds();
	if(b.boundsType==VariableBoundsDescription::LOWER){
	  cout << "[" << b.lowerBound << ":*[\n";
	} else if(b.boundsType==VariableBoundsDescription::UPPER){
	  cout << "]*:" << b.upperBound << "]\n";
	} else if(b.boundsType==VariableBoundsDescription::LOWERANDUPPER){
	  cout << "[" << b.lowerBound << ":" << b.upperBound << "]\n";
	} else {
	  throw(std::runtime_error("unsupported bounds type"));
	}
      };
      this->queries.push_back({"bounds-value",l});
    } else if (qn=="--has-physical-bounds"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto  vn = extracVariableNameAndArrayPosition(o);
	const auto& n  = std::get<0>(vn);
	const auto& bd = d.getBehaviourData(h);
	const auto& p  = bd.getVariableDescriptionByExternalName(n);
	if(std::get<1>(vn)){
	  cout << (p.hasPhysicalBounds(std::get<2>(vn)) ? "true" : "false") << '\n';
	} else {
	  cout << (p.hasPhysicalBounds() ? "true" : "false") << '\n';
	}
      };
      this->queries.push_back({"has-physical-bounds",l});
    } else if (qn=="--physical-bounds-type"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& bd = d.getBehaviourData(h);
	const auto  vn = extracVariableNameAndArrayPosition(o);
	const auto& n  = std::get<0>(vn);
	const auto& p  = bd.getVariableDescriptionByExternalName(n);
	const auto& b  = (std::get<1>(vn) ?
			  p.getPhysicalBounds(std::get<2>(vn)): 
			  p.getPhysicalBounds());
	if(b.boundsType==VariableBoundsDescription::LOWER){
	  cout << "Lower\n";
	} else if(b.boundsType==VariableBoundsDescription::UPPER){
	  cout << "Upper\n";
	} else if(b.boundsType==VariableBoundsDescription::LOWERANDUPPER){
	  cout << "LowerAndUpper\n";
	} else {
	  throw(std::runtime_error("unsupported physical bounds type"));
	}
      };
      this->queries.push_back({"physical-bounds-type",l});
    } else if (qn=="--physical-bounds-value"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& bd = d.getBehaviourData(h);
	const auto  vn = extracVariableNameAndArrayPosition(o);
	const auto& n  = std::get<0>(vn);
	const auto& p  = bd.getVariableDescriptionByExternalName(n);
	const auto& b  = (std::get<1>(vn) ?
			  p.getPhysicalBounds(std::get<2>(vn)): 
			  p.getPhysicalBounds());
	if(b.boundsType==VariableBoundsDescription::LOWER){
	  cout << "[" << b.lowerBound << ":*[\n";
	} else if(b.boundsType==VariableBoundsDescription::UPPER){
	  cout << "]*:" << b.upperBound << "]\n";
	} else if(b.boundsType==VariableBoundsDescription::LOWERANDUPPER){
	  cout << "[" << b.lowerBound << ":" << b.upperBound << "]\n";
	} else {
	  throw(std::runtime_error("unsupported bounds type"));
	}
      };
      this->queries.push_back({"physical-bounds-value",l});
    } else if (qn=="--parameter-type"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& bd = d.getBehaviourData(h);
	const auto& p  = bd.getParameterDescriptionByExternalName(o);
	cout << p.type << '\n';
      };
      this->queries.push_back({"parameter-type",l});
    } else if (qn=="--parameter-default-value"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& bd = d.getBehaviourData(h);
	const auto& p  = bd.getParameterDescriptionByExternalName(o);
	const auto& n  = p.name;
	if(p.type=="real"){
	  if(p.arraySize==1){
	    cout << bd.getFloattingPointParameterDefaultValue(n) << '\n';
	  } else {
	    for(unsigned short i=0;i!=p.arraySize;){
	      cout << bd.getFloattingPointParameterDefaultValue(n,i);
	      if(++i!=p.arraySize){
		cout << " ";
	      }
	    }
	    cout << '\n';
	  }
	} else if(p.type=="int"){
	  cout << bd.getIntegerParameterDefaultValue(n) << '\n';
	} else if(p.type=="ushort"){
	  cout << bd.getUnsignedShortParameterDefaultValue(n) << '\n';
	} else {
	  throw(runtime_error("Behaviour::treatStandardQuery2 : "
			      "unsupported parameter type"));
	}
      };
      this->queries.push_back({"parameter-default-value",l});
    } else if (qn=="--static-variable-value"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& bd  = d.getBehaviourData(h);
	for(const auto& sv : bd.getStaticVariables()){
	  if(sv.name==o){
	    std::cout << sv.value << '\n';
	    return;
	  }
	}
	throw(runtime_error("Behaviour::treatStandardQuery2 : "
			    "no static variable '"+o+"'"));
      };
      this->queries.push_back({"static-variable-value",l});
    } else {
	  throw(runtime_error("Behaviour::treatStandardQuery : "
			  "unsupported query '"+qn+"'"));
    }
  }

  template<const VariableDescriptionContainer& (BehaviourData::* m)(void) const>
  typename BehaviourQuery::query
  BehaviourQuery::generateVariablesListQuery(){
    return [](const FileDescription&,
	      const BehaviourDescription& bd,
	      const Hypothesis h){
      using namespace std;
      using namespace tfel::glossary;
      const auto& d        = bd.getBehaviourData(h);
      const auto& vars     = (d.*m)();
      for(const auto& v:vars){
	const auto& n = d.getExternalName(v.name);
	cout << "- " << n;
	if(v.arraySize!=1u){
	  cout << '[' << v.arraySize << ']';
	}
	if(n!=v.name){
	  cout << " (" << v.name << ")";
	}
	if(!v.description.empty()){
	  cout << ": " << v.description;
	} else {
	  const auto& glossary = Glossary::getGlossary();
	  if(glossary.contains(n)){
	    cout << ": " << glossary.getGlossaryEntry(n).getShortDescription();
	  }
	}
	cout << '\n';
      }
    };
  }

  void BehaviourQuery::treatGeneratedSources()
  {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,
		    const BehaviourDescription&,
		    const Hypothesis){
      for(const auto& l : ldsl->getTargetsDescription()){
	std::cout << l.name << " : "; //< library
	std::copy(l.sources.begin(),l.sources.end(),
		  std::ostream_iterator<std::string>(std::cout," "));
	std::cout << '\n';
      }
    };
    this->queries.push_back({"generated-sources",q});
  } // end of BehaviourQuery::treatGeneratedSources

  void BehaviourQuery::treatGeneratedHeaders()
  {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,
		    const BehaviourDescription&,
		    const Hypothesis){
      const auto headers = ldsl->getTargetsDescription().headers;
      std::copy(headers.begin(),headers.end(),
		std::ostream_iterator<std::string>(std::cout," "));
      std::cout << '\n';
    };
    this->queries.push_back({"generated-headers",q});
  } // end of BehaviourQuery::treatGeneratedHeaders

  void BehaviourQuery::treatCppFlags()
  {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,
		    const BehaviourDescription&,
		    const Hypothesis){
      for(const auto& l : ldsl->getTargetsDescription()){
	std::cout << l.name << " : ";
	std::copy(l.cppflags.begin(),l.cppflags.end(),
		  std::ostream_iterator<std::string>(std::cout," "));
	std::cout << '\n';
      }
    };
    this->queries.push_back({"cppflags",q});
  } // end of BehaviourQuery::treatCppFlags

  void BehaviourQuery::treatLibrariesDependencies()
  {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,
		    const BehaviourDescription&,
		    const Hypothesis){
      for(const auto& l : ldsl->getTargetsDescription()){
	std::cout << l.name << " : ";
	std::copy(l.ldflags.begin(),l.ldflags.end(),
		  std::ostream_iterator<std::string>(std::cout," "));
	std::cout << '\n';
      }
    };
    this->queries.push_back({"libraries-dependencies",q});
  } // end of BehaviourQuery::treatLibrariesDependencies

  void BehaviourQuery::treatSpecificTargets()
  {
    auto ldsl = this->dsl;
    auto l = [ldsl](const FileDescription&,
		    const BehaviourDescription&,
		    const Hypothesis){
      const auto targets = ldsl->getTargetsDescription().specific_targets;
      for(const auto& t : targets){
	std::cout << t.first << " : ";
	std::copy(t.second.first.begin(),t.second.first.end(),
		  std::ostream_iterator<std::string>(std::cout," "));
	std::cout << '\n' << "> rule : ";
	std::copy(t.second.second.begin(),t.second.second.end(),
		  std::ostream_iterator<std::string>(std::cout,"\n> rule : "));
	std::cout << '\n';
      }
    };
    this->queries.push_back({"specific-targets",l});
  } // end of BehaviourQuery::treatSpecificTargets

  void BehaviourQuery::treatModellingHypothesis()
  {
    using namespace std;
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    if(o.empty()){
      throw(runtime_error("BehaviourQuery::treatModellingHypothesis : "
			  "no option given to --modelling-hypothesis"
			  "command line option"));
    }
    if(this->hypothesis!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      throw(runtime_error("BehaviourQuery::treatModellingHypothesis : "
			  "modelling hypothesis already defined"));
    }
    this->hypothesis = ModellingHypothesis::fromString(o);
  } // end of BehaviourQuery::treatModellingHypothesis

  void BehaviourQuery::exe()
  {
    using namespace std;
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      getLogStream() << "Treating file '" << this->file << "'\n";
    }
    // analysing the file
    this->dsl->analyseFile(this->file,this->ecmds,this->substitutions);
    const auto& fd = this->dsl->getFileDescription();
    const auto& d  = this->dsl->getBehaviourDescription();
    // checks
    if(this->hypothesis==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(d.areAllMechanicalDataSpecialised()){
	ostringstream msg;
	const auto& mh = d.getModellingHypotheses();
	if(mh.size()==1u){
	  this->hypothesis = *(mh.begin());
	} else {
	  msg << "BehaviourQuery::exe : all mechanical data are specialised.\n"
	      << "A modelling hypothesis must be specified using "
	      << "the '--modelling-hypothesis' command line option.\n"
	      << "The supported modelling hypothesis are : \n";
	  for(const auto h : mh){
	    msg << "- " << ModellingHypothesis::toString(h) << '\n';
	  }
	  throw(runtime_error(msg.str()));
	}
      }
    } else  {
      const auto& mh = d.getDistinctModellingHypotheses();
      if(mh.find(this->hypothesis)==mh.end()){
	string msg("BehaviourQuery::exe : the specified modelling hypothesis ('"+
		   ModellingHypothesis::toString(this->hypothesis)+"') is not supported by the behaviour");
	throw(runtime_error(msg));
      }
    }
    // treating the queries
    for(const auto& q : queries){
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	getLogStream() << "Treating query '" << q.first << "'\n";
      }
      q.second(fd,d,this->hypothesis);
    }
  } // end of BehaviourQuery::exe

  const tfel::utilities::Argument&
  BehaviourQuery::getCurrentCommandLineArgument() const{
    return *(this->currentArgument);
  }
  
  void BehaviourQuery::treatUnknownArgument()
  {
    if(!MFrontBase::treatUnknownArgumentBase()){
#if ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
      ArgumentParserBase<BehaviourQuery>::treatUnknownArgument();
#else
      const auto& a = static_cast<const std::string&>(this->getCurrentCommandLineArgument());
      std::cerr << "mfront : unsupported option '" << a << "'\n";
      ::exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
  }

  std::string BehaviourQuery::getVersionDescription() const{
    return MFrontHeader::getHeader();
  }

  std::string BehaviourQuery::getUsageDescription() const
  {
    return "Usage: "+this->programName+" [options] [files]";
  }

  BehaviourQuery::~BehaviourQuery() = default;

} // end of namespace mfront
