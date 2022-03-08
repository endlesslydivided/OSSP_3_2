#include <iostream>
#include <Windows.h>
#include "OS10_HTAPI.h"
#pragma comment ( lib, "OS10_HTAPI.lib" )

using namespace HT;


int main()
{
    const wchar_t* FileName1 = (LPWSTR)L"D:\\ALEX\\STUDY\\6SEM_3COURSE\\Операционные системы и системное программирование\\Готовые лабораторные\\Lab10\\HTspace2.ht";
    const wchar_t* FileName2 = (LPWSTR)L"D:\\ALEX\\STUDY\\6SEM_3COURSE\\Операционные системы и системное программирование\\Готовые лабораторные\\Lab10\\HTspace3.ht";


    std::string key1 = "key1";
    std::string data1 = "data1";
    std::string key2 = "key2";
    std::string data2 = "data2";

    Element* element1 = new Element(key1.c_str(), key1.length(), data1.c_str(), data1.length());
    Element* element2 = new Element(key2.c_str(), key2.length(), data2.c_str(), data2.length());

    HTHANDLE* hashTable1 = Create(20, 5, 20, 20, FileName1);
    HTHANDLE* hashTable2 = Create(20, 5, 20, 20, FileName2);

    if (!Insert(hashTable1, element1)) {
        std::cout << hashTable1->LastErrorMessage;
    }
    if (!Insert(hashTable2, element2)) {
        std::cout << hashTable2->LastErrorMessage;
    }

    if (Get(hashTable1, element1) != NULL)
    {
        std::cout << "Element 1 in hashTable1" << std::endl;
    }
    if (Get(hashTable1, element2) != NULL)
    {
        std::cout << "Element 2 in hashTable1" << std::endl;
    }

    if (Get(hashTable1, element1) != NULL)
    {
        std::cout << "Element 1 in hashTable2" << std::endl;
    }
    if (Get(hashTable1, element2) != NULL)
    {
        std::cout << "Element 2 in hashTable2" << std::endl;
    }

    Close(hashTable1);
    Close(hashTable2);
}

