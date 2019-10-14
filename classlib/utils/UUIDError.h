#ifndef CLASSLIB_UUID_ERROR_H
# define CLASSLIB_UUID_ERROR_H
# include "classlib/utils/Error.h"

namespace lat {

class UUIDError : public Error
{
public:
    UUIDError (void);
    // default destructor
    // default copy constructor
    // default assignment operator

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);
};

} // namespace lat
#endif // CLASSLIB_UUID_ERROR_H
