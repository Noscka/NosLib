#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <fstream>

#include <NosStdLib/FileManagement.hpp>

class Item
{
public:
    enum Type { Function, Class, Namespace }; /* Item Type */
private:
    /* Self Properties */
    Type ItemType;
    std::wstring Name; /* Item name */

    /* Item Relation ships */
    std::vector<Item*> Children; /* Array Pointer of children */
    int ChildrenCount; /* Count of Children */
    Item* Parent; /* Pointer to parent */
public:
    /* Tracking Properties */
    int BracketCloseCount;

    Item(Type type, std::wstring name, Item* parent)
    {
        ItemType = type;
        Name = name;
        Parent = parent;
    }

    Item* GetParent()
    {
        return Parent;
    }

    std::wstring GetName()
    {
        return Name;
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
    Item* currentItem = new Item(Item::Type::Namespace, L"Root", nullptr);
    while (std::getline(ParserStream, line)) /* iterate over each line in the header file */
    {
        if (line.find(L"namespace") != std::string::npos)
        {
            wprintf(L"namespace\n");

            Item namespaceItem(Item::Type::Namespace, L"namespace", currentItem);

            if (currentItem != nullptr)
                currentItem->AddChild(&namespaceItem);

            currentItem = &namespaceItem;
        }
        else if (line.find(L"class") != std::string::npos)
        {
            wprintf(L"class\n");

            Item classItem(Item::Type::Class, L"class", currentItem);

            if (currentItem != nullptr)
                currentItem->AddChild(&classItem);

            currentItem = &classItem;
        }
        //else if (line.find(L"") != std::string::npos)
        //{
        //
        //}
        //wprintf((line + L"\n").c_str());

        if (line.find(L"{") != std::string::npos)
            currentItem->BracketCloseCount++;
        else if (line.find(L"}") != std::string::npos)
        {
            currentItem->BracketCloseCount--;
            if (currentItem->BracketCloseCount == 0)
                currentItem = currentItem->GetParent();
        }
    }

    std::wcout << currentItem->GetName() << std::endl;
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

    //RecureThrouDir(AbsoluteCurrentPath + LR"(\..\..\..\..\NosStdLib\NosStdLib)");
    RecureThrouDir(LR"(C:\Users\Adam\Documents\Programing Projects\C++\NosStdLib\Build\Library Structure Parser\x64\Release\)");

    wprintf(L"Press any button to continue"); getchar();
    return 0;
}