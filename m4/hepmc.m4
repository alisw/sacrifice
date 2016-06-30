#AC_SEARCH_HEPMC(actionIfFound, actionIfNotFound)
AC_DEFUN([AC_SEARCH_HEPMC],[

  afs_base_path="/afs/cern.ch/sw/lcg/external/HepMC/2.06.05/${LCG_TAG}"

  if test x$with_HepMC != x && test x$with_HepMC != xyes ; then
    AC_MSG_NOTICE([Adding $with_HepMC to search path for HepMC])
    if test -d $with_HepMC/include/HepMC && test -d $with_HepMC/lib ; then
      found_HepMC=yes
      HepMC_base_path=`echo ${with_HepMC%/}`
      HepMC_include=$with_HepMC/include/
      HepMC_lib=$with_HepMC/lib
    else
      found_HepMC=no
    fi
  fi

# If we failed to find it in the specified path then check some ~standard locations
# Add some default afs search paths here if LCG_TAG is defined
# Todo: maybe work out the most recent version of HepMC more intelligently.

  if test x$found_HepMC != xyes ; then
    
    searchPaths="/usr /usr/local /opt /opt/local"

    if test "x$HAVE_LCG" == "xyes" ; then
      searchPaths="$searchPaths $afs_base_path"
    fi
        
    for ac_hepmc_path_tmp in $searchPaths ; do
    
      AC_MSG_NOTICE([Testing $ac_hepmc_path_tmp for HepMC...])
    
      if test -d $ac_hepmc_path_tmp/include/HepMC && test -d $ac_hepmc_path_tmp/lib ; then
      
        AC_MSG_NOTICE([...found])
      
        found_HepMC=yes
        HepMC_base_path=$ac_hepmc_path_tmp
        HepMC_include=$ac_hepmc_path_tmp/include/
        HepMC_lib=$ac_hepmc_path_tmp/lib
      fi
    done
  fi

# final check that the headers and libraries are actually there
  if test x$found_HepMC = xyes ; then
    if test -f $HepMC_include/HepMC/GenEvent.h && test -f $HepMC_lib/libHepMC.$LIB_SUFFIX ; then
      HEPMC_LDFLAGS="-L$HepMC_lib -lHepMC"
      HEPMC_CPPFLAGS="-I$HepMC_include"
      HEPMC_LIBDIR="$HepMC_lib"
      
      if test -f $HepMC_include/HepMC/Units.h ; then
        HEPMC_CPPFLAGS="$HEPMC_CPPFLAGS -DHEPMC_HAS_UNITS"
      fi
      
    else
      found_HepMC=no
      HAVE_LCG=no
    fi
  fi

  if test x$found_HepMC = xyes ; then
  
    if test "x$HepMC_base_path" != "x$afs_base_path"; then
    
      if test "x$HAVE_LCG" == "xyes"; then
        AC_MSG_WARN([Use of LCG installations disabled: HepMC version found does not match])
      fi
    
      HAVE_LCG=no
    fi
  
    export HEPMC_LDFLAGS
    export HEPMC_CPPFLAGS
    export HEPMC_LIBDIR
    AC_SUBST([HEPMC_LDFLAGS])
    AC_SUBST([HEPMC_CPPFLAGS])
    AC_SUBST([HEPMC_LIBDIR])
    AC_MSG_NOTICE([Found HepMC package])
    AC_MSG_NOTICE([HEPMC_LDFLAGS = $HEPMC_LDFLAGS])
    AC_MSG_NOTICE([HEPMC_CPPFLAGS = $HEPMC_CPPFLAGS])
    $1
  else
    AC_MSG_NOTICE([Could not find HepMC package])
    $2
  fi
])