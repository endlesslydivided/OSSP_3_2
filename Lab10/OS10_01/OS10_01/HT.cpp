#include <Windows.h>
#include <filesystem>
#include <iostream>
#include "HT.h"
#include <tchar.h>

typedef unsigned char byte;
using namespace std;
#define FULL_SIZE (sizeof(Element) + hashTableHandle->MaxKeyLength + hashTableHandle->MaxPayloadLength)

namespace HT
{
	void SetErrorMessage(HTHANDLE* ht, string msg);
	DWORD WINAPI SnapWithInterval(LPVOID hashTableHandle);


#pragma region Constructors(HTHANDLE)

	HTHANDLE::HTHANDLE()
	{

	}

	HTHANDLE::HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const wchar_t FileName[512])
	{
		this->Capacity = Capacity;
		this->SecSnapshotInterval = SecSnapshotInterval;
		this->MaxKeyLength = MaxKeyLength;
		this->MaxPayloadLength = MaxPayloadLength;
		wcscpy_s(this->FileName, _countof(this->FileName), FileName);
	}
#pragma endregion

#pragma region Constructors(Element)

	Element::Element() 
	{
	
	}

	Element::Element(Element* oldelement, const void* newpayload, int  newpayloadlength)
	{
		this->payloadlength = newpayloadlength;
		this->payload = (void*)newpayload;
		this->keylength = oldelement->keylength;
		this->key = oldelement->key;
	}

	Element::Element(const void* key, int keylength, const void* payload, int payloadlength)
	{
		this->keylength = keylength;
		this->key = (void*)key;
		this->payloadlength = payloadlength;
		this->payload = (void*)payload;
	}

	Element::Element(const void* key, int keylength)
	{
		this->keylength = keylength;
		this->key = (void*)key;
	}

#pragma endregion

