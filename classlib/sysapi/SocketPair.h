#ifndef CLASSLIB_SYSAPI_SOCKET_PAIR_H
# define CLASSLIB_SYSAPI_SOCKET_PAIR_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <cerrno>
#  include <sys/socket.h>
# endif
#endif // CLASSLIB_SYSAPI_SOCKET_PAIR_H
