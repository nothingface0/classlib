DEFAULT_INCLUDES = -I. -I$(srcdir)
lib_LTLIBRARIES	= libclasslib.la

libclasslib_la_CXXFLAGS = @ZINC@ @BZ2INC@ @PCREINC@ @OPENSSLINC@ @LZOINC@ @LZMAINC@ @RXINC@
libclasslib_la_LDFLAGS =			\
  -avoid-version -no-undefined			\
  @ZLIB@ @BZ2LIB@ @PCRELIB@ @OPENSSLLIB@ @LZOLIB@ @LZMALIB@ @RXLIB@ @LIBS@ @EXTRA@
libclasslib_la_SOURCES =			\
  src/utils/Argz.cpp				\
  src/utils/AutoLoad.cpp			\
  src/utils/DebugAids.cpp			\
  src/utils/Environment.cpp			\
  src/utils/Error.cpp				\
  src/utils/HostInfo.cpp			\
  src/utils/Log.cpp				\
  src/utils/LogConsole.cpp			\
  src/utils/LogSink.cpp				\
  src/utils/LogStderr.cpp			\
  src/utils/LongLong.cpp			\
  src/utils/LowMemoryPool.cpp			\
  src/utils/MultiMethod.cpp			\
  src/utils/ProcessInfo.cpp			\
  src/utils/Regexp.cpp				\
  src/utils/RegexpError.cpp			\
  src/utils/RegexpMatch.cpp			\
  src/utils/ResourceInfo.cpp			\
  src/utils/SearchPath.cpp			\
  src/utils/SharedLibrary.cpp			\
  src/utils/SharedLibraryError.cpp		\
  src/utils/ShellEnvironment.cpp		\
  src/utils/Signal.cpp				\
  src/utils/StringFormat.cpp			\
  src/utils/StringOps.cpp			\
  src/utils/SystemError.cpp			\
  src/utils/SystemInfo.cpp			\
  src/utils/Time.cpp				\
  src/utils/TimeInfo.cpp			\
  src/utils/UUID.cpp				\
  src/utils/UUIDError.cpp			\
  src/utils/UserInfo.cpp			\
  src/utils/UnixUserInfo.cpp			\
  src/utils/WindowsUserInfo.cpp			\
  src/utils/Windows.cpp				\
  src/utils/XTypeInfo.cpp			\
  src/iobase/DirIterator.cpp			\
  src/iobase/UnixDirIterator.cpp		\
  src/iobase/WindowsDirIterator.cpp		\
  src/iobase/File.cpp				\
  src/iobase/UnixFile.cpp			\
  src/iobase/WindowsFile.cpp			\
  src/iobase/FileAcl.cpp			\
  src/iobase/UnixFileAcl.cpp			\
  src/iobase/WindowsFileAcl.cpp			\
  src/iobase/FileError.cpp			\
  src/iobase/FileLock.cpp			\
  src/iobase/UnixFileLock.cpp			\
  src/iobase/WindowsFileLock.cpp		\
  src/iobase/FileMapping.cpp			\
  src/iobase/UnixFileMapping.cpp		\
  src/iobase/WindowsFileMapping.cpp		\
  src/iobase/FileMappingError.cpp		\
  src/iobase/FileNameLock.cpp			\
  src/iobase/FileSystemError.cpp		\
  src/iobase/Filename.cpp			\
  src/iobase/UnixFilename.cpp			\
  src/iobase/WindowsFilename.cpp		\
  src/iobase/FilenameError.cpp			\
  src/iobase/IOChannel.cpp			\
  src/iobase/UnixIOChannel.cpp			\
  src/iobase/WindowsIOChannel.cpp		\
  src/iobase/IOChannelPair.cpp			\
  src/iobase/IOChannelTransfer.cpp		\
  src/iobase/IOError.cpp			\
  src/iobase/IOInput.cpp			\
  src/iobase/IOOutput.cpp			\
  src/iobase/IOSelectDevPoll.cpp		\
  src/iobase/IOSelectFdMap.cpp			\
  src/iobase/IOSelectKqueue.cpp			\
  src/iobase/IOSelectMethod.cpp			\
  src/iobase/IOSelectPoll.cpp			\
  src/iobase/IOSelectSelect.cpp			\
  src/iobase/IOSelectSig.cpp			\
  src/iobase/IOSelectSigFd.cpp			\
  src/iobase/IOSelectWindows.cpp		\
  src/iobase/IOSelector.cpp			\
  src/iobase/IOStatus.cpp			\
  src/iobase/UnixIOStatus.cpp			\
  src/iobase/WindowsIOStatus.cpp		\
  src/iobase/InetServerSocket.cpp		\
  src/iobase/InetSocket.cpp			\
  src/iobase/LocalServerSocket.cpp		\
  src/iobase/LocalSocket.cpp			\
  src/iobase/LogFile.cpp			\
  src/iobase/LogPipe.cpp			\
  src/iobase/LogSocket.cpp			\
  src/iobase/NamedPipeServer.cpp		\
  src/iobase/UnixNamedPipeServer.cpp		\
  src/iobase/WindowsNamedPipeServer.cpp		\
  src/iobase/NetworkError.cpp			\
  src/iobase/Pipe.cpp				\
  src/iobase/UnixPipe.cpp			\
  src/iobase/WindowsPipe.cpp			\
  src/iobase/PipeCmd.cpp			\
  src/iobase/Socket.cpp				\
  src/iobase/UnixSocket.cpp			\
  src/iobase/WindowsSocket.cpp			\
  src/iobase/SocketConst.cpp			\
  src/iobase/SocketPair.cpp			\
  src/iobase/UnixSocketPair.cpp			\
  src/iobase/WindowsSocketPair.cpp		\
  src/iobase/Storage.cpp			\
  src/iobase/SubProcess.cpp			\
  src/iobase/UnixSubProcess.cpp			\
  src/iobase/WindowsSubProcess.cpp		\
  src/iobase/SubProcessError.cpp		\
  src/iobase/TempFile.cpp			\
  src/iobase/UnixTempFile.cpp			\
  src/iobase/WindowsTempFile.cpp

