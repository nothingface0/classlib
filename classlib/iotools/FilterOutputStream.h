#ifndef CLASSLIB_FILTER_OUTPUT_STREAM_H
# define CLASSLIB_FILTER_OUTPUT_STREAM_H
# include "classlib/iotools/OutputStream.h"

namespace lat {
class FilterOutputStream : public OutputStream
{
public:
    FilterOutputStream (OutputStream *output);
    ~FilterOutputStream (void);

    using OutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual void	finish (void);
    virtual void	close (void);

protected:
    OutputStream	*m_output;	//< Actual output
};
} // namespace lat
#endif // CLASSLIB_FILTER_OUTPUT_STREAM_H
