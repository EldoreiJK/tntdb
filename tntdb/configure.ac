# This is upstream tntdb-1.3rc3 with tweaks for 42ity project
# The original version string drove the pkgconfig parser mad
# (it is not ">= 1.3.0" that we require in configre scripts)
AC_INIT(tntdb, 1.3.0, [Tommi Maekitalo <tommi@tntnet.org>])
AM_INIT_AUTOMAKE

abi_current=4
abi_revision=0
abi_age=0
sonumber=${abi_current}:${abi_revision}:${abi_age}
AC_SUBST(sonumber)
AC_SUBST(abi_current)

unset CDPATH

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([include/tntdb/config.h])
AC_CONFIG_SRCDIR([src/connect.cpp])

AC_PROG_CXX
AC_LANG(C++)

AX_COMPILER_VENDOR
AS_IF([test "$ax_cv_cxx_compiler_vendor" = "ibm"],
      [CPPFLAGS="$CPPFLAGS -qrtti -qlanglvl=newexcp -D__NOLOCK_ON_INPUT -D__NOLOCK_ON_OUTPUT"],
      AX_CHECK_COMPILE_FLAG([-Wno-long-long], [CPPFLAGS="$CPPFLAGS -Wno-long-long"])
      AX_CHECK_COMPILE_FLAG([-Wall], [CPPFLAGS="$CPPFLAGS -Wall"])
      AX_CHECK_COMPILE_FLAG([-pedantic], [CPPFLAGS="$CPPFLAGS -pedantic"]))

AC_PROG_LIBTOOL

AC_MSG_CHECKING([for sighandler_t])
AC_COMPILE_IFELSE(
  [AC_LANG_SOURCE([
    #include <signal.h>
    sighandler_t *f;])],
  has_sighandler_t=yes,has_sighandler_t=no)

AC_MSG_RESULT($has_sighandler_t)
if test "$has_sighandler_t" = "yes" ; then
    AC_DEFINE( HAVE_SIGHANDLER_T, 1, [Define if sighandler_t available] )
fi

#
# check standard integer types
#
AC_TYPE_LONG_LONG_INT
AC_TYPE_UNSIGNED_LONG_LONG_INT
if test "$ac_cv_type_long_long_int" = yes; then
    HAVE_LONG_LONG=HAVE_LONG_LONG
else
    HAVE_LONG_LONG=NO_LONG_LONG
fi

AC_SUBST(HAVE_LONG_LONG, "$HAVE_LONG_LONG")

if test "$ac_cv_type_unsigned_long_long_int" = yes; then
    HAVE_UNSIGNED_LONG_LONG=HAVE_UNSIGNED_LONG_LONG
else
    AC_MSG_WARN([unsigned long long not found ($ac_cv_type_unsigned_long_long_int)]);
    HAVE_UNSIGNED_LONG_LONG=NO_UNSIGNED_LONG_LONG
fi
AC_SUBST(HAVE_UNSIGNED_LONG_LONG, "$HAVE_UNSIGNED_LONG_LONG")


AC_COMPILE_IFELSE(
  [AC_LANG_SOURCE([
    #include <stdint.h>
    void foo(int) {}
    void foo(long) {}
    void foo(int32_t) {}
    ])],
INT_INT32_T_CONFLICT=0,
INT_INT32_T_CONFLICT=1)
AC_SUBST(INT_INT32_T_CONFLICT)

AC_COMPILE_IFELSE(
  [AC_LANG_SOURCE([
    #include <stdint.h>
    void foo(unsigned) {}
    void foo(unsigned long) {}
    void foo(uint32_t) {}
    ])],
UNSIGNED_UINT32_T_CONFLICT=0,
UNSIGNED_UINT32_T_CONFLICT=1)
AC_SUBST(UNSIGNED_UINT32_T_CONFLICT)

AC_COMPILE_IFELSE(
  [AC_LANG_SOURCE([
    #include <stdint.h>
    void foo(int) {}
    void foo(long) {}
    void foo(int64_t) {}
    ])],
INT_INT64_T_CONFLICT=0,
INT_INT64_T_CONFLICT=1)
AC_SUBST(INT_INT64_T_CONFLICT)

AC_COMPILE_IFELSE(
  [AC_LANG_SOURCE([
    #include <stdint.h>
    void foo(unsigned) {}
    void foo(unsigned long) {}
    void foo(uint64_t) {}
    ])],
UNSIGNED_UINT64_T_CONFLICT=0,
UNSIGNED_UINT64_T_CONFLICT=1)
AC_SUBST(UNSIGNED_UINT64_T_CONFLICT)

#
# Cxxtools
#
AC_CHECK_HEADER([cxxtools/condition.h], , AC_MSG_ERROR([cxxtools headers not found]))
LDFLAGS="$LDFLAGS -lcxxtools"

#
# Postgresql
#
AC_ARG_WITH(
  [postgresql],
  AS_HELP_STRING([--with-postgresql],
                 [build postgresql-driver (default: yes)]),
  [with_postgresql=$withval],
  [with_postgresql=yes])

