#ifndef CLASSLIB_STORAGE_INPUT_STREAM_H
# define CLASSLIB_STORAGE_INPUT_STREAM_H
# include "classlib/iotools/InputStream.h"

namespace lat {
class Storage;

class StorageInputStream : public InputStream
{
public:
    StorageInputStream (Storage *input);
    ~StorageInputStream (void);

    using InputStream::read;
    virtual IOOffset	available (void);
    virtual IOSize	read (void *into, IOSize n);
    virtual IOSize	readv (IOBuffer *into, IOSize buffers);
    virtual IOSize	skip (IOSize n);
    virtual void	close (void);
    virtual void	reset (void);

protected:
    Storage		*m_storage;	//< Actual input.
};

} // namespace lat
#endif // CLASSLIB_STORAGE_INPUT_STREAM_H
