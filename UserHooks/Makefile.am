lib_LTLIBRARIES             = SacrificePlugin.la

SacrificePlugin_la_SOURCES  = PoWHEGVetoedShower.cxx        \
                              PTRelVetoedShower.cxx         \
                              PTRel_boostVetoedShower.cxx   \
                              QCDVetoedShower.cxx           \
                              EnhanceMPI.cxx                \
                              SuppressMPI.cxx               \
                              WZVetoedShower.cxx            
#                              main31.cxx      

nobase_pkginclude_HEADERS   = UserHookUtils.hh

SacrificePlugin_la_CPPFLAGS = $(AM_CPPFLAGS) $(PYTHIA_CPPFLAGS)
SacrificePlugin_la_LDFLAGS  = $(AM_LDFLAGS) -module --avoid-version
