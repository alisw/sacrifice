#include "Sacrifice/MCUtilsHandler.hh"

#include "MCUtils/HepMCUtils.h"

#include "HepMC/IO_GenEvent.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <fstream>

namespace Sacrifice{
  
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
      
      foreach(int status, statuses->second){
        if(status == p->status()) return false;
      }
    }
    
    statuses = m_pids.find(0);
    if(statuses == m_pids.end() || statuses->second.size() == 0) return true;
    
    foreach(int status, statuses->second){
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
    
    if(m_keepList.getValue() != ""){
      std::ifstream file(m_keepList.getValue().c_str());
      
      if(file.is_open()){
        while(file.good()){
          string line;
          getline(file, line);
          
          vector<string> comments;
          boost::split(comments, line, boost::is_any_of("#!c"));
          
          if(comments.size() == 0) continue;
          
          vector<string> ids;
          boost::split(ids, comments[0], boost::is_any_of(":"));
        
          if(ids.size()==0) continue;
          
          boost::erase_all(ids[0], " ");
          
          int pid = (ids[0] == "")? 0: boost::lexical_cast<int>(ids[0]);
                    
          vector<int> statuses;
          
          if(ids.size() != 1){
            
            vector<string> statusStrings;
            
            boost::split(statusStrings, ids[1], boost::is_any_of(" ")); 
            
            foreach(string status, statusStrings){
              if(status != ""){
                statuses.push_back(boost::lexical_cast<int>(status));
              }
            }
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

