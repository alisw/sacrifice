AC_PREREQ(2.59)

AC_INIT( [Sacrifice], [1.0.0], [jmonk@cern.ch], [Sacrifice])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/PythiaMain.cxx])
AC_CONFIG_HEADERS([config/config.h])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AC_SUBST(LT_OBJDIR)
AC_LANG(C++)

AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AM_PROG_CC_C_O
## LT_INIT doesn't work with autoconf 2.59
#LT_INIT

##These are for tclap
AC_CXX_HAVE_SSTREAM
AC_CXX_HAVE_STRSTREAM

## This macro checks for boost in autoconf 2.59
AX_BOOST_BASE([1.33.0], [AC_MSG_NOTICE([Found Boost])], [AC_MSG_ERROR([Boost library cannot be found!])])

# lib suffix differs on OS X
AC_DEFINE_DYNAMIC_LIBS

# Do we have and LCG tag, AFS area.
# Can user-define the LCG tag for annoying cases (CENT OS!)

AC_ARG_WITH([LCGTag],
  [AC_HELP_STRING([--with-LCGTag], [Specify the LCG tag (default=auto detect)])],
  [auto_lcg=no], [auto_lcg=yes])

AS_IF([test x$auto_lcg == xno],
    [LCG_TAG=$with_LCGTag],
    [LCG_TAG=no]
    )

AC_SEARCH_LCG

AC_GET_MCUTILS([AC_MSG_NOTICE([MCUtils headers present in include/MCUtils])],[AC_MSG_ERROR([Unable to download MCUtils headers])])

AC_ARG_WITH([MCUtils],
  [AC_HELP_STRING([--with-MCUtils], [enable MCUtils for HepMC filtering and (optionally) give the installation location (default disabled)])],
  [],[with_MCUtils=yes])

AC_ARG_WITH([HepMC],
  [AC_HELP_STRING([--with-HepMC], [enable HepMC and give the installation location (=no to disable)])],
  )

AS_IF([test x$with_HepMC != xno],
    [AC_SEARCH_HEPMC([enable_HepMC=yes],
      [AS_IF( [test x$with_HepMC = x], [enable_HepMC=no], [AC_MSG_ERROR([HepMC package requested but not found!!])])])],
    [enable_HepMC=no])

if test x$enable_HepMC = xyes; then
  AC_MSG_NOTICE([Building with HepMC])
  AS_IF([test x$with_MCUtils != xno],
    [AC_SEARCH_MCUTILS([enable_MCUtils=yes],
                      [AC_MSG_ERROR([MCUtils support requested, but package not found!!])])],
    [enable_MCUtils=no])
else
  AC_MSG_NOTICE([*** Not building with HepMC - output of HepMC events will be disabled!! ***])
  if test x$with_MCUtils != xno ; then
    AC_MSG_ERROR([MCUtils support requested, but HepMC not found or requested!])
  fi
fi

AM_CONDITIONAL(ENABLE_HEPMC, [test x$enable_HepMC = xyes])
AM_CONDITIONAL(ENABLE_MCUTILS, [test x$enable_MCUtils = xyes])

AC_ARG_WITH([pythia_version],
  [AC_HELP_STRING([--with-pythia-version], [Request a particular Pythia 8 version if using AFS/LCG])],
  [do_pythia_version=yes], [do_pythia_version=no])

AS_IF([test x$do_pythia_version == xyes],
    [PYTHIA_VERSION=$with_pythia_version],
    [PYTHIA_VERSION=no]
    )

AC_ARG_WITH([LHAPDF],
  [AC_HELP_STRING([--with-LHAPDF], [enable LHAPDF and give the installation location (=no to disable)])],
  )

AS_IF([test x$with_LHAPDF != xno],
    [AC_SEARCH_LHAPDF([enable_LHAPDF=yes],
      [AS_IF( [test x$with_LHAPDF = x], [enable_LHAPDF=no], [AC_MSG_ERROR([LHAPDF package requested but not found!!])])])],
    [enable_LHAPDF=no])

if test x$enable_LHAPDF = xyes; then
  AC_MSG_NOTICE([Building with LHAPDF])
else
  AC_MSG_NOTICE([*** Not building with LHAPDF - use of external PDF sets will be disabled!! ***])
fi

AM_CONDITIONAL(ENABLE_LHAPDF, [test x$enable_LHAPDF = xyes])

AC_ARG_WITH([photos],
  [AC_HELP_STRING([--with-photos], [enable Photos and give the installation location (default disabled)])],
  [test_photos=yes],[test_photos=no])

##
AC_ARG_ENABLE([photos],
  [AC_HELP_STRING([--enable-photos], [Enable Photos++ and search default paths for libraries])],
  [test_photos=yes],
  [])

##


AS_IF( [test x$test_photos = xyes ],
  [AS_IF( [test x$with_photos != xno ],
    [AC_SEARCH_PHOTOS([enable_photos=yes],
                      [AC_MSG_ERROR([Photos++ support requested but package not found!!])])],
    [enable_photos=no])],
  [enable_photos=no])

if test x$enable_photos = xyes; then
  AC_MSG_NOTICE([Building with Photos++ support])
else
  AC_MSG_NOTICE([*** Building without Photos++ support!! ***])
fi

AM_CONDITIONAL(ENABLE_PHOTOS, [test x$enable_photos = xyes])

AC_ARG_WITH([pythia],
  [AC_HELP_STRING([--with-pythia], [Give the Pythia 8 library and header installation location])],
  )

AC_SEARCH_PYTHIA([AC_MSG_NOTICE([Found Pythia 8 library and headers])],
                  [AC_MSG_ERROR([Could not find Pythia 8 library and headers!!])])

AC_MSG_NOTICE([have zlib = $have_zlib ])
AM_CONDITIONAL(ENABLE_GZIP, [test x$have_zlib = xyes] )

AM_CXXFLAGS="$AM_CXXFLAGS -Wall -O2"
AM_CPPFLAGS="$AM_CPPFLAGS -Wall -O2 -I\$(top_builddir)/ -I\$(top_builddir)/include $BOOST_CPPFLAGS"
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(AM_CPPFLAGS)

AC_CONFIG_FILES([ Makefile
                  src/Makefile
                  include/Makefile
                  include/Sacrifice/Makefile
                  include/MCUtils/Makefile
                  include/tclap/Makefile
                  share/Makefile
                  share/setupPythia.sh
                  UserHooks/Makefile ])

AC_OUTPUT
