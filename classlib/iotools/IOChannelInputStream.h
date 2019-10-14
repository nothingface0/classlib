#ifndef CLASSLIB_IO_CHANNEL_INPUT_STREAM_H
# define CLASSLIB_IO_CHANNEL_INPUT_STREAM_H
# include "classlib/iotools/InputStream.h"

namespace lat {
class IOChannel;

class IOChannelInputStream : public InputStream
{
public:
    IOChannelInputStream (IOChannel *input);
    ~IOChannelInputStream (void);

    using InputStream::read;
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	readv (IOBuffer *into, IOSize buffers);
    virtual void	close (void);

protected:
    IOChannel		*m_input;	//< Actual input.
};

} // namespace lat
#endif // CLASSLIB_IO_CHANNEL_INPUT_STREAM_H
