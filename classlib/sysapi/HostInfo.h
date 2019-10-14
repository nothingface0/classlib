#ifndef CLASSLIB_SYSAPI_HOST_INFO_H
# define CLASSLIB_SYSAPI_HOST_INFO_H
# include "classlib/sysapi/system.h"
# include <cctype>
# ifdef _WIN32
#  include <windows.h>
#  include <wininet.h>
# else
#  include <netdb.h>
#  include <unistd.h>
#  include <sys/param.h>
# endif
#endif // CLASSLIB_SYSAPI_HOST_INFO_H
