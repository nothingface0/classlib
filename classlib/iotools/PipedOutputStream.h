#ifndef CLASSLIB_PIPED_OUTPUT_STREAM_H
# define CLASSLIB_PIPED_OUTPUT_STREAM_H
# include "classlib/iotools/OutputStream.h"

namespace lat {
class PipedInputStream;
class Pipe;

class PipedOutputStream : public OutputStream
{
public:
    PipedOutputStream (PipedInputStream *link = 0);
    ~PipedOutputStream (void);

    using OutputStream::write;
    virtual void	connect (PipedInputStream *to);
    virtual IOSize	write (const void *from, IOSize n);
    virtual void	close (void);

protected:
    friend class PipedInputStream;
    Pipe *		pipe (void) const;

    PipedInputStream	*m_link;
    Pipe		*m_pipe;
};

} // namespace lat
#endif // CLASSLIB_PIPED_OUTPUT_STREAM_H