if test "$with_postgresql" = yes
then
  AC_PATH_PROGS(PG_CONFIG, pg_config)
  if ! test -x "$PG_CONFIG"; then
    AC_MSG_ERROR([postgresql configuration script pg_config was not found])
  fi

  PG_CXXFLAGS=-I`$PG_CONFIG --includedir`
  PG_LDFLAGS="-L`$PG_CONFIG --libdir` -lpq"
  AC_SUBST(PG_CONFIG)
  AC_SUBST(PG_CXXFLAGS)
  AC_SUBST(PG_LDFLAGS)

  AC_MSG_CHECKING([for PQprepare in libpq-fe.h])
  save_CXXFLAGS=$CXXFLAGS
  CXXFLAGS="$CXXFLAGS $PG_CXXFLAGS"
  AC_COMPILE_IFELSE(
    [AC_LANG_SOURCE([#include <libpq-fe.h>
      void t() { PQprepare(0, 0, 0, 0, 0); }])],
    [AC_DEFINE(HAVE_PQPREPARE, 1, [Defined if postgresql supports PQprepare])
     AC_MSG_RESULT(yes)
    ],
     AC_MSG_RESULT(no))
  CXXFLAGS=$save_CXXFLAGS
fi

AM_CONDITIONAL(MAKE_POSTGRESQL, test "$with_postgresql" = yes)

#
# Replicate
#
AC_ARG_WITH(
  [replicate],
  AS_HELP_STRING([--with-replicate],
                 [build replicate-driver (default: yes)]),
  [with_replicate=$withval],
  [with_replicate=yes])

AM_CONDITIONAL(MAKE_REPLICATE, test "$with_replicate" = yes)

#
# mysql
#
AC_ARG_WITH(
  [mysql],
  AS_HELP_STRING([--with-mysql],
                 [build mysql-driver (default: yes)]),
  [ with_mysql=$withval ],
  [ with_mysql=yes ])

if test "$with_mysql" = yes
then
  AC_PATH_PROGS(MYSQL_CONFIG, mysql_config)
  if ! test -x "$MYSQL_CONFIG"; then
    AC_MSG_ERROR([mysql configuration script was not found])
  fi

  MYSQL_CFLAGS=`$MYSQL_CONFIG --cflags`
  MYSQL_LDFLAGS=`$MYSQL_CONFIG --libs_r`

  AC_SUBST(MYSQL_CONFIG)
  AC_SUBST(MYSQL_CFLAGS)
  AC_SUBST(MYSQL_LDFLAGS)
fi

AM_CONDITIONAL(MAKE_MYSQL, test "$with_mysql" = yes)

#
# Sqlite
#
AC_ARG_WITH(
  [sqlite],
  AS_HELP_STRING([--with-sqlite],
                 [build sqlite-driver (default: yes)]),
  [with_sqlite=$withval],
  [with_sqlite=yes])

if test "$with_sqlite" = yes
then
  AC_CHECK_HEADER([sqlite3.h], , AC_MSG_ERROR([header for sqlite3 not found]))

  # check for sqlite3_transfer_bindings
  AC_COMPILE_IFELSE(
    [AC_LANG_SOURCE([#include <sqlite3.h>
      void t() { sqlite3_transfer_bindings(0, 0); }])],
    [AC_MSG_RESULT(yes)],
    [AC_MSG_ERROR([sqlite3_transfer_bindings is missing])])

fi

AM_CONDITIONAL(MAKE_SQLITE, test "$with_sqlite" = yes)

#
# Oracle
#
AC_ARG_WITH(
  [oracle],
  AS_HELP_STRING([--with-oracle],
                 [build oracle-driver (default: no)]),
  [with_oracle=$withval],
  [with_oracle=no])

if test "$with_oracle" = yes
then
  AC_CHECK_HEADER([oci.h], , AC_MSG_ERROR([header for oracle not found]))
fi

AM_CONDITIONAL(MAKE_ORACLE, test "$with_oracle" = yes)

#
# Doxygen-documentation
#
AC_ARG_VAR(DOXYGEN, [doxygen documentation generator])
AC_ARG_WITH(
  [doxygen],
  AS_HELP_STRING([--with-doxygen],
                 [generate doxygen documentation]),
  [doxygenFlag=$withval],
  [doxygenFlag=no])

if test "$doxygenFlag" != "no"
  then
    AC_CHECK_PROG(DOXYGEN, doxygen, doxygen)
    if test "$DOXYGEN" == ""
      then
        AC_MSG_ERROR([doxygen not found])
      fi
    AC_DEFINE(WITH_DOXYGEN, 1, [build doxygen documentation])
  fi

AM_CONDITIONAL(MAKE_DOXYGEN, test "$doxygenFlag" != "no")
AC_SUBST(DOXYGEN)

# driverlibdir
AC_ARG_WITH(
  [driverdir],
  AS_HELP_STRING([--with-driverdir=dir],
                 [specify directory for driver files [[LIBDIR/tntdb]] ]),
  [driverdir=$withval],
  [driverdir=$libdir/tntdb])
AC_SUBST(driverdir)

if test "$driverdir" == "yes"
  then
    driverdir=$libdir/tntdb
  fi

# additional shared library checks
case "${host_cpu}-${host_os}" in
*-aix*)
    SHARED_LIB_FLAG=-qmkshrobj
    ;;
*)  
    SHARED_LIB_FLAG=
    ;;
esac
AC_SUBST(SHARED_LIB_FLAG)

AC_CONFIG_FILES([
  Makefile
  pkgconfig/tntdb.pc
  src/Makefile
  src/mysql/Makefile
  src/postgresql/Makefile
  src/replicate/Makefile
  src/sqlite/Makefile
  src/oracle/Makefile
  include/Makefile
  demo/Makefile
  test/Makefile
  doc/Makefile
  doc/tntdb.doxygen
  ])

AC_OUTPUT
