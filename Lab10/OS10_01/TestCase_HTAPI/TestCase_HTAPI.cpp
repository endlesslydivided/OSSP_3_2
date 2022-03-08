#include <iostream>
#pragma comment ( lib, "../Debug/OS10_HTAPI.lib" )

#include "OS10_HTAPI.h"
using namespace HT;

const wchar_t* FileName = (LPWSTR)L"D:\\ALEX\\STUDY\\6SEM_3COURSE\\Операционные системы и системное программирование\\Готовые лабораторные\\Lab10\\test.ht";

bool CREATE_TEST()
{
	HTHANDLE* hashTable = Create(100, 5, 20, 50, FileName);
	if (hashTable) 
	{
		Close(hashTable);
		return true;
	}
	return false;
}

bool INSERT_TEST()
{

	std::string key1 = "key1";
	std::string data1 = "data1";

	Element* element1 = new Element(key1.c_str(), key1.length(), data1.c_str(), data1.length());
	HTHANDLE* hashTable = Open(FileName);

	if (Insert(hashTable, element1))
	{
		Close(hashTable);
		return true;
	}
		return false;
}

bool GET_TEST()
{

	std::string key1 = "key1";
	std::string data1 = "data1";

	Element* element1 = new Element(key1.c_str(), key1.length(), data1.c_str(), data1.length());
	HTHANDLE* hashTable = Open(FileName);
	Element* finded1 = Get(hashTable, element1);

	if (!finded1) 
	{
		if (!Close(hashTable)) 
		{
			std::cout << "close error\n";
		}
		return false;
	}
	if (strcmp((char*)element1->key, (char*)finded1->key) == 0 && strcmp((char*)element1->payload, (char*)finded1->payload) == 0)
	{
		Close(hashTable);
		return true;
	}
	else
	{
		Close(hashTable);
		return false;
	}

}

int main()
{

	if (CREATE_TEST()) {
		std::cout << "Success create test" << std::endl;
	}
	else
	{
		std::cout << "Create test failed" << std::endl;
	}

	if (INSERT_TEST()) 
	{
		std::cout << "Success insert test" << std::endl;
	}
	else
	{
		std::cout << "Insert test failed" << std::endl;
	}

	if (GET_TEST()) 
	{
		std::cout << "Success get test" << std::endl;
	}
	else {
		std::cout << "Get test failed" << std::endl;
	}
}
