#include "Sacrifice/HepMCIOHandler.hh"

#include "zstr/zstr.hpp"

namespace Sacrifice{
 
  HepMCIOHandler::HepMCIOHandler(TCLAP::CmdLine &cmd):
  m_doInitialise(true), 
  m_zipArg("z", "gzip-HepMC", "Compress the output HepMC file using gzip", false),
  m_outputArg("o", "output", "Output HepMC file", false, "pythia.hepmc", "string"){
    
    cmd.add(m_zipArg);
    cmd.add(m_outputArg);
  }
  
  void HepMCIOHandler::initialise(){
    
    if(m_zipArg.getValue()){
      
      string name = m_outputArg.getValue() + ".gz";
      
      m_ostr.reset(new zstr::ofstream(name));
      
      m_io.reset(new HepMC::IO_GenEvent(*m_ostr));
      
    }else{
      string name = m_outputArg.getValue();
      m_ostr.reset(new std::ofstream(name.c_str(), std::ios::out));
      m_io.reset(new HepMC::IO_GenEvent(*m_ostr));      
    }
    
    m_doInitialise = false;
    
    return;
  }
  
}


