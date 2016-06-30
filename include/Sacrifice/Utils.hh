#ifndef SACRIFICE_UTILS_HH
#define SACRIFICE_UTILS_HH

#include "Sacrifice/binreloc.h"

#include <string>
#include <vector>
#include <fstream>

namespace Sacrifice{
 
  using std::string;
  using std::vector;
  using std::ifstream;
  
  string findParamFile(const string &filename){
    
//    BrInitError brError;
//    br_init_lib(&brError);
    string path = br_find_data_dir(DEFAULTPARAMDIR);
   
    vector<string> toCheck;
    toCheck.push_back(filename);
    toCheck.push_back(filename + ".params");
    if(path != ""){
      toCheck.push_back(path + "/" + filename);
      toCheck.push_back(path + "/" + filename + ".params");
    }
    vector<string>::const_iterator file = toCheck.begin();
    
    while(file != toCheck.end()){
  
      ifstream testFile(file->c_str(), ifstream::in);
      if(testFile.is_open()) break;
      ++file;
    }
    
    path = (file == toCheck.end())? filename: *file;
    return path;
  }
  
}
#endif
