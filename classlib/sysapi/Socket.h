#ifndef CLASSLIB_SYSAPI_SOCKET_H
# define CLASSLIB_SYSAPI_SOCKET_H
# include "classlib/sysapi/system.h"
# ifdef _WIN32
#  include <windows.h>
# else
#  include <sys/param.h>
#  include <sys/socket.h>
#  include <sys/ioctl.h>
#  include <netinet/in.h>
#  include <netinet/tcp.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <cerrno>
#  include <cstring>
# endif

# ifdef _WIN32
#  define WS_VERSION_REQD 	0x0200
#  define WS_VERSION_MAJOR	HIBYTE(WS_VERSION_REQD)
#  define WS_VERSION_MINOR	LOBYTE(WS_VERSION_REQD)

#  define ERRNO			WSAGetLastError()
#  define CLOSE			::closesocket

#  define EWOULDBLOCK		WSAEWOULDBLOCK
#  define ENOTCONN		WSAENOTCONN
#  define EINPROGRESS		WSAEINPROGRESS

#  ifndef MIN_SOCKETS_REQD
#   define MIN_SOCKETS_REQD	1
#  endif

#  ifndef EINTR
#   define EINTR 0
#  endif

#  define SOCKETFD()		(SOCKET) fd ()

   typedef int			socklen_t;

# else
#  define ERRNO			errno
#  define CLOSE			::close
#  define SOCKETFD()		fd ()
# endif

#endif // CLASSLIB_SYSAPI_SOCKET_H

