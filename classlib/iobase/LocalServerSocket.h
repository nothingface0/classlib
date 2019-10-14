#ifndef CLASSLIB_LOCAL_SERVER_SOCKET_H
# define CLASSLIB_LOCAL_SERVER_SOCKET_H
# include "classlib/iobase/LocalSocket.h"

namespace lat {

/** Unix domain socket set up to listen as a server.  */
class LocalServerSocket : public LocalSocket
{
public:
    LocalServerSocket (const LocalAddress &bindAddr,
		       int backlog = 1,
		       int type = SOCK_STREAM);

protected:
    virtual Socket *	copy (void);
};

} // namespace lat
#endif // CLASSLIB_LOCAL_SERVER_SOCKET_H
