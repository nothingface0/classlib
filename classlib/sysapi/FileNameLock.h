#ifndef CLASSLIB_SYSAPI_FILE_NAME_LOCK_H
# define CLASSLIB_SYSAPI_FILE_NAME_LOCK_H
# include "classlib/sysapi/system.h"
# include <cerrno>
# ifndef _WIN32
#  include <utmp.h>
#  include <sys/sysctl.h>
# endif
#endif // CLASSLIB_SYSAPI_FILE_NAME_LOCK_H
