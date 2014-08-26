/*! 
 * \file  MFrontZMATInterface.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juil. 2013
 */

#include<iostream>

#include<sstream>
#include<stdexcept>
#include<algorithm>

#include"TFEL/System/System.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontFileDescription.hxx"
#include"MFront/MFrontZMATInterface.hxx"

namespace mfront
{

  static void
  writeZMATUndefs(std::ostream& out)
  {
    using std::endl;
    out << "#include\"MFront/ZMAT/ZMATUndefs.hxx\"" << endl;
  } // end of writeZMATUndefs

  template<typename ArrayType>
  static void
  writeArray(std::ostream& out,
	     const ArrayType& a)
  {
    using namespace std;
    std::string buffer;
    out << "{";
    for(typename ArrayType::const_iterator pn=a.begin();
	pn!=a.end();){
      if(buffer.size()+pn->size()+3>15){
	out << buffer << endl;
	buffer.clear();
      }
      buffer += "\"" + *pn + "\"";
      if(++pn!=a.end()){
	buffer += ",";
      }
    }
    if(!buffer.empty()){
      out << buffer;
    }
    out << "};" << endl;
  }

  std::set<std::string>
  getAllMaterialPropertiesNames(const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    typedef MFrontZMATInterface::ModellingHypothesis ModellingHypothesis;
    typedef MFrontZMATInterface::Hypothesis Hypothesis;
    set<string> mp_names;
    const Hypothesis hypotheses[3u] = {ModellingHypothesis::TRIDIMENSIONAL,
				       ModellingHypothesis::GENERALISEDPLANESTRAIN,
				       ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN};
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
	const VariableDescriptionContainer& mps  = d.getMaterialProperties();
	for(VariableDescriptionContainer::const_iterator pv=mps.begin();
	    pv!=mps.end();++pv){
	  const VariableDescription& v = *pv;
	  const string& name = d.getGlossaryName(v.name);
	  if(v.arraySize==1u){
	    mp_names.insert(name);
	  } else {
	    for(unsigned short n=0;n!=v.arraySize;++n){
	      ostringstream str;
	      str << name << '[' << n << ']';
	      mp_names.insert(str.str());
	    }
	  }
	}
      }
    }
    return mp_names;
  }

  VariableDescriptionContainer
  getAllStateVariables(const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    typedef MFrontZMATInterface::ModellingHypothesis ModellingHypothesis;
    typedef MFrontZMATInterface::Hypothesis Hypothesis;
    VariableDescriptionContainer s;
    const Hypothesis hypotheses[3u] = {ModellingHypothesis::TRIDIMENSIONAL,
				       ModellingHypothesis::GENERALISEDPLANESTRAIN,
				       ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN};
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	const MechanicalBehaviourData& d         = mb.getMechanicalBehaviourData(h);
	const VariableDescriptionContainer& svs  = d.getPersistentVariables();
	for(VariableDescriptionContainer::const_iterator pv=svs.begin();
	    pv!=svs.end();++pv){
	  if(s.contains(pv->name)){
	    const VariableDescription& v = s.getVariable(pv->name);
	    if((pv->type!=v.type)||(pv->arraySize!=v.arraySize)){
	      string msg("getAllStateVariables : "
			 "inconsistent type for variable '"+pv->name+"'");
	      throw(runtime_error(msg));
	    }
	  } else {
	    s.push_back(*pv);
	  }
	}
      }
    }
    return s;
  }

  static unsigned short
  getSpaceDimension(const MFrontZMATInterface::Hypothesis h)
  {
    using namespace std;
    typedef MFrontZMATInterface::ModellingHypothesis ModellingHypothesis;
    if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return 3u;
    } else if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return 2u;
    } else if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return 1u;
    }
    string msg("getSpaceDimension: unsupported hypothesis");
    throw(runtime_error(msg));
    return 0;
  }

  static unsigned short
  getStensorSize(const MFrontZMATInterface::Hypothesis h)
  {
    using namespace std;
    typedef MFrontZMATInterface::ModellingHypothesis ModellingHypothesis;
    if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return 6u;
    } else if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return 4u;
    } else if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return 3u;
    }
    string msg("getSpaceDimension: unsupported hypothesis");
    throw(runtime_error(msg));
    return 0;
  }

  static std::string
  getSpaceDimensionSuffix(const MFrontZMATInterface::Hypothesis h)
  {
    using namespace std;
    typedef MFrontZMATInterface::ModellingHypothesis ModellingHypothesis;
    if(h==ModellingHypothesis::TRIDIMENSIONAL){
      return "3D";
    } else if(h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      return "2D";
    } else if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      return "1D";
    }
    string msg("getSpaceDimension: unsupported hypothesis");
    throw(runtime_error(msg));
    return "";
  }

  static std::string
  getLibraryName(const MechanicalBehaviourDescription& mb)
  {
    using namespace std;
    string lib;
    if(mb.getLibrary().empty()){
      if(!mb.getMaterialName().empty()){
	lib = "libZMAT"+mb.getMaterialName();
      } else {
	lib = "libZMATBehaviour";
      }
    } else {
      lib = "libZMAT"+mb.getLibrary();
    }
    return lib;
  } // end of MFrontUMATInterface::getLibraryName

  std::string
  MFrontZMATInterface::getName(void)
  {
    return "zmat";
  }

  MFrontZMATInterface::MFrontZMATInterface()
  {} // end of MFrontZMATInterface::MFrontZMATInterface

  void
  MFrontZMATInterface::allowDynamicallyAllocatedArrays(const bool b)
  {
    this->areDynamicallyAllocatedVectorsAllowed_ = b;
  } // end of MFrontZMATInterface::allowDynamicallyAllocatedArrays


  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontZMATInterface::treatKeyword(const std::string&,
				    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
				    const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  }
  
  bool
  MFrontZMATInterface::isModellingHypothesisHandled(const Hypothesis h,
						    const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    set<Hypothesis> ih(this->getModellingHypothesesToBeTreated(mb));
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      return !mb.areAllMechanicalDataSpecialised(ih);
    }
    return ih.find(h)!=ih.end();
  }
  
  std::set<MFrontZMATInterface::Hypothesis>
  MFrontZMATInterface::getModellingHypothesesToBeTreated(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    using tfel::material::ModellingHypothesis;
    typedef ModellingHypothesis::Hypothesis Hypothesis;
    // treatment 
    set<Hypothesis> h;
    // modelling hypotheses handled by the behaviour
    const set<Hypothesis>& bh = mb.getModellingHypotheses();
    if(bh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)!=bh.end()){
      h.insert(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    }
    if(bh.find(ModellingHypothesis::GENERALISEDPLANESTRAIN)!=bh.end()){
      h.insert(ModellingHypothesis::GENERALISEDPLANESTRAIN);
    }
    if(bh.find(ModellingHypothesis::TRIDIMENSIONAL)!=bh.end()){
      h.insert(ModellingHypothesis::TRIDIMENSIONAL);
    }
    if(h.empty()){
      string msg("MFrontZMATInterfaceModellingHypothesesToBeTreated : "
		 "no hypotheses selected. This means that the given beahviour "
		 "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
		 "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
		 "make sense to use the ZMAT interface");
      throw(runtime_error(msg));
    }
    return h;
  } // edn of MFrontZMATInterface::getModellingHypothesesToBeTreated

  void
  MFrontZMATInterface::reset(void)
  {
    SupportedTypes::reset();
  } // end of MFrontZMATInterface::reset

  void 
  MFrontZMATInterface::exportMechanicalData(std::ofstream& behaviourDataFile,
					    const Hypothesis h,
					    const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& persistentVarsHolder = d.getPersistentVariables();
    if(!persistentVarsHolder.empty()){
      behaviourDataFile << "void" << endl
    			<< "ZMATexportStateData("
    			<< "ZSET::TENSOR2_FLUX& ZMATsig,ZSET::MAT_DATA& ZMATdata) const" << endl;
    } else {
      behaviourDataFile << "void" << endl
    			<< "ZMATexportStateData("
    			<< "ZSET::TENSOR2_FLUX& ZMATsig,ZSET::MAT_DATA&) const" << endl;
    }
    behaviourDataFile << "{" << endl;
    behaviourDataFile << "using namespace tfel::math;" << endl;
    if(!((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
      string msg("MFrontZMATInterface::exportMechanicalData : "
		 "only small or finite strain behaviours are supported");
      throw(runtime_error(msg));
    }
    behaviourDataFile << "zmat::ZMATInterface::convert(&ZMATsig[0],this->sig);" << endl;
    if(!persistentVarsHolder.empty()){
      behaviourDataFile << "ZSET::INTERNAL_VARIABLE_VECTOR& ZMATstatev = ZMATdata.var_int()[0];";
      this->exportResults(behaviourDataFile,
    			  persistentVarsHolder,
    			  "ZMATstatev",
    			  mb.useQt());
    }
    behaviourDataFile << "} // end of ZMATexportStateData" << endl;
    behaviourDataFile << endl;
  }
  
  void 
  MFrontZMATInterface::writeInterfaceSpecificIncludes(std::ofstream& out,
						       const MechanicalBehaviourDescription&) const
  {
    using namespace std;
    out << "#include\"MFront/ZMAT/ZMATInterface.hxx\"" << endl;
    out << "#include\"Coefficient.h\"" << endl;
    out << "#include\"Behavior.h\"" << endl;
    writeZMATUndefs(out);
    out << endl;
  } // end of MFrontZMATInterface::writeInterfaceSpecificIncludes

  void
  MFrontZMATInterface::writeBehaviourDataConstructor(std::ofstream& out,
						     const Hypothesis h,
						     const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& mps = d.getMaterialProperties();
    const VariableDescriptionContainer& ivs = d.getPersistentVariables();
    const VariableDescriptionContainer& evs = d.getExternalStateVariables();
    if(!(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)){
      string msg("MFrontZMATInterface::writeBehaviourDataConstructor : "
		 "only small or finite strain behaviours are supported");
      throw(runtime_error(msg));
    }
    out << "/*!" << endl;
    out << " * \\brief constructor for the ZMAT interface" << endl;
    out << " *" << endl;
    if((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << " * \\param ZMATsig     : stress at the beginning of the time step" << endl;
    }
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << " \\param ZMATdeto     : strain at the beginning of the time step" << endl;
    }
    out << " * \\param ZMATmprops  : material properties" << endl;
    out << " * \\param ZMATdata    : material data handler" << endl;
    out << " * \\param ZMATT_pos   : position of the temperature" << endl;
    out << " * \\param ZMATevs_pos : position of the external state varaibles" << endl;
    out << " */" << endl;
    out << mb.getClassName() << "BehaviourData(";
    if((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << "const ZSET::TENSOR2_FLUX& ZMATsig," << endl;
    }
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "const real * const ZMATeto," << endl;
    }
    if(!mps.empty()){
      out << "const ZSET::ARRAY<ZSET::COEFF>& ZMATmprops," << endl;
    } else {
      out << "const ZSET::ARRAY<ZSET::COEFF>&," << endl;
    }
    if(!ivs.empty()){
      out << "const ZSET::MAT_DATA& ZMATdata," << endl;
    } else {
      out << "const ZSET::MAT_DATA&," << endl;
    }
    out << "const int ZMATT_pos," << endl;
    if(!evs.empty()){
      out << "const ZSET::ARRAY<int>& ZMATevs_pos){" << endl;
    } else {
      out << "const ZSET::ARRAY<int>&){" << endl;
    }
    if(!ivs.empty()){
      out << "const ZSET::INTERNAL_VARIABLE_VECTOR& ZMATstatev = ZMATdata.var_int_ini()[0];"  << endl;
    }
    out << "const ZSET::EXTERNAL_PARAMETER_VECTOR& ZMATextvars_t = *(ZMATdata.param_set_ini());"  << endl;
    out << "zmat::ZMATInterface::convert(this->sig,&ZMATsig[0]);" << endl;
    out << "zmat::ZMATInterface::convert(this->eto,&ZMATeto[0]);" << endl;
    if(!mps.empty()){
      SupportedTypes::TypeSize o;
      for(VariableDescriptionContainer::const_iterator pv=mps.begin();pv!=mps.end();++pv){
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(pv->type);
	if(flag==SupportedTypes::Scalar){
	  if(pv->arraySize==1u){
	    out << "this->" << pv->name << " = ZMATmprops[" << o << "]();" << endl;
	    o+=this->getTypeSize(pv->type,1u);
	  } else {
	    if(this->useDynamicallyAllocatedVector(pv->arraySize)){
	      out << "this->" << pv->name << ".resize(" << pv->arraySize << ");" << endl;
	      out << "for(unsigned short idx=0;idx!=" << pv->arraySize << ";++idx){" << endl;
	      out << "this->" << pv->name << "[idx] = ZMATmprops[" << o << "+idx]();" << endl;
	      out << "}" << endl;
	      o+=this->getTypeSize(pv->type,pv->arraySize);
	    } else {
	      for(unsigned short i=0;i!=pv->arraySize;++i){
		out << "this->" << pv->name << "[" << i << "] = ZMATmprops[" << o << "]();" << endl;
		o+=this->getTypeSize(pv->type,1u);
	      }
	    }
	  }
	} else {
	  string msg("MFrontZMATInterface::writeBehaviourDataConstructor : "
		     "unsupported material properties type (only scalar supported yet)");
	  throw(runtime_error(msg));
	}
      }
    }
    if(!ivs.empty()){
      SupportedTypes::TypeSize o;
      for(VariableDescriptionContainer::const_iterator pv=ivs.begin();pv!=ivs.end();++pv){
	const string n = "this->"+pv->name;
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(pv->type);
	if(pv->arraySize==1u){
	  switch(flag){
	  case SupportedTypes::Scalar : 	  
	    out << n << " = ZMATstate[" << o << "];" << endl;
	    break;
	  case SupportedTypes::Stensor :
	  case SupportedTypes::Tensor  :
	    out << "zmat::ZMATInterface::convert(" << n << "," << "&ZMATstatev[" << o << "]);" << endl;
	    break;
	  default:
	    string msg("MFrontZMATInterface::writeBehaviourDataConstructor : ");
	    msg += "unsupported variable type ('"+pv->type+"')";
	    throw(runtime_error(msg));
	  }
	  o+=this->getTypeSize(pv->type,1u);
	} else {
	  if(this->useDynamicallyAllocatedVector(pv->arraySize)){
	    out << n << ".resize(" << pv->arraySize << ");" << endl;
	    out << "for(unsigned short idx=0;idx!=" << pv->arraySize << ";++idx){" << endl;
	    switch(flag){
	    case SupportedTypes::Scalar : 
	      out << n << "[idx] = ZMATstatev[" 
		<< o << "+idx];\n";  
	      break;
	    // case SupportedTypes::TVector :
	    //   out << "tfel::fsalgo<TVectorSize>(&"+src+"[" 
	    // 	<< o << "+idx*TVectorSize],"
	    // 	<< n << "[idx].begin());\n";  
	    //   break;
	    case SupportedTypes::Stensor :
	    case SupportedTypes::Tensor :
	      out << "zmat::ZMATInterface::convert(" << n << "[idx],&ZMATstatev[" 
		  << o << "+idx*StensorSize]);\n";  
	      break;
	    default : 
	      string msg("MFrontZMATInterface::writeBehaviourDataConstructor : "
			 "unsupported variable type ('"+pv->type+"')");
	      throw(runtime_error(msg));
	    }
	    out << "}" << endl;
	    o+=this->getTypeSize(pv->type,pv->arraySize);
	  } else {
	    for(int i=0;i!=pv->arraySize;++i){
	      switch(flag){
	      case SupportedTypes::Scalar : 
		out << n << "[" << i << "] = ZMATstatev[" 
		  << o << "];\n";  
		break;
	      case SupportedTypes::Stensor :
	      case SupportedTypes::Tensor :
		out << "zmat::ZMATInterface::convert(" <<  n << "["<< i << "],&ZMATstatev[" 
		    << o << "]);\n";  
		break;
	      default : 
		string msg("MFrontZMATInterface::writeBehaviourDataConstructor : "
			   "unsupported variable type ('"+pv->type+"')");
		throw(runtime_error(msg));
	      }
	      o+=this->getTypeSize(pv->type,1u);
	    }
	  }
	}
      }
    }
    if(!evs.empty()){
      SupportedTypes::TypeSize o;
      for(VariableDescriptionContainer::const_iterator pv=evs.begin();pv!=evs.end();++pv){
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(pv->type);
	if(flag==SupportedTypes::Scalar){
	  if(pv->arraySize==1u){
	    out << "this->" << pv->name << " = ZMATextvars_t[ZMATevs_pos[" << o << "]];" << endl;
	    o+=this->getTypeSize(pv->type,1u);
	  } else {
	    if(this->useDynamicallyAllocatedVector(pv->arraySize)){
	      out << "this->" << pv->name << ".resize(" << pv->arraySize << ");" << endl;
	      out << "for(unsigned short idx=0;idx!=" << pv->arraySize << ";++idx){" << endl;
	      out << "this->" << pv->name << "[idx] = ZMATextvars_t[ZMATevs_pos[" << o << "+idx]];" << endl;
	      out << "}" << endl;
	      o+=this->getTypeSize(pv->type,pv->arraySize);
	    } else {
	      for(unsigned short i=0;i!=pv->arraySize;++i){
		out << "this->" << pv->name << "[" << i << "] = ZMATextvars_t[ZMATevs_pos[" << o << "]];" << endl;
		o+=this->getTypeSize(pv->type,1u);
	      }
	    }
	  }
	} else {
	  string msg("MFrontZMATInterface::writeBehaviourDataConstructor : "
		     "unsupported material properties type (only scalar supported yet)");
	  throw(runtime_error(msg));
	}
      }
    }
    out << "this->T = ZMATextvars_t[ZMATT_pos];" << endl;
    out << "}" << endl << endl;
  } // end of MFrontZMATInterface::writeBehaviourDataConstructor

  void
  MFrontZMATInterface::writeBehaviourConstructor(std::ofstream& out,
						 const MechanicalBehaviourDescription& mb,
						 const std::string& initStateVarsIncrements) const
  {
    using namespace std;
    if(!(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)){
      string msg("MFrontZMATInterface::writeBehaviourDataConstructor : "
		 "only small or finite strain behaviours are supported");
      throw(runtime_error(msg));
    }
    out << "/*!" << endl;
    out << " * \\brief constructor for the ZMAT interface" << endl;
    out << " *" << endl;
    if((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << " * \\param ZMATsig     : strains" << endl;
    }
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << " \\param ZMATeto     : strain at the beginning of the time step" << endl;
      out << " \\param ZMATdeto    : strain increment" << endl;
    }
    out << " * \\param ZMATmprops  : material properties" << endl;
    out << " * \\param ZMATdata    : material data handler" << endl;
    out << " * \\param ZMATT_pos   : position of the temperature" << endl;
    out << " * \\param ZMATevs_pos : position of the external state varaibles" << endl;
    out << " * \\param ZMATdt      : time step" << endl;
    out << " */" << endl;
    out << mb.getClassName() << "(";
    if((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      out << "const ZSET::TENSOR2_FLUX& ZMATsig," << endl;
    }
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << " const real* const ZMATeto," << endl;
      out << " const real* const ZMATdeto," << endl;
    }
    out << "const ZSET::ARRAY<ZSET::COEFF>& ZMATmprops," << endl
	<< "const ZSET::MAT_DATA& ZMATdata," << endl
	<< "const int ZMATT_pos," << endl
	<< "const ZSET::ARRAY<int>& ZMATevs_pos," << endl
	<< "const double ZMATdt)" << endl;
    if((mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      if(mb.useQt()){
	out << ": " << mb.getClassName() 
	    << "BehaviourData<hypothesis,Type,use_qt>(ZMATsig,ZMATeto,ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos)," << endl;
	out << mb.getClassName() 
	    << "IntegrationData<hypothesis,Type,use_qt>(ZMATdeto,ZMATdata,ZMATT_pos,ZMATevs_pos,ZMATdt)";
      } else {
	out << ": " << mb.getClassName() 
	    << "BehaviourData<hypothesis,Type,false>(ZMATsig,ZMATeto,ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos)," << endl;
	out << mb.getClassName() 
	    << "IntegrationData<hypothesis,Type,false>(ZMATdeto,ZMATdata,ZMATT_pos,ZMATevs_pos,ZMATdt)";
      }
    }
    if(!initStateVarsIncrements.empty()){
      out << "," << endl << initStateVarsIncrements;
    }
  }
  
  void 
  MFrontZMATInterface::writeIntegrationDataConstructor(std::ofstream& out,
						       const Hypothesis h,
						       const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& evs  = d.getExternalStateVariables();
    if(!(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)){
      string msg("MFrontZMATInterface::writeBehaviourDataConstructor : "
		 "only small or finite strain behaviours are supported");
      throw(runtime_error(msg));
    }
    out << "/*!" << endl;
    out << " * \\brief constructor for the ZMAT interface" << endl;
    out << " *" << endl;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << " \\param ZMATdeto    : strain increment " << endl;
    }
    out << " * \\param ZMATdata    : material data handler" << endl;
    out << " * \\param ZMATT_pos   : position of the temperature" << endl;
    out << " * \\param ZMATevs_pos : position of the external state varaibles" << endl;
    out << " * \\param ZMATdt      : time step" << endl;
    out << " */" << endl;
    out << mb.getClassName() << "IntegrationData(";
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "const real * const ZMATdeto," << endl;
    }
    out << "const ZSET::MAT_DATA& ZMATdata," << endl
	<< "const int ZMATT_pos," << endl;
    if(!evs.empty()){
      out << "const ZSET::ARRAY<int>& ZMATevs_pos," << endl;
    } else {
      out << "const ZSET::ARRAY<int>&," << endl;
    }
    out << "const double ZMATdt)" << endl;
    out << ": dt(ZMATdt)" << endl; 
    out << "{" << endl;
    out << "const ZSET::EXTERNAL_PARAMETER_VECTOR& ZMATextvars_t   = *(ZMATdata.param_set_ini());" << endl;
    out << "const ZSET::EXTERNAL_PARAMETER_VECTOR& ZMATextvars_tdt = *(ZMATdata.param_set());"     << endl;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "zmat::ZMATInterface::convert(this->deto,ZMATdeto);" << endl;
    }
    if(!evs.empty()){
      SupportedTypes::TypeSize o;
      for(VariableDescriptionContainer::const_iterator pv=evs.begin();pv!=evs.end();++pv){
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(pv->type);
	if(flag==SupportedTypes::Scalar){
	  if(pv->arraySize==1u){
	    out << "this->" << pv->name << " = ZMATextvars_tdt[ZMATevs_pos[" << o << "]]-ZMATextvars_t[ZMATevs_pos[" << o << "]];" << endl;
	    o+=this->getTypeSize(pv->type,1u);
	  } else {
	    if(this->useDynamicallyAllocatedVector(pv->arraySize)){
	      out << "this->" << pv->name << ".resize(" << pv->arraySize << ");" << endl;
	      out << "for(unsigned short idx=0;idx!=" << pv->arraySize << ";++idx){" << endl;
	      out << "this->" << pv->name << "[idx] = ZMATextvars_td-[ZMATevs_pos[" << o << "+idx]]-ZMATextvars_t[ZMATevs_pos[" << o << "+idx]];" << endl;
	      out << "}" << endl;
	      o+=this->getTypeSize(pv->type,pv->arraySize);
	    } else {
	      for(unsigned short i=0;i!=pv->arraySize;++i){
		out << "this->" << pv->name << "[" << i << "] = ZMATextvars_tdt[ZMATevs_pos[" << o << "]]-ZMATextvars_t[ZMATevs_pos[" << o << "]];" << endl;
		o+=this->getTypeSize(pv->type,1u);
	      }
	    }
	  }
	} else {
	  string msg("MFrontZMATInterface::writeBehaviourDataConstructor : "
		     "unsupported material properties type (only scalar supported yet)");
	  throw(runtime_error(msg));
	}
      }
    }
    out << "this->dT = ZMATextvars_tdt[ZMATT_pos]-ZMATextvars_t[ZMATT_pos];" << endl;
    out << "}" << endl << endl;
  }

  void 
  MFrontZMATInterface::writeBehaviourDataMainVariablesSetters(std::ofstream&,
							      const MechanicalBehaviourDescription&) const
  {
    //handled by the constructors
  }

  void 
  MFrontZMATInterface::writeIntegrationDataMainVariablesSetters(std::ofstream&,
								const MechanicalBehaviourDescription&) const
  {
    //handled by the constructors
  }

  void
  MFrontZMATInterface::endTreatement(const MechanicalBehaviourDescription& mb,
				     const MFrontFileDescription& fd) const
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/ZMAT");
    const Hypothesis hypotheses[3u] = {ModellingHypothesis::TRIDIMENSIONAL,
				       ModellingHypothesis::GENERALISEDPLANESTRAIN,
				       ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN};
    const string name = mb.getLibrary()+mb.getClassName();
    const string headerFileName("ZMAT"+name+".hxx");
    const string srcFileName("ZMAT"+name+".cxx");
    const std::set<std::string>& all_mp_names = getAllMaterialPropertiesNames(mb);
    const unsigned short nbh =
      (mb.isModellingHypothesisSupported(ModellingHypothesis::TRIDIMENSIONAL) ? 1 : 0) +
      (mb.isModellingHypothesisSupported(ModellingHypothesis::GENERALISEDPLANESTRAIN) ? 1 : 0) +
      (mb.isModellingHypothesisSupported(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ? 1 : 0);
    if(nbh==0){
      string msg("MFrontZMATInterface::endTreatement : "
		 "no suitable modelling hypothesis supported");
      throw(runtime_error(msg));
    }
    ofstream out(("include/MFront/ZMAT/"+headerFileName).c_str());
    if(!out){
      string msg("MFrontUMATInterface::endTreatement : "
		 "could not open header file '"+
		 headerFileName+"'");
      throw(runtime_error(msg));
    }
    out << "/*!" << endl;
    out << "* \\file   "  << headerFileName << endl;
    out << "* \\brief  This file declares the ZMAT interface for the " 
	<< mb.getClassName() << " behaviour law" << endl;
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/" << endl << endl;
    out << "#ifndef __MFRONT_ZMAT_" << makeUpperCase(mb.getClassName()) << "_HXX" << endl;
    out << "#define __MFRONT_ZMAT_" << makeUpperCase(mb.getClassName()) << "_HXX" << endl << endl;
    out << "#include\"Behavior.h\"" << endl;
    writeZMATUndefs(out);
    out << endl;
    out << "Z_START_NAMESPACE;" << endl;
    out << endl;
    out << "/*!" << endl
	<< "* ZMAT Interface to the " << mb.getClassName() << " behaviour"
	<< "*/" << endl;
    out << "struct ZMAT" << mb.getClassName() << endl;
    out << ": public BEHAVIOR" << endl;
    out << "{" << endl;
    out << "//! constructor;" << endl;
    out << "ZMAT" << mb.getClassName() << "();" << endl;
    out << "/*!" << endl;
    out << " * \\brief integrate the beahviour over a time step" << endl;
    out << " * \\param[in,out] file  : input file " << endl;
    out << " * \\param[in]     dim   : dimension (used by the BEHAVIOUR class)" << endl;
    out << " * \\param[in]     integ : pointer to the local integration" << endl;
    out << " */" << endl;
    out << "virtual void initialize(ASCII_FILE&,int,LOCAL_INTEGRATION*);" << endl;
    out << "/*!" << endl;
    out << " * \\brief integrate the beahviour over a time step" << endl;
    out << " * \\param[in,out] mat        : material properties" << endl;
    out << " * \\param[out]    delta_grad : increment of driving variables" << endl;
    out << " * \\param[out]    tg_matrix  : tangent operator" << endl;
    out << " * \\param[int]    flags      : integration option" << endl;
    out << " * \\return NULL on success" << endl;
    out << " */" << endl;
    out << "INTEGRATION_RESULT*" << endl;
    out << "integrate(MAT_DATA&,const VECTOR&,MATRIX*&,int);" << endl;
    out << "//! destructor" << endl;
    out << "virtual ~ZMAT" << mb.getClassName() << "();" << endl;
    out << endl;
    out << "protected:" << endl;
    out << endl;
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	out << "/*!" << endl;
	out << " * \\brief initialize material properties" << endl;
	out << " * \\param[in,out] file    : input file " << endl;
	out << " */" << endl;
	out << "virtual void initializeMaterialProperties" << getSpaceDimensionSuffix(*ph)
	    << "(ASCII_FILE&);" << endl;
      }
    }
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	out << "/*!" << endl;
	out << " * \\brief call the mfront beahviour in " << ModellingHypothesis::toString(*ph) << endl;
	out << " * \\param[in,out] mat     : material properties" << endl;
	out << " * \\param[out] delta_grad : increment of driving variables" << endl;
	out << " * \\param[out] tg_matrix  : tangent operator" << endl;
	out << " * \\param[int] flags      : integration option" << endl;
	out << " * \\return NULL on success" << endl;
	out << " */" << endl;
	out << "INTEGRATION_RESULT*" << endl;
	out << "callMFrontBehaviour" << getSpaceDimensionSuffix(*ph) 
	    <<"(MAT_DATA&,const VECTOR&,MATRIX*&,int);" << endl;
      }
    }
    out << "//! stresses" << endl;
    out << "TENSOR2_FLUX sig;" << endl;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "//! strains" << endl;
      out << "TENSOR2_GRAD eto;" << endl;
    } else {
      string msg("MFrontZMATInterface::endTreatement : "
		 "only small strain behaviour are supported");
      throw(runtime_error(msg));
    }
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "//! tangent operator" << endl;
      out << "MATRIX tg_mat; " << endl;
    }
    out << "//! material properties" << endl;
    out << "ARRAY<COEFF> mprops; " << endl;
    const VariableDescriptionContainer& svs = getAllStateVariables(mb);
    if(!svs.empty()){
      for(VariableDescriptionContainer::const_iterator pv=svs.begin();pv!=svs.end();++pv){
	out << "//! '" << pv->name << "' state variable" << endl;
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(pv->type);
	if(flag==SupportedTypes::Scalar){
	  if(pv->arraySize==1u){
	    out << "SCALAR_VINT " << pv->name << ";" << endl; 
	  } else {
	    out << "VECTOR_VINT " << pv->name << ";" << endl; 
	  }
	} else if ((flag==SupportedTypes::Stensor)||
		   (flag==SupportedTypes::Tensor)){
	  if(pv->arraySize==1u){
	    out << "TENSOR2_VINT " << pv->name << ";" << endl; 
	  } else {
	    out << "ARRAY<TENSOR2_VINT> " << pv->name << ";" << endl; 
	  }
	} else {
	  string msg("MFrontZMATInterface::endTreatement : "
		     "unsupported state variable type");
	  throw(runtime_error(msg));
	}
      }
    }
    out << "//! external state variables" << endl;
    out << "LIST<EXTERNAL_PARAM*>* zbb_keep_ep;" << endl;
    out << "//! external state variables positions" << endl;
    out << "ARRAY<int> evs_positions;" << endl;
    out << "//! position de la température" << endl;
    out << "int temperature_position;" << endl;
    out << "//! local clock" << endl;
    out << "CLOCK  local_clock; " << endl;
    out << "};" << endl << endl;
    out << "Z_END_NAMESPACE;" << endl << endl;
    out << "#endif /* __MFRONT_ZMAT_" << makeUpperCase(mb.getClassName()) << "_HXX*/" << endl;
    out.close();
    out.open(("src/"+srcFileName).c_str());
    if(!out){
      string msg("MFrontUMATInterface::endTreatement : "
		 "could not open src file '"+
		 srcFileName+"'");
      throw(runtime_error(msg));
    }
    // writing source file
    out << "/*!" << endl;
    out << "* \\file   "  << srcFileName << endl;
    out << "* \\brief  This file declares the ZMAT interface for the " 
	<< mb.getClassName() << " behaviour law" << endl;
    out << "* \\author "  << fd.authorName << endl;
    out << "* \\date   "  << fd.date       << endl;
    out << "*/" << endl << endl;
    out << endl;
    out << "#include<string>" << endl;
    out << "#include<vector>" << endl;
    out << "#include<algorithm>" << endl;
    out << endl;
    out << "#include\"External_parameter.h\"" << endl;
    out << "#include\"Verbose.h\"" << endl;
    writeZMATUndefs(out);
    out << endl;
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"" << endl;
    out << "#include\"MFront/ZMAT/" << headerFileName << "\"" << endl;
    out << endl;
    out << "Z_START_NAMESPACE;" << endl;
    out << endl;
    out << "ZMAT" << mb.getClassName() << "::ZMAT" << mb.getClassName() << "()" << endl;
    out << "{} // end of ZMAT" << mb.getClassName() << "::ZMAT" << mb.getClassName() << "()" << endl;     
    out << endl;
    out << "void" << endl;
    out << "ZMAT" << mb.getClassName()
	<< "::initialize(ASCII_FILE& file,int dim,LOCAL_INTEGRATION* integ){" << endl;
    out << "BEHAVIOR::initialize(file, dim,integ);" << endl;
    out << "using namespace std;" << endl;
    out << "int keep_verbose = ZSET::Verbose;" << endl;
    out << "this->coord.resize(dim);" << endl;
    out << "this->sig.initialize(this,\"sig\",this->tsz(),1);" << endl;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "this->eto.initialize(this,\"eto\",this->tsz(),1);" << endl;
    }
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "this->tg_mat.resize(this->tsz(), this->tsz());" << endl;
    }
    out << "// initialisation dispatch" << endl;
    out << "if(this->tsz()==6){" << endl;
    this->writeBehaviourInitialisation(out,mb,ModellingHypothesis::TRIDIMENSIONAL);
    out << "} else if(this->tsz()==4){" << endl;
    this->writeBehaviourInitialisation(out,mb,ModellingHypothesis::GENERALISEDPLANESTRAIN);
    out << "} else if(this->tsz()==3){" << endl;
    this->writeBehaviourInitialisation(out,mb,ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    out << "} else {" << endl;
    out << "ERROR(\"Invalid tensor size\");" << endl;
    out << "}" << endl;
    out << "this->temperature_position = EXTERNAL_PARAM::rank_of_nodal_ip(\"temperature\");" << endl;
    if(!all_mp_names.empty()){
      out << "// check that all material properties were initialised" << endl;
      out << "for(int pc=0;pc!=this->mprops.size();++pc){" << endl
	  << "if(!this->mprops[pc].ok()){" << endl
	  << "ERROR(\"Some material properties were not initialised\");" << endl
	  << "}" << endl
	  << "}" << endl;
    }
    out << "ZSET::Verbose = keep_verbose;" << endl;
    out << "} // end of ZMAT" << mb.getClassName() << "::initialize" << endl << endl;
    if(!all_mp_names.empty()){
      for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
	const Hypothesis h = *ph;
	if(mb.isModellingHypothesisSupported(h)){
	  this->writeMaterialPropertiesInitialisation(out,mb,*ph);
	}
      }
    }
    out << "INTEGRATION_RESULT*" << endl;
    out << "ZMAT" << mb.getClassName()
	<< "::integrate(MAT_DATA& mdat," << endl
	<< "const VECTOR& delta_grad," << endl
	<< "MATRIX*& tg_matrix," << endl
	<< "int flags){" << endl
	<< "int keep_verbose  = ZSET::Verbose; " << endl
	<< "CLOCK* keep_clock = ZSET::stored_thread_zbase_globals->ptr()->active_clock; " << endl
	<< "this->set_var_aux_to_var_aux_ini();" << endl
	<< "this->set_var_int_to_var_int_ini();" << endl
	<< "LIST<EXTERNAL_PARAM*>* ep_save = &EXTERNAL_PARAM::Get_EP_list();" << endl
	<< "EXTERNAL_PARAM::set_EP_list(zbb_keep_ep);" << endl
	// << "if(!this->curr_ext_param){" << endl
	// << "this->curr_ext_param = *mdat.param_set();" << endl
	// << "}" << endl
	<< "this->calc_local_coefs();" << endl
	<< "INTEGRATION_RESULT* r = NULL;" << endl
	<< "try{" << endl;
    out << "if(this->tsz()==6){" << endl;
    if(!mb.isModellingHypothesisSupported(ModellingHypothesis::TRIDIMENSIONAL)){
      out << "ERROR(\"Computations using the 'Tridimensional'\" " << endl
	  << "\"modelling hypothesis are not supported by the \"" << endl 
	  << "\"'" << mb.getClassName() << "' behaviour\");" << endl;
    } else {
      out << "r=this->callMFrontBehaviour3D"
	  << "(mdat,delta_grad,tg_matrix,flags);" << endl;
    }
    out << "} else if(this->tsz()==4){" << endl;
    if(!mb.isModellingHypothesisSupported(ModellingHypothesis::GENERALISEDPLANESTRAIN)){
      out << "ERROR(\"Computations using the 'GeneralisedPlaneStrain'\" " << endl
	  << "\"modelling hypothesis are not supported by the \"" << endl 
	  << "\"'" << mb.getClassName() << "' behaviour\");" << endl;
    } else {
      out << "r=this->callMFrontBehaviour2D"
	  << "(mdat,delta_grad,tg_matrix,flags);" << endl;
    }
    out << "} else if(this->tsz()==3){" << endl;
    if(!mb.isModellingHypothesisSupported(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)){
      out << "ERROR(\"Computations using the 'AxisymmetricalGeneralisedPlaneStrain'\" " << endl
	  << "\"modelling hypothesis are not supported by the \"" << endl 
	  << "\"'" << mb.getClassName() << "' behaviour\");" << endl;
    } else {
      out << "r=this->callMFrontBehaviour1D"
	  << "(mdat,delta_grad,tg_matrix,flags);" << endl;
    }
    out << "} else {" << endl;
    out << "ERROR(\"Invalid tensor size\");" << endl;
    out << "}" << endl;
    out << "} catch(...){" << endl
	<< "static INTEGRATION_RESULT bad_result;" << endl
	<< "bad_result.set_error(INTEGRATION_RESULT::UNDEFINED_BEHAVIOR);" << endl
	<< "ZSET::Verbose = keep_verbose; " << endl
	<< "ZSET::stored_thread_zbase_globals->ptr()->active_clock = keep_clock; " << endl
	<< "return &bad_result;" << endl
	<< "}" << endl
	<< "if(r!=NULL){" << endl
	<< "ZSET::Verbose = keep_verbose; " << endl
	<< "ZSET::stored_thread_zbase_globals->ptr()->active_clock = keep_clock; " << endl
	<< "return r;" << endl
    	<< "}" << endl
	<< "this->update_var_aux();" << endl
	<< "this->zbb_keep_ep = &EXTERNAL_PARAM::Get_EP_list();" << endl
	<< "EXTERNAL_PARAM::set_EP_list(ep_save);" << endl
	<< "ZSET::Verbose = keep_verbose;" << endl
	<< "ZSET::stored_thread_zbase_globals->ptr()->active_clock = keep_clock;" << endl
	<< "return NULL;" << endl;
    out << "} // end of ZMAT" << mb.getClassName() << "::integrate" << endl << endl;
    for(const Hypothesis* ph = hypotheses;ph!=hypotheses+3u;++ph){
      const Hypothesis h = *ph;
      if(mb.isModellingHypothesisSupported(h)){
	this->writeCallMFrontBehaviour(out,mb,*ph);
      }
    }
    out << "Z_END_NAMESPACE;" << endl;
  } // end of MFrontZMATInterface::endTreatement

  // ZEBU_PATH
  // libZMATBase.so
  // 

  void
  MFrontZMATInterface::writeBehaviourInitialisation(std::ostream& out,
						    const MechanicalBehaviourDescription& mb,
						    const MFrontZMATInterface::Hypothesis h) const
  {
    using namespace std;
    if(!mb.isModellingHypothesisSupported(h)){
      out << "ERROR(\"Computations using the " << ModellingHypothesis::toString(h) << "\" " << endl
	  << "\"modelling hypothesis are not supported by the \"" << endl 
	  << "\"'" << mb.getClassName() << "' behaviour\");" << endl;
      return;
    }
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& mps  = d.getMaterialProperties();
    const VariableDescriptionContainer& isvs = d.getPersistentVariables();
    const VariableDescriptionContainer& esvs = d.getExternalStateVariables();
    const unsigned short dime = getSpaceDimension(h);
    const int mps_size  = this->getTotalSize(mps).getValueForDimension(dime);
    if(!mps.empty()){
      out << "this->mprops.resize(" << mps_size << ");" << endl; 
    }
    if(!isvs.empty()){
      for(VariableDescriptionContainer::const_iterator pv=isvs.begin();pv!=isvs.end();++pv){
	const SupportedTypes::TypeFlag flag = this->getTypeFlag(pv->type);
	if(flag==SupportedTypes::Scalar){
	  out << "this->" << pv->name << ".initialize(this,\"" << pv->name 
	      << "\"," << pv->arraySize << ",1);" << endl; 
	} else if (flag==SupportedTypes::Stensor){
	  if(pv->arraySize==1u){
	    out << "this->" << pv->name << ".initialize(this,\"" << pv->name 
		<< "\",this->tsz(),1);" << endl; 
	  } else {
	    out << "this->" << pv->name << ".initialize(this,\"" << pv->name 
		<< "\"," << pv->arraySize << "*(this->tsz()),1);" << endl; 
	  }
	} else if (flag==SupportedTypes::Tensor){
	  if(pv->arraySize==1u){
	    out << "this->" << pv->name << ".initialize(this,\"" << pv->name 
		<< "\",this->utsz(),1);" << endl; 
	  } else {
	    out << "this->" << pv->name << ".initialize(this,\"" << pv->name 
		<< "\"," << pv->arraySize << "*(this->uts()),1);" << endl; 
	  }
	} else {
	  string msg("MFrontZMATInterface::endTreatement : "
		     "unsupported state variable type");
	  throw(runtime_error(msg));
	}
      }
    }
    out << "for(;;){" << endl;
    out << "STRING str=file.getSTRING();" << endl;
    out << "if(this->base_read(str,file)){" << endl;
    const set<string>& all_mp_names = getAllMaterialPropertiesNames(mb);
    if(!all_mp_names.empty()){
      out << "} else if((str==\"**model_coef\")||(str==\"**material_properties\")){" << endl;
      out << "this->initializeMaterialProperties" << getSpaceDimensionSuffix(h) << "(file);" << endl;
    }
    out << "} else if (str.start_with(\"***\")){" << endl
	<< "file.back();" << endl
	<< "break;" << endl
	<< "} else {" << endl
	<< "INPUT_ERROR(\"Invalid keyword '\"+str+\"'\");" << endl
	<< "}" << endl << "}" <<endl;
    if(!esvs.empty()){
      const int ext_size = this->getTotalSize(esvs).getValueForDimension(dime);
      out << "evs_positions.resize(" << ext_size << ");"<< endl;
      VariableDescriptionContainer::const_iterator pev;
      int i=0;
      for(pev=esvs.begin();pev!=esvs.end();++pev){
	const VariableDescription& v = *pev;
	const string& name = d.getGlossaryName(v.name);
	if(v.arraySize==1u){
	  out << "this->evs_positions[" << i << "] = " 
	      << "EXTERNAL_PARAM::rank_of_nodal_ip(\"" << name << "\");" << endl;
	  ++i;
	} else {
	  for(unsigned short n=0;n!=v.arraySize;++n,++i){
	    out << "this->evs_positions[" << i << "] = " 
		<< "EXTERNAL_PARAM::rank_of_nodal_ip(\"" << name
		<< "[" << n << "]\");" << endl;
	  }
	}
      }
    }
  }

  void
  MFrontZMATInterface::writeMaterialPropertiesInitialisation(std::ostream& out,
							     const MechanicalBehaviourDescription& mb,
							     const MFrontZMATInterface::Hypothesis h) const
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& mps  = d.getMaterialProperties();
    const set<string>& all_mp_names = getAllMaterialPropertiesNames(mb);
    const vector<string> mpnames = d.getGlossaryNames(mps);
    const unsigned short nbh =
      (mb.isModellingHypothesisSupported(ModellingHypothesis::TRIDIMENSIONAL) ? 1 : 0) +
      (mb.isModellingHypothesisSupported(ModellingHypothesis::GENERALISEDPLANESTRAIN) ? 1 : 0) +
      (mb.isModellingHypothesisSupported(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ? 1 : 0);
    const unsigned short dime = getSpaceDimension(h);
    const int mps_size  = this->getTotalSize(mps).getValueForDimension(dime);
    out << "void" << endl;
    out << "ZMAT" << mb.getClassName()
	<< "::initializeMaterialProperties" << getSpaceDimensionSuffix(h) << "(ASCII_FILE& file){" << endl
	<< "using std::find;" << endl;
    if(nbh!=1){
      if(!all_mp_names.empty()){
	out << "const STRING all_mp_names[" << all_mp_names.size() << "] = ";
	writeArray(out,all_mp_names);
      }
    }
    if(!mpnames.empty()){
      out << "const STRING mp_names" << getSpaceDimensionSuffix(h)
	  << "[" << mpnames.size() << "] = ";
      writeArray(out,mpnames);
    }
    out << "for(;;){" << endl
	<< "STRING str=file.getSTRING();" << endl
	<< "if(str[0]=='*'){" << endl
	<< "file.back();" << endl
	<< "break;" << endl
	<< "}" << endl;
    if(nbh!=1u){
      out << "if(find(all_mp_names,all_mp_names+" << all_mp_names.size() << ",str)==all_mp_names+"
	  << all_mp_names.size() << "){" << endl;
      out << "INPUT_ERROR(\"No material property named '\"+str+\"'\");" << endl;
      out << "}" << endl;
    }
    out << "const STRING * const pmat = find(mp_names"<<getSpaceDimensionSuffix(h)
	<<",mp_names" << getSpaceDimensionSuffix(h) << "+" << mps_size << ",str);" << endl;
    if(nbh==1u){
      out << "if(pmat==mp_names" << getSpaceDimensionSuffix(h) << "+" <<
	mps_size << "){" << endl;
      out << "INPUT_ERROR(\"No material property named '\"+str+\"'\");" << endl;
      out << "}" << endl;
    }
    out << "if(this->mprops[pmat-mp_names" << getSpaceDimensionSuffix(h) << "].ok()){" << endl;
    out << "INPUT_ERROR(\"material property '\"+str+\"' already defined\");" << endl;
    out << "}" << endl;
    out << "this->mprops[pmat-mp_names" << getSpaceDimensionSuffix(h) << "].read(str,file,this);" << endl;
    out << "}" << endl;
    out << "} // end of ZMAT" << mb.getClassName() 
	<< "::initializeMaterialProperties" << getSpaceDimensionSuffix(h) << endl << endl;
  }

  void
  MFrontZMATInterface::writeCallMFrontBehaviour(std::ostream& out,
						const MechanicalBehaviourDescription& mb,
						const MFrontZMATInterface::Hypothesis h) const
  {
    using namespace std;
    out << "INTEGRATION_RESULT*" << endl;
    out << "ZMAT" << mb.getClassName()
	<< "::callMFrontBehaviour" << getSpaceDimensionSuffix(h) << "(MAT_DATA& mdat," << endl
	<< "const VECTOR& delta_grad," << endl
	<< "MATRIX*& tg_matrix," << endl
	<< "int flags){" << endl
	<< "typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase;" << endl
	<< "typedef tfel::material::ModellingHypothesis ModellingHypothesis;" << endl
	<< "typedef tfel::material::" << mb.getClassName()
	<< "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",double,false> "
	<< mb.getClassName() << ";" << endl;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "// strain and strain increment" << endl;
      out << "double stran[" << getStensorSize(h) << "];" << endl;
      out << "double dstran[" << getStensorSize(h) << "];" << endl;
      out << "stran[0] = this->eto[0]-delta_grad[0];" << endl;
      out << "stran[1] = this->eto[1]-delta_grad[1];" << endl;
      out << "stran[2] = this->eto[2]-delta_grad[2];" << endl;
      if((h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
	 (h==ModellingHypothesis::TRIDIMENSIONAL)){
	out << "stran[3] = this->eto[3]-delta_grad[3];" << endl;
	if(h==ModellingHypothesis::TRIDIMENSIONAL){
	  out << "stran[4] = this->eto[4]-delta_grad[4];" << endl;
	  out << "stran[5] = this->eto[5]-delta_grad[5];" << endl;
	}
      }
      out << "dstran[0] = delta_grad[0];" << endl;
      out << "dstran[1] = delta_grad[1];" << endl;
      out << "dstran[2] = delta_grad[2];" << endl;
      if((h==ModellingHypothesis::GENERALISEDPLANESTRAIN)||
	 (h==ModellingHypothesis::TRIDIMENSIONAL)){
	out << "dstran[3] = delta_grad[3];" << endl;
	if(h==ModellingHypothesis::TRIDIMENSIONAL){
	  out << "dstran[4] = delta_grad[4];" << endl;
	  out << "dstran[5] = delta_grad[5];" << endl;
	}
      }
    } else {
      string msg("MFrontZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << "// stiffness matrix type" << endl
	<< "MechanicalBehaviourBase::SMType smtype = MechanicalBehaviourBase::NOSTIFFNESSREQUESTED;" << endl
	<< "if(flags&CALC_TG_MATRIX){" << endl
	<< "smtype = MechanicalBehaviourBase::CONSISTENTTANGENTOPERATOR;" << endl
	<< "}" << endl;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << mb.getClassName() << " b(this->sig,stran,dstran,this->mprops,mdat,this->temperature_position," << endl
	<< "this->evs_positions,ZSET::stored_thread_zbase_globals->ptr()->active_clock->get_dtime());" << endl;
    } else {
      string msg("MFrontZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << "if(b.integrate(smtype)!=" << mb.getClassName() << "::SUCCESS){" << endl
	<< "static INTEGRATION_RESULT bad_result;" << endl
	<< "bad_result.set_error(INTEGRATION_RESULT::UNDEFINED_BEHAVIOR);" << endl
	<< "return &bad_result;" << endl
	<< "}" << endl
	<< "b.ZMATexportStateData(this->sig,mdat);" << endl
	<< "if(smtype!=MechanicalBehaviourBase::NOSTIFFNESSREQUESTED){" << endl;
    if(mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      out << "zmat::ZMATInterface::convert(*tg_matrix,b.getTangentOperator());"<< endl;
    } else {
      string msg("MFrontZMATInterface::writeCallMFrontBehaviour : "
		 "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << "}" << endl
	<< "return NULL;" << endl
	<< "} // end of ZMAT" << mb.getClassName()
	<< "::callMFrontBehaviour" << getSpaceDimensionSuffix(h) << endl << endl;
  }

  std::map<std::string,std::vector<std::string> >
  MFrontZMATInterface::getGlobalIncludes(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > incs;
    incs[getLibraryName(mb)].push_back("`tfel-config --includes --zmat`");
#warning "HERE"
    incs[getLibraryName(mb)].push_back("-DLinux");
    return incs;
  } // end of MFrontZMATInterface::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontZMATInterface::getGlobalDependencies(const MechanicalBehaviourDescription&) const
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontZMATInterface::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontZMATInterface::getGeneratedSources(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    map<string,vector<string> > src;
    src[getLibraryName(mb)].push_back("ZMAT"+mb.getLibrary()+mb.getClassName()+".cxx");
    return src;
  } // end of MFrontZMATInterface::getGeneratedSources

  std::vector<std::string>
  MFrontZMATInterface::getGeneratedIncludes(const MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    vector<string> incs;
    incs.push_back("MFront/ZMAT/ZMAT"+mb.getLibrary()+mb.getClassName()+".hxx");
    return incs;
  } // end of MFrontZMATInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontZMATInterface::getLibrariesDependencies(const MechanicalBehaviourDescription&) const
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of MFrontZMATInterface::getLibrariesDependencies

  MFrontZMATInterface::~MFrontZMATInterface()
  {}

} // end of namespace mfront
