#ifndef CLASSLIB_SOCKET_H
# define CLASSLIB_SOCKET_H
# include "classlib/iobase/IOChannel.h"
# include "classlib/iobase/SocketConst.h"
# include <string>

namespace lat {

/** Generic socket address.  Derived socket classes implement
    addresses compatible with the socket type.  */
class SocketAddress
{
public:
    ~SocketAddress (void);

    int			family (void) const;
    socklen_t		size (void) const;
    socklen_t &		size (void);
    sockaddr *		address (void) const;

protected:
    SocketAddress (void);
    SocketAddress (const SocketAddress &x);
    SocketAddress &operator=(const SocketAddress &x);

    sockaddr_storage	m_address;
    socklen_t		m_addrlen;
};

/** Basic socket interface.  The actual implementation and connection
    details are handled by derived classes, but communication (reading
    and writing) can be done through this interface.

    The sockets support:
     - normal and out-of-band (urgent) data
     - message peeking (with source identification)
     - message source identification
     - class interface for useful conversions.  */
class Socket : public IOChannel
{
public:
    ~Socket (void);

    // Socket specific operations
    virtual void	oob (bool process = true);
    virtual void	shutdown (int mode);
    virtual void	close (void);
    virtual void	abort (void);

    virtual void	listen (int queuelen = 1);

    static bool		hasopt (int option);
    virtual int		getopt (int option, int level = SOL_SOCKET) const;
    virtual int		getopt (int option, void *p, size_t n, int level = SOL_SOCKET) const;
    virtual void	setopt (int option, int n, int level = SOL_SOCKET) const;
    virtual void	setopt (int option, const void *p, size_t n, int level = SOL_SOCKET) const;

    virtual int		type (void) const;
    virtual int		linger (int time = -1) const;
    // FIXME: easy access to other common options? broadcast,
    // receive/send buffer sizes, reuse address, timeout

    using IOChannel::read;
    using IOChannel::write;

    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	peek (void *into, IOSize n);
    IOSize		peek (IOBuffer into);
    virtual IOSize	write (const void *from, IOSize n);

    virtual Socket *	accept (void);

protected:
    Socket (void);

    void		create (int af, int type, int protocol);

    socklen_t		sockname (SocketAddress &addr);
    socklen_t		peername (SocketAddress &addr);
    
    void		bind (const SocketAddress &addr);
    void		connect (const SocketAddress &addr);
    virtual Socket *	copy (void) = 0; /*< Make a copy of the socket.  */

    IOSize		read (void *into, IOSize n, SocketAddress &source);
    IOSize		peek (void *into, IOSize n, SocketAddress &source);
    IOSize		write (const void *from, IOSize n, const SocketAddress &dest);

    virtual void	select (IOSelectMethod *to, const IOSelectRequest &req);

private:
    static void		sysinit (void);

    void		*m_extra;
    bool		m_oob;
};

} // namespace lat
#endif // CLASSLIB_SOCKET_H
