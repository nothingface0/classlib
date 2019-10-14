#ifndef CLASSLIB_SYSAPI_TEMP_FILE_H
# define CLASSLIB_SYSAPI_TEMP_FILE_H
# ifdef _WIN32
#  include <windows.h>
#  include "classlib/sysapi/WindowsIO.h"
# else
#  include <unistd.h>
#  include <sys/stat.h>
#  include <fcntl.h>
# endif
# include <cstdio>
# include <cstdlib>
# include <cerrno>

#ifndef TMP_MAX
# define TMP_MAX 16384
#endif
#endif // CLASSLIB_SYSAPI_TEMP_FILE_H
