#include <iostream>
#include <Windows.h>
#include <peconv.h>
// Link with the DLL (header file should match the DLL's export declaration)
#include "PEFileProcessor.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Args: <path to the exe>" << std::endl;
        return 0;
    }
    LPCSTR pe_path = argv[1];

    // manually load the PE file using libPeConv:
    size_t v_size = 0;
#ifdef LOAD_FROM_PATH
    //if the PE is dropped on the disk, you can load it from the file:
    BYTE* my_pe = peconv::load_pe_executable(pe_path, v_size);
#else
    size_t bufsize = 0;
    BYTE* buffer = peconv::load_file(pe_path, bufsize);

    // if the file is NOT dropped on the disk, you can load it directly from a memory buffer:
    BYTE* my_pe = peconv::load_pe_executable(buffer, bufsize, v_size);
#endif
    if (!my_pe) {
        return -1;
    }

    // if the loaded PE needs to access resources, you may need to connect it to the PEB:
    peconv::set_main_module_in_peb((HMODULE)my_pe);

    // load delayed imports (if present):
    const ULONGLONG load_base = (ULONGLONG)my_pe;
    peconv::load_delayed_imports(my_pe, load_base);

    // if needed, you can run TLS callbacks before the Entry Point:
    peconv::run_tls_callbacks(my_pe, v_size);

    //calculate the Entry Point of the manually loaded module
    DWORD ep_rva = peconv::get_entry_point_rva(my_pe);
    if (!ep_rva) {
        return -2;
    }
    ULONG_PTR ep_va = ep_rva + (ULONG_PTR)my_pe;
    //assuming that the payload is an EXE file (not DLL) this will be the simplest prototype of the main:
    int (*new_main)() = (int(*)())ep_va;

    //call the Entry Point of the manually loaded PE:
    return new_main();
}