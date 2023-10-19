#ifndef CLASSLIB_CHECKED_OUTPUT_STREAM_H
# define CLASSLIB_CHECKED_OUTPUT_STREAM_H
# include "classlib/sysapi/system.h"
# include "classlib/iotools/FilterOutputStream.h"

namespace lat {
class Checksum;

class CheckedOutputStream : public FilterOutputStream
{
public:
    CheckedOutputStream (OutputStream *output, Checksum *validator);
    ~CheckedOutputStream (void);

    using FilterOutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual Checksum *	checksum (void) const;

private:
    Checksum		*m_checksum;	//< Checksum calculator
};

} // namespace lat
#endif // CLASSLIB_CHECKED_OUTPUT_STREAM_H
