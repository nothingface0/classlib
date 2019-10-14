#ifndef CLASSLIB_SYSAPI_NAMED_PIPE_H
# define CLASSLIB_SYSAPI_NAMED_PIPE_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <cerrno>
#  include <unistd.h>
#  include <sys/stat.h>
#  include <fcntl.h>
# endif
#endif // CLASSLIB_SYSAPI_NAMED_PIPE_H
