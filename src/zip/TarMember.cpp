#include "classlib/zip/TarMember.h"

namespace lat {

TarMember::TarMember (void)
{ reset(); }

TarMember::TarMember (Filename name)
{ reset(); m_name = name; }

TarMember::TarMember (const TarMember &x)
    : m_name (x.m_name),
      m_mtime (x.m_mtime),
      m_mtimefrac (x.m_mtimefrac),
      m_atime (x.m_atime),
      m_atimefrac (x.m_atimefrac),
      m_size (x.m_size),
      m_mode (x.m_mode),
      m_uid (x.m_uid),
      m_uname (x.m_uname),
      m_gid (x.m_gid),
      m_gname (x.m_gname),
      m_type (x.m_type),
      m_link (x.m_link),
      m_devmajor (x.m_devmajor),
      m_devminor (x.m_devminor)
{}

TarMember::~TarMember (void)
{}

TarMember &
TarMember::operator= (const TarMember &x)
{
    m_name	= x.m_name;
    m_mtime	= x.m_mtime;
    m_mtimefrac	= x.m_mtimefrac;
    m_atime	= x.m_atime;
    m_atimefrac	= x.m_atimefrac;
    m_size	= x.m_size;
    m_mode	= x.m_mode;
    m_uid	= x.m_uid;
    m_uname	= x.m_uname;
    m_gid	= x.m_gid;
    m_gname	= x.m_gname;
    m_type	= x.m_type;
    m_link	= x.m_link;
    m_devmajor	= x.m_devmajor;
    m_devminor	= x.m_devminor;
    return *this;
}

//////////////////////////////////////////////////////////////////////
Filename
TarMember::name (void) const
{ return m_name; }

void
TarMember::name (Filename name)
{ m_name = name; }

uintmax_t
TarMember::mode (void) const
{ return m_mode; }

void
TarMember::mode (uintmax_t value)
{ m_mode = value; }

intmax_t
TarMember::mtime (void) const
{ return m_mtime; }

double
TarMember::mtimefrac (void) const
{ return m_mtimefrac; }

void
TarMember::mtime (intmax_t value, double subsec /* = 0 */)
{ m_mtime = value; m_mtimefrac = subsec; }

intmax_t
TarMember::atime (void) const
{ return m_atime; }

double
TarMember::atimefrac (void) const
{ return m_atimefrac; }

void
TarMember::atime (intmax_t value, double subsec /* = 0 */)
{ m_atime = value; m_atimefrac = subsec; }

intmax_t
TarMember::size (void) const
{ return m_size; }

void
TarMember::size (intmax_t value)
{ m_size = value; }
// If typeflag=={LNKTYPE,SYMTYPE,DIRTYPE} then size must be 0.

uintmax_t
TarMember::uid (void) const
{ return m_uid; }

void
TarMember::uid (uintmax_t value)
{ m_uid = value; }

std::string
TarMember::uname (void) const
{ return m_uname; }

void
TarMember::uname (const std::string &value)
{ m_uname = value; }

uintmax_t
TarMember::gid (void) const
{ return m_gid; }

void
TarMember::gid (uintmax_t value)
{ m_gid = value; }

std::string
TarMember::gname (void) const
{ return m_gname; }

void
TarMember::gname (const std::string &value)
{ m_gname = value; }

TarConstants::Type
TarMember::type (void) const
{ return m_type; }

void
TarMember::type (Type value)
{ m_type = value; }

void
TarMember::type (char value)
{ m_type = (Type) value; }

Filename
TarMember::linkname (void) const
{ return m_link; }

void
TarMember::linkname (Filename value)
{ m_link = value; }

uintmax_t
TarMember::devmajor (void) const
{ return m_devmajor; }

uintmax_t
TarMember::devminor (void) const
{ return m_devminor; }

void
TarMember::device (uintmax_t major, uintmax_t minor)
{ m_devmajor = major; m_devminor = minor; }

void
TarMember::reset (void)
{
    m_name = "";
    m_mtime = 0;
    m_mtimefrac = 0;
    m_atime = 0;
    m_atimefrac = 0;
    m_size = 0;
    m_mode = 0;
    m_uid = 0;
    m_gid = 0;
    m_type = REGULAR;
    m_devmajor = 0;
    m_devminor = 0;
}

} // namespace lat
