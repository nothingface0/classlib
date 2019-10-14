#ifndef CLASSLIB_STD_OUTPUT_STREAM_H
# define CLASSLIB_STD_OUTPUT_STREAM_H
# include "classlib/iotools/OutputStream.h"
# include <iostream>

namespace lat {

class StdOutputStream : public OutputStream
{
public:
    StdOutputStream (std::ostream *output);
    ~StdOutputStream (void);

    using OutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual void	finish (void);
    virtual void	close (void);

protected:
    std::ostream	*m_output;	//< Actual output.
};

} // namespace lat
#endif // CLASSLIB_STD_OUTPUT_STREAM_H
