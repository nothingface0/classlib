#ifndef CLASSLIB_INT_BITS_H
# define CLASSLIB_INT_BITS_H

// The __STDC_LIMIT_MACROS needs to be defined such that the nonstdint.h header
// knows that it has to define the INT<N>_MAX macros itself. However, this is
// only necessary when using the Microsoft compiler. In the other cases, the
// nonstdint.h header is just an alias to the system stdint.h, which is where
// the INT<N>_MAX macros get defined.
# if _MSC_VER
#  ifndef __STDC_LIMIT_MACROS
#   error This header requires __STDC_LIMIT_MACROS to be defined
#  endif
# endif // _MSC_VER

# include "classlib/utils/BitTraits.h"
# include "classlib/sysapi/nonstdint.h"

namespace lat {

/** Define types by the number of bits in them.  */
template <unsigned int N>
struct IntBits
{
    // `UExact' and `SExact' are defined only for the actual types
    typedef typename IntBits<N+1>::UFast	UFast;
    typedef typename IntBits<N+1>::SFast	SFast;
    typedef typename IntBits<N+1>::ULeast	ULeast;
    typedef typename IntBits<N+1>::SLeast	SLeast;
};

# ifdef INT8_MAX
template <>
struct IntBits<8>
{
    static const unsigned int	Bits = 8;

    typedef int8_t		SExact;
    typedef uint8_t		UExact;

    typedef int_least8_t	SLeast;
    typedef uint_least8_t	ULeast;

    typedef int_fast8_t		SFast;
    typedef uint_fast8_t	UFast;
};
# endif

# ifdef INT16_MAX
template <>
struct IntBits<16>
{
    static const unsigned int	Bits = 16;

    typedef int16_t		SExact;
    typedef uint16_t		UExact;

    typedef int_least16_t	SLeast;
    typedef uint_least16_t	ULeast;

    typedef int_fast16_t	SFast;
    typedef uint_fast16_t	UFast;
};
# endif

# ifdef INT32_MAX
template <>
struct IntBits<32>
{
    static const unsigned int	Bits = 32;

    typedef int32_t		SExact;
    typedef uint32_t		UExact;

    typedef int_least32_t	SLeast;
    typedef uint_least32_t	ULeast;

    typedef int_fast32_t	SFast;
    typedef uint_fast32_t	UFast;
};
# endif

# ifdef INT64_MAX
template <>
struct IntBits<64>
{
    static const unsigned int	Bits = 64;

    typedef int64_t		SExact;
    typedef uint64_t		UExact;

    typedef int_least64_t	SLeast;
    typedef uint_least64_t	ULeast;

    typedef int_fast64_t	SFast;
    typedef uint_fast64_t	UFast;
};
# endif

} // namespace lat
#endif // CLASSLIB_INT_BITS_H
