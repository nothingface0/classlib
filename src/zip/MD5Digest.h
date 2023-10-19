#ifndef CLASSLIB_MD5_DIGEST_H
# define CLASSLIB_MD5_DIGEST_H
# include "classlib/zip/Digest.h"

namespace lat {

/** Calculate a checksum of a data stream using the MD5 algorithm.  */
class MD5Digest : public Digest
{
public:
    MD5Digest (void);
    ~MD5Digest (void);

    using Digest::update;
    virtual Value	digest (void) const;
    virtual void	update (const void *data, IOSize n);
    virtual void	reset (void);

private:
    struct Context;
    Context		*m_context;
};

} // namespace lat
#endif // CLASSLIB_MD5_DIGEST_H
