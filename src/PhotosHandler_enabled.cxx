#include "Sacrifice/PhotosHandler.hh"

#include "Photos/Photos.h"
#include "Photos/PhotosHepMCEvent.h"
#include "Photos/Log.h"

#include <iostream>
#include <stdexcept>

#ifdef PHOTOS_HAS_NAMESPACE
using namespace Photospp;
#endif

namespace Sacrifice{
 
  bool PhotosHandler::isAvailable()const{
    return true; 
  }

  void PhotosHandler::process(HepMC::GenEvent *event)const{
    
    if(!m_initialised) throw std::runtime_error("Tried to process Photos event without proper initialisation");
    
    PhotosHepMCEvent photosEvent(event);
    photosEvent.process();
    
    return;
  }
  
  void PhotosHandler::initialise(){
   
    Photos::initialize();
    Photos::setAlphaQED(m_alphaQEDArg.getValue());
    Photos::setInterference(true);
    Photos::setCorrectionWtForW(true);
    Photos::maxWtInterference(3.);
    Photos::forceMassFrom4Vector(true);
    
    // since we explictly request events in GeV
    double MeV = 1.e-03;
    
    Photos::forceMassFromEventRecord(13);
    Photos::forceMassFromEventRecord(15);
    Photos::forceMass(11, 0.510998910 * MeV);
    Photos::forceMassFromEventRecord(211);
    Photos::setTopProcessRadiation(false);
    
#ifdef PHOTOS_HAS_NAMESPACE
    Photos::createHistoryEntries(true, 3);
    Photospp::Log::LogWarning(false);
#else
    Photos::createHistoryEntries(true);
#endif
    
    if(m_exponentiationSwitch.getValue()){
      std::cout<<"Using Photos exponentiation mode"<<std::endl;
      Photos::setExponentiation(true);
    }else{
      std::cout<<"Not using Photos exponentiation mode"<<std::endl;
      Photos::setInfraredCutOff(0.01);
      Photos::setDoubleBrem(true);
      Photos::setQuatroBrem(false);
      phokey_.iexp = 0;
    }
    
    if(m_irCutArg.getValue() > 0.){
      std::cout<<"Setting PHOTOS IR cutoff to "<<m_irCutArg.getValue()<<std::endl;
      Photos::setInfraredCutOff(m_irCutArg.getValue());
    }
    
    Photos::iniInfo();
    
    m_initialised = true;
    
    return;
  }
  
}

