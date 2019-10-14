#ifndef CLASSLIB_INTTYPES_H
# define CLASSLIB_INTTYPES_H 1
# if ! _MSC_VER
#  include <inttypes.h>

# else // _MSC_VER
#  include "classlib/sysapi/nonstdint.h"

typedef struct { intmax_t quot; intmax_t rem; } imaxdiv_t;

#  if ! defined __cplusplus || defined __STDC_FORMAT_MACROS
#   define PRId8        "d"
#   define PRId16       "hd"
#   define PRId32       "I32d"
#   define PRId64       "I64d"
#   define PRIdMAX      "I64d"
#   define PRIdPTR      "Id"
#   define PRIdLEAST8   "d"
#   define PRIdLEAST16  "hd"
#   define PRIdLEAST32  "I32d"
#   define PRIdLEAST64  "I64d"
#   define PRIdFAST8    "d"
#   define PRIdFAST16   "hd"
#   define PRIdFAST32   "I32d"
#   define PRIdFAST64   "I64d"

#   define PRIi8        "i"
#   define PRIi16       "hi"
#   define PRIi32       "I32i"
#   define PRIi64       "I64i"
#   define PRIiMAX      "I64i"
#   define PRIiPTR      "Ii"
#   define PRIiLEAST8   "i"
#   define PRIiLEAST16  "hi"
#   define PRIiLEAST32  "I32i"
#   define PRIiLEAST64  "I64i"
#   define PRIiFAST8    "i"
#   define PRIiFAST16   "hi"
#   define PRIiFAST32   "I32i"
#   define PRIiFAST64   "I64i"

#   define PRIo8        "o"
#   define PRIo16       "ho"
#   define PRIo32       "I32o"
#   define PRIo64       "I64o"
#   define PRIoMAX      "I64o"
#   define PRIoPTR      "Io"
#   define PRIoLEAST8   "o"
#   define PRIoLEAST16  "ho"
#   define PRIoLEAST32  "I32o"
#   define PRIoLEAST64  "I64o"
#   define PRIoFAST8    "o"
#   define PRIoFAST16   "ho"
#   define PRIoFAST32   "I32o"
#   define PRIoFAST64   "I64o"

#   define PRIu8        "u"
#   define PRIu16       "hu"
#   define PRIu32       "I32u"
#   define PRIu64       "I64u"
#   define PRIuMAX      "I64u"
#   define PRIuPTR      "Iu"
#   define PRIuLEAST8   "u"
#   define PRIuLEAST16  "hu"
#   define PRIuLEAST32  "I32u"
#   define PRIuLEAST64  "I64u"
#   define PRIuFAST8    "u"
#   define PRIuFAST16   "hu"
#   define PRIuFAST32   "I32u"
#   define PRIuFAST64   "I64u"

#   define PRIx8        "x"
#   define PRIx16       "hx"
#   define PRIx32       "I32x"
#   define PRIx64       "I64x"
#   define PRIxMAX      "I64x"
#   define PRIxPTR      "Ix"
#   define PRIxLEAST8   "x"
#   define PRIxLEAST16  "hx"
#   define PRIxLEAST32  "I32x"
#   define PRIxLEAST64  "I64x"
#   define PRIxFAST8    "x"
#   define PRIxFAST16   "hx"
#   define PRIxFAST32   "I32x"
#   define PRIxFAST64   "I64x"

#   define PRIX8        "X"
#   define PRIX16       "hX"
#   define PRIX32       "I32X"
#   define PRIX64       "I64X"
#   define PRIXMAX      "I64X"
#   define PRIXPTR      "IX"
#   define PRIXLEAST8   "X"
#   define PRIXLEAST16  "hX"
#   define PRIXLEAST32  "I32X"
#   define PRIXLEAST64  "I64X"
#   define PRIXFAST8    "X"
#   define PRIXFAST16   "hX"
#   define PRIXFAST32   "I32X"
#   define PRIXFAST64   "I64X"


#   define SCNd8        "d"
#   define SCNd16       "hd"
#   define SCNd32       "ld"
#   define SCNd64       "I64d"
#   define SCNdMAX      "I64d"
#   ifdef _WIN64
#    define SCNdPTR     "I64d"
#   else
#    define SCNdPTR     "ld"
#   endif
#   define SCNdLEAST8   "d"
#   define SCNdLEAST16  "hd"
#   define SCNdLEAST32  "ld"
#   define SCNdLEAST64  "I64d"
#   define SCNdFAST8    "d"
#   define SCNdFAST16   "hd"
#   define SCNdFAST32   "ld"
#   define SCNdFAST64   "I64d"

