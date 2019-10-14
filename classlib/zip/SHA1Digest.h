#ifndef CLASSLIB_SHA1_DIGEST_H
# define CLASSLIB_SHA1_DIGEST_H
# include "classlib/zip/Digest.h"

namespace lat {

/** Calculate a checksum of a data stream using the SHA1 algorithm.  */
class SHA1Digest : public Digest
{
public:
    SHA1Digest (void);
    ~SHA1Digest (void);

    using Digest::update;
    virtual Value	digest (void) const;
    virtual void	update (const void *data, IOSize n);
    virtual void	reset (void);

private:
    struct Context;
    Context		*m_context;
};

} // namespace lat
#endif // CLASSLIB_SHA1_DIGEST_H
