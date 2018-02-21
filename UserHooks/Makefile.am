lib_LTLIBRARIES             = SacrificePlugin.la

SacrificePlugin_la_SOURCES  = main31.cxx                  \
                              JetMatchingMadGraphHook.cxx \
                              EnhanceMPI.cxx              \
                              SuppressMPI.cxx

nobase_pkginclude_HEADERS   = UserHookUtils.hh

SacrificePlugin_la_CPPFLAGS = $(AM_CPPFLAGS) $(PYTHIA_CPPFLAGS)
SacrificePlugin_la_LDFLAGS  = $(AM_LDFLAGS) -module --avoid-version
