#!/bin/bash

msvcopts="/nologo /GF /Gy /EHsc /GR /MDd /Zi" # /Zc:forScope /W3 /WL /GZ /GS /RTC1 /G6
exec 21>&1 22>&2

pv_bzip2=1.0.2
pv_e2fsprogs=1.32
pv_pcre=8.00
pv_zlib=1.2.3
pv_rx=1.5
pv_classlib=tmp

######################################################################
here=$PWD
cmake=$(ls -d "$(cygpath -u 'c:\program files\')"/CMake* | tail -1)/bin/cmake.exe
me=$0; case $me in /* ) ;; * ) me=$PWD/$me ;; esac

unset INCLUDE LIB

######################################################################
# Tools

# Switch output to original standard output.
orig_stdout() { exec 1>&21 2>&22; }

# Download a file into the local cache and extract it if it's a tar ball;
# if it's a normal file, copy into the current directory.
download () {
  upload=$PWD
  mkdir -p $here/.downloads
  cd $here/.downloads

  filepart=`echo $1 | sed 's|.*/||'`
  [ -f $filepart ] || wget $1
  cd $upload
  case $filepart in
     *.tgz | *.tar.gz ) tar -zxvf $here/.downloads/$filepart ;;
     *.bz2 ) tar -jxvf $here/.downloads/$filepart ;;
     * ) cp $here/.downloads/$filepart . ;;
  esac
}

# Download a tar ball, extract it, and cd into it.
extract () {
  download $1
  cd `echo $filepart | sed 's/\.\(tar\.gz\|tgz\|tar.bz2\)$//'`
}

# Prepare for a package: zap any existing directory, make it,
# download and extract the tar ball, and cd into it.
indir () {
  orig_stdout
  echo "*** $(date +%Y%m%d%H%M%S): Building $1, log in $here/build/$1/log.txt"
  rm -fr $here/build/$1; mkdir -p $here/build/$1; cd $here/build/$1
  exec > log.txt 2>&1
}

# Prepare for a package: zap any existing directory, make it,
# download and extract the tar ball, and cd into it.
inpkg () {
  indir $1
  extract $2
  . $here/build/msvcenv/msvcenv
}

# Extract a named patch from the end of this installation script
getpatch () {
  sed "1,/^# BEGIN PATCH $1\$/d; /^# END PATCH $1/,\$d" < $me
}

######################################################################
######################################################################
######################################################################
build_msvcenv() {
  indir msvcenv
  getpatch msvcenv | patch -b
}

######################################################################
build_bzip2 () {
  inpkg bzip2 ftp://sources.redhat.com/pub/bzip2/v102/bzip2-${pv_bzip2}.tar.gz
  getpatch bzip2 | patch -b
  nmake /nolog /f makefile.msc EXTRA_CFLAGS="$msvcopts"

  mkdir -p $here/products/{lib,include,bin}
  cp -p *.{dll,exe} $here/products/bin
  cp -p *.{lib,exp} $here/products/lib
  cp -p bzlib.h $here/products/include
}

######################################################################
build_zlib () {
  inpkg zlib http://www.zlib.net/zlib-${pv_zlib}.tar.bz2
  nmake /nologo /f win32/Makefile.msc

  mkdir -p $here/products/{lib,include,bin}
  cp -p *.{dll,exe} $here/products/bin
  cp -p *.{lib,exp} $here/products/lib
  cp -p z*.h $here/products/include
}

######################################################################
build_pcre () {
  inpkg pcre http://switch.dl.sourceforge.net/sourceforge/pcre/pcre-${pv_pcre}.tar.bz2
  "$cmake" -G "NMake Makefiles" -DCMAKE_C_FLAGS="$msvcopts" -DCMAKE_CXX_FLAGS="$msvcopts" .
  nmake /nologo

  mkdir -p $here/products/{lib,include,bin}
  cp -p *.exe $here/products/bin
  cp -p *.lib $here/products/lib
  cp -p pcre.h $here/products/include
}

