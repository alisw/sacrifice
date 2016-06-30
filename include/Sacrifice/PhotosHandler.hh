#ifndef SACRIFICE_PHOTOSHANDLER_HH
#define SACRIFICE_PHOTOSHANDLER_HH

#include "tclap/CmdLine.h"

namespace HepMC{
  class GenEvent;
}

namespace Sacrifice{
    
  using TCLAP::SwitchArg;
  using TCLAP::ValueArg;
  
  class PhotosHandler{
    
  public:
    
    /**
     * Constructor.  Pass a TCLAP CmdLine
     *
     */
    PhotosHandler(TCLAP::CmdLine &cmd):
    m_initialised(false),
    m_photosSwitch("p", "photos", "Run Photos for QED radiation (default = off)", false),
    m_irCutArg("q", "qed-ir-cutoff", "Overrides infra-red cutoff for Photos++ QED radiation", false, -1., "double"),
    m_alphaQEDArg("a", "alpha-qed", "Alpha_QED for PHOTOS++ (default 0.00729735039)", false, 0.00729735039, "double"),
    m_exponentiationSwitch("x", "no-exponentiation", "Turn OFF exponentiation mode for PHOTOS++ (default = on)", true){
      
      cmd.add(m_photosSwitch);
      cmd.add(m_irCutArg);
      cmd.add(m_alphaQEDArg);
      cmd.add(m_exponentiationSwitch);      
    }
    
    /// Is support for Photos compiled and available
    bool isAvailable()const;
    
    /// Is Photos switched on in this run
    bool isEnabled(){
      return m_photosSwitch.getValue();
    }
    
    /// Initialise the photos setup
    void initialise();
    
    /// Process a HepMC event
    void process(HepMC::GenEvent *event)const;
    
  private:
    
    PhotosHandler();
    
    bool m_initialised;
    
    SwitchArg m_photosSwitch;
    ValueArg<double> m_irCutArg;
    ValueArg<double> m_alphaQEDArg;
    SwitchArg m_exponentiationSwitch;
    
  };
}
#endif
