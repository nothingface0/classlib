#ifndef CLASSLIB_MACROS_H
# define CLASSLIB_MACROS_H

# include "classlib/sysapi/system.h"

// Concatenate two tokens together
# define CLASSLIB_name2(a,b)		a ## b

// Concatenate three tokens together
# define CLASSLIB_name3(a,b,c)		a ## b ## c

// Concatenate four tokens together
# define CLASSLIB_name4(a,b,c,d)		a ## b ## c ## d

#endif // CLASSLIB_MACROS_H
