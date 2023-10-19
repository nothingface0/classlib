
#include "classlib/iotools/IOChannelInputStream.h"
#include "classlib/iobase/IOChannel.h"
#include "classlib/utils/DebugAids.h"

namespace lat {

IOChannelInputStream::IOChannelInputStream (IOChannel *input)
    : m_input (input)
{ ASSERT (m_input); }

// doesn't delete but closes the input
IOChannelInputStream::~IOChannelInputStream (void)
{ /* close (); */ }

IOSize
IOChannelInputStream::read (void *into, IOSize n)
{ return m_input->read (into, n); }

IOSize
IOChannelInputStream::readv (IOBuffer *from, IOSize buffers)
{ return m_input->readv (from, buffers); }

void
IOChannelInputStream::close (void)
{ m_input->close (); /* FIXME: delete m_input; m_input = 0; */ }

} // namespace lat
