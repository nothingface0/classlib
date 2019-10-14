
#include "classlib/utils/UUID.h"
#include "classlib/utils/UUIDError.h"
#include "classlib/utils/Time.h"
#include "classlib/utils/DebugAids.h"
#include "classlib/sysapi/inttypes.h"
#ifdef _WIN32
# include <windows.h>
# include <rpc.h>
#else
# include <unistd.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/file.h>
# include <sys/ioctl.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <net/if.h>
# include <ifaddrs.h>
# include <arpa/inet.h>
# ifdef AF_LINK
#  include <net/if_dl.h>
# endif
# include <inttypes.h>
#endif
#include <cerrno>
#include <ctime>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iomanip>

/// Assume time has microsecond granularity.
#define MAX_ADJUSTMENT 10

/// Number of bytes in ethernet address.
#define ETH_ADDR_LEN 6

namespace lat {

bool     UUID::s_nodeinit = false;
uint8_t  UUID::s_nodeid[ETH_ADDR_LEN] = { 0, 0, 0, 0, 0, 0 };
int      UUID::s_randomfd = -2;
uint64_t UUID::s_lasttime = 0;
int      UUID::s_adjustment = 10;
uint16_t UUID::s_clockseq = 0;


/** Output a UUID @a id into stream @a out.  Uses #UUID::format() to
    format @a id into the standard 36-character form.  Returns @a out
    after output.  */
std::ostream &
operator<< (std::ostream &out, const UUID &id)
{
    char buf [37];
    id.format (buf);
    ASSERT (strlen(buf)==36);
    return out << buf;
}


/** Compare two UUIDs. Returns -1 if @a a comes before @a b,
    1 if it comes after, and zero if @a a and @a b are equal. */
int
UUID::compare(const Info &a, const Info &b)
{
    if (a.timeLow != b.timeLow)
	return a.timeLow < b.timeLow ? -1 : 1;
    if (a.timeMid != b.timeMid)
	return a.timeMid < b.timeMid ? -1 : 1;
    if (a.timeHiVer != b.timeHiVer)
	return a.timeHiVer < b.timeHiVer ? -1 : 1;
    if (a.clockSeq != b.clockSeq)
	return a.clockSeq < b.clockSeq ? -1 : 1;
    return memcmp(a.node, b.node, ETH_ADDR_LEN);
}

/** Compare UUIDs for equality.  Returns @c true if @c this and @a x
    are the same UUID, @c false otherwise.  */
bool
UUID::operator== (const UUID &x) const
{
    Info me, other;
    unpack(m_uuid, me);
    unpack(x.m_uuid, other);
    return compare (me, other) == 0;
}

/** Compare UUIDs for equality.  Returns @c true if @c this and @a x
    are not the same UUID, @c false otherwise.  */
bool
UUID::operator!= (const UUID &x) const
{
    Info me, other;
    unpack(m_uuid, me);
    unpack(x.m_uuid, other);
    return compare (me, other) != 0;
}

/** Order UUIDs.  Returns @c true if @c this is less than @a x
    according to lexicographic ordering rules, @c false otherwise.  */
bool
UUID::operator< (const UUID &x) const
{
    Info me, other;
    unpack(m_uuid, me);
    unpack(x.m_uuid, other);
    return compare(me, other) < 0;
}

/** Format the UUID into the buffer @a buf, which must have space at
    least for 37 bytes: 36 bytes for the string plus the terminating
    null.  */
void
UUID::format (char *buf) const
{
    ASSERT (DebugAids::validate (buf, 37, true));
    Info info;
    unpack(m_uuid, info);
    sprintf(buf,
            "%08x-%04x-%04x-%04x-%02x%02x%02x%02x%02x%02x",
            info.timeLow, info.timeMid, info.timeHiVer, info.clockSeq,
            info.node[0], info.node[1], info.node[2],
            info.node[3], info.node[4], info.node[5]);
}

/** Construct a null UUID.  */
UUID::UUID (void)
{
    memset(m_uuid, 0, sizeof(m_uuid));
}

/** Construct a UUID by parsing the string @a text, which must be in
    the textual 37-byte format (36 characters terminated by a null).  */
UUID::UUID (const char *text)
{ parse (text, strlen(text), m_uuid); }

/** Construct a UUID by parsing the string @a text, which must be in
    the textual 37-byte format (36 characters terminated by a null).  */
UUID::UUID (const std::string &text)
{ parse (text.c_str (), text.size(), m_uuid); }

/** Construct a UUID from the binary representation @a raw.  */
UUID::UUID (Binary raw)
{ memcpy(m_uuid, raw, sizeof(m_uuid)); }

/** Copy UUID @a x.  */
UUID::UUID (const UUID &x)
{ memcpy(m_uuid, x.m_uuid, sizeof(m_uuid)); }

/** Copy UUID @a x.  Returns reference to @c this after copy.  */
UUID &
UUID::operator= (const UUID &x)
{ memcpy(m_uuid, x.m_uuid, sizeof(m_uuid)); return *this; }

//////////////////////////////////////////////////////////////////////
/** Check if the UUID is null: all zeroes.  */
bool
UUID::isNull (void) const
{
    for (int i = 0; i < 16; i++)
	if (m_uuid[i])
	    return false;

    return true;
}

/** Return the time component of the UUID.  This is 60-bit value
    representing the number of 100 nanosecond internvals since
    15 October 1582 00:00:00 UTC. */
int64_t
UUID::time (void) const
{
    Info info;
    unpack(m_uuid, info);

    int64_t high = info.timeMid | ((info.timeHiVer & 0xfff) << 16);
    return info.timeLow | (high << 32);
}

/** Return the type component of the UUID.  */
int
UUID::type (void) const
{
    return (m_uuid[6] >> 4) & 0xf;
}

/** Return the variant component of the UUID.  */
int
UUID::variant (void) const
{
    if ((m_uuid[8] & 0x80) == 0)
        return VARIANT_NCS;
    if ((m_uuid[8] & 0x40) == 0)
        return VARIANT_DCE;
    if ((m_uuid[8] & 0x20) == 0)
        return VARIANT_MICROSOFT;
    else
        return VARIANT_OTHER;
}

//////////////////////////////////////////////////////////////////////
/** Generate a new universally unique identifier (UUID).  If there is
    a high-quality randomness source such as @c /dev/urandom, it is
    used to produce a random identifier.  Otherwise the current time,
    the local ethernet MAC address (if available) and pseudo-random
    data are used.  This is the preferred method of creating unique
    identifiers.  To force the selection of the algorithm, please use
    #generateRandom() or #generateTime().  */
UUID
UUID::generate (void)
{
#ifdef _WIN32
    return generateRandom();
#else
    if (getRandomFD() >= 0)
	return generateRandom();
    else
	return generateTime();
#endif
}

/** Generate a new all-random universally unique identifier (UUID).  A
    high-quality randomness source such as @c /dev/urandom is used if
    one exists, otherwise a pseudo-random number generator is
    substituted.  Note that the use of pseudo-random number generator
    may compromise the uniqueness of the resulting identifiers.  It is
    usually better to use #generate(), but please do note the issues
    that #generateTime() has -- it will be used if no high-quality
    randomness source exists.  */
UUID
UUID::generateRandom (void)
{
    Binary id;
    Info info;

#ifdef _WIN32
    ::UUID winfo;
    UuidCreate(&winfo);
    info.timeLow = winfo.Data1;
    info.timeMid = winfo.Data2;
    info.timeHiVer = winfo.Data3;
    info.clockSeq = (winfo.Data4[0] << 8) | winfo.Data4[1];
    memcpy(info.node, winfo.Data4+2, ETH_ADDR_LEN);
#else
    getRandomData(id, sizeof(id));
    unpack(id, info);
    info.clockSeq = (info.clockSeq & 0x3fff) | 0x8000;
    info.timeHiVer = (info.timeHiVer & 0x0fff) | 0x4000;
#endif

    pack(info, id);
    return UUID (id);
}

/** Generate a new universally unique identifier (UUID) from current
    time and local ethernet MAC address (if available).  This method
    can leak information about where the origin of the identifier
    because the result includes the source MAC address.  This may be a
    privacy problem in some applications.  In general it is better to
    use #generate(), which uses this method only if no high-quality
    randomness source exists.  */
UUID
UUID::generateTime (void)
{
    Binary id;
    Info info;

#ifdef _WIN32
    ::UUID winfo;
    UuidCreateSequential(&winfo);
    info.timeLow = winfo.Data1;
    info.timeMid = winfo.Data2;
    info.timeHiVer = winfo.Data3;
    info.clockSeq = (winfo.Data4[0] << 8) | winfo.Data4[1];
    memcpy(info.node, winfo.Data4+2, ETH_ADDR_LEN);
#else 
    // FIXME: thread safety?
    if (! s_nodeinit)
    {
	if (! getNodeID(s_nodeid))
	{
	    // Manufacture random address, with multicast bit set
	    // to prevent conflicts with real network cards.
	    getRandomData(&s_nodeid[0], ETH_ADDR_LEN);
	    s_nodeid[0] |= 0x80;
	}
	s_nodeinit = true;
    }

    uint64_t time;
    while (true)
    {
        time = Time::current().ns();
        if (s_lasttime == 0)
        {
            getRandomData(&s_clockseq, sizeof(s_clockseq));
            s_clockseq &= 0x1fff;
	    s_lasttime = time - TimeConst::SEC_NSECS;
        }
        if (time < s_lasttime)
        {
	    s_clockseq = (s_clockseq+1) & 0x1fff;
	    s_adjustment = 0;
	    s_lasttime = time;
        }
        else if (time == s_lasttime)
        {
	    if (s_adjustment >= MAX_ADJUSTMENT)
	        continue;
	    s_adjustment++;
        }
        else
        {
	    s_adjustment = 0;
	    s_lasttime = time;
        }
	break;
    }

    uint64_t t = time / 100 + s_adjustment + UNIX_TIME_OFFSET;
    info.timeLow = t & 0xffffffff;
    info.timeMid = (t >> 32) & 0xffff;
    info.timeHiVer = (t >> 48) | 0x1000;
    info.clockSeq = s_clockseq | 0x8000;
    memcpy(info.node, s_nodeid, ETH_ADDR_LEN);
#endif

    pack(info, id);
    return UUID (id);
}

void
UUID::unpack(const Binary from, Info &to)
{
    to.timeLow   = (from[0]<<24) | (from[1]<<16) | (from[2]<<8) | from[3];
    to.timeMid   = (from[4]<<8)  | from[5];
    to.timeHiVer = (from[6]<<8)  | from[7];
    to.clockSeq  = (from[8]<<8)  | from[9];
    memcpy(to.node, from+10, ETH_ADDR_LEN);
}

void
UUID::pack(const Info &from, Binary to)
{
    to[0] = ((from.timeLow   >> 24) & 0xff);
    to[1] = ((from.timeLow   >> 16) & 0xff);
    to[2] = ((from.timeLow   >>  8) & 0xff);
    to[3] = ((from.timeLow   >>  0) & 0xff);
    to[4] = ((from.timeMid   >>  8) & 0xff);
    to[5] = ((from.timeMid   >>  0) & 0xff);
    to[6] = ((from.timeHiVer >>  8) & 0xff);
    to[7] = ((from.timeHiVer >>  0) & 0xff);
    to[8] = ((from.clockSeq  >>  8) & 0xff);
    to[9] = ((from.clockSeq  >>  0) & 0xff);
    memcpy(to+10, from.node, ETH_ADDR_LEN);
}

void
UUID::parse(const char *text, size_t len, Binary to)
{
    // Verify string is of expected length.
    if (len != 36)
	throw UUIDError ();

    // Verify every character is what we expect in that position.
    for (size_t i = 0; i <= 36; i++)
    {
	if (i == 8 || i == 13 || i == 18 || i == 23)
	{
	    if (text[i] != '-')
	        throw UUIDError ();
	    else
		continue;
	}

	if (i == 36)
	{
	    if (text[i] != 0)
	        throw UUIDError ();
	    else
		continue;
	}

	if (! isxdigit((unsigned char) text[i]))
	    throw UUIDError ();
    }

    // Convert the various fields.
    Info info = { (uint32_t)strtoul(text,    0, 16), // timeLow
	    	  (uint16_t)strtoul(text+9,  0, 16), // timeMid
		  (uint16_t)strtoul(text+14, 0, 16), // timeHiVer
		  (uint16_t)strtoul(text+19, 0, 16), // clockSeq
		  { 0, 0, 0, 0, 0, 0 } };  // node

#if __GNUC__
    __extension__ // shut up non-ansi format warnings
#endif
    sscanf(text+24,
	   "%2" SCNx8 "%2" SCNx8 "%2" SCNx8
	   "%2" SCNx8 "%2" SCNx8 "%2" SCNx8,
	   &info.node[0], &info.node[1], &info.node[2],
	   &info.node[3], &info.node[4], &info.node[5]);

    // Pack into binary form.
    pack(info, to);
}

#ifndef _WIN32
/** Generate @a len bytes of random data into @a buf. Uses /dev/urandom
    or /dev/random if one exists, otherwise uses srandom()/random(). */
void
UUID::getRandomData(void *buf, size_t len)
{
    unsigned char *to = (unsigned char *) buf;
    int fd;
    if ((fd = getRandomFD()) >= 0)
    {
	int ntries = 0;
	ssize_t nread;
	while (len > 0)
	{
	    if ((nread = read(fd, to, len)) <= 0)
	    {
	       if (++ntries > 16)
		   break;
	       continue;
	    }

	    len -= nread;
	    to += nread;
	    ntries = 0;
	}
    }

    for (size_t i = 0; i < len; ++i)
	to[i] = random() & 0xff;
}

/** Initialise the file descriptor for reading random data.
    If /dev/urandom or /dev/random isn't already open, tries
    to open it and seeds srandom() with entropy.  Cranks the
    random() PRNG engine a small random number of times. */
int
UUID::getRandomFD(void)
{
    uint64_t time  = Time::current ().ns();
    uint64_t usecs = time % TimeConst::SEC_NSECS;
    uint64_t secs  = time / TimeConst::SEC_NSECS / 1000;

    if (s_randomfd == -2)
    {
        s_randomfd = open("/dev/urandom", O_RDONLY);
        if (s_randomfd == -1)
	    s_randomfd = open("/dev/random", O_RDONLY | O_NONBLOCK);
	srandom((usecs << 16) ^ secs ^ getpid());
    }

    for (int i = (secs ^ usecs) & 0x1f; i > 0; i--)
	random();

    return s_randomfd;
}

/** Fill the ethernet hardware address of this system into @a id
    if the address can be determined.  Returns @c true if the
    address was found. */
bool
UUID::getNodeID(unsigned char *id)
{
    // See http://cplus.kompf.de/artikel/macaddr.html for missing platforms.
    // Solaris:
    //    http://cvs.opensolaris.org/source/xref/onnv/onnv-gate
    //      /usr/src/cmd/cmd-inet/usr.sbin/ifconfig/ifconfig.c#print_ifether
    //    http://cvs.opensolaris.org/source/xref/onnv/onnv-gate
    //      /usr/src/cmd/cmd-inet/usr.sbin/ifconfig/revarp.c#dlpi_print_address
    unsigned char *a;
    bool found = false;
    ifaddrs *addrs = 0;
    if (getifaddrs(&addrs) == 0)
    {
        for (ifaddrs *ifp = addrs; ifp && !found; ifp = ifp->ifa_next)
	{
	    if (ifp->ifa_flags & IFF_LOOPBACK)
		continue;

	    switch (ifp->ifa_addr->sa_family)
	    {
# ifdef AF_LINK // BSD, Mac OS X
	    case AF_LINK:
		if (sockaddr_dl *sdl = (sockaddr_dl *)ifp->ifa_addr)
		{
		    if (sdl->sdl_alen == ETH_ADDR_LEN
			&& (a = (unsigned char *) LLADDR(sdl))
			&& (a[0] || a[1] || a[2] || a[3] || a[4] || a[5]))
		    {
			memcpy(id, a, ETH_ADDR_LEN);
			found = true;
		    }
		}
		break;
# endif // AF_LINK
# ifdef SIOCGIFHWADDR // Linux
	    case AF_INET:
	    case AF_INET6:
		{
		    int s;
		    struct ifreq ifr;
		    memset(&ifr, 0, sizeof(ifr));
		    strncpy(ifr.ifr_name, ifp->ifa_name, sizeof(ifr.ifr_name));
		    if ((s = socket(ifp->ifa_addr->sa_family, SOCK_DGRAM, 0)) >= 0
			&& ioctl(s, SIOCGIFHWADDR, &ifr) >= 0
			&& (a = (unsigned char *) &ifr.ifr_hwaddr.sa_data)
			&& (a[0] || a[1] || a[2] || a[3] || a[4] || a[5]))
		    {
			memcpy(id, a, ETH_ADDR_LEN);
			found = true;
		    }
		    if (s >= 0)
			close(s);
		}
		break;
# endif // SIOCGIFHWADDR
	    }
	}

	freeifaddrs(addrs);
    }

    return found;
}
#endif // !_WIN32

} // namespace lat
