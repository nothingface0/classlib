#ifndef CLASSLIB_Z_ERROR_H
# define CLASSLIB_Z_ERROR_H
# include "classlib/sysapi/system.h"
# include "classlib/utils/Error.h"

namespace lat {

/** ZLIB error indication.  */
class ZError : public Error
{
public:
    ZError (int code, const char *message = 0);
    // implicit destructor
    // implicit copy constructor
    // implicit assignment operator

    virtual int			code (void) const;
    virtual const std::string &	message (void) const;
    virtual std::string		explainSelf (void) const;
    virtual Error *		clone (void) const;
    virtual void		rethrow (void);

private:
    int				m_code;		//< ZLIB error code
    std::string			m_message;	//< ZLIB error message
};

} // namespace lat
#endif // CLASSLIB_Z_ERROR_H