######################################################################
build_rx () {
  inpkg rx http://ftp.gnu.org/old-gnu/rx/rx-${pv_rx}.tar.gz
  cd rx
  download http://gcc.gnu.org/viewcvs/trunk/libiberty/strncasecmp.c?view=co
  grep -v ansidecl $here/.downloads/strncasecmp.c* > strncasecmp.c
  for f in *.h; do
    sed 's/ifdef __STDC__/if defined __STDC__ || defined __cplusplus/' \
       < $f > $f.out && mv $f.out $f
  done
  getpatch rx | patch -b
  CC=cl CFLAGS="/D__STDC__=1 $msvcopts" CPP="cl -E" ./configure --prefix=$PWD
  make

  mkdir -p $here/products/{lib,include,bin}
  cp -p *.dll $here/products/bin
  cp -p *.{exp,lib} $here/products/lib
  (echo '#ifdef __cplusplus'; echo 'extern "C" {'; echo '#endif'
   cat inst-rxposix.h; echo '#ifdef __cplusplus'; echo '}'; echo '#endif'
  ) > $here/products/include/regex.h
}

######################################################################
build_classlib () {
  inpkg classlib http://cmsmac01.cern.ch/~lat/exports/classlib-${pv_classlib}.tar.bz2
  "$cmake" -G "NMake Makefiles" \
     -DCMAKE_C_FLAGS="$msvcopts" \
     -DCMAKE_CXX_FLAGS="$msvcopts" \
     -DCMAKE_PREFIX_PATH="$(cygpath -w $here/products)" \
     -DPCRE_LIBRARY=pcred \
     -DCMAKE_VERBOSE_MAKEFILE=1 \
     -DCLASSLIB_BUILD_TESTS=1 \
     .
  nmake /nologo
}

######################################################################
######################################################################
######################################################################
[ -z "$*" ] && set -- msvcenv bzip2 zlib pcre rx classlib
for arg; do eval build_$arg; done
exit

######################################################################
######################################################################
######################################################################
# BEGIN PATCH msvcenv
--- /dev/null	2003-07-03 21:54:30.000000000 +0200
+++ msvcenv	2009-11-26 16:38:14.000000000 +0200
@@ -0,0 +1,19 @@
+export VSINSTALLDIR="C:\\Program Files\\Microsoft Visual Studio 9.0"
+export VS90COMNTOOLS="$VSINSTALLDIR\\Common7\\Tools"
+export DEVENVDIR="$VSINSTALLDIR\\Common7\\IDE"
+export VCINSTALLDIR="$VSINSTALLDIR\\VC"
+export WINDOWSSDKDIR="C:\\Program Files\\Microsoft SDKs\\Windows\\v6.0A"
+export FRAMEWORKDIR="C:\\WINNT\\Microsoft.NET\\Framework"
+export FRAMEWORKVERSION=v2.0.50727
+export FRAMEWORK35VERSION=v3.5
+
+WXPATH="$DEVENVDIR;$VCINSTALLDIR\\BIN;$VS90COMNTOOLS"
+WXPATH="$WXPATH;$FRAMEWORKDIR\\$FRAMEWORK35VERSION"
+WXPATH="$WXPATH;$FRAMEWORKDIR\\$FRAMEWORKVERSION"
+WXPATH="$WXPATH;$VCINSTALLDIR\\VCPackages"
+WXPATH="$WXPATH;$WINDOWSSDKDIR\\bin"
+XPATH="$(cygpath -up "$WXPATH")"
+export PATH="$XPATH:$PATH"
+export "INCLUDE=$VSINSTALLDIR\\VC\\ATLMFC\\INCLUDE;$VSINSTALLDIR\\VC\\INCLUDE;$WINDOWSSDKDIR\\include"
+export "LIB=$VSINSTALLDIR\\VC\\ATLMFC\\LIB;$VSINSTALLDIR\\VC\\LIB;$WINDOWSSDKDIR\\lib"
+export "LIBPATH=$FRAMEWORKDIR\\$FRAMEWORK35VERSION;$FRAMEWORKDIR\\$FRAMEWORKVERSION;$LIB"
# END PATCH msvcenv

######################################################################
# BEGIN PATCH bzip2
--- makefile.msc	2002-01-02 05:02:33.000000000 +0100
+++ makefile.msc	2009-11-26 14:00:44.487307500 +0100
@@ -4,7 +4,7 @@
 # Fixed up by JRS for bzip2-0.9.5d release.
 
 CC=cl
