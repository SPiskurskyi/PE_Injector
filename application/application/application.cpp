// ConsoleApp.cpp

#include <iostream>
#include <Windows.h>

// Link with the DLL (header file should match the DLL's export declaration)
#include "PEFileProcessor.h"

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 2)
    {
        std::wcerr << L"Usage: " << argv[0] << L" <PE_FILE_PATH>\n";
        return 1;
    }

    const wchar_t* peFilePath = argv[1];

    // Convert the wide-character string to a narrow-character string
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, peFilePath, -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0)
    {
        std::wcerr << L"Error converting the file path to a narrow-character string\n";
        return 1;
    }

    std::string narrowPath(bufferSize, '\0');
    if (WideCharToMultiByte(CP_UTF8, 0, peFilePath, -1, &narrowPath[0], bufferSize, nullptr, nullptr) == 0)
    {
        std::wcerr << L"Error converting the file path to a narrow-character string\n";
        return 1;
    }

    HMODULE hDll = LoadLibrary(L"libpe.dll");
    if (!hDll)
    {
        std::wcerr << L"Error loading PEFileProcessor.dll\n";
        return 1;
    }

    // Get the address of the exported function from the DLL
    ProcessPEFileFunc processPEFile = reinterpret_cast<ProcessPEFileFunc>(
        GetProcAddress(hDll, "ProcessPEFile"));

    if (!processPEFile)
    {
        std::wcerr << L"Error finding the ProcessPEFile function in the DLL\n";
        FreeLibrary(hDll);
        return 1;
    }

    // Call the exported function from the DLL with the narrow-character string
    if (!processPEFile(narrowPath.c_str()))
    {
        std::wcerr << L"Error processing the PE file\n";
        FreeLibrary(hDll);
        return 1;
    }

    FreeLibrary(hDll);
    return 0;
}