nobase_include_HEADERS =			\
  classlib/sysapi/config.h			\
  classlib/iobase/DirIterator.h			\
  classlib/iobase/File.h			\
  classlib/iobase/FileAcl.h			\
  classlib/iobase/FileError.h			\
  classlib/iobase/FileLock.h			\
  classlib/iobase/FileMapping.h			\
  classlib/iobase/FileMappingError.h		\
  classlib/iobase/FileNameLock.h		\
  classlib/iobase/FileSystemError.h		\
  classlib/iobase/Filename.h			\
  classlib/iobase/FilenameError.h		\
  classlib/iobase/IOBuffer.h			\
  classlib/iobase/IOChannel.h			\
  classlib/iobase/IOChannelPair.h		\
  classlib/iobase/IOError.h			\
  classlib/iobase/IOFlags.h			\
  classlib/iobase/IOInput.h			\
  classlib/iobase/IOOutput.h			\
  classlib/iobase/IOSelectCB.h			\
  classlib/iobase/IOSelectEvent.h		\
  classlib/iobase/IOSelector.h			\
  classlib/iobase/IOStatus.h			\
  classlib/iobase/InetServerSocket.h		\
  classlib/iobase/InetSocket.h			\
  classlib/iobase/LocalServerSocket.h		\
  classlib/iobase/LocalSocket.h			\
  classlib/iobase/NamedPipeServer.h		\
  classlib/iobase/NetworkError.h		\
  classlib/iobase/Pipe.h			\
  classlib/iobase/PipeCmd.h			\
  classlib/iobase/Socket.h			\
  classlib/iobase/SocketConst.h			\
  classlib/iobase/SocketPair.h			\
  classlib/iobase/Storage.h			\
  classlib/iobase/SubProcess.h			\
  classlib/iobase/SubProcessError.h		\
  classlib/iobase/TempFile.h			\
  classlib/sysapi/DebugAids.h			\
  classlib/sysapi/Detect.h                      \
  classlib/sysapi/DirIterator.h			\
  classlib/sysapi/ElfAbi.h			\
  classlib/sysapi/File.h			\
  classlib/sysapi/FileAcl.h			\
  classlib/sysapi/FileLock.h			\
  classlib/sysapi/FileMapping.h			\
  classlib/sysapi/FileNameLock.h		\
  classlib/sysapi/Filename.h			\
  classlib/sysapi/HostInfo.h			\
  classlib/sysapi/IOChannel.h			\
  classlib/sysapi/IOChannelTransfer.h		\
  classlib/sysapi/IOPlatformData.h		\
  classlib/sysapi/IOStatus.h			\
  classlib/sysapi/IOTypes.h			\
  classlib/sysapi/InetSocket.h			\
  classlib/sysapi/LocalSocket.h			\
  classlib/sysapi/NamedPipeServer.h		\
  classlib/sysapi/Pipe.h			\
  classlib/sysapi/PipeCmd.h			\
  classlib/sysapi/ProcessInfo.h			\
  classlib/sysapi/ResourceInfo.h		\
  classlib/sysapi/SharedLibrary.h		\
  classlib/sysapi/Signal.h			\
  classlib/sysapi/Socket.h			\
  classlib/sysapi/SocketPair.h			\
  classlib/sysapi/SubProcess.h			\
  classlib/sysapi/SystemError.h			\
  classlib/sysapi/SystemInfo.h			\
  classlib/sysapi/TempFile.h			\
  classlib/sysapi/Time.h			\
  classlib/sysapi/TimeInfo.h			\
  classlib/sysapi/UserInfo.h			\
  classlib/sysapi/Windows.h			\
  classlib/sysapi/WindowsIO.h			\
  classlib/sysapi/inttypes.h                    \
  classlib/sysapi/iosfwd.h			\
  classlib/sysapi/nonstdint.h			\
  classlib/sysapi/system.h			\
  classlib/utils/Argz.h				\
  classlib/utils/AutoLoad.h			\
  classlib/utils/BitIterator.h			\
  classlib/utils/BitOps.h			\
  classlib/utils/BitPattern.h			\
  classlib/utils/BitTraits.h			\
  classlib/utils/Callback.h			\
  classlib/utils/DebugAids.h			\
  classlib/utils/Deprecated.h			\
  classlib/utils/Environment.h			\
  classlib/utils/Error.h			\
  classlib/utils/Hook.h				\
  classlib/utils/HookList.h			\
  classlib/utils/HostInfo.h			\
  classlib/utils/IntBits.h			\
  classlib/utils/IntTraits.h			\
  classlib/utils/Log.h				\
  classlib/utils/LongLong.h			\
  classlib/utils/LowMemoryPool.h		\
  classlib/utils/Macros.h			\
  classlib/utils/MapIterator.h			\
  classlib/utils/MultiMethod.h			\
  classlib/utils/PODVector.h			\
  classlib/utils/ProcessInfo.h			\
  classlib/utils/Range.h			\
  classlib/utils/Regexp.h			\
  classlib/utils/RegexpError.h			\
  classlib/utils/RegexpMatch.h			\
  classlib/utils/ResourceInfo.h			\
  classlib/utils/SaveErrno.h			\
  classlib/utils/SearchPath.h			\
  classlib/utils/Sequence.h			\
  classlib/utils/SharedLibrary.h		\
  classlib/utils/SharedLibraryError.h		\
  classlib/utils/ShellEnvironment.h		\
  classlib/utils/Signal.h			\
  classlib/utils/StringFormat.h			\
  classlib/utils/StringList.h			\
  classlib/utils/StringOps.h			\
  classlib/utils/SysErr.h			\
  classlib/utils/SystemError.h			\
  classlib/utils/SystemInfo.h			\
  classlib/utils/Time.h				\
  classlib/utils/TimeConst.h			\
  classlib/utils/TimeInfo.h			\
  classlib/utils/UUID.h				\
  classlib/utils/UUIDError.h			\
  classlib/utils/Unwind.h			\
  classlib/utils/UserInfo.h			\
  classlib/utils/XTypeInfo.h			\
  classlib/utils/iosfwd.h

