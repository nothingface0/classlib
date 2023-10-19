#ifndef CLASSLIB_CHECKED_INPUT_STREAM_H
# define CLASSLIB_CHECKED_INPUT_STREAM_H
# include "classlib/sysapi/system.h"
# include "classlib/iotools/FilterInputStream.h"

namespace lat {
class Checksum;

class CheckedInputStream : public FilterInputStream
{
public:
    CheckedInputStream (InputStream *input, Checksum *validator);
    ~CheckedInputStream (void);

    using FilterInputStream::read;
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	skip (IOSize n);
    virtual void	reset (void);
    virtual Checksum *	checksum (void) const;

private:
    Checksum		*m_checksum;	//< Checksum calculator
};

} // namespace lat
#endif // CLASSLIB_CHECKED_INPUT_STREAM_H
