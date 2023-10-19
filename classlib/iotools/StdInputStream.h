#ifndef CLASSLIB_STD_INPUT_STREAM_H
# define CLASSLIB_STD_INPUT_STREAM_H
# include "classlib/iotools/InputStream.h"
# include <iostream>

namespace lat {

class StdInputStream : public InputStream
{
public:
    StdInputStream (std::istream *input);
    ~StdInputStream (void);

    using InputStream::read;
    virtual IOSize	read (void *into, IOSize n);
    virtual void	close (void);

protected:
    std::istream	*m_input;	//< Actual input.
};

} // namespace lat
#endif // CLASSLIB_STD_INPUT_STREAM_H