-CFLAGS= -DWIN32 -MD -Ox -D_FILE_OFFSET_BITS=64 -nologo
+CFLAGS= -DWIN32 $(EXTRA_CFLAGS) -Ox -D_FILE_OFFSET_BITS=64 -nologo
 
 OBJS= blocksort.obj  \
       huffman.obj    \
@@ -14,11 +14,14 @@
       decompress.obj \
       bzlib.obj
 
-all: lib bzip2 test
+all: lib dll bzip2 test
 
 bzip2: lib
-	$(CC) $(CFLAGS) -o bzip2 bzip2.c libbz2.lib setargv.obj
-	$(CC) $(CFLAGS) -o bzip2recover bzip2recover.c
+	$(CC) $(CFLAGS) -Febzip2 bzip2.c libbz2.lib setargv.obj
+	$(CC) $(CFLAGS) -Febzip2recover bzip2recover.c
+
+dll: $(OBJS)
+	$(CC) -LD -Fdbz2.pdb -Febz2.dll $(OBJS) -link -def:libbz2.def
 
 lib: $(OBJS)
 	lib /out:libbz2.lib $(OBJS)
@@ -59,5 +62,5 @@
 	del sample3.tst
 
 .c.obj: 
-	$(CC) $(CFLAGS) -c $*.c -o $*.obj
+	$(CC) $(CFLAGS) -c $*.c -Fo$*.obj
 
--- libbz2.def	2000-05-16 23:35:52.000000000 +0200
+++ libbz2.def	2003-07-03 20:22:21.000000000 +0200
@@ -1,5 +1,3 @@
-LIBRARY			LIBBZ2
-DESCRIPTION		"libbzip2: library for data compression"
 EXPORTS
 	BZ2_bzCompressInit
 	BZ2_bzCompress

# END PATCH bzip2

######################################################################
# BEGIN PATCH pcre
--- pcre.def	2003-05-22 15:25:53.000000000 +0200
+++ /home/lat/externals/src/pcre-4.2/pcre.def	2003-07-02 11:27:46.000000000 +0200
@@ -3,9 +3,11 @@
 pcre_malloc DATA
 pcre_free DATA
 
+pcre_config
 pcre_compile
 pcre_copy_substring
 pcre_exec
+pcre_get_stringnumber
 pcre_get_substring
 pcre_get_substring_list
 pcre_free_substring
--- Makefile.in	2003-07-03 21:06:56.000000000 +0200
+++ Makefile.in.new	2003-07-03 21:06:19.000000000 +0200
@@ -20,7 +20,7 @@
 # This setting enables Unix-style directory scanning in pcregrep, triggered
 # by the -f option. Maybe one day someone will add code for other systems.
 
-PCREGREP_OSTYPE=-DIS_UNIX
+PCREGREP_OSTYPE=-DHAVE_WIN32API
 
 #############################################################################
 
@@ -86,11 +86,11 @@
 # to do one or the other or both by ./configure.
 
 LIBTOOL = @LIBTOOL@
-LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) -c $(CFLAGS) -I. -I$(top_srcdir) $(NEWLINE) $(LINK_SIZE) $(MATCH_LIMIT)
+LTCOMPILE = $(CC) -c $(CFLAGS) -I. -I$(top_srcdir) $(NEWLINE) $(LINK_SIZE) $(MATCH_LIMIT) $(CPPFLAGS)
 @ON_WINDOWS@LINK = $(CC) $(CFLAGS) -I. -I$(top_srcdir) -L.libs
 @NOT_ON_WINDOWS@LINK = $(LIBTOOL) --mode=link $(CC) $(CFLAGS) -I. -I$(top_srcdir)
-LINKLIB = $(LIBTOOL) --mode=link $(CC) $(CFLAGS) -I. -I$(top_srcdir)
-LINK_FOR_BUILD = $(LIBTOOL) --mode=link $(CC_FOR_BUILD) $(CFLAGS_FOR_BUILD) -I. -I$(top_srcdir)
+LINKLIB = $(CC) $(CFLAGS) -I. -I$(top_srcdir)
+LINK_FOR_BUILD = $(CC_FOR_BUILD) $(CFLAGS_FOR_BUILD) -I. -I$(top_srcdir)
 
 # These are the version numbers for the shared libraries
 
