SUBDIRS = .

bin_PROGRAMS = run-pythia

run_pythia_SOURCES = BeamParticle.cxx UserHooksFactory.cxx PythiaMain.cxx 

run_pythia_CPPFLAGS  = $(AM_CPPFLAGS) $(PYTHIA_CPPFLAGS)
run_pythia_CPPFLAGS += -DENABLE__BINRELOC 
run_pythia_CPPFLAGS += -DDEFAULTDATADIR=\"$(PYTHIA_DATA)\" 
run_pythia_CPPFLAGS += -DDEFAULTPARAMDIR=\"$(datadir)/Sacrifice\"
run_pythia_CPPFLAGS += -DDEFAULTLIBDIR=\"$(libdir)\"
run_pythia_LDFLAGS   = $(AM_LDFLAGS) $(PYTHIA_LDFLAGS) -ldl -rdynamic
run_pythia_LDADD     = $(AM_LDADD)

if ENABLE_HEPMC
run_pythia_CPPFLAGS += $(HEPMC_CPPFLAGS) -DHAS_HEPMC
run_pythia_LDFLAGS  += $(HEPMC_LDFLAGS)
endif

if ENABLE_MCUTILS
run_pythia_CPPFLAGS += $(MCUTILS_CPPFLAGS) -DHAS_MCUTILS
run_pythia_SOURCES  += MCUtilsHandler_enabled.cxx
else
run_pythia_SOURCES  += MCUtilsHandler_disabled.cxx
endif

if ENABLE_LHAPDF
run_pythia_CPPFLAGS += $(LHAPDF_CPPFLAGS)
run_pythia_LDFLAGS  += $(LHAPDF_LDFLAGS)
endif

if ENABLE_PHOTOS
run_pythia_SOURCES  += PhotosHandler_enabled.cxx
run_pythia_CPPFLAGS += $(PHOTOS_CPPFLAGS) -DHAS_PHOTOS
run_pythia_LDFLAGS  += $(PHOTOS_LDFLAGS)
else
run_pythia_SOURCES  += PhotosHandler_disabled.cxx
endif

if ENABLE_GZIP
run_pythia_SOURCES  += HepMCIOHandler_gzip.cxx
else
run_pythia_SOURCES  += HepMCIOHandler_no_gzip.cxx
endif

run_pythia_SOURCES  += binreloc.c

EXTRA_DIST = zstr
