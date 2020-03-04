#pragma once

#ifdef DLLFS_EXPORTS
#define DLLFS_API __declspec(dllexport)
#else
#define DLLFS_API __declspec(dllimport)
#endif

#include <iostream>
#include <windows.h>
#include <Winioctl.h>
#include <wchar.h>
#include <algorithm>
#include <stdio.h>

using namespace std;

#pragma pack(1)
typedef struct
{
	BYTE jumpCode[3];
	BYTE oemID[8];
	WORD bytesPerSector;
	BYTE sectorPerCluster;
	WORD reservedSectors;
	BYTE always0[3];
	WORD notUsed;
	BYTE mediaDiscr;
	WORD always02;
	WORD sectorperTrack;
	WORD numOfHeaders;
	DWORD hiddenSectors;
	DWORD notUsed2;
	DWORD notused3;
	ULONGLONG totalSectors;
	ULONGLONG lcnForSMFT;
	ULONGLONG lcnForSMFTMirr;
	DWORD clusterPerFileRecordSegm;
	BYTE clusterPerIndexBuff;
	BYTE notUsed4[3];
	ULONGLONG volumeSerialNumber;
	DWORD checksum;
	BYTE bootstrap[426];
	WORD endMarker;
} NTFS_BootRecord;
#pragma pack()

extern "C++" DLLFS_API void PrintBootSectInfo(NTFS_BootRecord _check);
extern "C++" DLLFS_API string FindFSName(string diskName);
extern "C++" DLLFS_API bool fsIsSupported(string SysName);
extern "C++" DLLFS_API bool getFsInfo(string diskNameFormated, NTFS_BootRecord* _check);