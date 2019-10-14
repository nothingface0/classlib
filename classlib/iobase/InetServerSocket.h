#ifndef CLASSLIB_INET_SERVER_SOCKET_H
# define CLASSLIB_INET_SERVER_SOCKET_H
# include "classlib/iobase/InetSocket.h"

namespace lat {

/** Internet domain socket set up to listen as a server.  */
class InetServerSocket : public InetSocket
{
public:
    InetServerSocket (const InetAddress &bindAddr,
		      int backlog = 1,
		      int type = SOCK_STREAM);

protected:
    virtual Socket *	copy (void);
};

} // namespace lat
#endif // CLASSLIB_INET_SERVER_SOCKET_H