#pragma region CRUD
	HTHANDLE* Create(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const wchar_t FileName[512])
	{
		HTHANDLE* hashTable = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);
		HANDLE fileHandle = NULL;
		HANDLE mapHandle = NULL;
		HANDLE mutexHandle = NULL;
		DWORD hashTableSize = sizeof(HTHANDLE) + Capacity * (sizeof(Element) + MaxKeyLength + MaxPayloadLength);

		try
		{
			fileHandle = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (fileHandle == NULL)
			{
				throw "File creation error";
			}

			mapHandle = CreateFileMapping(fileHandle, NULL, PAGE_READWRITE, 0, hashTableSize, NULL);
			if (mapHandle == NULL)
			{
				throw "File Mapping creation error";
			}

			LPVOID lp = MapViewOfFile(mapHandle, FILE_MAP_ALL_ACCESS, 0, 0, hashTableSize);
			if (lp == NULL)
			{
				throw "File Mapping View error";
			}

			HTHANDLE* hashTableHandle = new(lp) HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);
			
			char* file = new char[512];
			wcstombs(file, FileName, wcslen(FileName));

			mutexHandle = CreateMutex(NULL, FALSE, (LPCWSTR)file);
			if (mutexHandle == NULL)
			{
				throw "Mutex creation error";
			}

			hashTableHandle->Addr = (byte*)lp + sizeof(HTHANDLE);
			hashTableHandle->File = fileHandle;
			hashTableHandle->FileMapping = mapHandle;
			hashTableHandle->Mutex = mutexHandle;

			hashTableHandle->SnapThread = CreateThread(NULL, 0, SnapWithInterval, hashTableHandle, 0, NULL);
			if (hashTableHandle->SnapThread == NULL)
			{
				throw "SnapWithInterval thread creation error";
			}

			return hashTableHandle;
		}
		catch (char* e) {
			DWORD er = _Post_equals_last_error_::GetLastError();
			cout << "\nerror: " << e << " " << er;
			CloseHandle(fileHandle);
			CloseHandle(mapHandle);
			CloseHandle(mutexHandle);
			return NULL;
		}
		catch (const char* e) {
			DWORD er = _Post_equals_last_error_::GetLastError();
			cout << "\nerror: " << e << " " << er;
			CloseHandle(fileHandle);
			CloseHandle(mapHandle);
			CloseHandle(mutexHandle);
			return NULL;
		}
	}

	HTHANDLE* Open(const wchar_t FileName[512])
	{
		HANDLE fileHandle = NULL;
		HANDLE mapHandle = NULL;
		HANDLE mutexHandle = NULL;

		try
		{
			fileHandle = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (fileHandle == NULL)
			{
				throw "File creation error";
			}

			mapHandle = CreateFileMapping(fileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
			if (mapHandle == NULL)
			{
				throw "File Mapping creation error";
			}

			LPVOID lp = MapViewOfFile(mapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if (lp == NULL)
			{
				throw "File Mapping View error";
			}

			HTHANDLE* hashTableHandle = (HTHANDLE*)lp;

			char* file = new char[512];
			wcstombs(file, FileName, wcslen(FileName));

			mutexHandle = CreateMutex(NULL, FALSE, (LPCWSTR)file);
			if (mutexHandle == NULL)
			{
				throw "Mutex creation error";
			}
			hashTableHandle->Mutex = mutexHandle;
			hashTableHandle->Addr = (byte*)lp + sizeof(HTHANDLE);
			hashTableHandle->File = fileHandle;
			hashTableHandle->FileMapping = mapHandle;

			byte* pointer = (byte*)hashTableHandle->Addr;
			for (int i = 0; i < hashTableHandle->CurrentCapacity; i++)
			{
				byte* currentPointer = pointer + (FULL_SIZE * i);
				Element* currentElement = (Element*)currentPointer;
				currentElement->key = currentPointer + sizeof(Element);
				currentElement->payload = currentPointer + sizeof(Element) + hashTableHandle->MaxKeyLength;
			}

			hashTableHandle->SnapThread = CreateThread(NULL, 0, SnapWithInterval, hashTableHandle, 0, NULL);
			if (hashTableHandle->SnapThread == NULL)
			{
				throw "SnapWithInterval thread creation error";
			}

			return hashTableHandle;
		}
		catch (char* e) {
			DWORD er = _Post_equals_last_error_::GetLastError();
			cout << "\nerror: " << e << " " << er;
			CloseHandle(fileHandle);
			CloseHandle(mapHandle);
			CloseHandle(mutexHandle);
			return NULL;
		}
		catch (const char* e) {
			DWORD er = _Post_equals_last_error_::GetLastError();
			cout << "\nerror: " << e << " " << er;
			CloseHandle(fileHandle);
			CloseHandle(mapHandle);
			CloseHandle(mutexHandle);
			return NULL;
		}
	}

	BOOL Insert(HTHANDLE* hashTableHandle, Element* element)
	{
		WaitForSingleObject(hashTableHandle->Mutex, INFINITE);
		if (hashTableHandle->CurrentCapacity == hashTableHandle->Capacity)
		{
			ReleaseMutex(hashTableHandle->Mutex);
			strcpy_s(hashTableHandle->LastErrorMessage,20, "Max capacity error");
			return false;
		}

		if (element->keylength > hashTableHandle->MaxKeyLength)
		{
			ReleaseMutex(hashTableHandle->Mutex);
			strcpy_s(hashTableHandle->LastErrorMessage, 21, "Max keylength error");
			return false;
		}

		if (element->payloadlength > hashTableHandle->MaxPayloadLength)
		{
			ReleaseMutex(hashTableHandle->Mutex);
			strcpy_s(hashTableHandle->LastErrorMessage, 25, "Max payloadlength error");
			return false;
		}

		byte* newElementAddress = (byte*)hashTableHandle->Addr + (FULL_SIZE * hashTableHandle->CurrentCapacity);
		Element* ptr = (Element*)newElementAddress;

		ptr->key = newElementAddress + sizeof(Element);
		ptr->payload = newElementAddress + sizeof(Element) + hashTableHandle->MaxKeyLength;
		ptr->keylength = element->keylength;
		ptr->payloadlength = element->payloadlength;
		CopyMemory(ptr->key,element->key, element->keylength);
		CopyMemory(ptr->payload, element->payload, element->payloadlength);

		hashTableHandle->CurrentCapacity++;

		ReleaseMutex(hashTableHandle->Mutex);

		return true;
	}

	Element* Get(const HTHANDLE* hashTableHandle, const Element* element)
	{
		WaitForSingleObject(hashTableHandle->Mutex, INFINITE);

		byte* pointer = (byte*)hashTableHandle->Addr;

		for (int i = 0; i < hashTableHandle->CurrentCapacity; i++)
		{
			byte* currentPointer = pointer + (FULL_SIZE * i);
			Element* currentElement = (Element*)currentPointer;
			if (currentElement->keylength == element->keylength &&
				strcmp((char*)currentElement->key, (char*)element->key) == 0)
			{
				ReleaseMutex(hashTableHandle->Mutex);
				return currentElement;
			}
		}

		ReleaseMutex(hashTableHandle->Mutex);
		SetErrorMessage((HTHANDLE*)hashTableHandle, "Element not found error");
		return NULL;
	}

	BOOL Update(const HTHANDLE* hashTableHandle, const Element* oldElement, const void* newPayload, int newPayloadLength)
	{
		WaitForSingleObject(hashTableHandle->Mutex, INFINITE);

		Element* element = Get(hashTableHandle, oldElement);
		if (element == NULL)
		{
			ReleaseMutex(hashTableHandle->Mutex);
			SetErrorMessage((HTHANDLE*)hashTableHandle, "Element not found error");
			return false;
		}

		element->payloadlength = newPayloadLength;
		CopyMemory(element->payload, newPayload, newPayloadLength);

		ReleaseMutex(hashTableHandle->Mutex);
		return true;
	}

	BOOL Delete(HTHANDLE* hashTableHandle, Element* element)
	{
		WaitForSingleObject(hashTableHandle->Mutex, INFINITE);

		Element* deletedElement = Get(hashTableHandle, element);
		if (deletedElement == NULL)
		{
			ReleaseMutex(hashTableHandle->Mutex);
			SetErrorMessage((HTHANDLE*)hashTableHandle, "Element not found error");
			return false;
		}

		byte* deletedElementPtr = (byte*)deletedElement;
		byte* firstElementPtr = (byte*)hashTableHandle->Addr;
		int deletedElementIndex = (deletedElementPtr - firstElementPtr) / FULL_SIZE;

		ZeroMemory(deletedElementPtr, FULL_SIZE);

		byte* currentElementPtr = deletedElementPtr;
		for (int i = deletedElementIndex; i < hashTableHandle->CurrentCapacity; i++)
		{
			if (i == hashTableHandle->CurrentCapacity - 1)
			{
				ZeroMemory(currentElementPtr, FULL_SIZE);
				hashTableHandle->CurrentCapacity--;
				ReleaseMutex(hashTableHandle->Mutex);
				return true;
			}

			byte* nextElementPtr = currentElementPtr + FULL_SIZE;
			CopyMemory(currentElementPtr, nextElementPtr, FULL_SIZE);
			currentElementPtr = nextElementPtr;
		}
		return false;
	}

	BOOL Close(const HTHANDLE* hashTableHandle)
	{
		WaitForSingleObject(hashTableHandle->Mutex, INFINITE);
		TerminateThread(hashTableHandle->SnapThread, 0);
		if (CloseHandle(hashTableHandle->SnapThread) &&
			CloseHandle(hashTableHandle->FileMapping) &&
			CloseHandle(hashTableHandle->File) &&
			ReleaseMutex(hashTableHandle->Mutex) &&
			CloseHandle(hashTableHandle->Mutex) &&
			UnmapViewOfFile(hashTableHandle)
			)
		{
			return true;
		}

		return false;
	}

	BOOL Snap(HTHANDLE* hashTableHandle)
	{
		WaitForSingleObject(hashTableHandle->Mutex, INFINITE);

		DWORD htSize = sizeof(HTHANDLE) + hashTableHandle->CurrentCapacity * FULL_SIZE;
		if (!FlushViewOfFile(hashTableHandle, htSize))
		{
			SetErrorMessage(hashTableHandle, "FlushViewOfFile error");
			ReleaseMutex(hashTableHandle->Mutex);
			return false;
		}

		time(&hashTableHandle->lastsnaptime);
		char* buffer = new char[250];
		ctime_s(buffer, 250, &hashTableHandle->lastsnaptime);

		tm* time = new tm();
		localtime_s(time, &hashTableHandle->lastsnaptime);
		strftime(buffer, 32, "%d.%m.%Y-%H:%M:%S", time);
		delete time;
		wstring snapFileName = wstring(hashTableHandle->FileName);

		const size_t cSize = strlen(buffer) + 1;
		wchar_t* wc = new wchar_t[cSize];
		mbstowcs(wc, buffer, cSize);

		snapFileName.replace(snapFileName.find('.'), 0, wc);
		delete[] buffer;

		HANDLE fileHandle = CreateFile((LPCWSTR)snapFileName.c_str(), GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			SetErrorMessage(hashTableHandle, "Snap file creation error");
			ReleaseMutex(hashTableHandle->Mutex);
			return false;
		}

		WriteFile(fileHandle, hashTableHandle, htSize, NULL, NULL);

		ReleaseMutex(hashTableHandle->Mutex);
		return true;
	}

	char* GetLastError(HTHANDLE* ht)
	{
		return ht->LastErrorMessage;
	}

	void print(const Element* element)
	{
		printf("\nElement key: %s, Value: %s", (char*)element->key, (char*)element->payload);
	}

#pragma endregion

	void SetErrorMessage(HTHANDLE* ht, string msg)
	{
		strcpy_s(ht->LastErrorMessage, msg.length() + 1, msg.c_str());
	}


	DWORD WINAPI SnapWithInterval(LPVOID hashTableHandle)
	{
		HTHANDLE* hashTable = (HTHANDLE*)hashTableHandle;
		while (1)
		{
			Sleep(hashTable->SecSnapshotInterval * 1000);
			Snap(hashTable);
		}
	}


}