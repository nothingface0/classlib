
#include "src/iobase/IOSelectSig.h"
#include "classlib/utils/DebugAids.h"
# if _POSIX_REALTIME_SIGNALS > 0 && defined F_SETSIG && defined F_SETAUXFL

namespace lat {

/** Attach the file descriptor from request @a req.  The caller should
    have already established handler (Signal::revert() is sufficient)
    for SIGIO and SIGURG.  */
void
IOSelectSigFd::attach (const IOSelectRequest &req)
{
    ASSERT (req.fd >= 0);
    int flags;
    if ((flags = fcntl (req.fd, F_GETAUXFL, 0)) == -1
	|| fcntl (req.fd, F_SETAUXFL, flags | O_ONESIGFD) == -1)
	throw IOError ("fcntl()", errno);

    IOSelectSig::attach (req);
}

/** Remove the client registration at @a index.  */
void
IOSelectSigFd::detach (const IOSelectRequest &req)
{
    ASSERT (req.fd >= 0);
    int flags;
    if ((flags = fcntl (req.fd, F_GETAUXFL, 0)) == -1
	|| fcntl (req.fd, F_SETAUXFL, flags & ~O_ONESIGFD) == -1)
	throw IOError ("fcntl()", errno);

    IOSelectSig::detach (req);
}

} // namespace lat
#endif // _POSIX_REALTIME_SIGNALS > 0 && defined F_SETSIG && defined F_SETAUXFL
