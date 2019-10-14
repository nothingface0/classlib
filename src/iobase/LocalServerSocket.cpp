
#include "classlib/iobase/LocalServerSocket.h"

namespace lat {

LocalServerSocket::LocalServerSocket (const LocalAddress &bindAddr,
				      int backLog /* = 1 */,
				      int type /* = SOCK_STREAM */)
    : LocalSocket (type)
{ bind (bindAddr); listen (backLog); }

Socket *
LocalServerSocket::copy (void)
{ return new LocalServerSocket (*this); }

} // namespace lat
