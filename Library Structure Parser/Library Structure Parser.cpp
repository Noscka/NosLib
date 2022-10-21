#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <filesystem>
#include <iostream>

#include <NosStdLib/FileManagement.hpp>

void ParseHeader()
{

}

void RecureThrouDir(std::wstring root)
{
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(root))
    {
        if (entry.is_directory())
        {
            wprintf(L"Directory");
            RecureThrouDir(entry.path());
        }
        else if (NosStdLib::FileManagement::GetFileExtension(entry.path()) == L"hpp" || NosStdLib::FileManagement::GetFileExtension(entry.path()) == L"h")
        {
            wprintf(L"Header File");
            ParseHeader();
        }
        else
        {
            wprintf(L"None Header File");
        }
    }
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    std::wstring AbsoluteCurrentPath = std::filesystem::current_path();

    RecureThrouDir(AbsoluteCurrentPath + LR"(\..\..\..\..\NosStdLib\NosStdLib)");

    wprintf(L"Press any button to continue"); getchar();
    return 0;
}