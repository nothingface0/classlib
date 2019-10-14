dnl +++######################################################################
AC_DEFUN([LAT_EXT_ZLIB],
 [ZINC= ZLIB=-lz
  AC_SUBST(ZINC)
  AC_SUBST(ZLIB)
  AC_ARG_WITH(zlib,
    [   --with-zlib                    ZLIB is in DIR],
    [ with_zlib_includes="${with_zlib_includes-$withval}"
      with_zlib_libraries="${with_zlib_libraries-$withval}" ])
  AC_ARG_WITH(zlib-includes,
    [   --with-zlib-includes=DIR       ZLIB headers are in DIR],
    [ zlib_includes="$withval"
      case "$zlib_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(zlib.h,[ZINC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$zlib_includes"
	      AC_CHECK_HEADER(zlib.h,[ZINC="-I$zlib_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(zlib-libraries,
    [   --with-zlib-libaries=DIR       ZLIB libraries are in DIR],
    [ zlib_libraries="$withval"
      case "$zlib_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(zlib.h,
	       [AC_CHECK_LIB(z, deflate, [ZLIB="-lz"])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$zlib_libraries"
	      AC_CHECK_HEADER(zlib.h,
	       [AC_CHECK_LIB(z, deflate,
	        [ZLIB="-L$zlib_libraries -lz"])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN([LAT_EXT_BZ2LIB],
 [BZ2INC= BZ2LIB=-lbz2
  AC_SUBST(BZ2INC)
  AC_SUBST(BZ2LIB)
  AC_ARG_WITH(bz2lib,
    [   --with-bz2lib                  BZ2LIB is in DIR],
    [ with_bz2lib_includes="${with_bz2lib_includes-$withval}"
      with_bz2lib_libraries="${with_bz2lib_libraries-$withval}" ])
  AC_ARG_WITH(bz2lib-includes,
    [   --with-bz2lib-includes=DIR     BZ2LIB headers are in DIR],
    [ bz2lib_includes="$withval"
      case "$bz2lib_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(bzlib.h,[BZ2INC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$bz2lib_includes"
	      AC_CHECK_HEADER(bzlib.h,[BZ2INC="-I$bz2lib_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(bz2lib-libraries,
    [   --with-bz2lib-libaries=DIR     BZ2LIB libraries are in DIR],
    [ bz2lib_libraries="$withval"
      case "$bz2lib_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(bzlib.h,
	       [AC_CHECK_LIB(bz2, BZ2_bzCompress, [BZ2LIB="-lbz2"])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$bz2lib_libraries"
	      AC_CHECK_HEADER(bzlib.h,
	       [AC_CHECK_LIB(bz2, BZ2_bzCompress,
	        [BZ2LIB="-L$bz2lib_libraries -lbz2"])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN([LAT_EXT_PCRE],
 [PCREINC= PCRELIB=-lpcre
  AC_SUBST(PCREINC)
  AC_SUBST(PCRELIB)
  AC_ARG_WITH(pcre,
    [   --with-pcre                    PCRE is in DIR],
    [ with_pcre_includes="${with_pcre_includes-$withval}"
      with_pcre_libraries="${with_pcre_libraries-$withval}" ])
  AC_ARG_WITH(pcre-includes,
    [   --with-pcre-includes=DIR       PCRE headers are in DIR],
    [ pcre_includes="$withval"
      case "$pcre_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(pcre.h,[PCREINC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$pcre_includes"
	      AC_CHECK_HEADER(pcre.h,[PCREINC="-I$pcre_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(pcre-libraries,
    [   --with-pcre-libaries=DIR       PCRE libraries are in DIR],
    [ pcre_libraries="$withval"
      case "$pcre_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(pcre.h,
	       [AC_CHECK_LIB(pcre, pcre_compile, [PCRELIB="-lpcre"])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$pcre_libraries"
	      AC_CHECK_HEADER(pcre.h,
	       [AC_CHECK_LIB(pcre, pcre_compile,
	        [PCRELIB="-L$pcre_libraries -lpcre"])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN([LAT_EXT_OPENSSL],
 [OPENSSLINC= OPENSSLLIB=-lcrypto
  AC_SUBST(OPENSSLINC)
  AC_SUBST(OPENSSLLIB)
  AC_ARG_WITH(openssl,
    [   --with-openssl                 OpenSSL is in DIR],
    [ with_openssl_includes="${with_openssl_includes-$withval}"
      with_openssl_libraries="${with_openssl_libraries-$withval}" ])
  AC_ARG_WITH(openssl-includes,
    [   --with-openssl-includes=DIR    OpenSSL headers are in DIR],
    [ openssl_includes="$withval"
      case "$openssl_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(openssl/evp.h,[OPENSSLINC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$openssl_includes"
	      AC_CHECK_HEADER(openssl/evp.h,[OPENSSLINC="-I$openssl_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(openssl-libraries,
    [   --with-openssl-libaries=DIR    OpenSSL libraries are in DIR],
    [ openssl_libraries="$withval"
      case "$openssl_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(openssl/evp.h,
	       [AC_CHECK_LIB(crypto, EVP_md5, [OPENSSLLIB="-lcrypto"])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$openssl_libraries"
	      AC_CHECK_HEADER(openssl/evp.h,
	       [AC_CHECK_LIB(crypto, EVP_md5,
		[OPENSSLLIB="-L$openssl_libraries -lcrypto"])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN([LAT_EXT_LZO],
 [LZOINC= LZOLIB=-llzo2
  AC_SUBST(LZOINC)
  AC_SUBST(LZOLIB)
  AC_ARG_WITH(lzo,
    [   --with-lzo                     LZO is in DIR],
    [ with_lzo_includes="${with_lzo_includes-$withval}"
      with_lzo_libraries="${with_lzo_libraries-$withval}" ])
  AC_ARG_WITH(lzo-includes,
    [   --with-lzo-includes=DIR        LZO headers are in DIR],
    [ lzo_includes="$withval"
      case "$lzo_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(lzo/lzo1x.h,[LZOINC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$lzo_includes"
	      AC_CHECK_HEADER(lzo/lzo1x.h,[LZOINC="-I$lzo_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(lzo-libraries,
    [   --with-lzo-libaries=DIR        LZO libraries are in DIR],
    [ lzo_libraries="$withval"
      case "$lzo_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(lzo/lzo1x.h,
	       [AC_CHECK_LIB(lzo2, lzo2a_decompress, [LZOLIB="-llzo2"])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$lzo_libraries"
	      AC_CHECK_HEADER(lzo/lzo1x.h,
	       [AC_CHECK_LIB(lzo2, lzo2a_decompress,
		[LZOLIB="-L$lzo_libraries -llzo2"])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
dnl +++######################################################################
AC_DEFUN([LAT_EXT_LZMA],
 [LZMAINC= LZMALIB=-llzma
  AC_SUBST(LZMAINC)
  AC_SUBST(LZMALIB)
  AC_ARG_WITH(lzma,
    [   --with-lzma                    LZMA is in DIR],
    [ with_lzma_includes="${with_lzma_includes-$withval}"
      with_lzma_libraries="${with_lzma_libraries-$withval}" ])
  AC_ARG_WITH(lzma-includes,
    [   --with-lzma-includes=DIR       LZMA headers are in DIR],
    [ lzma_includes="$withval"
      case "$lzma_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(lzma.h,[LZMAINC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$lzma_includes"
	      AC_CHECK_HEADER(lzma.h,[LZMAINC="-I$lzma_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(lzma-libraries,
    [   --with-lzma-libaries=DIR       LZMA libraries are in DIR],
    [ lzma_libraries="$withval"
      case "$lzma_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(lzma.h,
	       [AC_CHECK_LIB(lzma, lzma_raw_encoder, [LZMALIB="-llzma"])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$lzma_libraries"
	      AC_CHECK_HEADER(lzma.h,
	       [AC_CHECK_LIB(lzma, lzma_raw_encoder,
		[LZMALIB="-L$lzma_libraries -llzma"])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
dnl +++#####################################################################
AC_DEFUN([LAT_EXT_RX],
 [AC_CHECK_HEADER(regex.h)
  RXINC= RXLIB=
  AC_SUBST(RXINC)
  AC_SUBST(RXLIB)
  AC_ARG_WITH(rx,
    [   --with-rx                      RX is in DIR],
    [ with_rx_includes="${with_rx_includes-$withval}"
      with_rx_libraries="${with_rx_libraries-$withval}" ])
  AC_ARG_WITH(rx-includes,
    [   --with-rx-includes=DIR         RX headers are in DIR],
    [ rx_includes="$withval"
      case "$rx_includes" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(regex.h,[RXINC=]) ;;
	*   ) _cppflags="$CPPFLAGS"
	      CPPFLAGS="$CPPFLAGS -I$rx_includes"
	      AC_CHECK_HEADER(regex.h,[RXINC="-I$rx_includes"],
	       [CPPFLAGS="$_cppflags"]) ;;
       esac ])
  AC_ARG_WITH(rx-libraries,
    [   --with-rx-libaries=DIR         RX libraries are in DIR],
    [ rx_libraries="$withval"
      case "$rx_libraries" in
        no  ) ;;
	yes ) AC_CHECK_HEADER(regex.h,
	       [AC_CHECK_LIB(rx, regcomp, [ZLIB="-lrx"])]) ;;
	*   ) _ldflags="$LDFLAGS"
	      LDFLAGS="$LDFLAGS -L$rx_libraries"
	      AC_CHECK_HEADER(regex.h,
	       [AC_CHECK_LIB(rx, regcomp,
	        [RXLIB="-L$rx_libraries -lrx"])])
	      LDFLAGS="$_ldflags" ;;
       esac ])
])
dnl ---######################################################################
