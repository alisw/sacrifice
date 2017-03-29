#ifdef PYTHIA8176
#include "Pythia.h"
#include "HepMCInterface.h"
#define HepMCConverter HepMC::I_Pythia8
#else
#include "Pythia8/Pythia.h"
#ifdef PYTHIA8200
#include "Pythia8Plugins/HepMC2.h"
#else
#include "Pythia8/Pythia8ToHepMC.h"
#endif
#define HepMCConverter HepMC::Pythia8ToHepMC
#endif

#include "Sacrifice/UserHooksFactory.hh"
#include "Sacrifice/Exceptions.hh"
#include "Sacrifice/BeamParticle.hh"
#include "Sacrifice/Utils.hh"

#include "Sacrifice/HepMCIOHandler.hh"
#include "Sacrifice/MCUtilsHandler.hh"
#include "Sacrifice/PhotosHandler.hh"

#include "Sacrifice/binreloc.h"
#include "tclap/CmdLine.h"

#include "HepMC/GenEvent.h"
#include "HepMC/Units.h"

#include "boost/lexical_cast.hpp"

#include <string>
#include <stdexcept>

using namespace std;


int main(int argc, char **argv){

  char *envPath=0;
  envPath = getenv("PYTHIA8DATA");

  string path;

  if(envPath){
    path = envPath;
  }else{
    BrInitError brError;
    br_init_lib(&brError);
    path = br_find_data_dir(DEFAULTDATADIR);
  }

  //Do this first so we always get the Pythia banner and version information
  Pythia8::Pythia pythia(path);
  std::string pythiaVersion = boost::lexical_cast<std::string>(pythia.settings.parm("Pythia:versionNumber") + 0.00000000001);
  pythiaVersion.erase(5);
  std::string message = "Main program for steering Pythia " + pythiaVersion + ".  Bug reports to James Monk <jmonk@cern.ch>";

  TCLAP::CmdLine cmd(message, ' ', "0.9.9");

  Sacrifice::HepMCIOHandler hepMCHandler(cmd);
  Sacrifice::PhotosHandler photosHandler(cmd);
  Sacrifice::MCUtilsHandler mcutils(cmd);

  TCLAP::ValueArg<int> seedArg("r", "random-seed", "Random seed", false, -1, "int");
  TCLAP::MultiArg<std::string> inputArg("i", "input", "Input command file", false, "string");
  TCLAP::ValueArg<std::string> beam1Arg("f", "beam1", "Forward-going beam type (default PROTON)", false, "PROTON", "string");
  TCLAP::ValueArg<std::string> beam2Arg("b", "beam2", "Backward-going beam type (default PROTON)", false, "PROTON", "string");
  TCLAP::ValueArg<double> energyArg("e", "collision-energy", "Collision energy in GeV (default 7000 GeV)", false, 8000., "double");
  TCLAP::ValueArg<int> nEventsArg("n", "nEvents", "Number of events to generate (default 100)", false, 100, "int");
  TCLAP::MultiArg<std::string> paramsArgs("c", "command", "Commands to be read directly in to Pythia. Over-rides command file.", false, "string");
  TCLAP::ValueArg<std::string> dataArg("d", "particle-data", "XML file of particle data.  Over-rides default in $PYTHIA8DATA.", false, "", "string");
  TCLAP::ValueArg<std::string> lhefArg("l", "lhe-file"," Les Houches Event File input", false, "", "string");
  TCLAP::ValueArg<std::string> libArg("L", "load-library", "Library of user hooks to be loaded", false, "Plugin", "string");
  TCLAP::ValueArg<std::string> hooksArg("u", "user-hook", "Name of user hook to add", false, "", "string");
  TCLAP::SwitchArg printArg("w", "write", "Write HepMC events to stdout", false);
  TCLAP::ValueArg<int> maxFailureArg("m", "max-failures", "Maximum number of failed events", false, 10, "int");


  cmd.add(seedArg);
  cmd.add(inputArg);
  cmd.add(beam1Arg);
  cmd.add(beam2Arg);
  cmd.add(energyArg);
  cmd.add(nEventsArg);
  cmd.add(paramsArgs);
  cmd.add(dataArg);
  cmd.add(lhefArg);
  cmd.add(libArg);
  cmd.add(hooksArg);
  cmd.add(printArg);
  cmd.add(maxFailureArg);

  cmd.parse( argc, argv );

  HepMCConverter pythiaToHepMC;

  pythiaToHepMC.set_store_pdf(true);
  // pythiaToHepMC.set_crash_on_problem(true);
  
  if(photosHandler.isEnabled()){
    photosHandler.initialise();
  }

  for(vector<std::string>::const_iterator inputFile = inputArg.getValue().begin();
      inputFile != inputArg.getValue().end(); ++inputFile){

    std::string path = Sacrifice::findParamFile(*inputFile);
    if(!pythia.readFile(path)){
      throw std::runtime_error("Could not interpret all of the commands in the input file " + path + "!");
    }else{
      std::cout<<" Read commands from "<<path<<std::endl<<std::endl;
    }
  }

  for(vector<std::string>::const_iterator command = paramsArgs.getValue().begin();
      command != paramsArgs.getValue().end(); ++command){
    if(!pythia.readString(*command)){
      std::string msg = "Pythia could not understand the command " + *command;
      throw std::runtime_error(msg);
    }
  }

  if(seedArg.getValue() > 0){
    std::string seedString = "Random:seed=" + boost::lexical_cast<string>(seedArg.getValue());
    bool understood = pythia.readString("Random:setSeed=on");
    understood = understood && pythia.readString(seedString);
    if(! understood) throw std::runtime_error("Your version of Pythia does not understand the random seed commands!");
  }

  if(photosHandler.isEnabled()){
    bool understood = pythia.readString("TimeShower:QEDshowerByL = off");
    if(! understood) throw std::runtime_error("Your version of Pythia does not understand the command to turn off QED FSR!");
  }

  Sacrifice::BeamParticle beam1(beam1Arg.getValue());
  Sacrifice::BeamParticle beam2(beam2Arg.getValue());

  if(hooksArg.getValue() != ""){
    Sacrifice::UserHooksFactory::loadLibrary(libArg.getValue());
    if(!pythia.setUserHooksPtr(Sacrifice::UserHooksFactory::create(hooksArg.getValue())))
      throw std::runtime_error("Unable to use UserHook: " + hooksArg.getValue());
  }

  /// @todo This doesn't work at the moment... the beam particles are not correctly handled and process init fails
  if(dataArg.getValue() != ""){
    if(!pythia.particleData.reInit(dataArg.getValue(), true)) throw std::runtime_error("Could not read particle data file: " + dataArg.getValue());
  }

  bool isInitialised = true;
  bool doLHEF = false;

  if(lhefArg.getValue() != ""){
    isInitialised = isInitialised && pythia.readString("Beams:frameType = 4");
    isInitialised = isInitialised && pythia.readString("Beams:LHEF = " + lhefArg.getValue());
    doLHEF = true;
  }else{
    isInitialised = isInitialised && pythia.readString("Beams:frameType = 1");
    isInitialised = isInitialised && pythia.readString("Beams:idA = " + boost::lexical_cast<string>(beam1.asID()));
    isInitialised = isInitialised && pythia.readString("Beams:idB = " + boost::lexical_cast<string>(beam2.asID()));
    isInitialised = isInitialised && pythia.readString("Beams:eCM = " + boost::lexical_cast<string>(energyArg.getValue()));
  }

  isInitialised = isInitialised && pythia.init();

  if(!isInitialised) throw std::runtime_error("Could not initialise Pythia");

  std::vector<std::string> weightIDs;
  int failureCount=0;
  double nMerged=0.;
  double nAccepted=0.;

  for(int eventNumber = 0; eventNumber < nEventsArg.getValue(); ++eventNumber){

    if(!pythia.next()){
      if(pythia.info.atEndOfFile()){
        std::cout<<"Reached end of LHE file"<<std::endl;
        break;
      }

      ++failureCount;

      if(failureCount > maxFailureArg.getValue()){
        std::cout<<"Number of failed events exceeds maximum of "<<maxFailureArg.getValue()<<std::endl;
        break;
      }
      if(eventNumber == 0) --eventNumber;
    }

#ifdef HEPMC_HAS_UNITS ///
    HepMC::GenEvent *hepMCEvent = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);
#else
    HepMC::GenEvent *hepMCEvent = new HepMC::GenEvent();
#endif /// HEPMC_HAS_UNITS

    pythiaToHepMC.fill_next_event(pythia, hepMCEvent);

    if(photosHandler.isEnabled()){
      photosHandler.process(hepMCEvent);
    }

    if(mcutils.isAvailable()) mcutils.filter(hepMCEvent);

    double phaseSpaceWeight = pythia.info.weight();
    double mergingWeight    = pythia.info.mergingWeight();
    double eventWeight = phaseSpaceWeight*mergingWeight;

    nAccepted += 1.;

    if(fabs(eventWeight) < 1.e-18 ||
       pythia.event.size() < 2){

      if(eventNumber == 0) --eventNumber;
      continue;
    }else{
      nMerged += eventWeight;
    }

    hepMCEvent->weights().clear();

    std::vector<std::string>::const_iterator id = weightIDs.begin();

    if(pythia.info.getWeightsDetailedSize() != 0){

      for(std::map<std::string, Pythia8::LHAwgt>::const_iterator wgt = pythia.info.rwgt->wgts.begin();
          wgt != pythia.info.rwgt->wgts.end(); ++wgt){

        if(eventNumber == 0){
          weightIDs.push_back(wgt->first);
        }else{
          if(*id != wgt->first){
            throw std::runtime_error("Mismatch in LHE3 weight name.  Found" + wgt->first + ", expected " + *id);
          }
          ++id;
        }

        std::map<std::string, Pythia8::LHAweight>::const_iterator weightName = pythia.info.init_weights->find(wgt->first);
        if(weightName != pythia.info.init_weights->end()){
          hepMCEvent->weights()[weightName->second.contents] = mergingWeight * wgt->second.contents;
        }else{
          hepMCEvent->weights()[wgt->first] = mergingWeight * wgt->second.contents;
        }

      }

    }else{
      hepMCEvent->weights().push_back(eventWeight);
    }

    if(printArg.getValue()){
      hepMCEvent->print();
    }


    hepMCHandler.writeEvent(hepMCEvent);

    delete hepMCEvent;

  }

  pythia.stat();

  double xs = pythia.info.sigmaGen(); // in mb
  std::cout<<"Cross Section (nb) = "<<xs * 1000. *nMerged / nAccepted<<std::endl;

}
