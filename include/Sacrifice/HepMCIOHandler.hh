#ifndef SACRIFICE_HEPMCIO_HANDLER_HH
#define SACRIFICE_HEPMCIO_HANDLER_HH

#include <string>

#include <fstream>

#include "boost/smart_ptr.hpp"

#include "tclap/CmdLine.h"

#include "HepMC/IO_GenEvent.h"

namespace Sacrifice{
 
  using std::string;
  using std::ostream;
  using boost::shared_ptr;
  
  using TCLAP::SwitchArg;
  using TCLAP::ValueArg;
  
  class HepMCIOHandler{
    
  public:
    
    HepMCIOHandler(TCLAP::CmdLine &cmd);
    
    ~HepMCIOHandler(){
      m_io.reset();
      m_ostr.reset();
      m_zippedStream.reset();

    }
    
    void writeEvent(const HepMC::GenEvent *event){
     
      if(m_doInitialise)initialise();
      
      (*m_io) << event;
      
      return;
    }
    
  private:
    
    void initialise();
    
    bool m_doInitialise;
    
    shared_ptr<ostream> m_ostr;
    
    shared_ptr<ostream> m_zippedStream;
    
    shared_ptr<HepMC::IO_GenEvent> m_io;
    
    // Switch to turn on gzip compression
    SwitchArg m_zipArg;
    
    ValueArg<string> m_outputArg;
    
  };
}

#endif
