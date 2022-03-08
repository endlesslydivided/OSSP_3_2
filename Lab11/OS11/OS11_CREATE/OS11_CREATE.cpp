// OS11_CREATE.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <wchar.h>
#include <string>
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
	int capacity = 1000;
	int snapshotIntervalSec = 2;
	int maxKeyLength = 10;
	int maxPayloadLength = 10;
	wchar_t* fileName;

	const wchar_t* directoryPath = L"D:\\ALEX\\STUDY\\6SEM_3COURSE\\Операционные системы и системное программирование\\Готовые лабораторные\\Lab11";
	std::wstring filePath(L"D:\\ALEX\\STUDY\\6SEM_3COURSE\\Операционные системы и системное программирование\\Готовые лабораторные\\Lab11\\test.ht");

	try 
	{
		if (argv[1] && argv[2] && argv[3] && argv[4] && argv[5]) 
		{
			capacity = std::stoi(argv[2]);
			snapshotIntervalSec = std::stoi(argv[3]);
			maxKeyLength = std::stoi(argv[4]);
			maxPayloadLength = std::stoi(argv[5]);

			const size_t cSize = strlen(argv[1]) + 1;
			wchar_t* wc = new wchar_t[cSize];
			mbstowcs(wc, argv[1], cSize);

			fileName = wc;
			std::wstring s(directoryPath);
			s += std::wstring(fileName);
			filePath = s;
		}
		else 
		{

		}
		HT::HTHANDLE* HT;
		HT = HT::Create(capacity, snapshotIntervalSec, maxKeyLength, maxPayloadLength, filePath.c_str());
		if (HT == NULL)
			throw "Invalid handle";
		HT::Close(HT);
		std::cout << "HT-Storage Created " << std::endl << std::flush;
		std::wcout <<"Filepath: " << filePath << "\n";
		std::cout << "\nSnapshotIntervalSec: " << snapshotIntervalSec << std::endl;
		std::cout << "Capacity: " << capacity << std::endl;
		std::cout << "MaxKeyLength: " << maxKeyLength << std::endl;
		std::cout << "MaxPayloadLength: " << maxPayloadLength << std::endl;
	}
	catch (const char* err) 
	{
		std::cout << err << std::endl;
		return 1;
	}
	return 0;
}
