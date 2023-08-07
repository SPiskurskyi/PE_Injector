#pragma once

#ifdef PEFILEPROCESSOR_EXPORTS
#define PEFILEPROCESSOR_API __declspec(dllexport)
#else
#define PEFILEPROCESSOR_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // Define the type for the function pointer
    typedef bool (*ProcessPEFileFunc)(const char* peFilePath);

    // Exported function for processing the PE file
    PEFILEPROCESSOR_API bool ProcessPEFile(const char* peFilePath);

#ifdef __cplusplus
}
#endif
