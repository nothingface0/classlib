
#include "classlib/iobase/Pipe.h"
#include "classlib/iobase/IOError.h"
#include "classlib/sysapi/Pipe.h"

namespace lat {

Pipe::Pipe (bool create /* = true */)
{ if (create) open (); }

IOChannel *
Pipe::source (void)
{ return &m_source; }

IOChannel *
Pipe::sink (void)
{ return &m_sink; }

} // namespace lat
