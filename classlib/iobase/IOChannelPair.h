#ifndef CLASSLIB_IO_CHANNEL_PAIR_H
# define CLASSLIB_IO_CHANNEL_PAIR_H
# include "classlib/sysapi/IOTypes.h"

namespace lat {
class IOChannel;

/** Base class for paired I/O channels such as pipes, socket pairs,
    terminals etc.  (FIXME) */
class IOChannelPair
{
public:
    virtual ~IOChannelPair (void);
    // implicit copy constructor
    // implicit assignment operator

    virtual IOChannel *	sink (void) = 0;
    virtual IOChannel *	source (void) = 0;
};

} // namespace lat
#endif // CLASSLIB_IO_CHANNEL_PAIR_H
