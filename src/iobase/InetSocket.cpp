
#include "classlib/iobase/InetSocket.h"
#include "classlib/iobase/NetworkError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/InetSocket.h"
#include <cstdio>
#define UNUSED(x) (void)(x)
namespace lat {


static void
initInetAddr(sockaddr_storage &address,
	     socklen_t &addrlen,
	     const char *host,
	     const char *port,
	     int flags,
	     int family,
	     int type)
{
    int err;
    struct addrinfo hints;
    struct addrinfo *res = 0;

    // Set up hints for passive address lookup.
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = family;
    hints.ai_flags = flags;
    hints.ai_socktype = type;
    if ((err = getaddrinfo(host, port, &hints, &res)))
       throw NetworkError("getaddrinfo()", gai_strerror(err));
    else
    {
        // Take first address only; assumes dual-stack platform.
        // FIXME: Support handling all addresses for Windows XP?
        ASSERT(res);
        ASSERT(res->ai_addrlen <= sizeof(address));
        memcpy(&address, res->ai_addr, res->ai_addrlen);
        addrlen = res->ai_addrlen;
        freeaddrinfo(res);
    }
}

/** Create a local internet domain address to port number @a port.  */
InetAddress::InetAddress (unsigned short port /* = 0 */)
{
    // Always initialize size, needed by getpeername() etc. calls.
    m_addrlen = sizeof(sockaddr_in6);

    if (port != 0)
    {
        char srvnr[32];
        sprintf(srvnr, "%d", port);
	initInetAddr(m_address, m_addrlen, 0, srvnr,
		     AI_PASSIVE | AI_NUMERICSERV | AI_ADDRCONFIG,
		     AF_UNSPEC, SOCK_STREAM);
    }
}

/** Create an internet domain address to remote address @a address
    and port number @a port.  */
InetAddress::InetAddress (unsigned long address,
			  unsigned short port /* = 0 */,
			  int family /* = AF_INET */)
{
  UNUSED(family);
    ASSERT(family == AF_INET);

    char srvnr[32];
    sprintf(srvnr, "%d", port);

    in_addr addr;
    addr.s_addr = htonl(address);

    initInetAddr(m_address, m_addrlen, inet_ntoa(addr), srvnr,
		 (address == INADDR_ANY ? AI_PASSIVE : AI_NUMERICHOST)
		 | AI_NUMERICSERV | AI_ADDRCONFIG, AF_INET, SOCK_STREAM);
}

/** Create an internet domain address to a remote address with
    a named service using the named protocol.  */
InetAddress::InetAddress (unsigned long address,
			  const char *service,
			  const char *proto /* = "tcp" */,
			  int family /* = AF_INET */)
{
  UNUSED(family);
    ASSERT(family == AF_INET);
    protoent *p = getprotobyname(proto);
    if (! p) throw NetworkError ("getprotobyname()", proto);

    in_addr addr;
    addr.s_addr = htonl(address);

    initInetAddr(m_address, m_addrlen, inet_ntoa(addr), service,
		 (address == INADDR_ANY ? AI_PASSIVE : AI_NUMERICHOST)
		 | AI_ADDRCONFIG, AF_INET, p->p_proto);
}

/** Create an internet domain address to a named host and port.  */
InetAddress::InetAddress (const char *host,
			  unsigned short port /* = 0 */,
			  int family /* = AF_UNSPEC */,
			  int flags /* = AI_ADDRCONFIG */)
{
    char srvnr[32];
    sprintf(srvnr, "%d", port);
    initInetAddr(m_address, m_addrlen, host, srvnr,
		 AI_NUMERICSERV | flags, family, SOCK_STREAM);
}

/** Create an internet domain address to a named host using a
    named service using the numeric protocol.  */
InetAddress::InetAddress (const char *host,
			  const char *service,
			  int proto /* = SOCK_STREAM */,
			  int family /* = AF_UNSPEC */,
			  int flags /* = AI_ADDRCONFIG */)
{
    initInetAddr(m_address, m_addrlen, host, service,
		 flags, family, proto);
}

/** Create an internet domain address to a named host using a
    named service using the named protocol.  */
InetAddress::InetAddress (const char *host,
			  const char *service,
			  const char *proto,
			  int family /* = AF_UNSPEC */,
			  int flags /* = AI_ADDRCONFIG */)
{
    protoent *p = getprotobyname(proto);
    if (! p) throw NetworkError ("getprotobyname()", proto);
    initInetAddr(m_address, m_addrlen, host, service,
		 flags, family, p->p_proto);
}

/** Copy an internet domain address.  */
InetAddress::InetAddress (const InetAddress &a)
    : SocketAddress (a)
{}

/** Copy an internet domain address. */
InetAddress &
InetAddress::operator= (const InetAddress &a)
{ SocketAddress::operator=(a); return *this; }

/** Get the port number in host byte order.  */
int
InetAddress::port (void) const
{
    ASSERT(m_addrlen > 0);
    if (m_address.ss_family == AF_INET)
        return ntohs(((sockaddr_in *)&m_address)->sin_port);
    else if (m_address.ss_family == AF_INET6)
        return ntohs(((sockaddr_in6 *)&m_address)->sin6_port);
    else
    {
	ASSERT(false);
	return 0;
    }
}

/** Destruct internet domain address. */
InetAddress::~InetAddress(void)
{}

/** Get the service name designated by the address.  Returns an empty
    string if the name cannot be resolved.  */
std::string
InetAddress::service (int flags /* = 0 */) const
{
    char name[1024];
    int err = getnameinfo((sockaddr *) &m_address, m_addrlen,
			  0, 0, name, sizeof(name), flags);
    if (err == EAI_NONAME)
	//throw NetworkError("getnameinfo()", gai_strerror(err));
	return "";

    return name;
}

/** Get the host name designated by the address.  Returns an empty
    string if the name cannot be resolved.  */
std::string
InetAddress::hostname (int flags /* = 0 */) const
{
    char name[1024];
    int err = getnameinfo((sockaddr *) &m_address, m_addrlen,
			  name, sizeof(name), 0, 0, flags);
    if (err == EAI_NONAME
	&& m_address.ss_family == AF_INET6
	&& !memcmp(&((sockaddr_in6 *)&m_address)->sin6_addr,
		   &in6addr_any, sizeof(in6_addr)))
	return "::";
    else if (err == EAI_NONAME
	     && m_address.ss_family == AF_INET
	     && ((sockaddr_in *)&m_address)->sin_addr.s_addr == htonl(INADDR_ANY))
	return "0.0.0.0";
    else if (err)
	//throw NetworkError("getnameinfo()", gai_strerror(err));
	return "";

    return name;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
InetSocket::InetSocket (void)
{}

InetSocket::InetSocket (int type, int protocol /* = 0 */, int family /* = AF_UNSPEC */)
{ Socket::create (family, type, protocol); }

InetSocket::InetSocket (const InetAddress &to,
			int type /* = SOCK_STREAM */,
			int protocol /* = 0 */)
{ Socket::create (to.family(), type, protocol); Socket::connect (to); }

InetSocket::~InetSocket (void)
{ }

/// @reimp
void
InetSocket::create (int family /* = AF_UNSPEC */, int type /* = SOCK_STREAM */, int protocol /* = 0 */)
{ Socket::create (family, type, protocol); }

/// @reimp
void
InetSocket::bind (const InetAddress &a)
{ Socket::bind (a); }

/// @reimp
void
InetSocket::connect (const InetAddress &a)
{ Socket::connect (a); }

/** Retrieve the local address of the socket.  */
InetAddress
InetSocket::sockname (void)
{
    InetAddress addr;
    Socket::sockname (addr);
    return addr;
}

/** Retrieve the address of the peer of the socket.  */
InetAddress
InetSocket::peername (void)
{
    InetAddress addr;
    Socket::peername (addr);
    return addr;
}

/// @reimp
IOSize
InetSocket::read (void *into, IOSize n, InetAddress &source)
{ return Socket::read (into, n, source); }

/// @reimp
IOSize
InetSocket::read (IOBuffer into, InetAddress &source)
{ return read (into.data (), into.size (), source); }

/// @reimp
IOSize
InetSocket::peek (void *into, IOSize n, InetAddress &source)
{ return Socket::peek (into, n, source); }

/// @reimp
IOSize
InetSocket::peek (IOBuffer into, InetAddress &source)
{ return peek (into.data (), into.size (), source); }

/// @reimp
IOSize
InetSocket::write (const void *from, IOSize n, const InetAddress &dest)
{ return Socket::write (from, n, dest); }

/// @reimp
IOSize
InetSocket::write (IOBuffer from, const InetAddress &dest)
{ return write (from.data (), from.size (), dest); }

/// @reimp
Socket *
InetSocket::copy (void)
{ return new InetSocket (*this); }

} // namespace lat
