#ifndef CLASSLIB_SYSAPI_SIGNAL_H
# define CLASSLIB_SYSAPI_SIGNAL_H
# include "classlib/sysapi/system.h"
# include <cstdio>
# include <cstdlib>
# include <cerrno>

# ifdef _WIN32
#  include <windows.h>
# else
#  include <unistd.h>
#  include <sys/stat.h>
#  include <sys/ucontext.h>
# endif

// Not all systems with POSIX signals necessarily have SA_SIGINFO.
# ifndef SA_SIGINFO
#  define SA_SIGINFO 0
# endif

// Windows doesn't have this, so fake a suitable substitute
# ifdef _WIN32
#  define STDERR_HANDLE GetStdHandle (STD_ERROR_HANDLE)
# else
#  define STDERR_HANDLE STDERR_FILENO
# endif

// Define a suitable wrapper to write to system file descriptors.
// This is needed because on Windows we are using HANDLEs, not the
// compiler's crippled posixy interface.
# ifdef _WIN32
#  define MYWRITE(fd,data,n)	do { DWORD written; WriteFile(fd,data,n,\
					&written,0); } while (0)
# else
#  define MYWRITE(fd,data,n)	write(fd,data,n)
# endif

/** Maximum length of a signal message.  Used for local format buffers
    for the signal number and name message in #Signal::fataldump() and
    the currently loaded shared library message in #SignalLibDump().
    Make this long enough to fit long shared library names.  */
static const int SIGNAL_MESSAGE_BUFSIZE = 256;
#endif // CLASSLIB_SYSAPI_SIGNAL_H
