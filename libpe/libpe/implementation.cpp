#include "pch.h"
#include "implementation.h"

using namespace peconv;

class ListImportThunks : public ImportThunksCallback
{
public:
    ListImportThunks(BYTE* _modulePtr, size_t _moduleSize)
        : ImportThunksCallback(_modulePtr, _moduleSize)
    {
    }

    virtual bool processThunks(LPSTR lib_name, ULONG_PTR origFirstThunkPtr, ULONG_PTR firstThunkPtr)
    {
        if (this->is64b) {
            IMAGE_THUNK_DATA64* desc = reinterpret_cast<IMAGE_THUNK_DATA64*>(origFirstThunkPtr);
            ULONGLONG* call_via = reinterpret_cast<ULONGLONG*>(firstThunkPtr);
            return processThunks_tpl<ULONGLONG, IMAGE_THUNK_DATA64>(lib_name, desc, call_via, IMAGE_ORDINAL_FLAG64);
        }
        IMAGE_THUNK_DATA32* desc = reinterpret_cast<IMAGE_THUNK_DATA32*>(origFirstThunkPtr);
        DWORD* call_via = reinterpret_cast<DWORD*>(firstThunkPtr);
        return processThunks_tpl<DWORD, IMAGE_THUNK_DATA32>(lib_name, desc, call_via, IMAGE_ORDINAL_FLAG32);
    }

protected:
    template <typename T_FIELD, typename T_IMAGE_THUNK_DATA>
    bool processThunks_tpl(LPSTR lib_name, T_IMAGE_THUNK_DATA* desc, T_FIELD* call_via, T_FIELD ordinal_flag)
    {

        ULONG_PTR call_via_rva = (ULONG_PTR)call_via - (ULONG_PTR)this->modulePtr;
        std::cout << "lib : " << lib_name << " : via RVA:  " << std::hex << call_via_rva << " : ";
        bool is_by_ord = (desc->u1.Ordinal & ordinal_flag) != 0;
        if (is_by_ord) {
            T_FIELD raw_ordinal = desc->u1.Ordinal & (~ordinal_flag);
            std::cout << "ord: " << raw_ordinal << std::endl;
        }
        else {

            PIMAGE_IMPORT_BY_NAME by_name = (PIMAGE_IMPORT_BY_NAME)((ULONGLONG)modulePtr + desc->u1.AddressOfData);
            LPSTR func_name = reinterpret_cast<LPSTR>(by_name->Name);
            std::cout << "name: " << func_name << std::endl;
        }
        return true;
    }
};

bool list_imports(IN BYTE* modulePtr, IN size_t moduleSize)
{
    if (moduleSize == 0) {
        moduleSize = peconv::get_image_size((const BYTE*)modulePtr);
    }
    if (moduleSize == 0) return false;

    ListImportThunks callback(modulePtr, moduleSize);
    return peconv::process_import_table(modulePtr, moduleSize, &callback);
}


int libpe::implementation(LPCTSTR my_path)
{
    size_t v_size = 0;
    std::tcout << TEXT("Module: ") << my_path << ("\n");
    BYTE* loaded_pe = load_pe_module(my_path, v_size, true, true);
    if (!loaded_pe) {
        std::cout << "Loading failed!\n";
        return -1;
    }

    bool is_ok { false };

    peconv::default_func_resolver* exp_res = new peconv::default_func_resolver();
    exp_res->resolve_func("libpe.dll","printer");

    load_imports(loaded_pe, (peconv::t_function_resolver*)exp_res);
   
    is_ok = list_imports(loaded_pe, v_size);
    
    t_pe_dump_mode mode = PE_DUMP_AUTO;
    const ULONGLONG load_base = (ULONGLONG)loaded_pe;
    is_ok = dump_pe("modified_file.exe", loaded_pe, v_size, load_base , mode);

    peconv::free_pe_buffer(loaded_pe);
    return is_ok ? 0 : 1;
}

void LIBPE_API libpe::printer()
{
    for (int i = 0; i < 99999; i++)
    {
        std::cout << "terminate " << std::endl;
    }
}

