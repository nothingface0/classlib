#ifndef CLASSLIB_CHECKSUM_H
# define CLASSLIB_CHECKSUM_H
# include "classlib/sysapi/system.h"
# include "classlib/iobase/IOBuffer.h"

namespace lat {

/** Calculate a checksum of a data stream.  */
class Checksum
{
public:
    virtual ~Checksum (void);

    virtual unsigned	value (void) const = 0;
    virtual void	update (const void *data, IOSize n) = 0;
    void		update (unsigned char byte);
    void		update (IOBuffer data);
    virtual void	reset (void) = 0;
};

} // namespace lat
#endif // CLASSLIB_CHECKSUM_H
