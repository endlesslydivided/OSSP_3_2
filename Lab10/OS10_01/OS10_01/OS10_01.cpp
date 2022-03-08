#include <iostream>
#include <Windows.h>
#include "HT.h"

using namespace std;
using namespace HT;

int main()
{
	HTHANDLE* hashTable = nullptr;

	try
	{
		const wchar_t* FileName = (LPWSTR)L"D:\\ALEX\\STUDY\\6SEM_3COURSE\\Операционные системы и системное программирование\\Готовые лабораторные\\Lab10\\HTspace1.ht";

		string key1 = "key1";
		string data1 = "data1";
		string dataReplace = "replace";

		string key2 = "key2";
		string data2 = "data2";

		/*HTHANDLE* hashTable = Create(1000, 3, 10, 50, FileName);
		if (!hashTable)
		{
			throw "Create: error";
		}*/
		HTHANDLE* hashTable = Open(FileName);
		if (!hashTable)
		{
			throw "open: error";
		}

		Element* element1 = new Element(key1.c_str(), key1.length(), data1.c_str(), data1.length());
		Element* element2 = new Element(key2.c_str(), key2.length(), data2.c_str(), data2.length());

		if (!Insert(hashTable, element1)) 
		{
			cout << hashTable->LastErrorMessage;
		}

		if (!Insert(hashTable, element2)) 
		{
			cout << hashTable->LastErrorMessage;
		}

		Element* element1Get = Get(hashTable, element1);

		if (element1Get != NULL)
		{
			print(element1Get);
		}

		Element* element2Get = Get(hashTable, element2);
		if (element2Get != NULL)
		{
			print(element2Get);
		}

		Delete(hashTable, element2Get);
		if (Get(hashTable, element2Get) == NULL)
		{
			cout << "\nDeleted: success\n" << endl;
		}
		Update(hashTable, element1Get, dataReplace.c_str(), dataReplace.length());

		Element* updated1 = Get(hashTable, element1Get);

		print(updated1);
		Snap(hashTable);

		Sleep(15000);

		Close(hashTable);
	}
	catch (char* msg)
	{
		cout << "\n" << msg << "\n";
		if (hashTable != nullptr)
		{
			cout << HT::GetLastError(hashTable);
		}
	}
	catch (const char* msg)
	{
		cout << "\n"<< msg << "\n";
		if (hashTable != nullptr)
		{
			cout << HT::GetLastError(hashTable);
		}
	}
}

