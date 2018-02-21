#include "Sacrifice/MCUtilsHandler.hh"

#include "MCUtils/HepMCUtils.h"

#include "HepMC/IO_GenEvent.h"

#include <fstream>
#include <regex>
#include <stdexcept>

namespace Sacrifice{
  
  using std::regex;
  using std::regex_search;
  using std::regex_match;
  
  inline bool _isPID0(const HepMC::GenParticle* p) {
    return p->pdg_id() == 0;
  }
  
  // Identify non-transportable stuff _after_ hadronisation
  inline bool _isNonTransportableInDecayChain(const HepMC::GenParticle* p) {
    return !MCUtils::isTransportable(p) && MCUtils::fromDecay(p);
  }
  
  // Identify unstable hadrons from decay chains not containing heavy flavour or taus
  // Partons with status 2 are also kept, but only because PYTHIA6 *wrongly* sets shower partons to have status == 2
  inline bool _inBoringDecayChain(const HepMC::GenParticle* p) {
    return MCUtils::isDecayed(p) && !(MCUtils::isParton(p) || MCUtils::fromTauOrHFDecay(p));
  }
  
  /// Identify internal "loop" particles
  inline bool _isLoop(const HepMC::GenParticle* p) {
    return p->production_vertex() == p->end_vertex() && p->end_vertex() != NULL;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  
  bool MCUtilsHandler::Selector::operator()(const HepMC::GenParticle *p){
        
    if(p->is_beam()) return false;
    
    map<int, vector<int> >::const_iterator statuses = m_pids.find(abs(p->pdg_id()));
    if(statuses != m_pids.end()){
      
      if(statuses->second.size() == 0) return false;
      
      for(int status: statuses->second){
        if(status == p->status()) return false;
      }
    }
    
    statuses = m_pids.find(0);
    if(statuses == m_pids.end() || statuses->second.size() == 0) return true;
    
    for(int status: statuses->second){
      if(status == p->status()) return false;
    }
    
    return true;
  }

  //////////////////////////////////////////////////////////////////////////////
  
  MCUtilsHandler::MCUtilsHandler(TCLAP::CmdLine &cmd):
  m_doInitialise(true),
  m_slimArg("s", "slim-HepMC", "slim the HepMC record to remove gen-specific and non-interesting particles (default=false)", false),
  m_keepList("k", "keep-particles", "give a txt file specifying list of pdgids and statuses to keep.  All other particles removed", false, "", "string"){

    cmd.add(m_slimArg);
    cmd.add(m_keepList);
  }
  
  bool MCUtilsHandler::isAvailable(){return true;}
  
  void MCUtilsHandler::initialise(){
    
    if(m_slimArg.getValue()){
      std::cout<<std::endl<<"  ===  Applying default HepMC filtering  ==="<<std::endl<<std::endl;
      m_classifiers.push_back(MCUtils::isGenSpecific);
      m_classifiers.push_back(MCUtils::hasNonStandardStatus);
      m_classifiers.push_back(_isPID0);
      m_classifiers.push_back(_isNonTransportableInDecayChain);
      m_classifiers.push_back(_inBoringDecayChain);
      m_classifiers.push_back(_isLoop);
    }
    
    regex removeComment("[^\\[c!#\\]]*");
    regex matchCommand(" *[+-]?\\d* *: *([+-]?\\d+ *)+");
    regex findPID(" *[+-]?(\\d+)? *:");
    regex findInt("[+-]?\\d+[^ :]");
    
    if(m_keepList.getValue() != ""){
      std::ifstream file(m_keepList.getValue().c_str());
      
      if(file.is_open()){
        while(file.good()){
          string line;
          getline(file, line);
          
          std::smatch result;
          
          // Search for string up to comment command [c!#]
          regex_search(line, result, removeComment);
          
          string command = result.str();
          
          if(command == "") continue;
          
          // Check command matches form ID:status
          if(!regex_match(command, matchCommand)){
            throw std::runtime_error("Particle filtering command is not understood: " + command);
          }
          
          regex_search(command, result, findPID);
          command = result.suffix().str();
          regex_search(result.str(), result, findInt);
          int pid = (result.str() == "")? 0: std::stoi(result.str());
          
          vector<int> statuses;
          
          while(regex_search(command, result, findInt)){
            statuses.push_back(std::stoi(result.str()));
            command = result.suffix().str();
          }
          
          m_selector.addParticle(abs(pid), statuses);
          
        }
      }else{
        throw std::runtime_error("Unable to open file: " + m_keepList.getValue());
      }
     
      m_classifiers.push_back(m_selector);
    }
    
    m_doInitialise = false;
    return;
  }
  
  void MCUtilsHandler::filter(HepMC::GenEvent *event){
   
    if(m_doInitialise) initialise();
    
    const std::vector<HepMC::GenParticle*> toRemove 
    = MCUtils::particles_match_any(event, m_classifiers);
    
    MCUtils::reduce(event, toRemove);
    
    return;
  }
  
}

