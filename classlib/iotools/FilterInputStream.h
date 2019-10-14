#ifndef CLASSLIB_FILTER_INPUT_STREAM_H
# define CLASSLIB_FILTER_INPUT_STREAM_H
# include "classlib/iotools/InputStream.h"

namespace lat {
class FilterInputStream : public InputStream
{
public:
    FilterInputStream (InputStream *input);
    ~FilterInputStream (void);

    using InputStream::read;
    virtual IOOffset	available (void);
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	skip (IOSize n);
    virtual void	close (void);
    virtual void	reset (void);

protected:
    InputStream		*m_input;	//< Actual input
};
} // namespace lat
#endif // CLASSLIB_FILTER_INPUT_STREAM_H
