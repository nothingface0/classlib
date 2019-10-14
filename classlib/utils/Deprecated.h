#ifndef CLASSLIB_DEPRECATED_H
# define CLASSLIB_DEPRECATED_H

# if __GNUC__ && ! defined CLASSLIB_DEPRECATED
#  define CLASSLIB_DEPRECATED __attribute__((deprecated))
# else
#  define CLASSLIB_DEPRECATED
# endif

#endif // CLASSLIB_DEPRECATED_H
