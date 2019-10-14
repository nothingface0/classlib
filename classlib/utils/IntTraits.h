#ifndef CLASSLIB_INT_TRAITS_H
# define CLASSLIB_INT_TRAITS_H
# include "classlib/utils/LongLong.h"

namespace lat {

/** Define numerical features of integral types.  This is a poor man's
    @c std::numeric_limits while compilers are catching up.  */
template <class T> struct IntTraits;

template <>
struct IntTraits<unsigned char>
{
    static const unsigned char	Min = 0;		//< Minimum value
    static const unsigned char	Max = UCHAR_MAX;	//< Maximum value
};

template <>
struct IntTraits<signed char>
{
    static const signed char	Min = SCHAR_MIN;	//< Minimum value
    static const signed char	Max = SCHAR_MAX;	//< Maximum value
};

template <>
struct IntTraits<char>
{
    static const char		Min = CHAR_MIN;		//< Minimum value
    static const char		Max = CHAR_MAX;		//< Maximum value
};

template <>
struct IntTraits<unsigned short>
{
    static const unsigned short	Min = 0;		//< Minimum value
    static const unsigned short	Max = USHRT_MAX;	//< Maximum value
};

template <>
struct IntTraits<signed short>
{
    static const signed short	Min = SHRT_MIN;		//< Minimum value
    static const signed short	Max = SHRT_MAX;		//< Maximum value
};

template <>
struct IntTraits<unsigned int>
{
    static const unsigned int	Min = 0;		//< Minimum value
    static const unsigned int	Max = UINT_MAX;		//< Maximum value
};

template <>
struct IntTraits<signed int>
{
    static const signed int	Min = INT_MIN;		//< Minimum value
    static const signed int	Max = INT_MAX;		//< Maximum value
};

template <>
struct IntTraits<unsigned long>
{
    static const unsigned long	Min = 0;		//< Minimum value
    static const unsigned long	Max = ULONG_MAX;	//< Maximum value
};

template <>
struct IntTraits<signed long>
{
    static const signed long	Min = LONG_MIN;		//< Minimum value
    static const signed long	Max = LONG_MAX;		//< Maximum value
};

#  if _MSC_VER
template <>
struct IntTraits<unsigned __int64>
{
    static const unsigned __int64 Min = 0;		//< Minimum value
    static const unsigned __int64 Max = ULONG_LONG_MAX;	//< Maximum value
};

template <>
struct IntTraits<signed __int64>
{
    static const signed __int64	Min = LONG_LONG_MIN;	//< Minimum value
    static const signed __int64	Max = LONG_LONG_MAX;	//< Maximum value
};
#  else
template <>
struct IntTraits<unsigned long long>
{
    static const unsigned long long Min = 0;		//< Minimum value
    static const unsigned long long Max = ULONG_LONG_MAX;//< Maximum value
};

template <>
struct IntTraits<signed long long>
{
    static const signed long long Min = LONG_LONG_MIN;	//< Minimum value
    static const signed long long Max = LONG_LONG_MAX;	//< Maximum value
};
#  endif

} // namespace lat
#endif // CLASSLIB_INT_TRAITS_H
