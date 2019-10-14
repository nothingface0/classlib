#ifndef CLASSLIB_IOSELECT_REQUEST_H
# define CLASSLIB_IOSELECT_REQUEST_H
# include "classlib/sysapi/IOTypes.h"
# include "classlib/iobase/IOSelectCB.h"

namespace lat {

/** Structure for recording information about the client requests.
    Remembers the file descriptor and the owner channel (the two
    represent identical information for convenience of the actual
    selection method), the event selection mask and the client
    callback.  This structure is only for internal use.  */
struct IOSelectRequest
{
    IOFD	fd;		//< The file descriptor
    IOChannel	*channel;	//< The source channel
    unsigned	mask;		//< Event interest selection mask
    IOSelectCB	cb;		//< Notification callback
};

} // namespace lat
#endif // CLASSLIB_IOSELECT_REQUEST_H
