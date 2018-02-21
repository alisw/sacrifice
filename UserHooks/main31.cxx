#include "Sacrifice/UserHooksFactory.hh"
#include "Pythia8Plugins/PowhegHooks.h"

Sacrifice::UserHooksFactory::Creator<Pythia8::PowhegHooks> main31Creator("PowhegMain31");
