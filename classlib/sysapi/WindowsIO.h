#ifndef CLASSLIB_SYSAPI_WINDOWS_IO_H
# define CLASSLIB_SYSAPI_WINDOWS_IO_H
# ifdef _WIN32
#  include <io.h>
#  define access _access
#  define getcwd _getcwd
#  define open _open
#  define write _write
#  define close _close
#  define R_OK 04
#  define W_OK 02
#  define X_OK 06
# endif
#endif // CLASSLIB_SYSAPI_WINDOWS_IO_H
