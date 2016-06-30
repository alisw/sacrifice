#ifndef SACRIFICE_USERHOOKS_FACTORY_HH
#define SACRIFICE_USERHOOKS_FACTORY_HH

#ifdef PYTHIA8176
#include "UserHooks.h"
#else
#include "Pythia8/UserHooks.h"
#endif

#include <string>
#include <map>

namespace Sacrifice{

  using Pythia8::UserHooks;
  using std::string;
  using std::map;


  class UserHooksFactory{

  public:

    static UserHooks* create(const string &hookName);

    /**
     *  Loads a library of UserHooks
     */
    static void loadLibrary(const string &libName);

    /**
     *  \return the path to a user hooks library
     */
    static string libraryPath(const string &libName);

  private:

    UserHooksFactory(){};

    class ICreator{
    public:
      virtual UserHooks *create() const = 0;
      virtual ~ICreator(){};
    };

  public:

    template <class T>
    class Creator: public ICreator{

    public:

      Creator(const string &name){
        m_name = name;
        UserHooksFactory::s_creators()[name] = this;
      }

      ~Creator(){
        if(s_creators()[m_name] == this){
          s_creators().erase(m_name);
        }
      }

      UserHooks *create()const{
        return new T;
      }

    private:

      string m_name;
    };

  private:

    static map<string, const ICreator*> &s_creators();

  };
}


#endif
