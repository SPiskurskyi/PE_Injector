#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    // Define the type for the function pointer
    typedef bool (*ProcessPEFileFunc)(const char* peFilePath);

    // Exported function for processing the PE file
    bool ProcessPEFile(const char* peFilePath);

#ifdef __cplusplus
}
#endif
