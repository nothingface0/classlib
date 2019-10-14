#ifndef CLASSLIB_SYSAPI_PROCESS_INFO_H
# define CLASSLIB_SYSAPI_PROCESS_INFO_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
#  include <stdlib.h>
# else
#  include <unistd.h>
#  if __APPLE__
#   include <crt_externs.h>
#   include <mach-o/dyld.h>
#  endif
# endif
#endif // CLASSLIB_SYSAPI_PROCESS_INFO_H
