#ifndef CLASSLIB_BUFFER_INPUT_STREAM_H
# define CLASSLIB_BUFFER_INPUT_STREAM_H
# include "classlib/iotools/FilterInputStream.h"

namespace lat {
class BufferInputStream : public FilterInputStream
{
public:
    BufferInputStream (InputStream *input, IOSize bufsize = 0);
    ~BufferInputStream (void);

    using FilterInputStream::read;
    virtual IOOffset	available (void);
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	skip (IOSize n);
    virtual void	close (void);
    virtual void	reset (void);

protected:
    unsigned char	*m_buffer;
    unsigned char	*m_bottom;
    unsigned char	*m_top;
    unsigned char	*m_limit;
};
} // namespace lat
#endif // CLASSLIB_BUFFER_INPUT_STREAM_H
