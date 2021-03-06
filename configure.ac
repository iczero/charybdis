dnl TODO: clean up all the OpenSSL and shared module checking stuff;
dnl the most major changes have already been made and it looks like
dnl said functions need to be just about as complex as they already are.

AC_PREREQ(2.63)
AUTOMAKE_OPTIONS = 1.11

AC_INIT([charybdis], [5-dev])
AC_GNU_SOURCE
AC_USE_SYSTEM_EXTENSIONS
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([libltdl/config])
AC_PREFIX_DEFAULT($HOME/ircd)

AC_PATH_PROG(AUTOMAKE, automake)
AC_PATH_PROG(ACLOCAL, aclocal)
AC_PATH_PROG(AUTOHEADER, autoheader)
AC_PATH_PROG(AS, as)
AC_PATH_PROG(RM, rm)
AC_PATH_PROG(CP, cp)
AC_PATH_PROG(MV, mv)
AC_PATH_PROG(LN, ln)
AC_PATH_PROG(TOUCH, touch)

LT_INIT([dlopen shared disable-static])
LT_LANG([C++])

AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_CXX_C_O
AC_LANG(C++)
AC_CONFIG_HEADER(include/rb/config.h)
AX_CXX_COMPILE_STDCXX([14], [gnu], [mandatory], 201300)

LT_CONFIG_LTDL_DIR([libltdl])
LTDL_INIT([subproject])

AC_PROG_YACC
dnl AC_PROG_YACC defaults to yacc unconditionally if nothing can be found
if test "$YACC" = "yacc" -a -z "`which $YACC 2>/dev/null`"; then
	AC_MSG_ERROR([could not locate a suitable parser generator; install bison, yacc, or byacc])
fi

AC_PROG_LEX
if test "$LEX" = ":"; then
	AC_MSG_ERROR([could not locate a suitable lexical generator, install flex or lex.])
fi


dnl ***************************************************************************
dnl Makefile tree
dnl These (and Makefile.in) are generated from a Makefile.am in the same dir.
dnl

AC_CONFIG_FILES(\
	Makefile                \
	include/rb/Makefile     \
	rb/Makefile             \
	authd/Makefile          \
	charybdis/Makefile      \
	ircd/Makefile           \
	bandb/Makefile          \
	doc/Makefile            \
	extensions/Makefile     \
	help/Makefile           \
	modules/Makefile        \
	ssld/Makefile           \
	tools/Makefile          \
	tools/genssl            \
	wsockd/Makefile         \
)

AM_INIT_AUTOMAKE([subdir-objects])


dnl
dnl Recursive local targets (clean-local is implied)
dnl

#AM_EXTRA_RECURSIVE_TARGETS([
#	mrproper
#])


dnl
dnl General variables exported to Makefile.am
dnl

AC_SUBST(CC)
AC_SUBST(CXX)
AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)


dnl
dnl Coarse compiler test macros
dnl

AM_CONDITIONAL([GCC],        [[[[ $CXX = g\+\+* ]]]])
AM_CONDITIONAL([CLANG],      [[[[ $CXX = clang* ]]]])


dnl
dnl Coarse operating system test macros
dnl

AM_CONDITIONAL([LINUX],      [[[[ $host_os = *linux* ]]]])
AM_CONDITIONAL([NETBSD],     [[[[ $host_os = *netbsd* ]]]])
AM_CONDITIONAL([FREEBSD],    [[[[ $host_os = *freebsd* ]]]])
AM_CONDITIONAL([MINGW],      [[[[ $host_os = *mingw* ]] || [[ $host_os = *msys* ]]]])
AM_CONDITIONAL([CYGWIN],     [[[[ $host_os = *cygwin* ]]]])
AM_CONDITIONAL([INTERIX],    [[[[ $host_os = *interix* ]]]])
AM_CONDITIONAL([SOLARIS],    [[[[ $host_os = *solaris* ]]]])



dnl ***************************************************************************
dnl Preprocessor define macros
dnl
dnl Use RB_DEFINE() et al to generate defines in config.h
dnl Use CPPDEFINE() to generate defines on the command line if required
dnl

AC_DEFUN([RB_DEFINE],
[
	AC_DEFINE([RB_$1], [$2], [$3])
])

AC_DEFUN([RB_DEFINE_UNQUOTED],
[
	AC_DEFINE_UNQUOTED([RB_$1], [$2], [$3])
])

AC_DEFUN([IRCD_DEFINE],
[
	AC_DEFINE([IRCD_$1], [$2], [$3])
])

AC_DEFUN([IRCD_DEFINE_UNQUOTED],
[
	AC_DEFINE_UNQUOTED([IRCD_$1], [$2], [$3])
])

AC_DEFUN([CPPDEFINE],
[
	if [[ -z "$2" ]]; then
		CPPFLAGS+="-D$1 $CPPFLAGS"
	else
		CPPFLAGS="-D$1=$2 $CPPFLAGS"
	fi
])



dnl ***************************************************************************
dnl Development options
dnl


AC_MSG_CHECKING(if you want to enable debugging)
AC_ARG_ENABLE(debug, AC_HELP_STRING([--enable-debug], [Enable debugging suite for development.]),
[
	AC_SUBST(DEBUG, 1)
	CXXFLAGS="$CXXFLAGS -O0 -ggdb"
	RB_DEFINE_UNQUOTED([DEBUG], [1], [Not configured for release when lit.])
], [
	AC_SUBST(DEBUG, 0)
	CXXFLAGS="$CXXFLAGS -O3"
	CPPDEFINE([NDEBUG])
])


AC_MSG_CHECKING(if you want to do a profile build)
AC_ARG_ENABLE(profile, AC_HELP_STRING([--enable-profile], [Enable profiling]),
[
	if test "$ac_cv_c_compiler_gnu" = yes; then
		AC_MSG_RESULT([yes, adding -pg])
		CXXFLAGS="$CXXFLAGS -pg"
		AC_DEFINE(CHARYBDIS_PROFILE, 1, [Define this if you are profiling.])
	else
		AC_MSG_RESULT([no, profile builds only work with gcc])
	fi
], [
])


AC_MSG_CHECKING(if you want to disable precompiled headers)
AC_ARG_ENABLE(pch, AC_HELP_STRING([--disable-pch], [Disable precompiled header generation]),
[
	build_pch="no"
	AC_MSG_RESULT([no])
], [
	build_pch="yes"
	CPPDEFINE([PCH])
	AC_MSG_RESULT([yes])
])

