
#include "classlib/iotools/StorageOutputStream.h"
#include "classlib/iobase/Storage.h"
#include "classlib/utils/DebugAids.h"

namespace lat {

StorageOutputStream::StorageOutputStream (Storage *output)
    : m_storage (output)
{ ASSERT (m_storage); }

// doesn't delete storage, but does close it (FIXME: exceptions?)
StorageOutputStream::~StorageOutputStream (void)
{ /* close (); */ }

IOSize
StorageOutputStream::write (const void *from, IOSize n)
{ return m_storage->xwrite (from, n); }

IOSize
StorageOutputStream::writev (const IOBuffer *from, IOSize buffers)
{ return m_storage->xwritev (from, buffers); }

void
StorageOutputStream::close (void)
{ m_storage->close (); }

} // namespace lat
