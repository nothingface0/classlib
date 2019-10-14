#ifndef CLASSLIB_LONG_LONG_H
# define CLASSLIB_LONG_LONG_H
# include "classlib/utils/iosfwd.h"
# include <limits.h>

/** @macro LONG_LONG_CONST(n)
    @brief Construct a <tt>long long</tt> constant from @a n
           compliant to the compiler's conventions for large types.  */

/** @macro ULONG_LONG_CONST(n)
    @brief Construct a <tt>unsigned long long</tt> constant from @a n
           compliant to the compiler's conventions for large types.  */

# if _MSC_VER
#  define LONG_LONG_CONST(n)	n ## i64
#  define ULONG_LONG_CONST(n)	n ## ui64

#  define LONG_LONG_MAX		9223372036854775807i64
#  define LONG_LONG_MIN		(-LONG_LONG_MAX-1)
#  define ULONG_LONG_MAX 	(LONG_LONG_MAX * 2ui64 + 1)
# else
#  define LONG_LONG_CONST(n)	n ## LL
#  define ULONG_LONG_CONST(n)	n ## ULL
#  if ! defined ULONG_LONG_MAX && defined ULLONG_MAX // solaris
#   define ULONG_LONG_MAX	ULLONG_MAX
#   define LONG_LONG_MAX	LLONG_MAX
#   define LONG_LONG_MIN	LLONG_MIN
#  endif
# endif

namespace lat {
# if _MSC_VER
typedef __int64			LongLong;
typedef unsigned __int64	ULongLong;
# else
typedef long long		LongLong;
typedef unsigned long long	ULongLong;
# endif
} // namespace lat

#if CLASSLIB_NO_OSTREAM_LONG_LONG
std::ostream &operator<< (std::ostream &s, long long n);
std::ostream &operator<< (std::ostream &s, unsigned long long n);
#endif

#endif // CLASSLIB_LONG_LONG_H
