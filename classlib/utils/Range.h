#ifndef CLASSLIB_RANGE_H
# define CLASSLIB_RANGE_H
# include "classlib/sysapi/system.h"
# include <algorithm>

namespace lat {

template <class T>
struct Range
{
    Range (void);
    Range (const T &x, const T &y);
    template <class U>
    Range (const Range<U> &x);

    T		low (void) const;
    void	low (const T &value);
    T		high (void) const;
    void	high (const T &value);
    T		width (void) const;
    bool	empty (void) const;

    bool	contains (const T &value) const;
    bool	contains (const Range<T> &other) const;
    bool	containsStrictly (const Range<T> &other) const;
    bool	intersects (const Range &other) const;
    Range	intersection (const Range &other) const;

private:
    T		m_low;
    T		m_high;
};

template <class T>
inline bool operator== (const Range<T> &x, const Range<T> &y)
{ return x.low () == y.low () && x.high () == y.high (); }

template <class T>
inline bool operator< (const Range<T> &x, const Range<T> &y)
{ return x.low () < y.low () || (! (y.low () < x.low ()) && x.high () < y.high ()); }

template <class T>
inline Range<T> MakeRange (const T &a, const T &b)
{ return Range<T> (a, b); }

template <class T>
inline Range<T>::Range (void) { }

template <class T>
inline Range<T>::Range (const T &x, const T &y)
    : m_low (x < y ? x : y), m_high (x < y ? y : x)
{ }

template <class T>
template <class U>
inline Range<T>::Range (const Range<U> &x)
    : m_low (x.m_low), m_high (x.m_high)
{ }

template <class T>
inline T Range<T>::width (void) const
{ return m_high - m_low; }

template <class T>
inline bool Range<T>::empty (void) const
{ return m_high == m_low; }

template <class T>
inline T Range<T>::low (void) const
{ return m_low; }

template <class T>
inline void Range<T>::low (const T &value)
{ m_low = value; }

template <class T>
inline T Range<T>::high (void) const
{ return m_high; }

template <class T>
inline void Range<T>::high (const T &value)
{ m_high = value; }

template <class T>
inline bool Range<T>::contains (const T &value) const
{ return m_low <= value && value < m_high; }

template <class T>
inline bool Range<T>::contains (const Range<T> &other) const
{ return m_low <= other.m_low && other.m_high <= m_high; }

template <class T>
inline bool Range<T>::containsStrictly (const Range<T> &other) const
{ return m_low < other.m_low && other.m_high < m_high; }

template <class T>
inline bool Range<T>::intersects (const Range &other) const
{ return m_low < other.m_high && m_high > other.m_low; }

template <class T>
inline Range<T> Range<T>::intersection (const Range &other) const
{ return MakeRange (std::min (m_high, std::max (m_low, other.m_low)),
		    std::min (m_high, other.m_high)); }

} // namespace lat
#endif // CLASSLIB_RANGE_H
