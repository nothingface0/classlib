
#include "classlib/iobase/SocketPair.h"
#include "classlib/iobase/NetworkError.h"
#include "classlib/sysapi/SocketPair.h"

namespace lat {

SocketPair::SocketPair (bool create /* = true */)
{ if (create) open (); }

SocketPair::~SocketPair (void)
{}

IOChannel *
SocketPair::source (void)
{ return &m_source; }

IOChannel *
SocketPair::sink (void)
{ return &m_sink; }

} // namespace lat