TESTS_ENVIRONMENT = QUIET_ASSERT=a
TESTS = $(check_PROGRAMS)
check_LTLIBRARIES = libSharedLibrary01Lib.la
check_PROGRAMS = classlib-test
libSharedLibrary01Lib_la_SOURCES =		\
  test/utils/SharedLibrary01Lib.cpp

classlib_test_LDADD = -lclasslib
classlib_test_CPPFLAGS = $(AM_CPPFLAGS) \
  -DTEST_NAME=`echo $< | sed 's|^$(srcdir)/||; s|.cpp$$||; s|/|_|g'`
classlib_test_SOURCES =				\
  TestDriver.cpp				\
  test/iobase/DirIterator01.cpp			\
  test/iobase/DirIterator02.cpp			\
  test/iobase/DirIterator03.cpp			\
  test/iobase/DirIterator04.cpp			\
  test/iobase/File01.cpp			\
  test/iobase/File02.cpp			\
  test/iobase/File03.cpp			\
  test/iobase/File04.cpp			\
  test/iobase/File05.cpp			\
  test/iobase/File06.cpp			\
  test/iobase/FileAcl01.cpp			\
  test/iobase/FileLock01.cpp			\
  test/iobase/FileMapping01.cpp			\
  test/iobase/FileMapping02.cpp			\
  test/iobase/FileMapping03.cpp			\
  test/iobase/Filename000.cpp			\
  test/iobase/Filename01.cpp			\
  test/iobase/Filename02_01.cpp			\
  test/iobase/Filename02_02.cpp			\
  test/iobase/Filename02_03.cpp			\
  test/iobase/Filename02_04.cpp			\
  test/iobase/Filename02_05.cpp			\
  test/iobase/Filename02_06.cpp			\
  test/iobase/Filename02_07.cpp			\
  test/iobase/Filename02_08.cpp			\
  test/iobase/Filename02_09.cpp			\
  test/iobase/Filename02_10.cpp			\
  test/iobase/Filename02_11.cpp			\
  test/iobase/Filename02_12.cpp			\
  test/iobase/Filename02_13.cpp			\
  test/iobase/Filename03.cpp			\
  test/iobase/Filename04.cpp			\
  test/iobase/Filename05.cpp			\
  test/iobase/Filename06.cpp			\
  test/iobase/Filename07.cpp			\
  test/iobase/Filename08.cpp			\
  test/iobase/Filename09.cpp			\
  test/iobase/IOChannel01.cpp			\
  test/iobase/IOSelector01.cpp			\
  test/iobase/IOSelector02.cpp			\
  test/iobase/IOSelector03.cpp			\
  test/iobase/IOStatus01.cpp			\
  test/iobase/InetServerSocket01.cpp		\
  test/iobase/InetSocket01.cpp			\
  test/iobase/LocalServerSocket01.cpp		\
  test/iobase/LocalSocket01.cpp			\
  test/iobase/MemoryStorage01.cpp		\
  test/iobase/NamedPipeServer01.cpp		\
  test/iobase/Pipe01.cpp			\
  test/iobase/PipeCmd01.cpp			\
  test/iobase/SocketPair01.cpp			\
  test/iobase/SubProcess01.cpp			\
  test/iobase/SubProcess02.cpp			\
  test/iobase/SubProcess03.cpp			\
  test/iobase/SubProcess04.cpp			\
  test/iobase/SubProcess05.cpp			\
  test/iobase/SubStorage01.cpp			\
  test/iobase/SubStorage02.cpp			\
  test/iobase/SubStorage03.cpp			\
  test/iobase/SubStorage04.cpp			\
  test/iobase/SubStorage05.cpp			\
  test/utils/Argz01.cpp				\
  test/utils/AutoLoad01.cpp			\
  test/utils/BitIterator01.cpp			\
  test/utils/BitOps01.cpp			\
  test/utils/BitPattern01.cpp			\
  test/utils/BitTraits01.cpp			\
  test/utils/Callback01.cpp			\
  test/utils/Debug01.cpp			\
  test/utils/Environment01.cpp			\
  test/utils/Hook01.cpp				\
  test/utils/HookList01.cpp			\
  test/utils/HostInfo01.cpp			\
  test/utils/IntBits01.cpp			\
  test/utils/IntBits02.cpp			\
  test/utils/IntBits03.cpp			\
  test/utils/Log01.cpp				\
  test/utils/LogListener			\
  test/utils/LowMemoryPool01.cpp		\
  test/utils/MapIterator01.cpp			\
  test/utils/MapIterator02.cpp			\
  test/utils/MultiMethod01.cpp			\
  test/utils/MultiMethod02.cpp			\
  test/utils/MultiMethod03.cpp			\
  test/utils/ProcessInfo01.cpp			\
  test/utils/Range01.cpp			\
  test/utils/ResourceInfo01.cpp			\
  test/utils/SaveErrno01.cpp			\
  test/utils/SearchPath01.cpp			\
  test/utils/SearchPath02.cpp			\
  test/utils/Sequence01.cpp			\
  test/utils/SharedLibrary01.cpp		\
  test/utils/ShellEnvironment01.cpp		\
  test/utils/Signal01.cpp			\
  test/utils/Signal02.cpp			\
  test/utils/Signal03.cpp			\
  test/utils/StringFormat01.cpp			\
  test/utils/StringOps01.cpp			\
  test/utils/StringOps02.cpp			\
  test/utils/StringOps03.cpp			\
  test/utils/StringOps04.cpp			\
  test/utils/SystemInfo01.cpp			\
  test/utils/Time01.cpp				\
  test/utils/Time02.cpp				\
  test/utils/Time03.cpp				\
  test/utils/TimeInfo01.cpp			\
  test/utils/TimeInfo02.cpp			\
  test/utils/UUID01.cpp				\
  test/utils/UUID02.cpp				\
  test/utils/UUID03.cpp				\
  test/utils/UUID04.cpp				\
  test/utils/UUID05.cpp				\
  test/utils/UUID06.cpp				\
  test/utils/Unwind01.cpp			\
  test/utils/UserInfo01.cpp			\
  test/utils/XTypeInfo01.cpp			\
  test/utils/exa_Debug.cpp			\
  test/utils/exa_Log.cpp			\
  test/utils/exa_ResourceInfo.cpp		\
  test/utils/exa_Signal.cpp			\
  test/utils/LogListener.cpp

