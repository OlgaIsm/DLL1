#include <iostream>
#include <windows.h>
#include <Winioctl.h>
#include <wchar.h>
#include <algorithm>
#include "DLLFS1.h"

using namespace std;

int main()
{
	string diskName; // Имя диска (C, D или другое)

	while (1)
	{
		cout << "Please specify volume name (C, D etc.)." << endl;
		diskName = cin.get();
		if (diskName.length() == 1)
		{
			break;
		}
		else {
			cout << "NO" << endl;
		}
	}

	string diskNameFormated = "\\\\.\\" + diskName + ":"; // Формируем путь

	string SysName = FindFSName(diskName);
	if (!fsIsSupported(SysName)) // Определяем поддерживается ли ФС нашей программой
	{
		cout << "Sorry, " << SysName << " is not supported yet!" << endl;
		cin.get();
		cin.get();
		exit(0);
	}

	NTFS_BootRecord _check; // Создаем объект структуры из DLL
	if (getFsInfo(diskNameFormated, &_check)) // Выводим информацию о ФС
	{
		PrintBootSectInfo(_check);
	}
	cout << "Press Enter to exit";
	cin.get();
	cin.get();
	exit(0);
}