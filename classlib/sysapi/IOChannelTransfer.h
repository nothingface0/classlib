#ifndef CLASSLIB_SYSAPI_IO_CHANNEL_TRANSFER_H
# define CLASSLIB_SYSAPI_IO_CHANNEL_TRANSFER_H
# include "classlib/sysapi/system.h"
# include <errno.h>
# if __linux
#  include <sys/sendfile.h>
# elif __sun || __FreeBSD__ || __APPLE__
#  include <sys/uio.h>
#  include <sys/socket.h>
# endif
#endif // CLASSLIB_SYSAPI_IO_CHANNEL_TRANSFER_H