@@ -100,48 +100,46 @@
 ##############################################################################
 
 
-OBJ = maketables.@OBJEXT@ get.@OBJEXT@ study.@OBJEXT@ pcre.@OBJEXT@ @POSIX_OBJ@
+OBJ = maketables.@OBJEXT@ get.@OBJEXT@ study.@OBJEXT@ pcre.@OBJEXT@ pcreposix.obj windll.obj
 LOBJ = maketables.lo get.lo study.lo pcre.lo @POSIX_LOBJ@
 
-all:            libpcre.la @POSIX_LIB@ pcretest@EXEEXT@ pcregrep@EXEEXT@ @ON_WINDOWS@ winshared
+all:            libpcre.lib @POSIX_LIB@ pcretest@EXEEXT@ pcregrep@EXEEXT@ @ON_WINDOWS@ winshared
 
-pcregrep@EXEEXT@: libpcre.la pcregrep.@OBJEXT@ @ON_WINDOWS@ winshared
-		$(LINK) -o pcregrep@EXEEXT@ pcregrep.@OBJEXT@ -lpcre
+pcregrep@EXEEXT@: libpcre.lib pcregrep.@OBJEXT@ @ON_WINDOWS@ winshared
+		$(LINK) /Fepcregrep@EXEEXT@ pcregrep.@OBJEXT@ -lpcre
 
-pcretest@EXEEXT@: libpcre.la @POSIX_LIB@ pcretest.@OBJEXT@ @ON_WINDOWS@ winshared
-		$(LINK) $(PURIFY) $(EFENCE) -o pcretest@EXEEXT@  pcretest.@OBJEXT@ \
+pcretest@EXEEXT@: libpcre.lib @POSIX_LIB@ pcretest.@OBJEXT@ @ON_WINDOWS@ winshared
+		$(LINK) $(PURIFY) $(EFENCE) /Fepcretest@EXEEXT@  pcretest.@OBJEXT@ \
 		-lpcre @POSIX_LIB@
 
-libpcre.la:     $(OBJ)
-		-rm -f libpcre.la
-		$(LINKLIB) -rpath $(LIBDIR) -version-info \
-		'$(PCRELIBVERSION)' -o libpcre.la $(LOBJ)
-
-libpcreposix.la: pcreposix.@OBJEXT@
-		-rm -f libpcreposix.la
-		$(LINKLIB) -rpath $(LIBDIR) -L. -lpcre -version-info \
-		'$(PCREPOSIXLIBVERSION)' -o libpcreposix.la pcreposix.lo
+libpcre.lib:     $(OBJ)
+		-rm -f libpcre.lib
+		lib /out:$@ $(OBJ)
+
+libpcreposix.lib: pcreposix.@OBJEXT@
+		-rm -f libpcreposix.lib
+		lib /out:$@ pcreposix.obj
 
 pcre.@OBJEXT@:  $(top_srcdir)/chartables.c $(top_srcdir)/pcre.c \
 		$(top_srcdir)/internal.h $(top_srcdir)/printint.c \
 		pcre.h config.h Makefile
-		$(LTCOMPILE) $(UTF8) $(POSIX_MALLOC_THRESHOLD) $(top_srcdir)/pcre.c
+		$(LTCOMPILE) -DDLL_EXPORT $(UTF8) $(POSIX_MALLOC_THRESHOLD) $(top_srcdir)/pcre.c
 
 pcreposix.@OBJEXT@: $(top_srcdir)/pcreposix.c $(top_srcdir)/pcreposix.h \
 		$(top_srcdir)/internal.h pcre.h config.h Makefile
-		$(LTCOMPILE) $(POSIX_MALLOC_THRESHOLD) $(top_srcdir)/pcreposix.c
+		$(LTCOMPILE) -DDLL_EXPORT $(POSIX_MALLOC_THRESHOLD) $(top_srcdir)/pcreposix.c
 
 maketables.@OBJEXT@:   $(top_srcdir)/maketables.c $(top_srcdir)/internal.h \
 		pcre.h config.h Makefile
