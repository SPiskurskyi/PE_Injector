#pragma once

#ifdef LIBPE_EXPORTS
#define LIBPE_API __declspec(dllexport)
#else
#define LIBPE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

namespace libpe {

    int LIBPE_API implementation(LPCTSTR path);
    void LIBPE_API printer();
}; 

#ifdef __cplusplus
}
#endif
