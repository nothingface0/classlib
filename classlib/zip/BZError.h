#ifndef CLASSLIB_BZ_ERROR_H
# define CLASSLIB_BZ_ERROR_H
# include "classlib/sysapi/system.h"
# include "classlib/utils/Error.h"

namespace lat {

/** BZLIB error indication.  */
class BZError : public Error
{
public:
    BZError (int code);
    // implicit destructor
    // implicit copy constructor
    // implicit assignment operator

    virtual int		code (void) const;
    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    int			m_code;		//< BZLIB error code
};

} // namespace lat
#endif // CLASSLIB_BZ_ERROR_H
