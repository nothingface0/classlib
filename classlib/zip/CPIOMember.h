#ifndef CLASSLIB_CPIO_MEMBER_H
# define CLASSLIB_CPIO_MEMBER_H
# include "classlib/zip/CPIOConstants.h"
# include "classlib/iobase/Filename.h"
# include "classlib/sysapi/nonstdint.h"

namespace lat {

/** A CPIO archive member.

    This class describes a single CPIO archive member.  The member
    cannot be read or written through this class, use #CPIOInputStream
    or #CPIOOutputStream instead.

    Note that when adding a member to #CPIOOutputStream, the file size
    must be known or the stream must be told to buffer the data to
    find out the size on its own.  If the size is given, it must match
    the actual amount of data output (this is asserted).  */
class CPIOMember : public CPIOConstants
{
public:
    CPIOMember (Filename name);
    CPIOMember (const CPIOMember &x);
    CPIOMember &operator= (const CPIOMember &x);
    ~CPIOMember (void);

    Filename		name (void) const;
    uint32_t		mode (void) const;
    uint32_t		mtime (void) const;
    IOOffset		size (void) const;
    uint32_t		uid (void) const;
    uint32_t		gid (void) const;
    uint32_t		rdev (void) const;
    uint32_t		rdevmajor (void) const;
    uint32_t		rdevminor (void) const;
    uint32_t		dev (void) const;
    uint32_t		devmajor (void) const;
    uint32_t		devminor (void) const;
    uint32_t		ino (void) const;
    uint32_t		nlink (void) const;

    void		name (Filename name);
    void		mode (uint32_t value);
    void		mtime (uint32_t value);
    void		size (IOOffset value);
    void		uid (uint32_t value);
    void		gid (uint32_t value);
    void		rdev (uint32_t value);
    void		rdev (uint32_t major, uint32_t minor);
    void		dev (uint32_t value);
    void		dev (uint32_t major, uint32_t minor);
    void		ino (uint32_t value);
    void		nlink (uint32_t value);

private:
    Filename		m_name;		//< Name of the archive member
    uint32_t		m_mtime;	//< Modification time
    IOOffset		m_size;		//< File size
    uint32_t		m_mode;		//< UNIX file mode
    uint32_t		m_uid;		//< File owner id
    uint32_t		m_gid;		//< File group id
    uint32_t		m_rdevmajor;	//< Device type (major part)
    uint32_t		m_rdevminor;	//< Device type (minor part)
    uint32_t		m_devmajor;	//< Device id (major part)
    uint32_t		m_devminor;	//< Device id (minor part)
    uint32_t		m_ino;		//< File inode on the device
    uint32_t		m_nlink;	//< Number of links to the file
};

} // namespace lat
#endif // CLASSLIB_CPIO_MEMBER_H
