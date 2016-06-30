#ifndef SACRIFICE_BEAMPARTICLE_HH
#define SACRIFICE_BEAMPARTICLE_HH

#include <string>
#include <map>

namespace Sacrifice{

  class BeamParticle{
    
  public:
    
    enum PDGID {PROTON=2212, ANTIPROTON=-2212, ELECTRON=11, POSITRON=-11, INVALID=0};
    
    BeamParticle(const std::string &name){
      
      std::map<std::string, PDGID>::const_iterator it = s_particleIDs().find(name);
      if(it == s_particleIDs().end()){
        m_id = INVALID;
        m_name = "INVALID";
      }else{
        m_id = it->second;
        m_name = name;
      }
    }
    
    PDGID asID()const{
      return m_id;
    }
    
    const std::string &asString()const{
      return m_name;
    }
    
  private:
    
    static std::map<std::string, PDGID> &s_particleIDs();
    static bool s_init;
    
    PDGID m_id;
    std::string m_name;
    
  };
}

#endif
