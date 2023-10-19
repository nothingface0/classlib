#include "classlib/zip/CPIOMember.h"

namespace lat {

CPIOMember::CPIOMember (Filename name)
    : m_name (name),
      m_mtime (0),
      m_size (0),
      m_mode (0),
      m_uid (0),
      m_gid (0),
      m_rdevmajor (0),
      m_rdevminor (0),
      m_devmajor (0),
      m_devminor (0),
      m_ino (0),
      m_nlink (0)
{}

CPIOMember::CPIOMember (const CPIOMember &x)
    : m_name (x.m_name),
      m_mtime (x.m_mtime),
      m_size (x.m_size),
      m_mode (x.m_mode),
      m_uid (x.m_uid),
      m_gid (x.m_gid),
      m_rdevmajor (x.m_rdevmajor),
      m_rdevminor (x.m_rdevminor),
      m_devmajor (x.m_devmajor),
      m_devminor (x.m_devminor),
      m_ino (x.m_ino),
      m_nlink (x.m_nlink)
{}

CPIOMember::~CPIOMember (void)
{}

CPIOMember &
CPIOMember::operator= (const CPIOMember &x)
{
    m_name	= x.m_name;
    m_mtime	= x.m_mtime;
    m_size	= x.m_size;
    m_mode	= x.m_mode;
    m_uid	= x.m_uid;
    m_gid	= x.m_gid;
    m_rdevmajor	= x.m_rdevmajor;
    m_rdevminor	= x.m_rdevminor;
    m_devmajor	= x.m_devmajor;
    m_devminor	= x.m_devminor;
    m_ino	= x.m_ino;
    m_nlink	= x.m_nlink;
    return *this;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Filename
CPIOMember::name (void) const
{ return m_name; }

void
CPIOMember::name (Filename name)
{ m_name = name; } // FIXME: check length?

//////////////////////////////////////////////////////////////////////
uint32_t
CPIOMember::mode (void) const
{ return m_mode; }

void
CPIOMember::mode (uint32_t value)
{ m_mode = value; } // FIXME: check against permitted values?

//////////////////////////////////////////////////////////////////////
uint32_t
CPIOMember::mtime (void) const
{ return m_mtime; }

void
CPIOMember::mtime (uint32_t value)
{ m_mtime = value; }

//////////////////////////////////////////////////////////////////////
IOOffset
CPIOMember::size (void) const
{ return m_size; }

void
CPIOMember::size (IOOffset value)
{ m_size = value; }

// If typeflag=={LNKTYPE,SYMTYPE,DIRTYPE} then size must be 0.

//////////////////////////////////////////////////////////////////////
uint32_t
CPIOMember::uid (void) const
{ return m_uid; }

void
CPIOMember::uid (uint32_t value)
{ m_uid = value; }

//////////////////////////////////////////////////////////////////////
uint32_t
CPIOMember::gid (void) const
{ return m_gid; }

void
CPIOMember::gid (uint32_t value)
{ m_gid = value; }

//////////////////////////////////////////////////////////////////////
uint32_t
CPIOMember::rdev (void) const
{ return (m_rdevmajor << 8) | m_rdevminor; }

void
CPIOMember::rdev (uint32_t value)
{ m_rdevmajor = (value >> 8) & 0xff; m_rdevminor = value & 0xff; }

uint32_t
CPIOMember::rdevmajor (void) const
{ return m_rdevmajor; }

uint32_t
CPIOMember::rdevminor (void) const
{ return m_rdevminor; }

void
CPIOMember::rdev (uint32_t major, uint32_t minor)
{ m_rdevmajor = major; m_rdevminor = minor; }

//////////////////////////////////////////////////////////////////////
uint32_t
CPIOMember::dev (void) const
{ return (m_devmajor << 8) | m_devminor; }

void
CPIOMember::dev (uint32_t value)
{ m_devmajor = (value >> 8) & 0xff; m_devminor = value & 0xff; }

uint32_t
CPIOMember::devmajor (void) const
{ return m_devmajor; }

uint32_t
CPIOMember::devminor (void) const
{ return m_devminor; }

void
CPIOMember::dev (uint32_t major, uint32_t minor)
{ m_devmajor = major; m_devminor = minor; }

//////////////////////////////////////////////////////////////////////
uint32_t
CPIOMember::ino (void) const
{ return m_ino; }

void
CPIOMember::ino (uint32_t value)
{ m_ino = value; }

//////////////////////////////////////////////////////////////////////
uint32_t
CPIOMember::nlink (void) const
{ return m_nlink; }

void
CPIOMember::nlink (uint32_t value)
{ m_nlink = value; }

} // namespace lat
