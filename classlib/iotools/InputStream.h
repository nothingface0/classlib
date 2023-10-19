#ifndef CLASSLIB_INPUT_STREAM_H
# define CLASSLIB_INPUT_STREAM_H
# include "classlib/sysapi/system.h"
# include "classlib/iobase/IOInput.h"

namespace lat {

/** An input data source.  */
class InputStream : public virtual IOInput
{
public:
    virtual ~InputStream (void);

    virtual IOOffset	available (void);
    virtual IOSize	skip (IOSize n);
    virtual void	close (void);
    virtual void	reset (void);

protected:
    static const int	SKIP_BUFFER_SIZE = 1024; //< Local buffer size for skip
    InputStream (void);

private:
    // undefined, no semantics
    InputStream (const InputStream &);
    InputStream &operator= (const InputStream &);
};

} // namespace lat
#endif // CLASSLIB_INPUT_STREAM_H
