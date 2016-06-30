#AC_SEARCH_MCUTILS(actionIfFound, actionIfNotFound)
AC_DEFUN([AC_SEARCH_MCUTILS], [

if test x$with_MCUtils != x && test x$with_MCUtils != xyes ; then
  AC_MSG_NOTICE([Adding $with_MCUtils to search path for MCUtils])
  if test -d $with_MCUtils/include/MCUtils ; then
    found_mcutils=yes
    mcutils_include=$with_MCUtils/include
  else
    found_mcutils=no
  fi
  
else
  toplevel=`pwd`
  mcutils_include=$toplevel/include/
  found_mcutils=yes
fi


# final check that the headers and libraries are actually there

if test x$found_mcutils = xyes ; then
  if test -f $mcutils_include/MCUtils/HepMCUtils.h ; then
    MCUTILS_CPPFLAGS="-I$mcutils_include"
    
  else
    found_mcutils=no
  fi
fi

if test x$found_mcutils = xyes ; then
  
  export MCUTILS_CPPFLAGS
  AC_SUBST([MCUTILS_CPPFLAGS])
  AC_MSG_NOTICE([Found MCUtils header package])
  AC_MSG_NOTICE([MCUTILS_CPPFLAGS = $MCUTILS_CPPFLAGS])
  $1
else
  AC_MSG_NOTICE([Could not find MCUtils headers])
  $2
fi

])
