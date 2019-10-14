
#include "classlib/iobase/SocketPair.h"
#include "classlib/iobase/NetworkError.h"
#include "classlib/sysapi/SocketPair.h"
#ifdef _WIN32

namespace lat {

void
SocketPair::open (void)
{
    // FIXME
}

} // namespace lat
#endif // _WIN32
