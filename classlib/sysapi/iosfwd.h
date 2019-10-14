#ifndef CLASSLIB_SYSAPI_IOSFWD_H
# define CLASSLIB_SYSAPI_IOSFWD_H
# include "classlib/sysapi/system.h"
# include <iosfwd>

namespace std
{
    class ios_base;
# if _MSC_VER
    typedef basic_streambuf< char,ios_traits<char> > streambuf;
# endif
}

#endif // CLASSLIB_SYSAPI_IOSFWD_H
