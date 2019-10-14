#ifndef CLASSLIB_OUTPUT_STREAM_H
# define CLASSLIB_OUTPUT_STREAM_H
# include "classlib/sysapi/system.h"
# include "classlib/iobase/IOOutput.h"

namespace lat {

/** An output data destination.  */
class OutputStream : public virtual IOOutput
{
public:
    virtual ~OutputStream (void);

    virtual void	finish (void);
    virtual void	close (void);

protected:
    OutputStream (void);

private:
    // undefined, no semantics
    OutputStream (const OutputStream &);
    OutputStream &operator= (const OutputStream &);
};

} // namespace lat
#endif // CLASSLIB_OUTPUT_STREAM_H
