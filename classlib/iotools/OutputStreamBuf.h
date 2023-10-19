#ifndef CLASSLIB_OUTPUT_STREAM_BUF_H
# define CLASSLIB_OUTPUT_STREAM_BUF_H
# include "classlib/iotools/CustomStreamBuf.h"
# include "classlib/iotools/WriteBuffer.h"

namespace lat {
class OutputStream;

class OutputStreamBuf : public CustomStreamBuf
{
public:
    OutputStreamBuf (OutputStream *data = 0);
    ~OutputStreamBuf (void);

    virtual OutputStream *stream (void);
    virtual void	attach (OutputStream *to);
    virtual bool	attached (void) const;
    virtual void	detach (void);
    virtual bool	close (void);

    virtual int_type	overflow (int_type c = traits_type::eof ());
    virtual int_type	sync (void);
    virtual size_type	xsputn (const char_type *data, size_type length);

private:
    bool		syncwrite (void);
    size_type		writable (void);

    OutputStream	*m_data;
    WriteBuffer		m_buffer;
};

} // namespace lat
#endif // CLASSLIB_OUTPUT_STREAM_BUF_H
