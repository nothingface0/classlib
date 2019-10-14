#ifndef CLASSLIB_SYSAPI_DETECT_H
# define CLASSLIB_SYSAPI_DETECT_H

namespace lat
{
  template <typename T, typename U, U T::*const>
  struct have_struct_field
  {
    typedef T struct_type;
    typedef U field_type;
    static const bool value = true;
  };

  template <bool B, class T = void>
  struct define_if { typedef T type; };

  template <class T>
  struct define_if<false, T> {};

  struct this_overload_does_not_exist {};
}

#endif // CLASSLIB_SYSAPI_DETECT_H
