
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <wchar.h>
#include <string>
#include <sstream>
#include <ctime>
#include "../OS11_HTAPI/OS11_HTAPI.h"
#pragma comment(lib,"..\\Debug\\OS11_HTAPI.lib")


void printStr(char* str)
{
	std::cout << "ERROR:\t";
	int i = 0;
	while (str[i])
	{
		std::cout << str[i];
		i++;
	}
	std::cout << std::endl;
}


int main(int argc, char* argv[])
{
	srand(time(0));
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
			fileName = wc;
			std::wstring s(directoryPath);
			s += std::wstring(fileName);
			filePath = s;
		}
		std::wcout << "filename: " << filePath << std::endl;
		HT::HTHANDLE* HT;
		HT = HT::OpenExist(filePath.c_str());
		if (HT == NULL)
			throw "Invalid handle";

		for (; ; ) {
			int numberKey = rand() % 50 + 1;
			std::string key;
			std::stringstream convert;
			convert << numberKey;
			key = convert.str();
			std::cout << key << ' ' << key.length() << std::endl;
			HT::Element* el;
			HT::Element* el1 = new HT::Element(key.c_str(), key.length() + 1, "0", 2);
			if ((el = HT::Get(HT, el1)) == NULL)
			{
				printStr(HT::GetLastErrorProg(HT));
			}
			else 
			{
				HT::print(el);
				if (!HT::Delete(HT, el1)) 
				{
					printStr(HT::GetLastErrorProg(HT));
				}
				else {
					std::cout << "DELETE SUCCESS" << std::endl;
				}
			}
			Sleep(1000);
		}

		int keyPress;
		std::cin >> keyPress;
		// HT::CloseExist(HT);
	}
	catch (const char* err) {
		std::cout << err << std::endl;
		return 1;
	}
	return 0;
}
