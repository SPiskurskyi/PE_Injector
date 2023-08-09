#include <iostream>
#include <Windows.h>
#include <peconv.h>

#include "implementation.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Args: <path to the exe>" << std::endl;
        return 0;
    }
    libpe::implementation(argv[1]);
}