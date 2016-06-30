#include "Sacrifice/PhotosHandler.hh"
#include "Sacrifice/Exceptions.hh"

namespace Sacrifice{
 
  bool PhotosHandler::isAvailable()const{
    return false; 
  }
  
  void PhotosHandler::process(HepMC::GenEvent *event)const{
    throw PhotosException();
  }
  
  void PhotosHandler::initialise(){
    throw PhotosException();
    return;
  }
  
}
