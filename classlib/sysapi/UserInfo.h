#ifndef CLASSLIB_SYSAPI_USER_INFO_H
# define CLASSLIB_SYSAPI_USER_INFO_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
#  include <wininet.h>
#  include <ntsecapi.h>
#  include <lm.h>
# else
#  include <pwd.h>
#  include <unistd.h>
# endif
# include <cerrno>
# include <cstdlib>
#endif // CLASSLIB_SYSAPI_USER_INFO_H
