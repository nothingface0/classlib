#ifndef CLASSLIB_CRC32_H
# define CLASSLIB_CRC32_H
# include "classlib/zip/Checksum.h"

namespace lat {

/** Compute a CRC-32 checksum of a data stream.  */
class CRC32 : public Checksum
{
public:
    CRC32 (void);
    // implicit copy constructor
    // implicit destructor
    // implicit copy constructor

    virtual unsigned	value (void) const;
    virtual void	update (const void *data, IOSize n);
    virtual void	reset (void);

private:
    unsigned 		m_value;	//< Current checksum value
};

} // namespace lat
#endif // CLASSLIB_CRC32_H
