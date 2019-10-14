#ifndef CLASSLIB_SYSTEM_H
# define CLASSLIB_SYSTEM_H

/** @def CLASSLIB_API
  @brief A macro that controls how entities of this shared library are
         exported or imported on Windows platforms (the macro expands
         to nothing on all other platforms).  The definitions are
         exported if #CLASSLIB_BUILD_DLL is defined, imported
         if #CLASSLIB_BUILD_ARCHIVE is not defined, and left
         alone if latter is defined (for an archive library build).  */

/** @def CLASSLIB_BUILD_DLL
  @brief Indicates that the header is included during the build of
         a shared library of this package, and all entities marked
	 with #CLASSLIB_API should be exported.  */

/** @def CLASSLIB_BUILD_ARCHIVE
  @brief Indicates that this library is or was built as an archive
         library, not as a shared library.  Lack of this indicates
         that the header is included during the use of a shared
         library of this package, and all entities marked with
         #CLASSLIB_API should be imported.  */

# ifndef CLASSLIB_API
#  ifdef _WIN32
#    if defined CLASSLIB_BUILD_DLL
#      define CLASSLIB_API __declspec(dllexport)
#    elif ! defined CLASSLIB_BUILD_ARCHIVE
#      define CLASSLIB_API __declspec(dllimport)
#    endif
#  endif
# endif

# ifndef CLASSLIB_API
#  define CLASSLIB_API
# endif

/* ---------------------------------------------------------------------- */

# include <sys/types.h>
# ifndef _WIN32
#  include <unistd.h>
# endif

# ifdef _WIN32
#  ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0502
#  elif _WIN32_WINNT < 0x0502
#   error _WIN32_WINNT >= 0x0502 required
#  endif
# endif

#if _MSC_VER
# pragma warning(disable:4786)		    // '255' characters in the debug information
# pragma warning(disable:4305)		    // conversion from double to float (5.0)
# pragma warning(disable:4800)		    // forcing value to bool 'true' or 'false' (5.0)
# define CLASSLIB_NO_OSTREAM_LONG_DOUBLE 1  //< Set if no long double ostream overload.
# define pid_t int                          //< Defined to 'int' for unix compatibility.
# define uid_t int                          //< Defined to 'int' for unix compatibility.
# define gid_t int                          //< Defined to 'int' for unix compatibility.
#endif

# if __GNUC__
#  define CLASSLIB_FUNCTION __PRETTY_FUNCTION__
# elif defined _MSC_VER
#  define CLASSLIB_FUNCTION __FUNCTION__
# else
#  define CLASSLIB_FUNCTION __func__
# endif

# include "classlib/sysapi/config.h"
#endif /* CLASSLIB_SYSTEM_H */
