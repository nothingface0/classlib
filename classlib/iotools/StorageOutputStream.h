#ifndef CLASSLIB_STORAGE_OUTPUT_STREAM_H
# define CLASSLIB_STORAGE_OUTPUT_STREAM_H
# include "classlib/iotools/OutputStream.h"

namespace lat {
class Storage;

class StorageOutputStream : public OutputStream
{
public:
    StorageOutputStream (Storage *output);
    ~StorageOutputStream (void);

    using OutputStream::write;
    virtual IOSize	write (const void *from, IOSize n);
    virtual IOSize	writev (const IOBuffer *from, IOSize buffers);
    virtual void	close (void);

protected:
    Storage		*m_storage;	//< Actual output
};


} // namespace lat
#endif // CLASSLIB_STORAGE_OUTPUT_STREAM_H
