
#include "classlib/iobase/NamedPipeServer.h"
#include "classlib/iobase/FileSystemError.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/NamedPipeServer.h"
#ifndef _WIN32

namespace lat {

void
NamedPipeServer::sysinit (const char *name, FileAcl access)
{
    // FIXME: if no mkfifo, use mknod(name, access | S_IFIFO, 0)?
    if (mkfifo (name, access.native ()) == -1)
	throw FileSystemError ("mkfifo()", errno);
}

void
NamedPipeServer::sysclose (void)
{
    if (unlink (m_name.c_str ()) < 0)
	throw FileSystemError ("unlink()", errno);
}

void
NamedPipeServer::sysdetach (void)
{}

} // namespace lat
#endif // !_WIN32
