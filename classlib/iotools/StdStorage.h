#ifndef CLASSLIB_STD_STORAGE_H
# define CLASSLIB_STD_STORAGE_H
# include "classlib/sysapi/system.h"
# include "classlib/iobase/Storage.h"
# include <string>
# include <iostream>

namespace lat {

class StdStorage : public Storage
{
public:
    StdStorage (void);
    StdStorage (const char *name, std::ios_base::openmode mode
		= std::ios_base::in | std::ios_base::out);
    StdStorage (const std::string &name, std::ios_base::openmode mode
		= std::ios_base::in | std::ios_base::out);
    StdStorage (std::iostream *stream);
    ~StdStorage (void);

    virtual void	open (const char *name, std::ios_base::openmode mode
			      = std::ios_base::in | std::ios_base::out);
    virtual void	open (const std::string &name, std::ios_base::openmode mode
			      = std::ios_base::in | std::ios_base::out);

    virtual bool	hasError (void) const;
    virtual void	clearError (void);

    using Storage::read;
    using Storage::write;

    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	write (const void *from, IOSize n);

    virtual IOOffset	position (void) const;
    virtual IOOffset	position (IOOffset offset, Relative whence = SET);
    virtual void	resize (IOOffset size);

    virtual void	flush (void);
    virtual void	close (void);

protected:
    std::iostream	*m_stream;
};

} // namespace lat
#endif // CLASSLIB_STD_STORAGE_H
