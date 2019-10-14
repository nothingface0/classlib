#ifndef CLASSLIB_FILE_SYSTEM_ERROR_H
# define CLASSLIB_FILE_SYSTEM_ERROR_H
# include "classlib/iobase/IOError.h"

namespace lat {

/** File-system related exception class. */
class FileSystemError : public IOError
{
public:
    FileSystemError (const char *context, int code = 0);
    FileSystemError (const char *context, Error *chain);
    FileSystemError (Error *chain);

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);
};

} // namespace lat
#endif // CLASSLIB_FILE_SYSTEM_ERROR_H
