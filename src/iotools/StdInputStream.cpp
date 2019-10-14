
#include "classlib/iotools/StdInputStream.h"
#include "classlib/iotools/StdError.h"
#include "classlib/utils/DebugAids.h"

namespace lat {

StdInputStream::StdInputStream (std::istream *input)
    : m_input (input)
{}

StdInputStream::~StdInputStream (void)
{ /* FIXME: delete m_input; */ }

IOSize
StdInputStream::read (void *into, IOSize n)
{
    ASSERT (m_input);
    m_input->read ((char *) into, n);
    IOSize nread = m_input->gcount ();

    // If nread < n, there might have been an error.  Ignore it if
    // nread > 0, otherwise check if it is an end-of-file; if so,
    // return 0, otherwise throw an exception.
    if (nread > 0 || m_input->eof ())
	return nread;
    else
	throw StdError (m_input->rdstate ());
}

void
StdInputStream::close (void)
{
    ASSERT (m_input);
    // FIXME: delete m_input; m_input = 0;
}

} // namespace lat
