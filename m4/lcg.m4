#AC_SEARCH_LCG(actionIfFound, actionIfNotFound)
AC_DEFUN([AC_SEARCH_LCG],[

  HAVE_LCG=no

  if test "x$LCG_TAG" != "xno" ; then
    lcg_string=$LCG_TAG
  else

    if test -d  /afs/cern.ch/sw/lcg/external/ ; then
  
      if test -e "/etc/redhat-release"; then
        slcstring=`cat /etc/redhat-release | grep "Scientific Linux"`
      
        if test "x$slcstring" != "x"; then
          slcVersion=`cat /etc/redhat-release | cut -d " " -f 6 | cut -d . -f 1`
          dist=slc${slcVersion}
        
          machine=`uname -m`
        
          if test -z "$CXX"; then
            echo "running ac_path_prog for CXX"
            AC_PATH_PROG(CXX, [g++])
          fi
        
          cxx_path=`which $CXX`
        
          if test -x "$cxx_path"; then
            gccVersion=`$cxx_path --version | head -1 | cut -d " " -f 3`
            major=`echo $gccVersion | cut -d . -f 1`
            minor=`echo $gccVersion | cut -d . -f 2`
            gccCode=gcc${major}${minor}
          fi
        
          # Many packages (e.g. Pythia8!) not built for gcc 41, but 43 is "ok"
          if test "x${dist}-${gccCode}" == "xslc5-gcc41" ; then
            gccCode="gcc43"
          fi
        
          # And in SLC6 we want to use gcc 46
          if test "x${dist}-${gccCode}" == "xslc6-gcc44"; then
            gccCode="gcc46"
          fi
        
          lcg_string=${machine}-${dist}-${gccCode}-opt
        
        fi
      fi
    fi
  fi

  if test "x$lcg_string" != "x" ; then
    HAVE_LCG=yes
    LCG_TAG=$lcg_string
    export LCG_TAG
    AC_SUBST([LCG_TAG])
    AC_MSG_NOTICE([Found lcg tag = $LCG_TAG])
  else 
    AC_MSG_NOTICE([Cannot interpret lcg tag - not searching AFS paths!])
  fi

  export HAVE_LCG
  AC_SUBST([HAVE_LCG])

])