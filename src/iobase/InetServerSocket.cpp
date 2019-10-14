
#include "classlib/iobase/InetServerSocket.h"

namespace lat {

// FIXME: Default ctor + open?

InetServerSocket::InetServerSocket (const InetAddress &bindAddr,
				    int backLog /* = 1 */,
				    int type /* = SOCK_STREAM */)
    : InetSocket (type, 0, bindAddr.family())
{ bind (bindAddr); listen (backLog); }

Socket *
InetServerSocket::copy (void)
{ return new InetServerSocket (*this); }

} // namespace lat
