#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <NosStdLib/Global.hpp>

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    std::wcout << (NosStdLib::Global::String::IsNumber(L"+123", true) ? "True" : "False") << std::endl;
    NosStdLib::Global::Console::ClearLine(0);

    wprintf(L"Press any button to continue"); getchar();
    return 0;
}