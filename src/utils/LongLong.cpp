
#include "classlib/utils/LongLong.h"
#include "classlib/utils/BitTraits.h"
#include <cstdio>

namespace lat {

#if CLASSLIB_NO_OSTREAM_LONG_LONG
CLASSLIB_BASE_API std::ostream &operator<< (std::ostream &s, long long n)
{
    // FIXME: Format flags?
    char buf [BitTraits<long long>::Digits + 2];
    sprintf (buf, "%Ld", n);  // FIXME: ll, L, or q?
    return out << buf;
}

CLASSLIB_BASE_API std::ostream &operator<< (std::ostream &s, unsigned long long n)
{
    // FIXME: Format flags?
    char buf [BitTraits<long long>::Digits + 2];
    sprintf (buf, "%Lu", n);  // FIXME: ll, L, or q?
    return out << buf;
}
#endif


} // namespace lat