-		$(LTCOMPILE) $(top_srcdir)/maketables.c
+		$(LTCOMPILE) -DDLL_EXPORT $(top_srcdir)/maketables.c
 
 get.@OBJEXT@:          $(top_srcdir)/get.c $(top_srcdir)/internal.h \
 		pcre.h config.h Makefile
-		$(LTCOMPILE) $(top_srcdir)/get.c
+		$(LTCOMPILE) -DDLL_EXPORT $(top_srcdir)/get.c
 
 study.@OBJEXT@:        $(top_srcdir)/study.c $(top_srcdir)/internal.h \
 		pcre.h config.h Makefile
-		$(LTCOMPILE) $(UTF8) $(top_srcdir)/study.c
+		$(LTCOMPILE) -DDLL_EXPORT $(UTF8) $(top_srcdir)/study.c
 
 pcretest.@OBJEXT@:     $(top_srcdir)/pcretest.c $(top_srcdir)/internal.h \
 		$(top_srcdir)/printint.c \
@@ -153,53 +151,24 @@
 
 # Some Windows-specific targets, for Cygwin and MinGW
 
-winshared : .libs/@WIN_PREFIX@pcre.dll .libs/@WIN_PREFIX@pcreposix.dll
+winshared : .libs/pcre.dll
 
-.libs/@WIN_PREFIX@pcre.dll : libpcre.la
-	$(CC) $(CFLAGS) -shared -o $@ \
-	-Wl,--whole-archive .libs/libpcre.a \
-	-Wl,--out-implib,.libs/libpcre.dll.a \
-	-Wl,--output-def,.libs/@WIN_PREFIX@pcre.dll-def \
-	-Wl,--export-all-symbols \
-	-Wl,--no-whole-archive
-	sed -e "s#dlname=''#dlname='../bin/@WIN_PREFIX@pcre.dll'#" \
-	-e "s#library_names=''#library_names='libpcre.dll.a'#" \
-	< .libs/libpcre.lai > .libs/libpcre.lai.tmp && \
-	mv .libs/libpcre.lai.tmp .libs/libpcre.lai
-	sed -e "s#dlname=''#dlname='../bin/@WIN_PREFIX@pcre.dll'#" \
-	-e "s#library_names=''#library_names='libpcre.dll.a'#" \
-	< libpcre.la > libpcre.la.tmp && \
-	mv libpcre.la.tmp libpcre.la
-
-
-.libs/@WIN_PREFIX@pcreposix.dll: libpcreposix.la libpcre.la
-	$(CC) $(CFLAGS) -shared -o $@ \
-	-Wl,--whole-archive .libs/libpcreposix.a \
-	-Wl,--out-implib,.libs/lib@WIN_PREFIX@pcreposix.dll.a \
-	-Wl,--output-def,.libs/@WIN_PREFIX@pcreposix.dll-def \
-	-Wl,--export-all-symbols \
-	-Wl,--no-whole-archive .libs/libpcre.a
-	sed -e "s#dlname=''#dlname='../bin/@WIN_PREFIX@pcreposix.dll'#" \
-	-e "s#library_names=''#library_names='libpcreposix.dll.a'#"\
-	< .libs/libpcreposix.lai > .libs/libpcreposix.lai.tmp && \
-	mv .libs/libpcreposix.lai.tmp .libs/libpcreposix.lai
-	sed -e "s#dlname=''#dlname='../bin/@WIN_PREFIX@pcreposix.dll'#" \
-	-e "s#library_names=''#library_names='libpcreposix.dll.a'#"\
-	< libpcreposix.la > libpcreposix.la.tmp && \
-	mv libpcreposix.la.tmp libpcreposix.la
+windll.c:
+	(echo "#include <windows.h>";						\
+	 echo "BOOL APIENTRY DllMain (HINSTANCE h, DWORD dw, LPVOID p)";	\
+	 echo "{ return TRUE; }";						\
+	) > $@
+windll.obj: windll.c
+	$(CC) -c $(CFLAGS) windll.c
 
