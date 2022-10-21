#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <fstream>

#include <NosStdLib/FileManagement.hpp>

class Item
{
    enum Type { Function, Class, Namespace }; /* Item Type */
private:
    Type ItemType;
    std::vector<Item*> Children; /* Array Pointer of children */
    int ChildrenCount; /* Count of Children */
    Item* Parent; /* Pointer to parent */
public:
    Item(const Type& type, Item* parent)
    {
        ItemType = type;
        Parent = parent;
    }

    void AddChild(Item* child)
    {
        Children.push_back(child);
    }
};

/// <summary>
/// Parse a specified header to get the library structer
/// </summary>
/// <param name="filePath">- file Path to header file</param>
void ParseHeader(const std::wstring& filePath)
{
    std::wifstream ParserStream(filePath);
    std::wstring line;
    while (std::getline(ParserStream, line)) /* iterate over each line in the header file */
    {
        if (line.find(L"namespace") != std::string::npos)
        {
            wprintf(L"namespace\n");
        }
        else if (line.find(L"class") != std::string::npos)
        {
            wprintf(L"class\n");
        }
        //else if (line.find(L"") != std::string::npos)
        //{
        //
        //}
        //wprintf((line + L"\n").c_str());
    }
}

/// <summary>
/// Recure through the root directory and any sub directories
/// </summary>
/// <param name="root">- root directory to recure through</param>
void RecureThrouDir(const std::wstring& root)
{
    /* for each file/directory in dir */
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(root))
    {
        if (entry.is_directory()) /* if Directory, call itself to iterate through the dir */
        {
            wprintf(L"Directory\n");
            RecureThrouDir(entry.path());
        }
        else if (NosStdLib::FileManagement::GetFileExtension(entry.path()) == L"hpp" || NosStdLib::FileManagement::GetFileExtension(entry.path()) == L"h") /* if not a directory and is a header (hpp or h extention), parse it */
        {
            wprintf(L"Header File\n");
            ParseHeader(entry.path());
        }
        else /* if not a header or directory, do nothing */
        {
            wprintf(L"None Header File\n");
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