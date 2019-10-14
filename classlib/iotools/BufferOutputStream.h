#ifndef CLASSLIB_BUFFER_OUTPUT_STREAM_H
# define CLASSLIB_BUFFER_OUTPUT_STREAM_H
# include "classlib/iotools/FilterOutputStream.h"

namespace lat {
class BufferOutputStream : public FilterOutputStream
{
public:
    BufferOutputStream (OutputStream *output, IOSize bufsize = 0);
    ~BufferOutputStream (void);

    using FilterOutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual IOSize	flush (void);
    virtual void	finish (void);
    virtual void	close (void);

protected:
    unsigned char	*m_buffer;
    unsigned char	*m_top;
    unsigned char	*m_limit;
};
} // namespace lat
#endif // CLASSLIB_BUFFER_OUTPUT_STREAM_H
