#ifndef SACRIFICE_HEPMCIO_HANDLER_HH
#define SACRIFICE_HEPMCIO_HANDLER_HH

#include "tclap/CmdLine.h"

#include "HepMC/IO_GenEvent.h"

#include <string>
#include <fstream>
#include <memory>

namespace Sacrifice{
 
  using std::string;
  using std::ostream;
  using std::unique_ptr;
  
  using TCLAP::SwitchArg;
  using TCLAP::ValueArg;
  
  class HepMCIOHandler{
    
  public:
    
    HepMCIOHandler(TCLAP::CmdLine &cmd);
    
    ~HepMCIOHandler(){
      //m_io.reset();
      //m_ostr.reset();
      //m_zippedStream.reset();

    }
    
    void writeEvent(const HepMC::GenEvent *event){
     
      if(m_doInitialise)initialise();
      
      (*m_io) << event;
      
      return;
    }
    
  private:
    
    void initialise();
    
    bool m_doInitialise;
    
    unique_ptr<ostream> m_ostr;
    
    unique_ptr<ostream> m_zippedStream;
    
    unique_ptr<HepMC::IO_GenEvent> m_io;
    
    // Switch to turn on gzip compression
    SwitchArg m_zipArg;
    
    ValueArg<string> m_outputArg;
    
  };
}

#endif
