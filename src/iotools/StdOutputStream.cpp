
#include "classlib/iotools/StdOutputStream.h"
#include "classlib/iotools/StdError.h"
#include "classlib/utils/DebugAids.h"

namespace lat {

StdOutputStream::StdOutputStream (std::ostream *output)
    : m_output (output)
{}

StdOutputStream::~StdOutputStream (void)
{ /* FIXME: delete m_output; */ }

IOSize
StdOutputStream::write (const void *from, IOSize n)
{
    ASSERT (m_output);
    IOSize before = m_output->tellp ();
    if (m_output->write ((char *) from, n).fail ())
	throw StdError (m_output->rdstate ());
    else
	return IOSize (m_output->tellp ()) - before;
}

void
StdOutputStream::finish (void)
{
    ASSERT (m_output);
    if (m_output->flush ().fail ())
	throw StdError (m_output->rdstate ());
}

void
StdOutputStream::close (void)
{ ASSERT (m_output); /* FIXME: delete m_output; m_output = 0; */ }

} // namespace lat