+.libs/pcre.dll : $(OBJ)
+	$(CC) /link /dll /Fe$@ $(OBJ) /link /def:pcre.def
 
 wininstall : winshared
 	$(mkinstalldirs) $(DESTDIR)$(LIBDIR)
 	$(mkinstalldirs) $(DESTDIR)$(BINDIR)
-	$(INSTALL) .libs/@WIN_PREFIX@pcre.dll $(DESTDIR)$(BINDIR)/@WIN_PREFIX@pcre.dll
-	$(INSTALL) .libs/@WIN_PREFIX@pcreposix.dll $(DESTDIR)$(BINDIR)/@WIN_PREFIX@pcreposix.dll
-	$(INSTALL) .libs/@WIN_PREFIX@libpcreposix.dll.a $(DESTDIR)$(LIBDIR)/@WIN_PREFIX@libpcreposix.dll.a
-	$(INSTALL) .libs/@WIN_PREFIX@libpcre.dll.a $(DESTDIR)$(LIBDIR)/@WIN_PREFIX@libpcre.dll.a
-	-strip -g $(DESTDIR)$(BINDIR)/@WIN_PREFIX@pcre.dll
-	-strip -g $(DESTDIR)$(BINDIR)/@WIN_PREFIX@pcreposix.dll
-	-strip $(DESTDIR)$(BINDIR)/pcregrep@EXEEXT@
-	-strip $(DESTDIR)$(BINDIR)/pcretest@EXEEXT@
+	$(INSTALL) .libs/pcre.dll $(DESTDIR)$(BINDIR)/pcre.dll
+	$(INSTALL) .libs/pcre.lib $(DESTDIR)$(LIBDIR)/pcre.lib
 
 # An auxiliary program makes the default character table source
 
@@ -261,7 +230,7 @@
 # chartables.c shouldn't change, and if people have edited the tables by hand,
 # you don't want to throw them away.
 
-clean:;         -rm -rf *.@OBJEXT@ *.lo *.a *.la .libs pcretest@EXEEXT@ pcregrep@EXEEXT@ testtry
+clean:;         -rm -rf *.@OBJEXT@ *.lo *.a *.lib .libs pcretest@EXEEXT@ pcregrep@EXEEXT@ testtry
 
 # But "make distclean" should get back to a virgin distribution
 
@@ -271,12 +240,12 @@
 
 check:          runtest
 
-@WIN_PREFIX@pcre.dll : winshared
-	cp .libs/@WIN_PREFIX@pcre.dll .
+pcre.dll : winshared
+	cp .libs/pcre.dll .
 
 test:           runtest
 
-runtest:        all @ON_WINDOWS@ @WIN_PREFIX@pcre.dll
+runtest:        all @ON_WINDOWS@ pcre.dll
 		./RunTest
 
 # End
# END PATCH pcre

######################################################################
# BEGIN PATCH rx
--- /dev/null	2003-07-03 22:15:23.000000000 +0200
+++ rx.def	2003-07-03 22:14:41.000000000 +0200
@@ -0,0 +1,7 @@
+EXPORTS
+regncomp
+regcomp
+regerror
+regnexec
+regexec
+regfree
--- Makefile.in	2003-07-03 22:07:37.000000000 +0200
+++ Makefile.in.new	2003-07-03 22:12:43.000000000 +0200
@@ -41,7 +41,7 @@
 RANLIB = @RANLIB@
 opt_objs=@opt_objs@
 
-source= hashrexp.c rx.c rxanal.c rxbasic.c rxbitset.c rxcset.c rxdbug.c rxgnucomp.c rxhash.c rxnfa.c rxnode.c rxposix.c rxsimp.c rxspencer.c rxstr.c rxsuper.c rxunfa.c rgx.c
+source= hashrexp.c rx.c rxanal.c rxbasic.c rxbitset.c rxcset.c rxdbug.c rxgnucomp.c rxhash.c rxnfa.c rxnode.c rxposix.c rxsimp.c rxspencer.c rxstr.c rxsuper.c rxunfa.c rgx.c strncasecmp.c
 
 headers=_rx.h rx.h rxanal.h rxbasic.h rxbitset.h rxcontext.h rxcset.h rxgnucomp.h rxhash.h rxnfa.h rxnode.h inst-rxposix.h  rxposix.h rxproto.h rxsimp.h rxspencer.h rxstr.h rxsuper.h rxunfa.h  rxall.h
 