AM_CONDITIONAL([BUILD_PCH], [[[[ "$build_pch" = "yes" ]]]])


CWARNS=""
AC_MSG_CHECKING(if you want to disable warnings)
AC_ARG_ENABLE(warnings,
AC_HELP_STRING([--disable-warnings],[Disable all sorts of warnings like a rockstar]),
[],
[
CHARYBDIS_C_GCC_TRY_FLAGS([-Wall], charybdis_cv_c_gcc_w_all)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wextra], charybdis_cv_c_gcc_w_extra)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wpointer-arith], charybdis_cv_c_gcc_w_pointer_arith)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wcast-align], charybdis_cv_c_gcc_w_cast_align)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wcast-qual], charybdis_cv_c_gcc_w_cast_qual)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wfloat-equal], charybdis_cv_c_gcc_w_float_equal)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wwrite-strings], charybdis_cv_c_gcc_w_write_strings)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wno-missing-field-initializers], charybdis_cv_c_gcc_w_missing_field_initializers)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wparentheses], charybdis_cv_c_gcc_parentheses)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wmissing-noreturn], charybdis_cv_c_gcc_w_missing_noreturn)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wundef], charybdis_cv_c_gcc_w_undef)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wpacked], charybdis_cv_c_gcc_w_packed)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wno-unused -Wno-unused-function -Wno-unused-label -Wno-unused-value -Wno-unused-variable -Wno-unused-parameter], charybdis_cv_c_gcc_w_unused)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wformat -Wformat-y2k -Wformat-security -Wformat-nonliteral], charybdis_cv_c_gcc_w_format)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wstrict-aliasing -Wstrict-overflow], charybdis_cv_c_gcc_w_strict)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wdisabled-optimization], charybdis_cv_c_gcc_w_disabled_optimization)
CHARYBDIS_C_GCC_TRY_FLAGS([-Winvalid-pch], charybdis_cv_c_gcc_w_invalid_pch)
CHARYBDIS_C_GCC_TRY_FLAGS([-Winit-self], charybdis_cv_c_gcc_w_init_self)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wuninitialized], charybdis_cv_c_gcc_w_uninitialized)
CHARYBDIS_C_GCC_TRY_FLAGS([-Wunreachable-code], charybdis_cv_c_gcc_w_unreachable_code)

AM_COND_IF([GCC],
[
	CHARYBDIS_C_GCC_TRY_FLAGS([-Wlogical-op], charybdis_cv_c_gcc_w_logical_op)
	CHARYBDIS_C_GCC_TRY_FLAGS([-Wsuggest-attribute=format], charybdis_cv_c_gcc_w_suggest_attribute_format)
],[])

])

CPPFLAGS+="$CWARNS"



dnl ***************************************************************************
dnl Header file checks
dnl

AC_DEFUN([RB_CHK_SYSHEADER],
[
	AC_CHECK_HEADER([$1],
	[
		AC_DEFINE([HAVE_$2], [1], [ Indication $1 is available. ])
		RB_DEFINE_UNQUOTED([INC_$2], [$1>], [ The computed-include location of $1. ])
	], [
		RB_DEFINE_UNQUOTED([INC_$2], [stddef.h>], [ The dead-header in place of $1. ])
	])
])

AC_HEADER_STDBOOL
dnl AC_HEADER_STDC
RB_CHK_SYSHEADER([errno.h],        [ERRNO_H])
RB_CHK_SYSHEADER([assert.h],       [ASSERT_H])
RB_CHK_SYSHEADER([stddef.h],       [STDDEF_H])
RB_CHK_SYSHEADER([stdarg.h],       [STDARG_H])
RB_CHK_SYSHEADER([stdint.h],       [STDINT_H])
RB_CHK_SYSHEADER([inttypes.h],     [INTTYPES_H])
RB_CHK_SYSHEADER([ctype.h],        [CTYPE_H])
RB_CHK_SYSHEADER([limits.h],       [LIMITS_H])
RB_CHK_SYSHEADER([stdlib.h],       [STDLIB_H])
RB_CHK_SYSHEADER([unistd.h],       [UNISTD_H])
RB_CHK_SYSHEADER([time.h],         [TIME_H])
RB_CHK_SYSHEADER([fcntl.h],        [FCNTL_H])
RB_CHK_SYSHEADER([signal.h],       [SIGNAL_H])
RB_CHK_SYSHEADER([dirent.h],       [DIRENT_H])
RB_CHK_SYSHEADER([sys/types.h],    [SYS_TYPES_H])
RB_CHK_SYSHEADER([sys/time.h],     [SYS_TIME_H])
RB_CHK_SYSHEADER([sys/stat.h],     [SYS_STAT_H])
RB_CHK_SYSHEADER([sys/file.h],     [SYS_FILE_H])
RB_CHK_SYSHEADER([sys/param.h],    [SYS_PARAM_H])
RB_CHK_SYSHEADER([sys/resource.h], [SYS_RESOURCE_H])
RB_CHK_SYSHEADER([sys/socket.h],   [SYS_SOCKET_H])
RB_CHK_SYSHEADER([sys/syslog.h],   [SYS_SYSLOG_H])
RB_CHK_SYSHEADER([sys/wait.h],     [SYS_WAIT_H])
RB_CHK_SYSHEADER([wait.h],         [WAIT_H])
RB_CHK_SYSHEADER([arpa/inet.h],    [ARPA_INET_H])
RB_CHK_SYSHEADER([netinet/in.h],   [NETINET_IN_H])
RB_CHK_SYSHEADER([netinet/tcp.h],  [NETINET_TCP_H])
RB_CHK_SYSHEADER([string.h],       [STRING_H])
RB_CHK_SYSHEADER([strings.h],      [STRINGS_H])
RB_CHK_SYSHEADER([stdio.h],        [STDIO_H])
RB_CHK_SYSHEADER([crypt.h],        [CRYPT_H])
RB_CHK_SYSHEADER([sys/uio.h],      [SYS_UIO_H])
RB_CHK_SYSHEADER([spawn.h],        [SPAWN_H])
RB_CHK_SYSHEADER([sys/poll.h],     [SYS_POLL_H])
RB_CHK_SYSHEADER([sys/epoll.h],    [SYS_EPOLL_H])
RB_CHK_SYSHEADER([sys/select.h],   [SYS_SELECT_H])
RB_CHK_SYSHEADER([sys/devpoll.h],  [SYS_DEVPOLL_H])
RB_CHK_SYSHEADER([sys/event.h],    [SYS_EVENT_H])
RB_CHK_SYSHEADER([port.h],         [PORT_H])
RB_CHK_SYSHEADER([sys/signalfd.h], [SYS_SIGNALFD_H])
RB_CHK_SYSHEADER([sys/timerfd.h],  [SYS_TIMERFD_H])
RB_CHK_SYSHEADER([execinfo.h],     [EXECINFO_H])
RB_CHK_SYSHEADER([machine/endian.h], [MACHINE_ENDIAN_H])

