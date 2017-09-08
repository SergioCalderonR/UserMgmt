#include "Internal.h"

#pragma comment(lib, "Netapi32.lib")

int wmain(int argc, WCHAR *argv[])
{
	//Functions
	NET_API_STATUS localGroups, globalGroups, enumUsers;

	//NetUserEnum
	DWORD infoLevel = 1;	//USER_INFO_1 structure
	DWORD filter;
	PUSER_INFO_1 enumGroup;
	DWORD enumEntriesRead;
	DWORD enumTotalEntries;

	//NetUserGetLocalGroups
	DWORD level = 0;	//array of LOCALGROUP_USER_INFO_0 structures
	DWORD flags = LG_INCLUDE_INDIRECT;
	PLOCALGROUP_USERS_INFO_0 groups;
	DWORD prefMaxLen=MAX_PREFERRED_LENGTH;
	DWORD entriesRead;
	DWORD totalEntries;

	//NetUserGetGroups
	DWORD globalLevel = 0;	//array of GROUP_USERS_INFO_0 structures
	PGROUP_USERS_INFO_0 domainGroups;
	DWORD globalEntriesRead;
	DWORD globalTotalEntries;


	DWORD index;

	if (argc == 1)
	{
		ShowHelp();
	}

	switch (argc)
	{
	//UserMgmt.exe -all UserName
	case 3:
		
		if (_wcsicmp(argv[1], L"-all") == 0)
		{
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

			//Retrieves global groups
			globalGroups = NetUserGetGroups(NULL, argv[2], globalLevel, (LPBYTE*)&domainGroups,
											prefMaxLen, &globalEntriesRead, &globalTotalEntries);

			if (globalGroups != NERR_Success)
				ShowErrorMsg(globalGroups);
			else
			{
				wprintf(L"\nGlobal Group (s) to which the user belongs:\n");

				for (index = 0; index < globalEntriesRead; index++, domainGroups++)
				{
					wprintf(L"--%s\n", domainGroups->grui0_name);
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