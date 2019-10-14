#ifndef CLASSLIB_SYSAPI_INET_SOCKET_H
# define CLASSLIB_SYSAPI_INET_SOCKET_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
#  include <ws2ipdef.h>
# else
#  include <unistd.h>
#  include <arpa/inet.h>
#  include <cerrno>
# endif
# include <cstring>
# include <cctype>

// FIXME: h_errno vs. errno?

#ifdef _WIN32
# define ERRNO			WSAGetLastError()
#else
# define ERRNO			errno
#endif

#endif // CLASSLIB_SYSAPI_INET_SOCKET_H
