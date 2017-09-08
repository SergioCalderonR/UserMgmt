#pragma once
#include <Windows.h>
#include <tchar.h>
#include <LM.h>
#include <string.h>

VOID ShowHelp(VOID)
{
	wprintf(L"\nUser Management Tool (UserMgmt) 1.0\n"
			L"Manage common user operations in Windows\n"
			L"\nUsage: UserMgmt.exe [UserName | Domain\\UserName]\n");

}

VOID ShowErrorMsg(DWORD messageId)
{
	DWORD flags=FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD languageID = LANG_USER_DEFAULT;
	LPWSTR errorMsg;
	DWORD size = 0;

	if (!FormatMessageW(flags, NULL, messageId, languageID, (LPWSTR)&errorMsg, size, NULL))
	{
		wprintf(L"Format message failed with error code: %lu\n", GetLastError());
	}

	wprintf(L"\n%s", errorMsg);

	LocalFree(errorMsg);

}


