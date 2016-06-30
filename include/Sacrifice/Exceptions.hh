#ifndef SACRIFICE_EXCEPTIONS_HH
#define SACRIFICE_EXCEPTIONS_HH

#include <stdexcept>
#include <sstream>
#include <iostream>
#include <string>

namespace Sacrifice{

  using std::string;
  using std::stringstream;
  
  class PhotosException: public std::runtime_error{
    
  public:
    
    PhotosException(): std::runtime_error(PhotosException::errMessage()){};
    
  private:
    static string errMessage(){
      static stringstream ss;
      ss << "Photos requested, but support missing.  Problems: "<<std::endl;
      
#ifndef HAS_PHOTOS
      ss << "  Photos++ library not linked"<<std::endl;
#endif
      
#ifndef HAS_HEPMC
      ss << "  HepMC not available" << std::endl;
#else
#ifndef HEPMC_HAS_UNITS
      ss << "  HepMC does not provide units support" <<std::endl;
#endif
#endif
      return ss.str();
    }
  };
  
}
  
#endif
