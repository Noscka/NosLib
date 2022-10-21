#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <filesystem>
#include <iostream>

void RecuireThrouDir(std::wstring root)
{
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(root))
    {
        std::wcout << L"Path: " << entry.path() << std::endl;
    }
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    std::wstring AbsoluteCurrentPath = std::filesystem::current_path();

    RecuireThrouDir(AbsoluteCurrentPath + LR"(\..\..\..\..\NosStdLib\NosStdLib)");

    wprintf(L"Press any button to continue"); getchar();
    return 0;
}