@@ -49,19 +49,19 @@
 	  COPYING.LIB ChangeLog PLUGIN  \
 	  ../doc/rx.texi ../doc/rx.info ../doc/texinfo.tex
 distfiles=$(source) $(headers) $(ancillery)
-libobjs= hashrexp.o rx.o rxanal.o rxbasic.o rxbitset.o rxcset.o rxdbug.o rxgnucomp.o rxhash.o rxnfa.o rxnode.o rxposix.o rxsimp.o rxspencer.o rxstr.o rxsuper.o rxunfa.o
+libobjs= hashrexp.obj rx.obj rxanal.obj rxbasic.obj rxbitset.obj rxcset.obj rxdbug.obj rxgnucomp.obj rxhash.obj rxnfa.obj rxnode.obj rxposix.obj rxsimp.obj rxspencer.obj rxstr.obj rxsuper.obj rxunfa.obj strncasecmp.obj
 gen_c_files=rgx.x
 
 .SUFFIXES:
-.SUFFIXES: .o .c .h .ps .dvi .info .texinfo .scm .cd .x
+.SUFFIXES: .obj .c .h .ps .dvi .info .texinfo .scm .cd .x
 
 .c.x:
 	$(CC) $(ALL_CFLAGS) -DSCM_MAGIC_SNARFER -E $< | grep "^%%%" | sed -e "s/^%%%//" > $@ ; \
 
-.c.o:
+.c.obj:
 	$(CC) -c $(ALL_CFLAGS) $(DEFS) -I$(srcdir) $<
 
-all: librx.a
+all: rx.dll regex.h
 
 install: all
 	test -d $(libdir) || mkdir $(libdir)
@@ -110,6 +110,13 @@
 	tar chzf `cat .fname`.tar.gz `cat .fname`
 	rm -rf `cat .fname` .fname
 
+rx.dll: $(libobjs) $(opt_objs)
+	$(CC) /Fe$@ $(libobjs) $(opt_objs) /link /dll /def:rx.def
+
+regex.h:
+	(echo '#ifdef __cplusplus'; echo 'extern "C" {'; echo '#endif'; \
+	 cat $(srcdir)/rxposix.h; \
+	 echo '#ifdef __cplusplus'; echo '}'; echo '#endif';) > $@
 
 librx.a: $(libobjs) $(opt_objs)
 	rm -f librx.a
@@ -122,31 +129,31 @@
 	gcc -MM -I. -I../libsystas $(source)
 	rm $(gen_c_files) scmconfig.h
 ###
-hashrexp.o : hashrexp.c rxall.h rxnode.h rxbitset.h rxcset.h rxhash.h 
-rx.o : rx.c rx.h rxhash.h rxbitset.h rxall.h rxnfa.h _rx.h rxcset.h rxnode.h rxsuper.h
-rxanal.o : rxanal.c rxall.h rxanal.h rxcset.h rxbitset.h rxnode.h rxsuper.h \
+hashrexp.obj : hashrexp.c rxall.h rxnode.h rxbitset.h rxcset.h rxhash.h 
+rx.obj : rx.c rx.h rxhash.h rxbitset.h rxall.h rxnfa.h _rx.h rxcset.h rxnode.h rxsuper.h
+rxanal.obj : rxanal.c rxall.h rxanal.h rxcset.h rxbitset.h rxnode.h rxsuper.h \
   rxnfa.h _rx.h rxhash.h 
-rxbasic.o : rxbasic.c rxall.h rxbasic.h rxcontext.h rxnode.h rxbitset.h rxcset.h \
+rxbasic.obj : rxbasic.c rxall.h rxbasic.h rxcontext.h rxnode.h rxbitset.h rxcset.h \
   rxspencer.h rxproto.h rxunfa.h _rx.h rxhash.h rxanal.h rxsuper.h rxnfa.h inst-rxposix.h \
   rxstr.h 
