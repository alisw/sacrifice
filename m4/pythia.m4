#AC_SEARCH_PYTHIA(actionIfFound, actionIfNotFound)
AC_DEFUN([AC_SEARCH_PYTHIA],[

if test x$with_pythia != x && test x$with_pythia != xyes ; then
  AC_MSG_NOTICE([Adding $with_pythia to search path for Pythia])
  if test -d $with_pythia/include/ && test -d $with_pythia/lib ; then 
    found_pythia=yes
    pythia_base=$with_pythia
    pythia_include=$with_pythia/include/
    pythia_lib=$with_pythia/lib
    pythia_data=$with_pythia/xmldoc
  else
    found_pythia=no
  fi
fi

# If we failed to find it in the specified path then check some ~standard locations
# Add some default afs search paths here if LCG_TAG is defined
# If no PYTHIA_VERSION is defined then try to use the most recent

if test x$found_pythia != xyes ; then

  searchPaths="/usr /usr/local /opt /opt/local"

  if test "x$HAVE_LCG" == "xyes" ; then
  
    afs_base_path="/afs/cern.ch/sw/lcg/external/MCGenerators_lcgcmt64/pythia8/"
    if test "x$PYTHIA_VERSION" == "xno" ; then
      afs_subdirs=`ls $afs_base_path`
      PYTHIA_VERSION=`for dir in $afs_subdirs; do echo $dir; done | sort -r | head -n 1`
    fi
    
    AC_MSG_NOTICE([Testing for Pythia version $PYTHIA_VERSION in LCG AFS area])
    searchPaths="$searchPaths $afs_base_path/$PYTHIA_VERSION/${LCG_TAG}"
  fi

  for ac_pythia_path_tmp in $searchPaths ; do
    
    AC_MSG_NOTICE([Testing $ac_pythia_path_tmp for Pythia 8...])
    
    if test -d $ac_pythia_path_tmp/include && test -d $ac_pythia_path_tmp/lib && test -d $ac_pythia_path_tmp/xmldoc && test x$found_pythia != xyes; then
    
      AC_MSG_NOTICE([...found])
    
      found_pythia=yes
      pythia_base=$ac_pythia_path_tmp
      pythia_include=$ac_pythia_path_tmp/include/
      pythia_lib=$ac_pythia_path_tmp/lib
      pythia_data=$ac_pythia_path_tmp/xmldoc
    fi
  done
fi

if test x$found_pythia = xno ; then
  AC_MSG_ERROR([Could not find Pythia 8 libraries])
fi

# check for Pythia version > 8.180

if test -d $pythia_include/Pythia8Plugins ; then
  pythiaversion="8200"
  test_include="$pythia_include/Pythia8"
  hepmclib="no"
  pythia_data=$pythia_base/share/Pythia8/xmldoc
elif test -d $pythia_include/Pythia8 ; then
  pythiaversion="8180"
  test_include="$pythia_include/Pythia8"
  hepmclib="pythia8tohepmc"
else
  pythiaversion="8170"
  test_include="$pythia_include"
  hepmclib="hepmcinterface"
fi

# final check that the headers and libraries are actually there
if test x$found_pythia = xyes ; then
  if test -f $test_include/Pythia.h && test -f $pythia_lib/libpythia8.$LIB_SUFFIX && test -f $pythia_data/ParticleData.xml; then
    PYTHIA_LIBDIR="$pythia_lib"
    PYTHIA_LDFLAGS="-L$pythia_lib -lpythia8"
    PYTHIA_CPPFLAGS="-I$pythia_include"
    if test "x$pythiaversion" == "x8170" ; then
      PYTHIA_CPPFLAGS="$PYTHIA_CPPFLAGS -DPYTHIA8176"
    elif test "x$pythiaversion" == "x8180" ; then
      PYTHIA_CPPFLAGS="$PYTHIA_CPPFLAGS -DPYTHIA8180"
    else
      PYTHIA_CPPFLAGS="$PYTHIA_CPPFLAGS -DPYTHIA8200"
    fi
    PYTHIA_DATA="$pythia_data"
    
    # test for z lib dependency
    
    have_zlib=no
    
    zlib=`nm -P $pythia_lib/libpythia8.$LIB_SUFFIX | grep -i zlib | head -n 1`
    
    if test "x$zlib" != "x" ; then
      have_zlib=yes
      PYTHIA_LDFLAGS="$PYTHIA_LDFLAGS $BOOST_LDFLAGS -lboost_iostreams"
    fi
    
    if test x$enable_HepMC = xyes && test x$hepmclib != xno ; then
      if test -f $pythia_lib/lib$hepmclib.$LIB_SUFFIX ; then
        PYTHIA_LDFLAGS="$PYTHIA_LDFLAGS -l$hepmclib"
      else
        AC_MSG_NOTICE([HepMC requested, but Pythia 8 interface library libhepmcinterface not found!])
        found_pythia=no
      fi
    fi
    
    if test x$enable_LHAPDF != xyes && test x$pythiaversion != x8200 ; then
      if test -f $pythia_lib/liblhapdfdummy.$LIB_SUFFIX ; then
        PYTHIA_LDFLAGS="$PYTHIA_LDFLAGS -llhapdfdummy"
      else
        AC_MSG_NOTICE([LHAPDF not linked, but dummy Pythia 8 LHAPDf library not found!])
        found_pythia=no
      fi
    fi
    
  else
    found_pythia=no
  fi
fi

if test x$found_pythia = xyes ; then
  
  export PYTHIA_LDFLAGS
  export PYTHIA_CPPFLAGS
  export PYTHIA_DATA
  export PYTHIA_LIBDIR
  AC_SUBST([PYTHIA_LDFLAGS])
  AC_SUBST([PYTHIA_CPPFLAGS])
  AC_SUBST([PYTHIA_DATA])
  AC_SUBST([PYTHIA_LIBDIR])
  AM_CONDITIONAL(ENABLE_GZIP, [test x$have_zlib = xyes])
  AC_MSG_NOTICE([Found Pythia 8 libraries and headers])
  AC_MSG_NOTICE([PYTHIA_LDFLAGS = $PYTHIA_LDFLAGS])
  AC_MSG_NOTICE([PYTHIA_CPPFLAGS = $PYTHIA_CPPFLAGS])
  AC_MSG_NOTICE([PYTHIA_DATA = $PYTHIA_DATA])
  $1
else
  AC_MSG_NOTICE([Could not find complete set of Pythia 8 libraries and headers])
  $2
fi

])




