#ifndef CLASSLIB_IO_CHANNEL_OUTPUT_STREAM_H
# define CLASSLIB_IO_CHANNEL_OUTPUT_STREAM_H
# include "classlib/iotools/OutputStream.h"

namespace lat {
class IOChannel;

class IOChannelOutputStream : public OutputStream
{
public:
    IOChannelOutputStream (IOChannel *output);
    ~IOChannelOutputStream (void);

    using OutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual IOSize	writev (const IOBuffer *from, IOSize buffers);
    virtual void	close (void);

protected:
    IOChannel		*m_output;	//< Actual output
};

} // namespace lat
#endif // CLASSLIB_IO_CHANNEL_OUTPUT_STREAM_H
