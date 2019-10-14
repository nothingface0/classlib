
#include "classlib/utils/Log.h" 
#include <new>

namespace lat {

/** Logging device that discard all output.  #logstream uses this as a
    fallback when the user-specified device fails to initialise.  It
    may also be used explicitly at run time via the @c $LOG.  */
class SinkLog : public logstream
{
public:
    SinkLog (void);
    ~SinkLog (void);

protected:
    virtual void	emit (const char *string, int length);
};


logstream *
logstream::createSinkLogDevice (const char *)
{
    // make sure the log stays around beyond destructors
    static char		data[sizeof(SinkLog)];
    static SinkLog	*log = new (&data[0]) SinkLog;
    return log;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/** Initialise the sink.  */
SinkLog::SinkLog (void)
{ }

/** No-op.  */
SinkLog::~SinkLog (void)
{ }

/** Discard output. */
void
SinkLog::emit (const char *, int)
{ }

} // namespace lat
