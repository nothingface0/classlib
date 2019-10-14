#ifndef CLASSLIB_SYSAPI_LOCAL_SOCKET_H
# define CLASSLIB_SYSAPI_LOCAL_SOCKET_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <sys/socket.h>
#  include <sys/un.h>
# endif
# include <cstring>
# include <cctype>
# include <string>
#endif // CLASSLIB_SYSAPI_LOCAL_SOCKET_H
