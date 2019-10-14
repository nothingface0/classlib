
#include "classlib/iobase/LocalSocket.h"
#include "classlib/utils/SystemError.h"
#include "classlib/utils/HostInfo.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/LocalSocket.h"

#if _POSIX_VERSION <= 0
// Stub type just to keep compiler happy.  Don't expect this to actually work!
struct sockaddr_un { int sun_family; char sun_path [1]; };
#endif

namespace lat {

/** Create a UNIX domain address bound to the give path.  */
LocalAddress::LocalAddress (const char *path /* = "" */)
{
    sockaddr_un *addr = (sockaddr_un *) &m_address;
    ASSERT(strlen(path) < sizeof(addr->sun_path)-1);
    addr->sun_family = AF_UNIX;
    strcpy (addr->sun_path, path);
    m_addrlen = sizeof (*addr) - sizeof (addr->sun_path) + strlen(path);
}

LocalAddress::LocalAddress (const std::string &path)
{
    sockaddr_un *addr = (sockaddr_un *) &m_address;
    addr->sun_family = AF_UNIX;
    strcpy (addr->sun_path, path.c_str());
    m_addrlen = sizeof (*addr) - sizeof (addr->sun_path) + path.size();
}

/** Copy a UNIX domain address.  */
LocalAddress::LocalAddress (const LocalAddress &a)
    : SocketAddress (a)
{}

LocalAddress &
LocalAddress::operator= (const LocalAddress &a)
{ SocketAddress::operator=(a); return *this; }

LocalAddress::~LocalAddress (void)
{}

const char *
LocalAddress::path (void) const
{ return ((sockaddr_un *)&m_address)->sun_path; }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
LocalSocket::LocalSocket (void)
{}

LocalSocket::LocalSocket (int type, int protocol /* = 0 */)
{ Socket::create (AF_UNIX, type, protocol); }

LocalSocket::LocalSocket (const LocalAddress &to,
			  int type /* = SOCK_STREAM */,
			  int protocol /* = 0 */)
{ Socket::create (AF_UNIX, type, protocol); Socket::connect (to); }

LocalSocket::~LocalSocket (void)
{ }

/// @reimp
void
LocalSocket::create (int type /* = SOCK_STREAM */, int protocol /* = 0*/)
{ Socket::create (AF_UNIX, type, protocol); }

/// @reimp
void
LocalSocket::bind (const LocalAddress &a)
{ Socket::bind (a); }

/// @reimp
void
LocalSocket::connect (const LocalAddress &a)
{ Socket::connect (a); }

/** Retrieve the local address of the socket.  */
LocalAddress
LocalSocket::sockname (void)
{
    // FIXME: is this right?  do we need to adjust things for path length?
    LocalAddress addr;
    Socket::sockname (addr);
    return addr;
}

/** Retrieve the address of the peer of the socket.  */
LocalAddress
LocalSocket::peername (void)
{
    // FIXME: is this right?  do we need to adjust things for path length?
    LocalAddress addr;
    Socket::peername (addr);
    return addr;
}

/// @reimp
IOSize
LocalSocket::read (void *into, IOSize n, LocalAddress &source)
{ return Socket::read (into, n, source); }

/// @reimp
IOSize
LocalSocket::read (IOBuffer into, LocalAddress &source)
{ return read (into.data (), into.size (), source); }

/// @reimp
IOSize
LocalSocket::peek (void *into, IOSize n, LocalAddress &source)
{ return Socket::peek (into, n, source); }

/// @reimp
IOSize
LocalSocket::peek (IOBuffer into, LocalAddress &source)
{ return peek (into.data (), into.size (), source); }

/// @reimp
IOSize
LocalSocket::write (const void *from, IOSize n, const LocalAddress &dest)
{ return Socket::write (from, n, dest); }

/// @reimp
IOSize
LocalSocket::write (IOBuffer from, const LocalAddress &dest)
{ return write (from.data (), from.size (), dest); }

/// @reimp
Socket *
LocalSocket::copy (void)
{ return new LocalSocket (*this); }

} // namespace lat
