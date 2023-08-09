#include <iostream>
#include <Windows.h>
#include <peconv.h>
// Link with the DLL (header file should match the DLL's export declaration)
#include "imp_list.h"

int main(int argc, char* argv[])
{

    libpe::imp_list("c:\\Users\\admin\\Desktop\\Projects\\Casino\\build\\Release\\casino.exe");
    
}