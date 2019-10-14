#ifndef CLASSLIB_IOSELECT_EVENT_H
# define CLASSLIB_IOSELECT_EVENT_H
# include "classlib/sysapi/system.h"

namespace lat {
class IOChannel;

/** Event to be sent to #IOSelector clients when a channel becomes
    ready for I/O activity.  */
struct IOSelectEvent
{
    IOChannel	*source; //< The channel that is ready for I/O.
    unsigned	events;  //< The kind of I/O #source is ready for.
};

} // namespace lat
#endif // CLASSLIB_IOSELECT_EVENT_H
