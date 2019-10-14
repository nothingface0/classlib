#ifndef CLASSLIB_IOSELECT_SIG_FD_H
# define CLASSLIB_IOSELECT_SIG_FD_H
# include "IOSelectSig.h"
# if _POSIX_REALTIME_SIGNALS > 0 && defined F_SETSIG && defined F_SETAUXFL

namespace lat {

/** Implementation of #IOSelectMethod using POSIX real-time queued
    SIGIO signal per file descriptor.  */
class IOSelectSigFd : public IOSelectBySig
{
public:
    // Attaching and detaching descriptors
    virtual void	attach   (const IOSelectRequest &req);
    virtual void	detach   (const IOSelectRequest &req);
};

} // namespace lat
# endif // _POSIX_REALTIME_SIGNALS > 0 && defined F_SETSIG && defined F_SETAUXFL
#endif // CLASSLIB_IOSELECT_SIG_FD_H
