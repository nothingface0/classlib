#ifndef CLASSLIB_TAR_MEMBER_H
# define CLASSLIB_TAR_MEMBER_H
# include "classlib/zip/TarConstants.h"
# include "classlib/iobase/Filename.h"
# include "classlib/sysapi/nonstdint.h"

namespace lat {

/** A TAR archive member.

    This class describes a single TAR archive member.  The member
    cannot be read or written through this class, use @TarInputStream
    or @TarOutputStream instead.

    Note that when adding a member to @TarOutputStream, the file size
    must be known or the stream must be told to buffer the data to
    find out the size on its own.  If the size is given, it must match
    the actual amount of data output (this is verified).  */
class TarMember : public TarConstants
{
public:
    TarMember (void);
    TarMember (Filename name);
    TarMember (const TarMember &x);
    TarMember &operator= (const TarMember &x);
    ~TarMember (void);

    Filename		name (void) const;
    uintmax_t		mode (void) const;
    intmax_t		mtime (void) const;
    double		mtimefrac (void) const;
    intmax_t		atime (void) const;
    double		atimefrac (void) const;
    intmax_t		size (void) const;
    uintmax_t		uid (void) const;
    std::string		uname (void) const;
    uintmax_t		gid (void) const;
    std::string		gname (void) const;
    Type		type (void) const;
    Filename		linkname (void) const;
    uintmax_t		devmajor (void) const;
    uintmax_t		devminor (void) const;

    void		name (Filename name);
    void		mode (uintmax_t value);
    void		mtime (intmax_t value, double subsec = 0);
    void		atime (intmax_t value, double subsec = 0);
    void		size (intmax_t value);
    void		uid (uintmax_t value);
    void		uname (const std::string &name);
    void		gid (uintmax_t value);
    void		gname (const std::string &name);
    void		type (Type value);
    void		type (char value);
    void		linkname (Filename value);
    void		device (uintmax_t major, uintmax_t  minor);

    void		reset (void);

private:
    Filename		m_name;		//< Name of the archive member
    intmax_t		m_mtime;	//< Modification time
    double		m_mtimefrac;	//< Modification time, subsecond fraction
    intmax_t		m_atime;	//< Access time
    double		m_atimefrac;	//< Access time, subsecond fraction
    // unsigned long	m_ctime;	//< Creation time (GNU extension)
    IOOffset		m_size;		//< File size
    uintmax_t		m_mode;		//< UNIX file mode
    uintmax_t		m_uid;		//< File owner id
    std::string		m_uname;	//< File owner name
    uintmax_t		m_gid;		//< File group id
    std::string		m_gname;	//< File group name
    Type		m_type;		//< File type
    Filename		m_link;		//< Symlink target for links
    uintmax_t		m_devmajor;	//< Device major number for devices
    uintmax_t		m_devminor;	//< Device minor number for devices
};

} // namespace lat
#endif // CLASSLIB_TAR_MEMBER_H
