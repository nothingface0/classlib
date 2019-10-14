#ifndef CLASSLIB_PIPE_H
# define CLASSLIB_PIPE_H
# include "classlib/iobase/IOChannelPair.h"
# include "classlib/iobase/IOChannel.h"

namespace lat {

/** Pair of file descriptors connected to each other.  */
class Pipe : public IOChannelPair
{
    // FIXME: block SIGPIPE -> EPIPE -> throw?  (write only, read -> eof)
public:
    Pipe (bool create = true);
    // implicit copy constructor
    // implicit destructor
    // implicit assignment operator

    virtual void	open (void);
    virtual IOChannel *	source (void);
    virtual IOChannel *	sink (void);

private:
    IOChannel		m_source;	//< Source channel
    IOChannel		m_sink;		//< Sink channel
};

} // namespace lat
#endif // CLASSLIB_PIPE_H
