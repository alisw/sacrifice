#AC_GET_MCUTILS(actionIfSuccess, actionIfFail)
AC_DEFUN([AC_GET_MCUTILS],[

if test ! -f include/MCUtils/MCUtils.h ; then
  mkdir tmp
  curl https://bitbucket.org/andybuckley/mcutils/get/mcutils-1.3.2.tar.gz > tmp/mcutils-1.3.2.tar.gz
  tar -zxvf tmp/mcutils-1.3.2.tar.gz -C tmp/
  cp -r tmp/andybuckley-mcutils-c5d9a7cfd128/include/MCUtils include
  rm -r tmp
fi

if test -f include/MCUtils/MCUtils.h ; then
  $1
else
  $2
fi

if test ! -f include/HEPUtils/Vectors.h ; then
  mkdir tmp
  curl https://bitbucket.org/andybuckley/heputils/get/heputils-1.3.0.tar.gz > tmp/heputils.tar.gz
  tar -zxvf tmp/heputils.tar.gz -C tmp/
  cp -r tmp/andybuckley-heputils-301e1b37bd2f/include/HEPUtils include
  rm -r tmp
fi

if test -f include/HEPUtils/Vectors.h ; then
  $1
else
  $2
fi



])
