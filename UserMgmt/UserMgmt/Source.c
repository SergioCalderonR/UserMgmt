#include "Internal.h"

#pragma comment(lib, "Netapi32.lib")

int wmain(int argc, WCHAR *argv[])
{
	//Functions
	NET_API_STATUS localGroups, globalGroups, enumUsers;

	//NetUserEnum
	DWORD infoLevel = 1;	//USER_INFO_1 structure
	DWORD filter=FILTER_NORMAL_ACCOUNT | FILTER_INTERDOMAIN_TRUST_ACCOUNT;
	LPUSER_INFO_1 enumGroup=NULL;
	LPUSER_INFO_1 tmpEnumGroup;
	DWORD enumEntriesRead;
	DWORD enumTotalEntries;
	DWORD resumeHandle = 0;

	//NetUserGetLocalGroups
	DWORD level = 0;	//array of LOCALGROUP_USER_INFO_0 structures
	DWORD flags = LG_INCLUDE_INDIRECT;
	PLOCALGROUP_USERS_INFO_0 groups=NULL;
	PLOCALGROUP_USERS_INFO_0 tmpGroups;
	DWORD prefMaxLen=MAX_PREFERRED_LENGTH;
	DWORD entriesRead;
	DWORD totalEntries;

	//NetUserGetGroups
	DWORD globalLevel = 0;	//array of GROUP_USERS_INFO_0 structures
	PGROUP_USERS_INFO_0 domainGroups=NULL;
	PGROUP_USERS_INFO_0 tmpDomainGroups;
	DWORD globalEntriesRead;
	DWORD globalTotalEntries;

	NET_API_STATUS freeEnum;


	DWORD index;

	if (argc == 1)
	{
		ShowHelp();
	}

	switch (argc)
	{
	//UserMgmt.exe -enum
	case 2:
		if (_wcsicmp(argv[1], L"-enum") == 0)
		{
			enumUsers = NetUserEnum(NULL, infoLevel, filter, (LPBYTE*)&enumGroup,
				prefMaxLen, &enumEntriesRead, &enumTotalEntries, &resumeHandle);

			tmpEnumGroup = enumGroup;

			if (enumUsers != NERR_Success)
				ShowErrorMsg(enumUsers);
			else
			{

				wprintf(L"\nAll users on this PC:\n");
				for (index = 0; index < enumEntriesRead; index++, enumGroup++)
				{
					
					switch (enumGroup->usri1_priv)
					{
					case USER_PRIV_GUEST:
						wprintf(L"--%s (Guest User)\n", enumGroup->usri1_name);
						break;
					case USER_PRIV_USER:
						wprintf(L"--%s (Standard User)\n", enumGroup->usri1_name);
						break;
					case USER_PRIV_ADMIN:
						wprintf(L"--%s (Admin User)\n", enumGroup->usri1_name);
						break;
					}
				}

			}

			freeEnum = NetApiBufferFree(tmpEnumGroup);

			if (freeEnum != NERR_Success)
			{
				ShowErrorMsg(freeEnum);
				enumGroup = NULL;
			}



		}
		else
			ShowHelp();

		break;

	//UserMgmt.exe -all UserName
	case 3:
		
		if (_wcsicmp(argv[1], L"-all") == 0)
		{
			//Retrieves local groups
			localGroups = NetUserGetLocalGroups(NULL, argv[2], level, flags, (LPBYTE*)&groups,
				prefMaxLen, &entriesRead, &totalEntries);

			tmpGroups = groups;

			if (localGroups != NERR_Success)
			{
				wprintf(L"\nLocal Group (s) to which the user belongs:\n");
				ShowErrorMsg(localGroups);
			}
				
			else
			{
				wprintf(L"\nLocal Group (s) to which the user belongs:\n");

				for (index = 0; index < entriesRead; index++, groups++)
				{
					wprintf(L"--%s\n", groups->lgrui0_name);
				}
			}

			freeEnum = NetApiBufferFree(tmpGroups);

			if (freeEnum != NERR_Success)
			{
				ShowErrorMsg(freeEnum);
				groups = NULL;
			}

			//Retrieves global groups
			globalGroups = NetUserGetGroups(NULL, argv[2], globalLevel, (LPBYTE*)&domainGroups,
											prefMaxLen, &globalEntriesRead, &globalTotalEntries);

			tmpDomainGroups = domainGroups;

			if (globalGroups != NERR_Success)
			{
				wprintf(L"\nGlobal Group (s) to which the user belongs:\n");
				ShowErrorMsg(globalGroups);
			}
				
			else
			{
				wprintf(L"\nGlobal Group (s) to which the user belongs:\n");

				for (index = 0; index < globalEntriesRead; index++, domainGroups++)
				{
					wprintf(L"--%s\n", domainGroups->grui0_name);
				}
			}

			freeEnum = NetApiBufferFree(tmpDomainGroups);

			if (freeEnum != NERR_Success)
			{
				ShowErrorMsg(freeEnum);
				domainGroups = NULL;
			}


			
		}
		break;

	default:
		ShowHelp();		
		break;
	}	
	

	return 0;
}