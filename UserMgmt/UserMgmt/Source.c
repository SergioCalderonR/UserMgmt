#include "Internal.h"

#pragma comment(lib, "Netapi32.lib")

int wmain(int argc, WCHAR *argv[])
{
	//Functions
	NET_API_STATUS localGroups, globalGroups;

	//NetUserGetLocalGroups
	DWORD level = 0;	//array of LOCALGROUP_USER_INFO_0 structures
	DWORD flags = LG_INCLUDE_INDIRECT;
	PLOCALGROUP_USERS_INFO_0 groups;
	DWORD prefMaxLen=MAX_PREFERRED_LENGTH;
	DWORD entriesRead;
	DWORD totalEntries;

	if (argc != 2)
	{
		ShowHelp();
	}

	//Retrieves local groups
	localGroups = NetUserGetLocalGroups(NULL, argv[1], level, flags, (LPBYTE*)&groups,
										prefMaxLen, &entriesRead, &totalEntries);

	if (localGroups != NERR_Success)
		ShowErrorMsg(localGroups);
	else
	{
		ShowErrorMsg(localGroups);
	}
	

	return 0;
}