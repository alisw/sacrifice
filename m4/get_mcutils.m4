#AC_GET_MCUTILS(actionIfSuccess, actionIfFail)
AC_DEFUN([AC_GET_MCUTILS],[

if test ! -f include/MCUtils/MCUtils.h ; then
  mkdir tmp
  curl https://bitbucket.org/andybuckley/mcutils/get/mcutils-1.0.0.tar.gz > tmp/mcutils-1.0.0.tar.gz
  tar -zxvf tmp/mcutils-1.0.0.tar.gz -C tmp/
  cp -r tmp/andybuckley-mcutils-5da172bcca92/include/MCUtils include
  rm -r tmp
fi

if test -f include/MCUtils/MCUtils.h ; then
  $1
else
  $2
fi

])
