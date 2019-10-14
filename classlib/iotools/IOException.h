#ifndef CLASSLIB_IO_EXCEPTION_H
# define CLASSLIB_IO_EXCEPTION_H
# include "classlib/sysapi/system.h"
# include "classlib/utils/Error.h"

namespace lat {

/** An I/O-related error condition.  */
class IOException : public Error
{
public:
    IOException (Error *next = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual std::string		explainSelf (void) const;
    virtual Error *		clone (void) const;
    virtual void		rethrow (void);
};

} // namespace lat
#endif // CLASSLIB_IO_EXCEPTION_H
