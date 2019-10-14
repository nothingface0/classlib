#ifndef CLASSLIB_SYSAPI_SYSTEM_ERROR_H
# define CLASSLIB_SYSAPI_SYSTEM_ERROR_H
# include "classlib/sysapi/system.h"
# include <errno.h>
# ifndef _WIN32
#  include <netdb.h>
# else
#  include <windows.h>
#  include <winsock2.h>
# endif
#endif // CLASSLIB_SYSAPI_SYSTEM_ERROR_H
