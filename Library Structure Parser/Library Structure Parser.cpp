#include <NosStdLib/FileManagement.hpp>
#include <NosStdLib/DynamicArray.hpp>
#include <NosStdLib/String.hpp>
#include <NosStdLib/Console.hpp>

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <regex>
#include <cwctype>

class Item
{
public:
    enum Type { Function, Class, Namespace }; /* Item Type */
private:
    /* Self Properties */
    Type ItemType;
    std::wstring Name; /* Item name */

    /* Item Relation ships */
    NosStdLib::DynamicArray<Item*> Children; /* Array Pointer of children */
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
        Children.Append(child);
    }
};

struct FunctionDetectionTracking
{
    enum StageEnum : UINT8
    {
        DatatypeStage = 0,
        NameStage = 1,
        ArgumentsStage = 2,
    };

    std::wstring Datatype = L"",
                 Name = L"",
                 Arguments = L"";

    bool StartedTracking = false;

    StageEnum CurrentStage = DatatypeStage;
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
        FunctionDetectionTracking functionTracking{};

        for (int i = 0; i <= line.length(); i++) /* check for comments, namespaces and classes */
        {
            switch (line[i])
            {
            case L'/': /* if comment */
                if (line[i + 1] == L'/') /* if its an '//' comment then ignore the whole line */
                {
                    wprintf(L"// comment\n");
                    goto continueGoto; /* Finish/Continue the outer loop */
                }
                else if (line[i + 1] == L'*')
                {
                    wprintf(L"comment: /*");

                    for (i+=2; i <= line.length(); i++)  /* second loop for finding/ignoring everything inside the comment */
                    {
                        if (line[i] == L'*' && line[i + 1] == L'/') /* looking for the ending part */
                        {
                            wprintf(L"*/\n");
                            break;
                        }

                        wprintf(L"%c", line[i]);
                    }

                    wprintf(L"/**/ comment\n");
                }
                break;

            case L'n': /* n for namespace */
                if (line.substr(i, 10) == L"namespace ") /* if is an class */
                {
                    std::wstring namespaceName;

                    for (i+=10; i <= line.length(); i++)
                    {
                        if (std::iswspace(line[i]))
                        {
                            break;
                        }

                        namespaceName += line[i];
                    }

                     
                    wprintf(std::format(L"namespace {}", namespaceName).c_str());
                    wprintf(L"\n"); /* TODO: Fix wprintf or c_str deleting the newline (\n) character */
                }
                break;

            case L'c': /* c for class */
                if (line.substr(i, 6) == L"class ") /* if is an class */
                {
                    std::wstring className;

                    for (i += 6; i <= line.length(); i++)
                    {
                        if (std::iswspace(line[i]))
                        {
                            break;
                        }

                        className += line[i];
                    }

                    wprintf(std::format(L"class {}", className).c_str());
                    wprintf(L"\n"); /* TODO: Fix wprintf or c_str deleting the newline (\n) character */
                }

                break;

            default:
                if (!std::iswspace(line[i]))
                {
                    functionTracking.StartedTracking = true;

                }
                break;
            }
        }

    continueGoto:;
        continue;

        if (size_t point = line.find(L"namespace") != std::string::npos)
        {
            wprintf((L"namespace " + NosStdLib::String::FindNthWord<wchar_t>(line, point, 2, L' ') + L"\n").c_str());

            Item *namespaceItem = new Item(Item::Type::Namespace, NosStdLib::String::FindNthWord<wchar_t>(line, point, 2, L' '), currentItem);

            if (currentItem != nullptr)
                currentItem->AddChild(namespaceItem);

            currentItem = namespaceItem;
        }
        else if (size_t point = line.find(L"class") != std::string::npos)
        {
            wprintf((L"class " + NosStdLib::String::FindNthWord<wchar_t>(line, point, 2, L' ') + L"\n").c_str());

            Item *classItem = new Item(Item::Type::Class, NosStdLib::String::FindNthWord<wchar_t>(line, point, 2, L' '), currentItem);

            if (currentItem != nullptr)
                currentItem->AddChild(classItem);

            currentItem = classItem;
        }
        else
        {
            std::wregex wideRegex(L"([A-Za-z0-9:]+) ([A-Za-z0-9]+)([()])");
            std::wsmatch wideSmartMatch;

            if (std::regex_search(line, wideSmartMatch, wideRegex))
            {
                std::wcout << wideSmartMatch[1].str() << L" | " << wideSmartMatch[2].str() << L" | " << wideSmartMatch[3].str() << std::endl;
            }
        }

        if (line.find(L"{") != std::string::npos)
            currentItem->BracketCloseCount++;
        else if (line.find(L"}") != std::string::npos)
        {
            currentItem->BracketCloseCount--;
            if (currentItem->BracketCloseCount == 0)
                currentItem = currentItem->GetParent();
        }
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
        else if (NosStdLib::FileManagement::GetFileExtension<wchar_t>(entry.path()) == L"hpp" || NosStdLib::FileManagement::GetFileExtension<wchar_t>(entry.path()) == L"h") /* if not a directory and is a header (hpp or h extention), parse it */
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
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();

    ParseHeader(LR"(D:\Libraries\NosStdLib\Build\Library Structure Parser\x64\Release\abc.hpp)");
    
    wprintf(L"Press any button to continue"); _getch();
    return 0;

    std::wstring AbsoluteCurrentPath = std::filesystem::current_path();

    //RecureThrouDir(AbsoluteCurrentPath + LR"(\..\..\..\..\NosStdLib\NosStdLib)");
    RecureThrouDir(LR"(D:\Libraries\NosStdLib\Build\Library Structure Parser\x64\Release)");

    wprintf(L"Press any button to continue"); getchar();
    return 0;
}