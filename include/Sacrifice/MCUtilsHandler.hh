#ifndef SACRIFICE_MCUTILS_HANDLER_HH
#define SACRIFICE_MCUTILS_HANDLER_HH

#include "tclap/CmdLine.h"

#include "boost/function.hpp"

#include <string>
#include <vector>
#include <map>

namespace HepMC{
  class GenEvent;
  class GenParticle;
}

namespace Sacrifice{
  
  using TCLAP::SwitchArg;
  using TCLAP::ValueArg;
  
  using std::string;
  using std::vector;
  using std::map;
  
  typedef boost::function<bool(const HepMC::GenParticle*)> Classifier;
  
  class MCUtilsHandler{
    
  public:
    
    /**
     *  Constructor.  Pass a TCLAP CmdLine and add the appropriate args if
     *  MCUtils support is available
     */
    MCUtilsHandler(TCLAP::CmdLine &cmd);
    
    static bool isAvailable();
    
    void filter(HepMC::GenEvent *event);
    
    class Selector{
      
    public:
      
      bool operator()(const HepMC::GenParticle* p);
      
      void addParticle(int pid, const vector<int> &statuses){

	m_pids[pid].insert(m_pids[pid].end(), statuses.begin(), statuses.end());
        return;
      }
      
    private:
      
      /// map list of particle ids onto allowed statuses
      map<int, vector<int> > m_pids;
      
    };
    
  private:
    
    void initialise();
    
    // is this instance to be initialised
    bool m_doInitialise;
    
    // switch to turn on default "standard" slimming of HepMC record
    // slims non-interesting, gen-specific and loop particles.
    // Keeps full phase space though
    SwitchArg m_slimArg;
    
    // filename specifying a list of particle definitions to keep
    // if this exists then all other particles removed from the event
    ValueArg<string> m_keepList;
    
    // list of classifiers to apply to the event
    vector<Classifier> m_classifiers;
    
    Selector m_selector;
    
  };
}
#endif
