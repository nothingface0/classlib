#ifndef CLASSLIB_SYSAPI_IO_CHANNEL_H
# define CLASSLIB_SYSAPI_IO_CHANNEL_H
# include "classlib/sysapi/system.h"
# include <errno.h>
# ifdef _WIN32
#  include <windows.h>
#  include <winsock2.h>
# else
#  include <unistd.h>
#  include <fcntl.h>
#  if _XOPEN_VERSION > 0
#   include <sys/uio.h>
#  endif
# endif
#endif // CLASSLIB_SYSAPI_IO_CHANNEL_H
