#ifndef CLASSLIB_UUID_H
# define CLASSLIB_UUID_H
# include "classlib/utils/Error.h"
# include "classlib/utils/iosfwd.h"
# include "classlib/sysapi/nonstdint.h"
# define CONST_UUID(a,b,c,d,e,f,g,h,i,j,k) \
    {0x##a,0x##b,0x##c,0x##d,0x##e,0x##f,0x##g,0x##h,0x##i,0x##j,0x##k}

namespace lat {
class UUID;

std::ostream &operator<< (std::ostream &out, const UUID &id);

class UUID
{
public:
    /// Offset between 15-Oct-1582 and 1-Jan-70
    static const int64_t UNIX_TIME_OFFSET
	= ((int64_t) 0x01b21dd2 << 32) + 0x13814000;

    static const int VARIANT_NCS       = 0;
    static const int VARIANT_DCE       = 1;
    static const int VARIANT_MICROSOFT = 2;
    static const int VARIANT_OTHER     = 3;

    typedef unsigned char Binary [16];

    UUID (void);
    UUID (const char *text);
    UUID (const std::string &text);
    UUID (Binary raw);
    UUID (const UUID &x);
    UUID &operator= (const UUID &x);

    bool		operator== (const UUID &x) const;
    bool		operator!= (const UUID &x) const;
    bool		operator<  (const UUID &x) const;

    bool		isNull (void) const;
    int64_t		time (void) const;
    int			type (void) const;
    int			variant (void) const;
    void		format (char *buf) const;
    void		clear (void);

    static UUID		generate (void);
    static UUID		generateRandom (void);
    static UUID		generateTime (void);

private:
    struct Info
    {
	uint32_t timeLow;   //< Low 32 bits of the time part.
	uint16_t timeMid;   //< Mid 16 bits of the time part.
	uint16_t timeHiVer; //< Top bits of time and version.
	uint16_t clockSeq;  //< Clock sequence.
	uint8_t  node[6];   //< Node ethernet address.
    };

    static void		parse (const char *text, size_t len, Binary to);
    static int		compare (const Info &a, const Info &b);
    static void		pack (const Info &from, Binary to);
    static void		unpack (const Binary from, Info &to);
    static bool		getNodeID(uint8_t *id);
    static void         getRandomData (void *buf, size_t len);
    static int		getRandomFD (void);

    Binary		m_uuid;

    static bool		s_nodeinit;
    static uint8_t	s_nodeid[6];
    static int		s_randomfd;
    static uint64_t	s_lasttime;
    static int		s_adjustment;
    static uint16_t	s_clockseq;
};


} // namespace lat
#endif // CLASSLIB_UUID_H
