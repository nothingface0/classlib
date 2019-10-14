#include "classlib/zip/MD5Digest.h"
#include "openssl/evp.h"

namespace lat {

struct MD5Digest::Context : EVP_MD_CTX
{
  unsigned char resval[EVP_MAX_MD_SIZE];
  unsigned int reslen;
};

/** Construct a new MD5 digest.  */
MD5Digest::MD5Digest (void)
    : m_context (new Context)
{
  EVP_MD_CTX_init(m_context);
  EVP_DigestInit_ex(m_context, EVP_md5(), 0);
  m_context->reslen = 0;
}

/** Destroy a digest.  No-op.  */
MD5Digest::~MD5Digest (void)
{
  EVP_MD_CTX_cleanup(m_context);
  delete m_context;
}

/** Return the final digest value.  */
Digest::Value
MD5Digest::digest (void) const
{
  if (! m_context->reslen)
    EVP_DigestFinal_ex(m_context, &m_context->resval[0], &m_context->reslen);

  return Value(&m_context->resval[0], &m_context->resval[0] + m_context->reslen);
}

/** Update the digest with @a data of length @a n.  */
void
MD5Digest::update (const void *data, IOSize n)
{
  EVP_DigestUpdate(m_context, data, n);
}

/** Reset the digest.  */
void
MD5Digest::reset (void)
{
  EVP_MD_CTX_cleanup(m_context);
  EVP_MD_CTX_init(m_context);
  EVP_DigestInit_ex(m_context, EVP_md5(), 0);
  m_context->reslen = 0;
}

} // namespace lat
