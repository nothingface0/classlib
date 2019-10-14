#ifndef CLASSLIB_SYSAPI_TIME_H
# define CLASSLIB_SYSAPI_TIME_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <unistd.h>
#  include <sys/time.h>
# endif
# include <cerrno>
# include <ctime>
#endif // CLASSLIB_SYSAPI_TIME_H
