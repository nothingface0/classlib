#ifndef CLASSLIB_LZO_ERROR_H
# define CLASSLIB_LZO_ERROR_H
# include "classlib/sysapi/system.h"
# include "classlib/utils/Error.h"

namespace lat {

/** LZO error indication.  */
class LZOError : public Error
{
public:
    LZOError (const char *context, int code);
    // implicit destructor
    // implicit copy constructor
    // implicit assignment operator

    virtual int		code (void) const;
    virtual const char *context (void) const;
    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    const char		*m_context;	//< LZO error context
    int			m_code;		//< LZO error code
};

} // namespace lat
#endif // CLASSLIB_LZO_ERROR_H
