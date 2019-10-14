#ifndef CLASSLIB_SYSAPI_SUB_PROCESS_H
# define CLASSLIB_SYSAPI_SUB_PROCESS_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <cerrno>
#  include <unistd.h>
#  include <sys/wait.h>
# endif
#endif // CLASSLIB_SYSAPI_SUB_PROCESS_H
