#ifndef CLASSLIB_SYSAPI_FILE_MAPPING_H
# define CLASSLIB_SYSAPI_FILE_MAPPING_H
# include "classlib/sysapi/system.h"
# include <cerrno>
# ifdef _WIN32
#  include <windows.h>
# else
#  include <sys/mman.h>
# endif

#if !defined MAP_ANONYMOUS && defined MAP_ANON
# define MAP_ANONYMOUS MAP_ANON
#endif
#endif // CLASSLIB_SYSAPI_FILE_MAPPING_H
