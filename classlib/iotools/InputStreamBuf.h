#ifndef CLASSLIB_INPUT_STREAM_BUF_H
# define CLASSLIB_INPUT_STREAM_BUF_H
# include "classlib/iotools/CustomStreamBuf.h"
# include "classlib/iotools/ReadBuffer.h"

namespace lat {
class InputStream;

class InputStreamBuf : public CustomStreamBuf
{
public:
    InputStreamBuf (InputStream *data = 0);
    ~InputStreamBuf (void);

    virtual InputStream	*stream (void);
    virtual void	attach (InputStream *to);
    virtual bool	attached (void) const;
    virtual void	detach (void);
    virtual bool	close (void);

    virtual size_type	showmanyc (void);
    virtual int_type	underflow (void);
    virtual int_type	uflow (void);
    virtual int_type	pbackfail (int_type c = traits_type::eof ());
    virtual size_type	xsgetn (char_type *data, size_type length);

private:
    size_type		refill (void);

    InputStream		*m_data;
    ReadBuffer		m_buffer;
};

} // namespace lat
#endif // CLASSLIB_INPUT_STREAM_BUF_H
