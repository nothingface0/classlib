#ifndef CLASSLIB_SHA1_ERROR_H
# define CLASSLIB_SHA1_ERROR_H
# include "classlib/sysapi/system.h"
# include "classlib/utils/Error.h"

namespace lat {

/** Error indicator for SHA1 archives.  */
class SHA1Error : public Error
{
public:
    SHA1Error (int error);
    // implicit destructor
    // implicit copy constructor
    // implicit assignment operator

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    int			m_error;
};

} // namespace lat
#endif // CLASSLIB_SHA1_ERROR_H
