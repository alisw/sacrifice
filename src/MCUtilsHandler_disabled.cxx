#include "Sacrifice/MCUtilsHandler.hh"
#include <stdexcept>

namespace Sacrifice{
 
  bool MCUtilsHandler::Selector::operator()(const HepMC::GenParticle *p){
    
    throw std::runtime_error("MCUtils support was not compiled.  HepMC filtering is not available.");
    return false;
  }
  
  MCUtilsHandler::MCUtilsHandler(TCLAP::CmdLine &cmd):
  m_slimArg("s", "slim-HepMC", "slim the HepMC record to remove gen-specific and non-interesting particles (default=true)", true),
  m_keepList("k", "keep-particles", "give a txt file specifying list of pdgids and statuses to keep.  All other particles removed", false, "", "string"){
    
  }
  
  bool MCUtilsHandler::isAvailable(){return false;}
  
  void MCUtilsHandler::initialise(){
    
    throw std::runtime_error("MCUtils support was not compiled.  HepMC filtering is not available.");
    return;
  }
  
  void MCUtilsHandler::filter(HepMC::GenEvent *event){
    
    throw std::runtime_error("MCUtils support was not compiled.  HepMC filtering is not available.");
    return;
  }
  
}