RB_CHK_SYSHEADER([cstddef],        [CSTDDEF])
RB_CHK_SYSHEADER([cstdint],        [CSTDINT])
RB_CHK_SYSHEADER([limits],         [LIMITS])
RB_CHK_SYSHEADER([type_traits],    [TYPE_TRAITS])
RB_CHK_SYSHEADER([utility],        [UTILITY])
RB_CHK_SYSHEADER([functional],     [FUNCTIONAL])
RB_CHK_SYSHEADER([algorithm],      [ALGORITHM])
RB_CHK_SYSHEADER([memory],         [MEMORY])
RB_CHK_SYSHEADER([exception],      [EXCEPTION])
RB_CHK_SYSHEADER([cerrno],         [CERRNO])
RB_CHK_SYSHEADER([system_error],   [SYSTEM_ERROR])
RB_CHK_SYSHEADER([map],            [MAP])
RB_CHK_SYSHEADER([set],            [SET])
RB_CHK_SYSHEADER([list],           [LIST])
RB_CHK_SYSHEADER([deque],          [DEQUE])
RB_CHK_SYSHEADER([array],          [ARRAY])
RB_CHK_SYSHEADER([vector],         [VECTOR])
RB_CHK_SYSHEADER([forward_list],   [FORWARD_LIST])
RB_CHK_SYSHEADER([unordered_map],  [UNORDERED_MAP])
RB_CHK_SYSHEADER([string],         [STRING])
RB_CHK_SYSHEADER([sstream],        [SSTREAM])
RB_CHK_SYSHEADER([fstream],        [FSTREAM])
RB_CHK_SYSHEADER([iostream],       [IOSTREAM])
RB_CHK_SYSHEADER([cstdio],         [CSTDIO])

RB_CHK_SYSHEADER([windows.h],      [WINDOWS_H])
RB_CHK_SYSHEADER([winsock2.h],     [WINSOCK2_H])
RB_CHK_SYSHEADER([ws2tcpip.h],     [WS2TCPIP_H])
RB_CHK_SYSHEADER([iphlpapi.h],     [IPHLPAPI_H])



dnl ***************************************************************************
dnl Philology checks
dnl

AC_ISC_POSIX
AC_C_CONST
AC_C_INLINE
if test "$ac_cv_header_machine_endian_h" = "no" ; then
    AC_C_BIGENDIAN
fi

AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_PID_T

AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)

AC_CHECK_TYPES([intptr_t])
AC_CHECK_TYPES([uintptr_t])

AC_FUNC_MMAP
AC_FUNC_VFORK
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([        \
	strlcpy             \
	strlcat             \
	strnlen             \
	strcasestr          \
	strcasecmp          \
	strncasecmp         \
	strdup              \
	strndup             \
	strtok_r            \
	snprintf            \
	vsnprintf           \
	gettimeofday        \
	getrusage           \
	gmtime_r            \
	usleep              \
	fstat               \
	writev              \
	sendmsg             \
	socketpair          \
	signalfd            \
	timerfd_create      \
	port_create         \
	epoll_ctl           \
	kevent              \
	poll                \
	select              \
	getexecname         \
	posix_spawn         \
	arc4random          \
])

AC_SEARCH_LIBS(dlinfo, dl, AC_DEFINE(HAVE_DLINFO, 1, [Define if you have dlinfo]))
AC_SEARCH_LIBS(nanosleep, rt posix4, AC_DEFINE(HAVE_NANOSLEEP, 1, [Define if you have nanosleep]))
AC_SEARCH_LIBS(timer_create, rt, AC_DEFINE(HAVE_TIMER_CREATE, 1, [Define if you have timer_create]))


dnl
dnl Networking Functions
dnl

AC_SEARCH_LIBS(socket, [socket],,)
AC_CHECK_MEMBER([struct sockaddr.sa_len], [AC_DEFINE(SOCKADDR_IN_HAS_LEN, 1, [Define to 1 if sockaddr has a 'sa_len' member.])],,[[
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
]])

