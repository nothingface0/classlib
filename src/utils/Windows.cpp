
#include "classlib/sysapi/Windows.h"
#ifdef _WIN32


// Implementation of auto-loaded code.

lat::AutoLoadLib WinStubNTDLL ("ntdll");
lat::AutoLoadLib WinStubKernel32 ("kernel32");
lat::AutoLoadLib WinStubPowrprof ("powrprof");

lat::AutoLoad<NTSTATUS (HANDLE hProcess, PROCESSINFOCLASS pic, PVOID pi, ULONG len, PULONG plen)>
    MyNtQueryInformationProcess (WinStubNTDLL, "NtQueryInformationProcess"); // FIXME: NTAPI?

lat::AutoLoad<NTSTATUS (POWER_INFORMATION_LEVEL level, PVOID pin, ULONG nin, PVOID pout, ULONG nout)>
    MyCallNtPowerInformation (WinStubPowrprof, "CallNtPowerInformation"); // FIXME: WINAPI?

lat::AutoLoad<BOOL (COMPUTER_NAME_FORMAT fmt, LPWSTR name, LPDWORD size)>
    MyGetComputerNameExW (WinStubKernel32, "GetComputerNameExW"); // FIXME: WINAPI?

lat::AutoLoad<BOOL (COMPUTER_NAME_FORMAT fmt, LPSTR name, LPDWORD size)>
    MyGetComputerNameExA (WinStubKernel32, "GetComputerNameExA"); // FIXME: WINAPI?


#endif // _WIN32