#   define SCNi8        "i"
#   define SCNi16       "hi"
#   define SCNi32       "li"
#   define SCNi64       "I64i"
#   define SCNiMAX      "I64i"
#   ifdef _WIN64
#    define SCNiPTR     "I64i"
#   else
#    define SCNiPTR     "li"
#   endif
#   define SCNiLEAST8   "i"
#   define SCNiLEAST16  "hi"
#   define SCNiLEAST32  "li"
#   define SCNiLEAST64  "I64i"
#   define SCNiFAST8    "i"
#   define SCNiFAST16   "hi"
#   define SCNiFAST32   "li"
#   define SCNiFAST64   "I64i"

#   define SCNo8        "o"
#   define SCNo16       "ho"
#   define SCNo32       "lo"
#   define SCNo64       "I64o"
#   define SCNoMAX      "I64o"
#   ifdef _WIN64
#    define SCNoPTR     "I64o"
#   else
#    define SCNoPTR     "lo"
#   endif
#   define SCNoLEAST8   "o"
#   define SCNoLEAST16  "ho"
#   define SCNoLEAST32  "lo"
#   define SCNoLEAST64  "I64o"
#   define SCNoFAST8    "o"
#   define SCNoFAST16   "ho"
#   define SCNoFAST32   "lo"
#   define SCNoFAST64   "I64o"

#   define SCNu8        "u"
#   define SCNu16       "hu"
#   define SCNu32       "lu"
#   define SCNu64       "I64u"
#   define SCNuMAX      "I64u"
#   ifdef _WIN64
#    define SCNuPTR     "I64u"
#   else
#    define SCNuPTR     "lu"
#   endif
#   define SCNuLEAST8   "u"
#   define SCNuLEAST16  "hu"
#   define SCNuLEAST32  "lu"
#   define SCNuLEAST64  "I64u"
#   define SCNuFAST8    "u"
#   define SCNuFAST16   "hu"
#   define SCNuFAST32   "lu"
#   define SCNuFAST64   "I64u"

#   define SCNx8        "x"
#   define SCNx16       "hx"
#   define SCNx32       "lx"
#   define SCNx64       "I64x"
#   define SCNxMAX      "I64x"
#   ifdef _WIN64
#    define SCNxPTR     "I64x"
#   else
#    define SCNxPTR     "lx"
#   endif
#   define SCNxLEAST8   "x"
#   define SCNxLEAST16  "hx"
#   define SCNxLEAST32  "lx"
#   define SCNxLEAST64  "I64x"
#   define SCNxFAST8    "x"
#   define SCNxFAST16   "hx"
#   define SCNxFAST32   "lx"
#   define SCNxFAST64   "I64x"

#   define SCNX8        "X"
#   define SCNX16       "hX"
#   define SCNX32       "lX"
#   define SCNX64       "I64X"
#   define SCNXMAX      "I64X"
#   ifdef _WIN64
#    define SCNXPTR     "I64X"
#   else
#    define SCNXPTR     "lX"
#   endif
#   define SCNXLEAST8   "X"
#   define SCNXLEAST16  "hX"
#   define SCNXLEAST32  "lX"
#   define SCNXLEAST64  "I64X"
#   define SCNXFAST8    "X"
#   define SCNXFAST16   "hX"
#   define SCNXFAST32   "lX"
#   define SCNXFAST64   "I64X"
#  endif

#  define imaxabs _abs64
#  ifdef STATIC_IMAXDIV
static
#  else
_inline
#  endif
imaxdiv_t __cdecl imaxdiv(intmax_t numer, intmax_t denom)
{
  imaxdiv_t result;
  result.quot = numer / denom;
  result.rem = numer % denom;
  if (numer < 0 && result.rem > 0)
  {
    ++result.quot;
    result.rem -= denom;
  }

  return result;
}

#  define strtoimax _strtoi64
#  define strtoumax _strtoui64
#  define wcstoimax _wcstoi64
#  define wcstoumax _wcstoui64
# endif // _MSC_VER
#endif // CLASSLIB_NONSTDINT_H
