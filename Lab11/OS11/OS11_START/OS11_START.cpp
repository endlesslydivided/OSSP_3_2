
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <wchar.h>
#include "../OS11_HTAPI/OS11_HTAPI.h"
#pragma comment(lib,"..\\Debug\\OS11_HTAPI.lib")

wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

int main(int argc, char* argv[])
{
	int capacity = 10;
	int snapshotIntervalSec = 2;
	int maxKeyLength = 10;
	int maxPayloadLength = 10;
	wchar_t* fileName;
	const wchar_t* directoryPath = L"D:\\ALEX\\STUDY\\6SEM_3COURSE\\Операционные системы и системное программирование\\Готовые лабораторные\\Lab11\\";
	std::wstring filePath(L"D:\\ALEX\\STUDY\\6SEM_3COURSE\\Операционные системы и системное программирование\\Готовые лабораторные\\Lab11\\test.ht");
	try
	{
		if (argv[1]) 
		{
			const size_t cSize = strlen(argv[1]) + 1;
			wchar_t* wc = new wchar_t[cSize];
			mbstowcs(wc, argv[1], cSize);
			std::wstring s(directoryPath);
			s += std::wstring(filePath);
			filePath = s;
		}
		HT::HTHANDLE* HT;
		HT = HT::Open(filePath.c_str());
		if (HT == NULL)
		{
			throw "Invalid handle";
		}
		std::cout << "HT-Storage Created " << std::endl;
		std::wcout <<"Filename: " << filePath << std::endl;
		std::cout << "SnapshotIntervalSec: " << HT->SecSnapshotInterval << std::endl;
		std::cout << "Capacity: " << HT->Capacity << std::endl;
		std::cout << "MaxKeyLength: " << HT->MaxKeyLength << std::endl;
		std::cout << "MaxPayloadLength: " << HT->MaxPayloadLength << std::endl;
		int key;
		std::cin >> key;
		HT::Close(HT);
	}
	catch (const char* err) 
	{
		std::cout << err << std::endl;
		return 1;
	}
	return 0;
}