AC_CHECK_TYPE([socklen_t], rb_cv_socklen_t=socklen_t,[
	AC_MSG_CHECKING([for socklen_t equivalent])
	AC_CACHE_VAL([rb_cv_socklen_t],
	[
	# Systems have either "struct sockaddr *" or
	# "void *" as the second argument to getpeername
	rb_cv_socklen_t=
	for arg2 in "struct sockaddr" void; do
		for t in int size_t unsigned long "unsigned long"; do
			AC_TRY_COMPILE([
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
int getpeername (int, $arg2 *, $t *);
],[
$t len;
getpeername(0,0,&len);
],[
	rb_cv_socklen_t="$t"
	break
])
		done
	done

	if test "x$rb_cv_socklen_t" = x; then
		AC_MSG_WARN([Cannot find a type to use in place of socklen_t using int])
		rb_cv_socklen_t=int
	fi
	])
	AC_MSG_RESULT($rb_cv_socklen_t)],
	[#include <sys/types.h>
#include <sys/socket.h>])

rb_socklen_t=$rb_cv_socklen_t

AC_CHECK_TYPE([sa_family_t], [],
[AC_DEFINE(sa_family_t, [u_int16_t], [If system does not define sa_family_t, define it here.])],
[[#include <sys/types.h>
#include <sys/socket.h>]])

AC_ARG_ENABLE(ipv6, AC_HELP_STRING([--disable-ipv6], [Disable IPv6 support]), [ipv6=$enableval], [ipv6=yes])

dnl TODO: IPV6 CHECKS

if test x$ipv6 = "Xyes"; then
	AC_DEFINE([HAVE_IPV6], [1], [IPv6 is supported])
else
	AC_DEFINE([HAVE_IPV6], [0], [IPv6 not supported])
fi

AC_CHECK_TYPES([struct sockaddr_storage],[
	rb_have_sockaddr_storage=yes
        ],[], [
		#include <sys/types.h>
		#include <sys/socket.h>
	])


AC_MSG_CHECKING(for /dev/poll)
if test -c "/dev/poll"; then
        AC_MSG_RESULT(yes)
	AC_DEFINE(HAVE_DEVPOLL, [1], [Define to 1 if you have devpoll])
else
	AC_MSG_RESULT(no)
fi


dnl SunOS/Solaris required libnsl for inet_ntoa()
if test x"$SUN" = xyes; then
	AC_SEARCH_LIBS(inet_ntoa, nsl,, [AC_MSG_ERROR([libnsl not found! Aborting.])])
fi

AC_SEARCH_LIBS(crypt, [crypt descrypt],,)

CRYPT_LIB=$ac_cv_search_crypt

if test "$CRYPT_LIB" = "none required"; then
	unset CRYPT_LIB
elif test "$CRYPT_LIB" = no; then
	unset CRYPT_LIB
fi

AC_SUBST(CRYPT_LIB)

dnl definitions are in m4/charybdis.m4
RB_CHECK_TIMER_CREATE
RB_CHECK_TIMERFD_CREATE

AC_CHECK_TYPE([sa_family_t], [],
[AC_DEFINE(sa_family_t, [u_int16_t], [If system does not define sa_family_t, define it here.])],
[[#include <sys/types.h>
#include <sys/socket.h>]])

dnl check for various functions...


dnl check for nanosleep
AC_CHECK_FUNC(nanosleep,,[AC_CHECK_LIB(rt,nanosleep,
                 LIBS="${LIBS} -lrt",
                 [AC_CHECK_LIB(posix4,nanosleep, LIBS="${LIBS} -lposix4"
                 )])])
if test x$ac_cv_func_nanosleep = xno && test x$ac_cv_lib_posix4_nanosleep = xno  && test x$ac_cv_lib_rt_nanosleep = xno
then
         AC_MSG_RESULT("nanosleep not found..using select for delay")
else
	 AC_DEFINE([HAVE_NANOSLEEP], 1, [Define if nanosleep exists])
fi

dnl check for /dev/null so we can use it to hold evil fd's
AC_MSG_CHECKING([for /dev/null])
if test -c /dev/null ; then
	AC_DEFINE(PATH_DEVNULL, "/dev/null", [Path to /dev/null])
	AC_MSG_RESULT(yes)
else
	AC_DEFINE(PATH_DEVNULL, "devnull.log", [Path to /dev/null])
	AC_MSG_RESULT(no - using devnull.log)
fi



dnl ***************************************************************************
dnl TODO: sort this OS-specific section
dnl

AM_COND_IF(MINGW,
[
	AC_DEFINE(HAVE_WIN32, [1], [Define to 1 if you are on windows])
	LIBS="$LIBS -lws2_32 -liphlpapi"
],[])


AM_COND_IF(INTERIX,
[
	CPPDEFINE([_ALL_SOURCE])
	CPPDEFINE([_XOPEN_SOURCE], [500])
	CPPDEFINE([_POSIX_PTHREAD_SEMANTICS])
	CPPDEFINE([_XPG4_2])
],[])



dnl ***************************************************************************
dnl Installation Layout
dnl

PKG_PROG_PKG_CONFIG

if test "$prefix" = "NONE"; then
	AC_DEFINE_UNQUOTED(RB_PREFIX, "$ac_default_prefix", [Prefix where librb is installed.])
else
	dnl Don't get bitten by Cygwin's stupidity if the user specified
	dnl a custom prefix with a trailing slash
	prefix=`echo $prefix | sed 's/\/$//'`
	AC_DEFINE_UNQUOTED(RB_PREFIX, "$prefix", [Prefix where librb is installed.])
	AC_SUBST(RB_PREFIX)
fi


if test "$prefix" = "NONE"; then
	AC_DEFINE_UNQUOTED(IRCD_PREFIX, "$ac_default_prefix", [Prefix where the ircd is installed.])
else
	dnl Don't get bitten by Cygwin's stupidity if the user specified
	dnl a custom prefix with a trailing slash
	prefix=`echo $prefix | sed 's/\/$//'`
	AC_DEFINE_UNQUOTED(IRCD_PREFIX, "$prefix", [Prefix where the ircd is installed.])
fi


AC_ARG_ENABLE([fhs-paths],
	[AS_HELP_STRING([--enable-fhs-paths], [Use more FHS-like pathnames (for packagers).])],
	[],
	[dnl detect if the user appears to want --enable-fhs-paths
	AS_IF([test "$libexecdir" = '${exec_prefix}/libexec' && \
		test "$localstatedir" = '${prefix}/var' && \
		test "$libdir" = '${exec_prefix}/lib'],
		[enable_fhs_paths=no],
		[enable_fhs_paths=yes])
	])
dnl use directory structure of cached as default (hack)
AS_IF([test "x$enable_fhs_paths" = "xyes"],
	[dnl Avoid name collisions.
	pkglibexecdir='${libexecdir}/${PACKAGE_TARNAME}'
	rundir=${rundir-'${prefix}/run'}
	pkgrundir='${rundir}/${PACKAGE_TARNAME}'
	pkglocalstatedir='${localstatedir}/${PACKAGE_TARNAME}'
	AC_DEFINE([ENABLE_FHS_PATHS], [1], [Uncomment if FHS pathnames are enabled])],
	[libexecdir='${bindir}'
	pkglibexecdir='${libexecdir}'
	rundir='${sysconfdir}'
	pkgrundir='${rundir}'
	localstatedir='${prefix}'
	pkglocalstatedir='${sysconfdir}'])
pkglibdir='${libdir}/${PACKAGE_TARNAME}'
AC_SUBST([pkglibdir])
AC_SUBST([rundir])
AC_SUBST([pkgrundir])
AC_SUBST([pkglocalstatedir])
AC_DEFINE_DIR([PKGLOCALSTATEDIR], [pkglocalstatedir], [[Directory in which to store state, such as ban database]])
AC_SUBST([pkglibexecdir])
AC_DEFINE_DIR([PKGLIBEXECDIR], [pkglibexecdir], [Directory where binaries the IRCd itself spawns live])


dnl
dnl Check for --with-confdir [deprecated, use --sysconfdir instead]
dnl

AC_ARG_WITH([confdir],
	[AC_HELP_STRING([--with-confdir=DIR],
	       [Directory to install config files [deprecated, use --sysconfdir instead].])],
	[ sysconfdir=`echo $withval | sed 's/\/$//'` ],
	[ confdir='etc' ])
RB_DEFINE_UNQUOTED([ETC_DIR], ["${prefix}/${confdir}"], [Prefix where config files are installed.])


dnl
dnl Check for --with-bindir
dnl

AC_MSG_CHECKING([whether to modify binary directory])
AC_ARG_WITH(bindir,
AC_HELP_STRING([--with-bindir=DIR],
	       [Directory where binary executables are placed.]),
	       [ logdir=`echo $withval | sed 's/\/$//'`
		 AC_MSG_RESULT(yes)],
	       [ AS_IF([test "x$enable_fhs_paths" = "xyes"],
			[bindir="${prefix}/bin"],
			[bindir="${prefix}/bin"])
		 AC_MSG_RESULT(no)])
RB_DEFINE_UNQUOTED([BIN_DIR], ["${bindir}"], [Directory where binary executables are to be found.])
AC_SUBST_DIR([bindir])


dnl
dnl Check for --with-logdir
dnl

AC_MSG_CHECKING([whether to modify logdir])
AC_ARG_WITH(logdir,
AC_HELP_STRING([--with-logdir=DIR],
	       [Directory where to write logfiles.]),
	       [ logdir=`echo $withval | sed 's/\/$//'`
		 AC_MSG_RESULT(yes)],
	       [ AS_IF([test "x$enable_fhs_paths" = "xyes"],
			[logdir="${prefix}/var/log"],
			[logdir="${prefix}/var/${PACKAGE_TARNAME}/log"])
		 AC_MSG_RESULT(no)])
RB_DEFINE_UNQUOTED([LOG_DIR], ["${logdir}"], [Prefix where to write logfiles.])
AC_SUBST_DIR([logdir])


dnl
dnl Check for --with-helpdir
dnl

AC_MSG_CHECKING([whether to modify helpdir])
AC_ARG_WITH(helpdir,
AC_HELP_STRING([--with-helpdir=DIR],
	       [Directory to install help files.]),
	       [ helpdir=`echo $withval | sed 's/\/$//'`
		 AC_MSG_RESULT(yes) ],
	       [ AS_IF([test "x$enable_fhs_paths" = "xyes"],
			[helpdir="${prefix}/share/help"],
			[helpdir="${prefix}/share/${PACKAGE_TARNAME}/help"])
		 AC_MSG_RESULT(no) ])
RB_DEFINE_UNQUOTED([HELP_DIR], ["${helpdir}"], [Prefix where help files are installed.])
AC_SUBST_DIR([helpdir])


dnl
dnl Check for --with-moduledir
dnl

AC_MSG_CHECKING([whether to modify moduledir])
AC_ARG_WITH(moduledir,
	[AC_HELP_STRING([--with-moduledir=DIR],
	       [Directory to install modules.])],
	       [ moduledir=`echo $withval | sed 's/\/$//'`
		 AC_MSG_RESULT(yes)],
	       [ AS_IF([test "x$enable_fhs_paths" = "xyes"],
			[moduledir="${prefix}/lib/modules"],
			[moduledir="${prefix}/lib/${PACKAGE_TARNAME}/modules"])
		 AC_MSG_RESULT(no)
		])
RB_DEFINE_UNQUOTED([MODULE_DIR], ["${moduledir}"], [Prefix where modules are installed.])
AC_SUBST_DIR([moduledir])


dnl
dnl Check for --with-rundir
dnl

AC_MSG_CHECKING([whether or modify rundir])
AC_ARG_WITH([rundir],
	[AC_HELP_STRING([--with-rundir=DIR],
		[Directory in which to store pidfile.])],
	[AC_MSG_RESULT([yes])
	rundir=`echo $withval | sed 's/\/$//'`],
	[AC_MSG_RESULT([no])
	AS_IF([test "x$enable_fhs_paths" = "xyes"],
		[rundir="${prefix}/var/run"],
		[rundir="${prefix}/${PACKAGE_TARNAME}/run"])])
AC_SUBST([rundir])
AC_DEFINE_DIR([PKGRUNDIR], ["${rundir}"], [Directory to store pidfile in.])


dnl
dnl Check for --with-program-prefix
dnl

dnl Installed utility program prefixes (does not affect binaries
dnl installed into pkglibexecdir)
AC_MSG_CHECKING([for program prefix])
AC_ARG_WITH([program-prefix],
	[AS_HELP_STRING([--with-program-prefix=], [If set, programs installed into PATH will be installed with names prefixed by this prefix.])],
	[test "x$with_program_prefix" = "xno" && with_program_prefix=],
	[with_program_prefix=])
AC_MSG_RESULT(["$with_program_prefix"])
PROGRAM_PREFIX="$with_program_prefix"
AC_SUBST([PROGRAM_PREFIX])
AC_DEFINE_UNQUOTED([PROGRAM_PREFIX], ["$with_program_prefix"], [String with which all programs intended to be in PATH are prefixed.])



dnl ***************************************************************************
dnl Third party
dnl


dnl
dnl ltdl support
dnl

AM_CONDITIONAL([BUILD_LTDL], [[[ "$with_included_ltdl" = "yes" ]]])



dnl
dnl boost support
dnl

AC_MSG_CHECKING([whether to use boost sources included here])
AC_ARG_WITH(included-boost,
AC_HELP_STRING([--with-included-boost[[[=shared]]]], [Use the boost sources from included submodule]),
[
	AC_MSG_RESULT([yes])
	AC_MSG_NOTICE([Bootstrapping boost])

	AM_COND_IF([MINGW],
	[
		BTOOLSET="mingw"
	], [
		dnl the CXX may have a space and then arguments so we must chop them
		BTOOLSET=`echo $CXX | cut -d' ' -f1`
	])

	bash tools/boostrap.sh $BTOOLSET system release shared single
	if [[ $? != 0 ]]; then
		AC_MSG_ERROR([Failed to build included boost.])
	fi

	AM_COND_IF([MINGW],
	[
		boost_libdir="$PWD/boost/stage/lib"
	], [
		boost_libdir="$PWD/boost/lib"
	])

	AC_SUBST(BOOST_CPPFLAGS, ["-isystem $PWD/boost/include"])
	AC_SUBST(BOOST_LDFLAGS, ["-L$boost_libdir"])

	boost_linkage="$withval"
	AC_MSG_CHECKING([whether to use shared boost])
	if [[ $withval = "shared" ]]; then
		AC_MSG_RESULT([yes])
		BOOST_LDFLAGS += " -Wl,-rpath -Wl,$boost_libdir"
	else
		AC_MSG_RESULT([no])
	fi

	AC_DEFINE(HAVE_LIB_BOOST_SYSTEM, 1, [Define if you have boost_system])
	AC_MSG_NOTICE([Bootstrapping boost: done])
],[
	AC_MSG_RESULT([no])

	with_included_boost="no"
	AX_BOOST_BASE([1.60], [
		AC_DEFINE(HAVE_LIB_BOOST_SYSTEM, 1, [Define if you have boost_system])
	], [
		AC_MSG_ERROR([Failed to find boost_system library. (try --with-included-boost)])
	])
	boost_linkage="shared"
	AX_BOOST_ASIO
	AX_BOOST_SYSTEM
])

if [[ "$boost_linkage" = "shared" ]]; then
	AM_COND_IF([MINGW],
	[
		AC_SUBST(BOOST_LIBS, ["-lboost_system-mgw53-1_61.dll"])
	], [
		AC_SUBST(BOOST_LIBS, ["-lboost_system"])
	])
else
	AM_COND_IF([MINGW],
	[
		AC_SUBST(BOOST_LIBS, ["$boost_libdir/libboost_system-mgw53-1_61.dll.a"])
	], [
		AC_SUBST(BOOST_LIBS, ["$boost_libdir/libboost_system.a"])
	])
fi

AC_DEFINE([HAVE_BOOST_ASIO_HPP], [1], [ boost asio. ])
RB_DEFINE_UNQUOTED([INC_BOOST_ASIO_HPP], [boost/asio.hpp>], [ boost asio. ])

AC_DEFINE([HAVE_BOOST_CONTEXT_ALL_HPP], [1], [ boost context. ])
RB_DEFINE_UNQUOTED([INC_BOOST_CONTEXT_ALL_HPP], [boost/context/all.hpp>], [ boost context. ])

AC_DEFINE([HAVE_BOOST_ASIO_SPAWN_HPP], [1], [ boost asio spawn. ])
RB_DEFINE_UNQUOTED([INC_BOOST_ASIO_SPAWN_HPP], [boost/asio/spawn.hpp>], [ boost asio spawn. ])

AC_DEFINE([HAVE_BOOST_LEXICAL_CAST_HPP], [1], [ boost asio. ])
RB_DEFINE_UNQUOTED([INC_BOOST_LEXICAL_CAST_HPP], [boost/lexical_cast.hpp>], [ boost lexical_cast. ])

AC_DEFINE([HAVE_BOOST_TOKENIZER_HPP], [1], [ boost tokenizer. ])
RB_DEFINE_UNQUOTED([INC_BOOST_TOKENIZER_HPP], [boost/tokenizer.hpp>], [ boost tokenizer. ])



dnl
dnl OpenSSL support
dnl

AC_MSG_CHECKING([for OpenSSL])
AC_ARG_ENABLE(openssl,
[AC_HELP_STRING([--enable-openssl[=DIR]],[Enable OpenSSL support (DIR optional).])
AC_HELP_STRING([--disable-openssl],[Disable OpenSSL support.])],
[cf_enable_openssl=$enableval],
[cf_enable_openssl="auto"])

if test "$cf_enable_openssl" != "no" ; then
	cf_openssl_basedir=""
	if test "$cf_enable_openssl" != "auto" &&
	test "$cf_enable_openssl" != "yes" ; then
		dnl Support for --enable-openssl=/some/place
		cf_openssl_basedir="`echo ${cf_enable_openssl} | sed 's/\/$//'`"
	else
	dnl Do the auto-probe here.  Check some common directory paths.
		for dirs in /usr/local/ssl /usr/pkg /usr/local \
		/usr/local/openssl ; do
			if test -f "${dirs}/include/openssl/opensslv.h" ; then
				cf_openssl_basedir="${dirs}"
			break
			fi
		done
		unset dirs
	fi
	dnl Now check cf_openssl_found to see if we found anything.
	if test ! -z "$cf_openssl_basedir"; then
		if test -f "${cf_openssl_basedir}/include/openssl/opensslv.h" ; then
			SSL_CFLAGS="-I${cf_openssl_basedir}/include"
			SSL_LIBS="-L${cf_openssl_basedir}/lib"
		else
		dnl OpenSSL wasn't found in the directory specified.  Naughty
		dnl administrator...
		cf_openssl_basedir=""
		fi
	else
	dnl Check for stock FreeBSD 4.x and 5.x systems, since their files
	dnl are in /usr/include and /usr/lib.  In this case, we don't want to
	dnl change INCLUDES or LIBS, but still want to enable OpenSSL.
	dnl We can't do this check above, because some people want two versions
	dnl of OpenSSL installed (stock FreeBSD 4.x/5.x and /usr/local/ssl)
	dnl and they want /usr/local/ssl to have preference.
		if test -f "/usr/include/openssl/opensslv.h" ; then
			cf_openssl_basedir="/usr"
		fi
	fi

	dnl If we have a basedir defined, then everything is okay.  Otherwise,
	dnl we have a problem.
	if test ! -z "$cf_openssl_basedir"; then
		AC_MSG_RESULT($cf_openssl_basedir)
		cf_enable_openssl="yes"
	else
		AC_MSG_RESULT([not found. Specify a correct path?])
		cf_enable_openssl="no"
	fi
	unset cf_openssl_basedir
else
	dnl If --disable-openssl was specified
	AC_MSG_RESULT(disabled)
fi

CPPFLAGS="$CPPFLAGS $SSL_CFLAGS"
if test "$cf_enable_openssl" != no; then
	dnl Check OpenSSL version (must be 0.9.7 or above!)
	AC_MSG_CHECKING(for OpenSSL 0.9.7 or above)
	AC_RUN_IFELSE(
		[AC_LANG_PROGRAM(
	    	[#include <openssl/opensslv.h>
		#include <stdlib.h>],
		[[if (OPENSSL_VERSION_NUMBER >= 0x00907000)
		exit(0); else exit(1);]])],
	cf_enable_openssl=yes,
	cf_enable_openssl=no,
	cf_enable_openssl=no)

	if test "$cf_enable_openssl" != no; then
		AC_MSG_RESULT(found)
	else
	        AC_MSG_RESULT(no - OpenSSL support disabled)
	fi

fi

if test "$cf_enable_openssl" != no; then
	CPPFLAGS="$CPPFLAGS $SSL_LIBS"
	AC_CHECK_LIB(crypto, RAND_status,
		[cf_enable_openssl=yes],
		[cf_enable_openssl=no])
fi

if test "$cf_enable_openssl" != no; then
	CPPFLAGS="$CPPFLAGS $SSL_LIBS"
	AC_CHECK_LIB(ssl, SSL_read,
		[SSL_LIBS="$SSL_LIBS -lssl -lcrypto"],
		[cf_enable_openssl=no], [-lcrypto])
fi

AC_SUBST(SSL_SRCS_ENABLE)
AC_SUBST(SSL_INCLUDES)
AC_SUBST(SSL_CFLAGS)
AC_SUBST(SSL_LIBS)



dnl
dnl mbedTLS support
dnl

AC_ARG_ENABLE(mbedtls,
[AC_HELP_STRING([--enable-mbedtls], [Enable mbedTLS support.])
AC_HELP_STRING([--disable-mbedtls], [Disable mbedTLS support.])],
[cf_enable_mbedtls=$enableval],
[cf_enable_mbedtls="auto"])

if test "$cf_enable_mbedtls" != no; then
	LIBS="$LIBS $MBEDTLS_LIBS"
	AC_CHECK_LIB(mbedtls, mbedtls_ssl_init, [
		MBEDTLS_LIBS="$MBEDTLS_LIBS -lmbedtls -lmbedx509 -lmbedcrypto"
		cf_enable_mbedtls=yes
	], [cf_enable_mbedtls=no], [-lmbedx509 -lmbedcrypto])
fi

AC_SUBST(MBEDTLS_CFLAGS)
AC_SUBST(MBEDTLS_LIBS)



dnl
dnl GnuTLS support
dnl

AC_MSG_CHECKING(for GnuTLS)
AC_ARG_ENABLE(gnutls,
[AC_HELP_STRING([--enable-gnutls],[Enable GnuTLS support.])
AC_HELP_STRING([--disable-gnutls],[Disable GnuTLS support.])],
[cf_enable_gnutls=$enableval],
[cf_enable_gnutls="auto"])

if test "$cf_enable_gnutls" != no; then
       PKG_CHECK_MODULES(GNUTLS, [gnutls], [
               cf_enable_gnutls="yes"
       ], [cf_enable_gnutls="no"])
fi

AC_SUBST(GNUTLS_CFLAGS)
AC_SUBST(GNUTLS_LIBS)


dnl mbed TLS is given highest preference when no specified TLS library is provided
if test "$cf_enable_openssl" = "auto" -a "$cf_enable_mbedtls" = "yes"; then
       cf_enable_openssl="no"
fi

if test "$cf_enable_gnutls" = "auto" -a "$cf_enable_openssl" = "yes" -a "$cf_enable_mbedtls" = "yes"; then
       cf_enable_gnutls="no"
fi

if test x"$cf_enable_openssl" != xno; then
	AC_DEFINE(HAVE_OPENSSL,1,[Has OpenSSL])
	GNUTLS_CFLAGS=""
	GNUTLS_LIBS=""
	MBEDTLS_LIBS=""
	MBEDTLS_CFLAGS=""
	SSL_TYPE="openssl"
elif test x"$cf_enable_mbedtls" != xno; then
	AC_DEFINE(HAVE_MBEDTLS, 1, [Has mbedTLS])
	SSL_LIBS=""
	SSL_CFLAGS=""
	GNUTLS_CFLAGS=""
	GNUTLS_LIBS=""
	SSL_TYPE="mbedtls"
elif test x"$cf_enable_gnutls" != xno; then
	AC_DEFINE(HAVE_GNUTLS, 1, [Has GnuTLS])
	SSL_LIBS=""
	SSL_CFLAGS=""
	MBEDTLS_LIBS=""
	MBEDTLS_CFLAGS=""
	SSL_TYPE="gnutls"
fi



dnl
dnl zlib support
dnl

AC_ARG_WITH(zlib-path,
AC_HELP_STRING([--with-zlib-path=DIR],[Path to libz.so for ziplinks support.]),
[LIBS="$LIBS -L$withval"],)

AC_ARG_ENABLE(zlib,
AC_HELP_STRING([--disable-zlib],[Disable ziplinks support]),
[zlib=$enableval],[zlib=yes])

if test "$zlib" = yes; then

AC_CHECK_HEADER(zlib.h, [
	AC_CHECK_LIB(z, zlibVersion,
	[
		AC_SUBST(ZLIB_LD, -lz)
		AC_DEFINE(HAVE_LIBZ, 1, [Define to 1 if zlib (-lz) is available.])
	], zlib=no)
], zlib=no)

fi



dnl
dnl sqlite support
dnl

AC_ARG_WITH(shared-sqlite,
AC_HELP_STRING([--with-shared-sqlite],[Use shared sqlite]),
[shared_sqlite=$withval],[shared_sqlite=no])

if test "$shared_sqlite" = yes; then
	PKG_CHECK_MODULES(SQLITE, [sqlite3],
	[
		shared_sqlite=yes
	], shared_sqlite=no)
fi

if test "$shared_sqlite" = no; then
	SQLITE_OBJ='sqlite3.$(OBJEXT)'
fi

AC_SUBST(SQLITE_CFLAGS, "$SQLITE_CFLAGS")
AC_SUBST(SQLITE_LDFLAGS, "$SQLITE_LDFLAGS")
AC_SUBST(SQLITE_LIBS, "$SQLITE_LIBS")
AC_SUBST(SQLITE_OBJ)



dnl
dnl PCRE2 support
dnl

PKG_CHECK_MODULES(PCRE, [libpcre2-8], [have_pcre="yes"], [have_pcre="no"])
AM_CONDITIONAL([PCRE], [test "x$have_pcre" = "xyes"])



dnl ***************************************************************************
dnl Application settings
dnl

AC_DEFINE([NICKNAMEHISTORYLENGTH], 15000, [Size of the WHOWAS array.])
AC_DEFINE([CHANNEL_HEAP_SIZE],     8192, [Size of the channel heap.])
AC_DEFINE([BAN_HEAP_SIZE],         4096, [Size of the ban heap.])
AC_DEFINE([CLIENT_HEAP_SIZE],      8192, [Size of the client heap.])
AC_DEFINE([LCLIENT_HEAP_SIZE],     1024, [Size of the local client heap.])
AC_DEFINE([PCLIENT_HEAP_SIZE],     256, [Size of the pre-client heap.])
AC_DEFINE([USER_HEAP_SIZE],        8192, [Size of the user heap.])
AC_DEFINE([DNODE_HEAP_SIZE],       8192, [Size of the dlink_node heap.])
AC_DEFINE([TOPIC_HEAP_SIZE],       4096, [Size of the topic heap.])
AC_DEFINE([LINEBUF_HEAP_SIZE],     2048, [Size of the linebuf heap.])
AC_DEFINE([MEMBER_HEAP_SIZE],      32768, [Sizeof member heap.])
AC_DEFINE([ND_HEAP_SIZE],          512, [Size of the nick delay heap.])
AC_DEFINE([CONFITEM_HEAP_SIZE],    256, [Size of the confitem heap.])
AC_DEFINE([MONITOR_HEAP_SIZE],     1024, [Size of the monitor heap.])
AC_DEFINE([FD_HEAP_SIZE],          1024, [Size of fd heap.])
AC_DEFINE([AWAY_HEAP_SIZE],        512, [Size of away heap.])


dnl
dnl --enable-oper-chghost
dnl

AC_ARG_ENABLE(oper-chghost,
AC_HELP_STRING([--enable-oper-chghost],[Enable opers to use the CHGHOST command]),
[operchghost=$enableval],[operchghost=no])

if test "$operchghost" = yes; then
	AC_DEFINE(ENABLE_OPER_CHGHOST, 1, [Define this to enable opers to use the CHGHOST command.])
fi


dnl
dnl --with-nicklen
dnl

AC_ARG_WITH(nicklen,
AC_HELP_STRING([--with-nicklen=LENGTH],[Set the upper-bound nick length to LENGTH (default 31, max 50)]),
[
  if ! expr "$withval" + 0 >/dev/null 2>&1; then
	AC_ERROR([NICKLEN must be a numeric value])
  fi
  if test $withval -gt 50; then
	NICKLEN=50
	AC_MSG_WARN([NICKLEN has a hard limit of 50. Setting NICKLEN=50])
  elif test $withval -lt 9; then
	NICKLEN=9
	AC_MSG_WARN([NICKLEN has a lower limit of 9. Setting NICKLEN=9])
  else
	NICKLEN="$withval"
  fi
], [NICKLEN=31])

AC_DEFINE_UNQUOTED(NICKLEN, (${NICKLEN}+1), [Nickname length])


dnl
dnl --with-topiclen
dnl

AC_ARG_WITH(topiclen,
AC_HELP_STRING([--with-topiclen=NUMBER],[Set the max topic length to NUMBER (default 390, max 390)]),
[
 if test $withval -gt 390; then
	TOPICLEN=390
	AC_MSG_WARN([TOPICLEN has a hard limit of 390. Setting TOPICLEN=390])
 else
 	TOPICLEN=$withval
 fi
], [TOPICLEN=390])

AC_DEFINE_UNQUOTED(TOPICLEN, ${TOPICLEN}, [Maximum topic length (<=390)])



dnl ***************************************************************************
dnl Branding defined in the compilation
dnl

VERSION_CMD="git describe --tags"
RB_VERSION=`$VERSION_CMD`
RB_DEFINE_UNQUOTED([VERSION], ["$RB_VERSION"], [Version generated at configuration time.])

DATESTR_CMD="date"
RB_DATESTR=`$DATESTR_CMD`
RB_DEFINE_UNQUOTED([DATESTR], ["$RB_DATESTR"], [Readable date string of configuration time.])

DATECODE_CMD="date +%s"
RB_DATECODE=`$DATECODE_CMD`
RB_DEFINE_UNQUOTED([DATECODE], [$RB_DATECODE], [UNIX epoch time at configuration time.])


dnl
dnl --with-custom-branding
dnl

AC_MSG_CHECKING([whether custom branding is requested])
AC_ARG_WITH(custom-branding, AC_HELP_STRING([--with-custom-branding=NAME], [Custom branding name.]),
[
	AC_DEFINE_UNQUOTED(BRANDING_NAME, ["$withval"], [Custom branding name.])
	AC_DEFINE(CUSTOM_BRANDING, 1, [Define if custom branding is enabled.])
	AC_MSG_RESULT([yes])
],[
	AC_DEFINE_UNQUOTED(BRANDING_NAME, ["$PACKAGE_NAME"], [Current package])
	AC_MSG_RESULT([no])
])


dnl
dnl --with-custom-version
dnl

AC_MSG_CHECKING([whether a custom version is requested])
AC_ARG_WITH(custom-version, AC_HELP_STRING([--with-custom-version=NAME], [Custom version branding.]),
[
	AC_DEFINE_UNQUOTED(BRANDING_VERSION, ["$withval"], [Custom branding version.])
	AC_MSG_RESULT([yes])
], [
	AC_DEFINE_UNQUOTED(BRANDING_VERSION, ["$RB_VERSION"], [Current version])
	AC_MSG_RESULT([no])
])



dnl ***************************************************************************
dnl Misc
dnl


AC_SUBST(MODULES_LIBS)
AC_SUBST(MOD_TARGET)
AC_SUBST(PICFLAGS)
AC_SUBST(SEDOBJ)
AC_SUBST(CFLAGS)


AC_CONFIG_COMMANDS([tools/genssl_chmod], [chmod 755 tools/genssl])



dnl ***************************************************************************
dnl Output
dnl


AC_OUTPUT


if [[ $DEBUG ]]; then
	debug="yes"
else
	debug="no"
fi

dnl Make it look sexay!
echo
echo "Configured ........................ $PACKAGE_NAME $PACKAGE_VERSION"
echo "Version ........................... $RB_VERSION"
echo "Configuration time ................ $RB_DATESTR"
echo "Compiler .......................... $CXX"
echo "Compiler flags (CXXFLAGS) ......... $CXXFLAGS"
echo "Building boost .................... $with_included_boost"
echo "Building libltdl .................. $with_included_ltdl"
echo "Precompiled headers ............... $build_pch"
echo "Developer debug ................... $debug"
echo "IPv6 support ...................... $ipv6"
echo "SSL type........................... $SSL_TYPE"
echo "Installing into ................... $prefix"
echo
echo "* Ready to build $PACKAGE_NAME"
echo "* Now run 'make'"
