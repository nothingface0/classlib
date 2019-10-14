#ifndef CLASSLIB_NETWORK_ERROR_H
# define CLASSLIB_NETWORK_ERROR_H
# include "classlib/iobase/IOError.h"

namespace lat {

/** Network-related exception class. */
class NetworkError : public IOError
{
public:
    NetworkError (const char *context, int code = 0);
    NetworkError (const char *context, const char *errmsg);
    NetworkError (const char *context, Error *chain);
    NetworkError (Error *chain);

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);
};

} // namespace lat
#endif // CLASSLIB_NETWORK_ERROR_H
