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

	DWORD index;

	if (argc == 1)
	{
		ShowHelp();
	}

	switch (argc)
	{
	//UserMgmt.exe -all UserName
	case 3:
		if (lstrcmpW(argv[1], L"-all") == 0)
		{
			wprintf(L"\n-all switch.\n");

			//Retrieves local groups
			localGroups = NetUserGetLocalGroups(NULL, argv[2], level, flags, (LPBYTE*)&groups,
				prefMaxLen, &entriesRead, &totalEntries);

			if (localGroups != NERR_Success)
				ShowErrorMsg(localGroups);
			else
			{
				wprintf(L"\nLocal Group (s) to which the user belongs:\n");

				for (index = 0; index < entriesRead; index++, groups++)
				{
					wprintf(L"--%s\n", groups->lgrui0_name);
				}
			}
			
		}
		break;

	default:
		ShowHelp();		
		break;
	}

	
	

	return 0;
}