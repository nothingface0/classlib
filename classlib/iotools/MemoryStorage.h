#ifndef CLASSLIB_MEMORY_STORAGE_H
# define CLASSLIB_MEMORY_STORAGE_H
# include "classlib/sysapi/system.h"
# include "classlib/iobase/Storage.h"

namespace lat {

class MemoryStorage : public Storage
{
public:
    MemoryStorage (IOBuffer data = IOBuffer ((void *) 0, 0));
    MemoryStorage (const void *data, IOSize n);
    ~MemoryStorage (void);

    using Storage::read;
    using Storage::write;

    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	write (const void *from, IOSize n);

    virtual IOOffset	size (void) const;
    virtual IOOffset	position (void) const;
    virtual IOOffset	position (IOOffset offset, Relative whence = SET);
    virtual void	resize (IOOffset size);

    virtual IOBuffer	data (void) const;

protected:
    virtual void	extend (IOSize by);

    unsigned char	*m_buffer;	//< Buffer base pointer.
    unsigned char	*m_current;	//< Current "file position".
    unsigned char	*m_top;		//< High mark of anything written.
    unsigned char	*m_limit;	//< Top of allocated buffer extent.
    bool		m_growable;	//< Flag indicating whether buffer can grow.
};

} // namespace lat
#endif // CLASSLIB_MEMORY_STORAGE_H
