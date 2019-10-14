#ifndef CLASSLIB_LOCAL_SOCKET_H
# define CLASSLIB_LOCAL_SOCKET_H
# include "classlib/iobase/Socket.h"
# include <string>

namespace lat {

class LocalAddress : public SocketAddress
{
public:
    LocalAddress (const std::string &path);
    LocalAddress (const char *path = "");
    LocalAddress (const LocalAddress &a);
    ~LocalAddress (void);
    LocalAddress &operator= (const LocalAddress &x);

    const char *path (void) const;
};

/** Unix domain stream sockets bound to file system paths. */
class LocalSocket : public Socket
{
public:
    LocalSocket (void);
    LocalSocket (int type, int protocol = 0);
    LocalSocket (const LocalAddress &to,
		 int type = SOCK_STREAM,
		 int protocol = 0);
    ~LocalSocket (void);
    
    void		create (int type = SOCK_STREAM,
				int protocol = 0);

    void		bind (const LocalAddress &a);
    void		connect (const LocalAddress &a);

    virtual LocalAddress sockname (void);
    virtual LocalAddress peername (void);

    // Overrides
    using Socket::read;
    using Socket::peek;
    using Socket::write;

    virtual IOSize	read (void *into, IOSize n, LocalAddress &source);
    IOSize		read (IOBuffer into, LocalAddress &source);
    virtual IOSize	peek (void *into, IOSize n, LocalAddress &source);
    IOSize		peek (IOBuffer into, LocalAddress &source);
    virtual IOSize	write (const void *from, IOSize n, const LocalAddress &dest);
    IOSize		write (IOBuffer from, const LocalAddress &dest);

protected:
    virtual Socket *	copy (void);
};

} // namespace lat
#endif // CLASSLIB_LOCAL_SOCKET_H
