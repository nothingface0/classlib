#ifndef CLASSLIB_SYSAPI_SHARED_LIBRARY_H
# define CLASSLIB_SYSAPI_SHARED_LIBRARY_H
# include "classlib/sysapi/ElfAbi.h"
# ifdef _WIN32
#  include <windows.h>
#  include <winnt.h>
#  include <imagehlp.h>
#  define putenv putenv_
# else
#  if __hpux
#   include <dl.h>
#  else
#   include <dlfcn.h>
#  endif
#  if __osf
#   include <loader.h>
#  endif
#  if __ELF__
#   include <link.h>
#   include <limits.h>
#   include <sys/stat.h>
#   include <unistd.h>
#   include <elf.h>
#  endif
#  if __sgi
#   include <sgidefs.h>
#   include <objlist.h>
#   include <obj_list.h>
#   include <obj.h>
#  endif
#  if __APPLE__
#   include <mach-o/dyld.h>
#   include <mach-o/getsect.h>
#  endif
# endif // _WIN32
# include <cstdio>
# include <cstdlib>
# include <errno.h>

#if __aix
# define PATH		"LIBPATH"
#elif __hpux
# define PATH		"SHLIB_PATH"
#elif defined _WIN32
# define PATH		"PATH"
#elif __APPLE__
# define PATH		"DYLD_LIBRARY_PATH"
#else
#  define PATH		"LD_LIBRARY_PATH"
#endif
#endif // CLASSLIB_SYSAPI_SHARED_LIBRARY_H
