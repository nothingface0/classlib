#include "classlib/zip/SHA1Digest.h"
#include "classlib/zip/SHA1Error.h"
#include "openssl/evp.h"

namespace lat {

struct SHA1Digest::Context : EVP_MD_CTX
{
  unsigned char resval[EVP_MAX_MD_SIZE];
  unsigned int reslen;
};

/** Construct a new SHA1 digest.  */
SHA1Digest::SHA1Digest (void)
    : m_context (new Context)
{
  EVP_MD_CTX_init(m_context);
  EVP_DigestInit_ex(m_context, EVP_sha1(), 0);
  m_context->reslen = 0;
}

/** Destroy a digest.  No-op.  */
SHA1Digest::~SHA1Digest (void)
{
  EVP_MD_CTX_cleanup(m_context);
  delete m_context;
}

/** Return the final digest value.  */
Digest::Value
SHA1Digest::digest (void) const
{
  if (! m_context->reslen)
    EVP_DigestFinal_ex(m_context, &m_context->resval[0], &m_context->reslen);

  return Value(&m_context->resval[0], &m_context->resval[0] + m_context->reslen);
}

/** Update the digest with @a data of length @a n.  */
void
SHA1Digest::update (const void *data, IOSize n)
{
  EVP_DigestUpdate(m_context, data, n);
}

/** Reset the digest.  */
void
SHA1Digest::reset (void)
{
  EVP_MD_CTX_cleanup(m_context);
  EVP_MD_CTX_init(m_context);
  EVP_DigestInit_ex(m_context, EVP_sha1(), 0);
  m_context->reslen = 0;
}

} // namespace lat
