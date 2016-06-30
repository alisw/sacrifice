#include "Sacrifice/BeamParticle.hh"
#include <iostream>

namespace Sacrifice{
  
  bool BeamParticle::s_init = true;
  
  std::map<std::string, BeamParticle::PDGID> &BeamParticle::s_particleIDs(){
    
    static std::map<std::string, PDGID> particleIDs;
    
    if(s_init){
      
      particleIDs["PROTON"] = PROTON;
      particleIDs["ANTIPROTON"] = ANTIPROTON;
      particleIDs["ELECTRON"]   = ELECTRON;
      particleIDs["POSITRON"]   = POSITRON;
      s_init = false;
    }
    return particleIDs;
  }
}
