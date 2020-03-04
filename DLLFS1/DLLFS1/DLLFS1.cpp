#include "pch.h"
#include <iostream>
#include <windows.h>
#include <Winioctl.h>
#include <wchar.h>
#include <algorithm>
#include "DLLFS1.h"
#include <string>

using namespace std;


void PrintBootSectInfo(NTFS_BootRecord _check)
{
	printf("NTFS Disk Information:\n");
	printf("Assembly Instruction to jump to Boot code: 0x%X\n", _check.jumpCode);
	printf("OEM Name: %s\n", _check.oemID);
	printf("Bytes per sector: %d\n", _check.bytesPerSector);
	printf("Sector per cluster: %d\n", 0x01 << _check.sectorPerCluster);
	printf("Reserved Sectors: %d\n", _check.reservedSectors);
	printf("Media Descriptor: 0x%X\n", _check.mediaDiscr);
	printf("Sectors Per Track: %d\n", _check.sectorperTrack);
	printf("Number Of Heads: %d\n", _check.numOfHeaders);
	printf("Hidden Sectors: %d\n", _check.hiddenSectors);
	printf("Total Sectors: %dll\n", _check.totalSectors);
	printf("Logical Cluster Number for the file $MFT: %dl\n", _check.lcnForSMFT);
	printf("Logical Cluster Number for the file $MFTMirr: %dl\n", _check.lcnForSMFTMirr);
	printf("Clusters Per File Record Segment: %d\n", _check.clusterPerFileRecordSegm);
	printf("Clusters Per Index Buffer: %d\n", _check.clusterPerIndexBuff);
	printf("Volume Serial Number: %dl\n", _check.volumeSerialNumber);
	printf("End of Sector Marker: 0x%X\n", _check.endMarker);
}

string FindFSName(string diskName)
{
	char NameBuffer[MAX_PATH];
	char _SysBuffer[MAX_PATH];
	DWORD VSNumber;
	DWORD MCLength;
	DWORD FileSF;

	string forVolumeInf = diskName + ":\\";


	if (GetVolumeInformationA(forVolumeInf.c_str(), NameBuffer, sizeof(NameBuffer),
		&VSNumber, &MCLength, &FileSF, _SysBuffer, sizeof(_SysBuffer)))
	{
		cout << "Detected file system is " << _SysBuffer << endl << endl;
		string SysName = _SysBuffer;

		return SysName;
	}

	return string("");
}

bool fsIsSupported(string SysName)
{
	if (SysName.find("NTFS") == string::npos)
	{
		return false;
	}
	return true;
}

bool getFsInfo(string diskNameFormated, NTFS_BootRecord * _check)
{
	BYTE bBootSector[512];
	memset(bBootSector, 0, 512);
	DWORD dwBytesRead(0);

	HANDLE hDisk = CreateFileA(diskNameFormated.c_str(),
		GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, 0, NULL);
	if (hDisk == INVALID_HANDLE_VALUE)
	{
		wprintf(L"CreateFile() failed! You'll rrobably need to run the app as administrator\n");
		wprintf(L" %u \n", GetLastError());
		if (CloseHandle(hDisk) != 0)
			wprintf(L"hVolume handle was closed successfully!\n");
		else
		{
			wprintf(L"Failed to close hVolume handle!\n");
		}
		return false;
	}

	if (!ReadFile(hDisk, bBootSector, 512, &dwBytesRead, NULL))
	{
		printf("Error in reading the disk\n");
		CloseHandle(hDisk);
		return false;
	}

	CloseHandle(hDisk);
	*_check = *reinterpret_cast<NTFS_BootRecord*>(bBootSector);
	return true;
}