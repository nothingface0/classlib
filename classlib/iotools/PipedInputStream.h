#ifndef CLASSLIB_PIPED_INPUT_STREAM_H
# define CLASSLIB_PIPED_INPUT_STREAM_H
# include "classlib/iotools/InputStream.h"

namespace lat {
class PipedOutputStream;

class PipedInputStream : public InputStream
{
    friend class PipedOutputStream;
public:
    PipedInputStream (PipedOutputStream *link = 0);
    ~PipedInputStream (void);

    virtual void	connect (PipedOutputStream *to);

    using InputStream::read;
    virtual IOSize	read (void *into, IOSize n);
    virtual void	close (void);

protected:
    PipedOutputStream	*m_link;
};

} // namespace lat
#endif // CLASSLIB_PIPED_INPUT_STREAM_H
