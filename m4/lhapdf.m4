#AC_SEARCH_LHAPDF(actionIfFound, actionIfNotFound)
AC_DEFUN([AC_SEARCH_LHAPDF], [
LHAPDF_CONFIG_PATH=$PATH

if test x$with_LHAPDF != x ; then
    AC_MSG_NOTICE([Adding $with_LHAPDF to lhapdf-config search path])
    LHAPDF_CONFIG_PATH=$with_LHAPDF/bin:$with_LHAPDF:$LHAPDF_CONFIG_PATH
fi

AC_MSG_NOTICE([lhapdf-config search path is $LHAPDF_CONFIG_PATH])

AC_PATH_PROG(LHAPDF_CONFIG, [lhapdf-config], [no], [$LHAPDF_CONFIG_PATH])

# if failed to find lhapdf-config in the default $PATH, check some other default areas

if test "x$LHAPDF_CONFIG" == "xno" ; then

  searchPaths="/usr/bin /usr/local/bin /opt/bin /opt/local/bin"

  if test "x$HAVE_LCG" == "xyes" ; then
    afs_base_path="/afs/cern.ch/sw/lcg/external/MCGenerators_lcgcmt64/lhapdf/"
    afs_subdirs=`ls $afs_base_path`
    lhapdf_version=`for dir in $afs_subdirs; do echo $dir; done | sort -r | head -n 1`
    searchPaths="$searchPaths ${afs_base_path}${lhapdf_version}/${LCG_TAG}/bin/"
  fi

  for ac_lhapdf_path_tmp in $searchPaths ; do
    AC_MSG_NOTICE([Testing $ac_lhapdf_path_tmp for lhapdf-config...])
    if test -x ${ac_lhapdf_path_tmp}lhapdf-config && test "x$LHAPDF_CONFIG" == "xno"; then
      AC_MSG_NOTICE([...found])
      LHAPDF_CONFIG=${ac_lhapdf_path_tmp}lhapdf-config
    fi
  done

fi

if test x$LHAPDF_CONFIG != xno ; then

  AC_MSG_NOTICE([Found lhapdf-config])
  LHAPDF_CPPFLAGS="`$LHAPDF_CONFIG --cppflags`"
  LHAPDF_LDFLAGS="`$LHAPDF_CONFIG --ldflags`"
  LHAPDF_LIBDIR="`$LHAPDF_CONFIG --libdir`"
  LHAPDF_LHAPATH="`$LHAPDF_CONFIG --pdfsets-path`"
  
  export LHAPDF_CPPFLAGS
  export LHAPDF_LDFLAGS
  export LHAPDF_LIBDIR
  export LHAPDF_LHAPATH
  AC_SUBST([LHAPDF_CPPFLAGS])
  AC_SUBST([LHAPDF_LDFLAGS])
  AC_SUBST([LHAPDF_LIBDIR])
  AC_SUBST([LHAPDF_LHAPATH])
  
  AC_MSG_NOTICE([LHAPDF_CPPFLAGS = $LHAPDF_CPPFLAGS])
  AC_MSG_NOTICE([LHAPDF_LDFLAGS = $LHAPDF_LDFLAGS])
  
  $1
else
  AC_MSG_NOTICE([LHAPDF not found!!])
  $2
fi
])
