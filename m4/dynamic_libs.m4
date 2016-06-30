#AC_DEFINE_DYNAMIC_LIBS()
#Defines LIB_SUFFIX to be either so or dylib for linux or mac
#Also sets LIBPATHVARNAME to either LD_LIBRARY_PATH or DYLD_LIBRARY_PATH
AC_DEFUN([AC_DEFINE_DYNAMIC_LIBS],[

  LIB_SUFFIX="so"
  LIBPATHVARNAME="LD_LIBRARY_PATH" 

  AC_CHECK_TOOL(SWVERS, sw_vers)
  if test x$SWVERS != x; then
    PROD_NAME=$($SWVERS -productName | cut -f 2 -d:)
  fi

  if test "$PROD_NAME" = "Mac OS X"; then
    LIB_SUFFIX="dylib"
    LIBPATHVARNAME="DYLD_LIBRARY_PATH"
  fi

  AC_SUBST(LIB_SUFFIX)
  AC_SUBST(LIBPATHVARNAME)
])