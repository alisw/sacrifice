#include "Sacrifice/UserHooksFactory.hh"
#include "Sacrifice/binreloc.h"

#include <dlfcn.h>
#include <stdexcept>
#include <vector>

namespace Sacrifice{

  using std::vector;
  using std::ifstream;

  //////////////////////////////////////////////////////////////////////////////
  UserHooks *UserHooksFactory::create(const string &name){
    map<string, const ICreator*>::const_iterator it = s_creators().find(name);
    if(it == s_creators().end()){
      //eek!
      throw std::runtime_error("UserHooksFactory: cannot create user hook " + name);
    }
    return it->second->create();
  }

  //////////////////////////////////////////////////////////////////////////////
  string UserHooksFactory::libraryPath(const string &libName){
    vector<string> libNames;
    libNames.push_back(libName);
    libNames.push_back(libName + ".so");
    libNames.push_back("Sacrifice" + libName);
    libNames.push_back("Sacrifice" + libName + ".so");

    vector<string> toCheck;

    char *envPath=0;
    envPath = getenv("SACRIFICE_LIB_PATH");
    if(envPath){
      string sPath = string(envPath);
      for(vector<string>::const_iterator name = libNames.begin();
          name != libNames.end(); ++name){
        toCheck.push_back(sPath + "/" + *name);
      }
    }

    for(vector<string>::const_iterator name = libNames.begin();
        name != libNames.end(); ++name){
      toCheck.push_back("./" + *name);
    }

    BrInitError brError;
    br_init_lib(&brError);
    string defaultPath = br_find_data_dir(DEFAULTLIBDIR);

    for(vector<string>::const_iterator name = libNames.begin();
        name != libNames.end(); ++name){
      toCheck.push_back(defaultPath + "/" + *name);
    }

    vector<string>::const_iterator file = toCheck.begin();

    while(file != toCheck.end()){

      ifstream testFile(file->c_str(), ifstream::in);
      if(testFile.is_open()) break;
      ++file;
    }

    string path = (file == toCheck.end())? libName: *file;
    return path;
  }

  //////////////////////////////////////////////////////////////////////////////
  void UserHooksFactory::loadLibrary(const string &libName){

    string path = libraryPath(libName);

    std::cout<<"Opening UserHook library at "<<path<<std::endl;

    void *libptr = dlopen(path.c_str(), RTLD_LAZY);

    if(libptr == 0){
      std::cout<<dlerror()<<std::endl;
      throw std::runtime_error("UserHooksFactory: Unable to open library: " + path);
    }

    return;
  }

  ///static function to instantiate map of string name Vs. creator object on first use
  map<string, const UserHooksFactory::ICreator*> &UserHooksFactory::s_creators(){
    static map<string, const UserHooksFactory::ICreator*> creators;
    return creators;
  }

}
