
#include "classlib/iotools/StdStorage.h"
#include "classlib/iotools/StdError.h"
#include "classlib/utils/DebugAids.h"
#include <fstream>

namespace lat {

StdStorage::StdStorage (void)
    : m_stream (0)
{}

StdStorage::StdStorage (const char *name, std::ios_base::openmode mode /* =...*/)
    : m_stream (new std::fstream (name, mode))
{}

StdStorage::StdStorage (const std::string &name, std::ios_base::openmode mode /* =...*/)
    : m_stream (new std::fstream (name.c_str (), mode))
{}

StdStorage::StdStorage (std::iostream *stream)
    : m_stream (stream)
{}

StdStorage::~StdStorage (void)
{ /* FIXME: delete m_stream; */ }

//////////////////////////////////////////////////////////////////////
void
StdStorage::open (const std::string &name, std::ios_base::openmode mode /* = ... */)
{ open (name.c_str (), mode); }

void
StdStorage::open (const char *name, std::ios_base::openmode mode /* = ... */)
{
    ASSERT (! m_stream);
    ASSERT (name && *name);
    m_stream = new std::fstream (name, mode);
    if (! m_stream->good ())
	throw StdError (m_stream->rdstate ());
}

// FIXME: This doesn't work because eof() becomes true only when we've
// tried to read past the end: bool StdStorage::eof (void) const {
// ASSERT (m_stream); return m_stream->eof (); }

bool
StdStorage::hasError (void) const
{
    ASSERT (m_stream);
    return m_stream->fail ();
}

void
StdStorage::clearError (void)
{
    ASSERT (m_stream);
    m_stream->clear ();
}

//////////////////////////////////////////////////////////////////////
IOSize
StdStorage::read (void *into, IOSize n)
{
    ASSERT (m_stream);
    m_stream->read ((char *) into, n);
    IOSize nread = m_stream->gcount ();

    // If nread < n, there might have been an error.  Ignore it if
    // nread > 0, otherwise check if it is an end-of-file; if so,
    // return 0, otherwise throw an exception.
    if (nread > 0 || m_stream->eof ())
	return nread;
    else
	throw StdError (m_stream->rdstate ());
}

IOSize
StdStorage::write (const void *from, IOSize n)
{
    ASSERT (m_stream);
    IOSize before = m_stream->tellp ();
    if (m_stream->write ((char *) from, n).fail ())
	throw StdError (m_stream->rdstate ());
    else
	return IOSize (m_stream->tellp ()) - before;
}

//////////////////////////////////////////////////////////////////////
IOOffset
StdStorage::position (void) const
{
    // FIXME: LFS support?
    ASSERT (m_stream);
    return m_stream->tellp ();
}

IOOffset
StdStorage::position (IOOffset offset, Relative whence /* = SET */)
{
    // FIXME: LFS support?
    ASSERT (m_stream);
    if (m_stream->seekp (systemoffconv<std::streamoff>(offset),
			 (whence == SET ? std::ios_base::beg
			  : whence == CURRENT ? std::ios_base::cur
			  : std::ios_base::end)).fail ())
	throw StdError (m_stream->rdstate ());

    return m_stream->tellp ();
}

//////////////////////////////////////////////////////////////////////
void
StdStorage::resize (IOOffset /* size */)
{
    // FIXME: Anything we can do?
    ASSERT (m_stream);
    throw StdError ();
}

//////////////////////////////////////////////////////////////////////
void
StdStorage::flush (void)
{
    ASSERT (m_stream);
    if (m_stream->flush ().fail ())
	throw StdError (m_stream->rdstate ());
}

void
StdStorage::close (void)
{ ASSERT (m_stream); /* FIXME: delete m_stream; m_stream = 0; */ }

} // namespace lat