-rxbitset.o : rxbitset.c rxall.h rxbitset.h 
-rxcset.o : rxcset.c rxall.h rxcset.h rxbitset.h 
-rxdbug.o : rxdbug.c rxall.h rxgnucomp.h rxcset.h rxbitset.h rxnode.h rxnfa.h \
+rxbitset.obj : rxbitset.c rxall.h rxbitset.h 
+rxcset.obj : rxcset.c rxall.h rxcset.h rxbitset.h 
+rxdbug.obj : rxdbug.c rxall.h rxgnucomp.h rxcset.h rxbitset.h rxnode.h rxnfa.h \
   _rx.h rxhash.h 
-rxgnucomp.o : rxgnucomp.c rxall.h rxgnucomp.h rxcset.h rxbitset.h rxnode.h 
-rxhash.o : rxhash.c rxall.h rxhash.h rxbitset.h 
-rxnfa.o : rxnfa.c rxall.h rxnfa.h _rx.h rxhash.h rxbitset.h rxcset.h rxnode.h 
-rxnode.o : rxnode.c rxall.h rxnode.h rxbitset.h rxcset.h 
-rxposix.o : rxposix.c rxall.h rxposix.h rxspencer.h rxproto.h rxnode.h rxbitset.h \
+rxgnucomp.obj : rxgnucomp.c rxall.h rxgnucomp.h rxcset.h rxbitset.h rxnode.h 
+rxhash.obj : rxhash.c rxall.h rxhash.h rxbitset.h 
+rxnfa.obj : rxnfa.c rxall.h rxnfa.h _rx.h rxhash.h rxbitset.h rxcset.h rxnode.h 
+rxnode.obj : rxnode.c rxall.h rxnode.h rxbitset.h rxcset.h 
+rxposix.obj : rxposix.c rxall.h rxposix.h rxspencer.h rxproto.h rxnode.h rxbitset.h \
   rxcset.h rxunfa.h _rx.h rxhash.h rxanal.h rxsuper.h rxnfa.h inst-rxposix.h rxcontext.h \
   rxgnucomp.h rxbasic.h rxsimp.h 
-rxsimp.o : rxsimp.c rxall.h rxsimp.h rxcset.h rxbitset.h rxnode.h 
-rxspencer.o : rxspencer.c rxall.h rxspencer.h rxproto.h rxnode.h rxbitset.h \
+rxsimp.obj : rxsimp.c rxall.h rxsimp.h rxcset.h rxbitset.h rxnode.h 
+rxspencer.obj : rxspencer.c rxall.h rxspencer.h rxproto.h rxnode.h rxbitset.h \
   rxcset.h rxunfa.h _rx.h rxhash.h rxanal.h rxsuper.h rxnfa.h inst-rxposix.h 
-rxstr.o : rxstr.c rxall.h rxstr.h rxspencer.h rxproto.h rxnode.h rxbitset.h \
+rxstr.obj : rxstr.c rxall.h rxstr.h rxspencer.h rxproto.h rxnode.h rxbitset.h \
   rxcset.h rxunfa.h _rx.h rxhash.h rxanal.h rxsuper.h rxnfa.h inst-rxposix.h rxcontext.h 
-rxsuper.o : rxsuper.c rxall.h rxsuper.h rxnfa.h _rx.h rxhash.h rxbitset.h rxcset.h \
+rxsuper.obj : rxsuper.c rxall.h rxsuper.h rxnfa.h _rx.h rxhash.h rxbitset.h rxcset.h \
   rxnode.h 
-rxunfa.o : rxunfa.c rxall.h rx.h rxhash.h rxbitset.h rxunfa.h _rx.h rxcset.h \
+rxunfa.obj : rxunfa.c rxall.h rx.h rxhash.h rxbitset.h rxunfa.h _rx.h rxcset.h \
   rxnfa.h rxnode.h 
-rgx.o : rgx.c rgx.x
+rgx.obj : rgx.c rgx.x
--- rxall.h	2003-07-03 22:13:36.000000000 +0200
+++ rxall.h.new	2003-07-03 22:13:41.000000000 +0200
@@ -21,8 +21,8 @@
  */
 
 
-#if 0
 #include <stdlib.h>
+#if 0
 #include "malloc.h"
 #endif
 
# END PATCH rx
