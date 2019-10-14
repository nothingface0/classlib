#ifndef CLASSLIB_PUSH_BACK_BUFFER_H
# define CLASSLIB_PUSH_BACK_BUFFER_H
# include "classlib/iotools/FilterInputStream.h"

namespace lat {

class PushBackBuffer
{
public:
    enum { DEFAULT_SIZE = 100 }; //< Default buffer size.
    PushBackBuffer (IOSize bufsize = DEFAULT_SIZE);
    PushBackBuffer (const PushBackBuffer &x);
    PushBackBuffer &operator= (const PushBackBuffer &x);
    ~PushBackBuffer (void);

    IOSize		size (void) const;
    IOSize		capacity (void) const;

    unsigned char *	current (void) const;
    unsigned char *	end (void) const;

    void		reset (void);

    void		skip (IOSize n);
    void		get (void *into, IOSize n);
    void		unget (const void *data, IOSize n);

private:
    unsigned char	*m_buffer;
    unsigned char	*m_current;
    unsigned char	*m_limit;
};

} // namespace lat
#endif // CLASSLIB_PUSHBACK_INPUT_STREAM_H
