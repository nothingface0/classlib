#ifndef CLASSLIB_SYSAPI_DEBUG_AIDS_H
# define CLASSLIB_SYSAPI_DEBUG_AIDS_H
# include "classlib/sysapi/system.h"

# include <cctype>
# include <cstdio>
# include <cstdlib>
# include <iostream>
# include <iomanip>

# ifdef _WIN32
#  include <windows.h>
#  include <winnt.h>
#  include <imagehlp.h>
//#  include <io.h>
# else
#  include <unistd.h>
#  include <sys/uio.h>
#  include <sys/wait.h>
#  if __GLIBC__
#   include <execinfo.h>
#  endif
#  if CLASSLIB_HAVE_DLFCN_H
#   include <dlfcn.h>
#  endif
# if __sgi
    // This is yucky.  KCC's <exception.h> that has nothing to do with the
    // header we are looking for (it redirect to <exception>).  This ugly
    // workaround allows us to find the (IRIX) header we are looking for.
#   if defined __KCC
#     include </usr/include/exception.h>
#    else
#     include <exception.h>
#    endif
#    include <excpt.h>
#    include <rld_interface.h>
#    undef try	  // Defined on SGI to structured exception handling goop
#    undef catch   // Defined on SGI to structured exception handling goop
#  endif
#  if __osf
#   include <rld_interface.h>
#   include <pdsc.h>
#  endif
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

// Helper to write literals
# define MYWRITELIT(fd,str) MYWRITE(fd,str,sizeof(str)-1)

#if __GLIBC__
/** The maximum stack trace depth for systems where we request the
    stack depth separately (GNU libc-based systems).  */
static const int MAX_BACKTRACE_DEPTH = 128;
#endif

#endif // CLASSLIB_SYSAPI_DEBUG_AIDS_H
