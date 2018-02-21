#include "Sacrifice/UserHooksFactory.hh"
#include "Pythia8Plugins/JetMatching.h"

Sacrifice::UserHooksFactory::Creator<Pythia8::JetMatchingMadgraph> JetMatchingMadGraphCreator("JetMatchingMadgraph");
