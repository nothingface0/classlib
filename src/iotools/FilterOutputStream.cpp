
#include "classlib/iotools/FilterOutputStream.h"
#include "classlib/utils/DebugAids.h"

namespace lat {

/** FIXME */
FilterOutputStream::FilterOutputStream (OutputStream *output)
    : m_output (output)
{ ASSERT (m_output); }

/** FIXME */
FilterOutputStream::~FilterOutputStream (void)
{ ASSERT (m_output); /* FIXME: delete m_output; */ }

/** FIXME */
IOSize
FilterOutputStream::write (const void *from, IOSize n)
{ ASSERT (m_output); return m_output->write (from, n); }

/** FIXME */
void
FilterOutputStream::finish (void)
{ ASSERT (m_output); m_output->finish (); }

/** FIXME */
void
FilterOutputStream::close (void)
{
    ASSERT (m_output);
    finish ();
    m_output->close ();
}

} // namespace lat