TestDriver.cpp: Makefile
	@(echo '#include <ctype.h>';				\
	  echo '#include <stdio.h>';				\
	  echo '#include <string.h>';				\
	  echo '#include <stdlib.h>';				\
	  for f in $(classlib_test_SOURCES); do			\
	    case $$f in TestDriver*) continue ;; esac;		\
	    echo "int $$(echo $$f | sed 's|/|_|g; s|[.]cpp$$||')"\
	         "(int, char **);";				\
	  done;							\
	  echo 'typedef int (*TFunc) (int, char **);';		\
	  echo 'struct TEntry { const char *name; TFunc func; };';\
	  echo 'struct TEntry TESTS[] = {';			\
	  for f in $(classlib_test_SOURCES); do			\
	    case $$f in TestDriver*) continue ;; esac;		\
	    echo "{ "\""$$(echo $$f | sed 's|[.]cpp$$||')"\""," \
	         "$$(echo $$f | sed 's|/|_|g; s|[.]cpp$$||') },";\
	  done;							\
	  echo '{ 0, 0 } };';					\
	  echo 'int main (int argc, char **argv) {';		\
	  echo ' int count=0; while (TESTS[count].name) ++count;';\
	  echo ' int test=-1;';					\
	  echo ' if (argc < 2) {';				\
	  echo '  for (int i = 0; i < count; ++i)';		\
	  echo '   printf("%3d. %s\\n", i, TESTS[i].name);';	\
	  echo '  printf("Enter test number: "); fflush(stdout);';\
	  echo '  scanf("%d", &test);';				\
	  echo ' } else { test = atoi(argv[1]); }';		\
	  echo ' if (test < 0 || test >= count) {';		\
	  echo '  printf("%d is not a valid test.\\n", test);';	\
	  echo '  return 1;';					\
	  echo ' } else { --argc; ++argv; }';			\
	  echo ' return TESTS[test].func(argc, argv); }') > $@

check-programs: $(check_PROGRAMS)
