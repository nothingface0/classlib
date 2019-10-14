#ifndef CLASSLIB_INET_SOCKET_H
# define CLASSLIB_INET_SOCKET_H
# include "classlib/iobase/Socket.h"
# include "classlib/utils/Deprecated.h"
# include <string>

namespace lat {

class InetAddress : public SocketAddress
{
public:
    InetAddress (unsigned short port = 0);
    InetAddress (unsigned long address,
		 unsigned short port,
		 int family = AF_INET)
	    	CLASSLIB_DEPRECATED;
    InetAddress (unsigned long address,
		 const char *service,
		 const char *proto = "tcp",
		 int family = AF_INET)
		CLASSLIB_DEPRECATED;
    InetAddress (const char *host,
		 unsigned short port,
		 int family = AF_UNSPEC,
		 int flags = AI_ADDRCONFIG);
    InetAddress (const char *host,
		 const char *service,
		 int proto = SOCK_STREAM,
		 int family = AF_UNSPEC,
		 int flags = AI_ADDRCONFIG);
    InetAddress (const char *host,
		 const char *service,
		 const char *proto,
		 int family = AF_UNSPEC,
		 int flags = AI_ADDRCONFIG);
    InetAddress (const InetAddress &a);
    ~InetAddress (void);
    InetAddress &operator= (const InetAddress &x);

    int			port (void) const;
    std::string		service (int flags = 0) const;
    std::string		hostname (int flags = 0) const;
};

/** Internet domain stream socket.  */
class InetSocket : public Socket
{
public:
    InetSocket (void);
    InetSocket (int type, int protocol = 0, int family = AF_UNSPEC);
    InetSocket (const InetAddress &to,
		int type = SOCK_STREAM,
		int protocol = 0);
    ~InetSocket (void);

    virtual void	create (int type = SOCK_STREAM,
				int protocol = 0,
				int family = AF_UNSPEC);

    virtual void	bind (const InetAddress &a);
    virtual void	connect (const InetAddress &a);

    virtual InetAddress	sockname (void);
    virtual InetAddress	peername (void);

    // Overrides
    using Socket::read;
    using Socket::peek;
    using Socket::write;

    virtual IOSize	read (void *into, IOSize n, InetAddress &source);
    IOSize		read (IOBuffer into, InetAddress &source);
    virtual IOSize	peek (void *into, IOSize n, InetAddress &source);
    IOSize		peek (IOBuffer into, InetAddress &source);
    virtual IOSize	write (const void *from, IOSize n, const InetAddress &dest);
    IOSize		write (IOBuffer from, const InetAddress &dest);

protected:
    virtual Socket *	copy (void);
};

} // namespace lat
#endif // CLASSLIB_INET_SOCKET_H
