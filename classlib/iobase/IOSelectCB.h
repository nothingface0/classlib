#ifndef CLASSLIB_IOSELECT_CB_H
# define CLASSLIB_IOSELECT_CB_H
# include "classlib/iobase/IOSelectEvent.h"
# include "classlib/utils/Hook.h"

namespace lat {

/** Callback for clients to receive notifications from #IOSelector.
    It gets as an argument the event that triggered notification.
    Return value @c false indicates the client and the channel to be
    removed, @c true indicates all is ok.  */
typedef Hook1<bool,IOSelectEvent *> IOSelectCB;

} // namespace lat
#endif // CLASSLIB_IOSELECT_CB_H
