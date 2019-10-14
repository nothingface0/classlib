#ifndef CLASSLIB_SYSAPI_FILE_H
# define CLASSLIB_SYSAPI_FILE_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <unistd.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <utime.h>
#  include <limits.h>
#  include <cerrno>
#  include <cstdlib>
# endif

# if !defined O_SYNC && defined O_SYNCIO
#  define O_SYNC O_SYNCIO
# endif

# if !defined O_NONBLOCK && defined O_NDELAY
#  define O_NONBLOCK O_NDELAY
# endif

# ifndef O_NONBLOCK
#  define O_NONBLOCK 0
# endif

#endif // CLASSLIB_SYSAPI_FILE_H
