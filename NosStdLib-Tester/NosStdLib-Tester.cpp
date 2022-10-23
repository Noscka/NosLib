#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <NosStdLib/Global.hpp>
#include <NosStdLib/DynamicArray.hpp>

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    NosStdLib::DynamicArray<wchar_t> FirstArray = NosStdLib::DynamicArray<wchar_t>();
    NosStdLib::DynamicArray<wchar_t> SecondArray = NosStdLib::DynamicArray<wchar_t>();
    NosStdLib::DynamicArray<NosStdLib::DynamicArray<wchar_t>*> TwoDimentionalCharArray = NosStdLib::DynamicArray<NosStdLib::DynamicArray<wchar_t>*>();

    FirstArray.ArrayAppend((wchar_t*)L"Some char in first array", 25, false); // add some data
    SecondArray.ArrayAppend((wchar_t*)L"Some char in second array", 26, false); // add some data

    TwoDimentionalCharArray.Append(&FirstArray); // Add both arrays to the 2 dimentional array
    TwoDimentionalCharArray.Append(&SecondArray);

    for (NosStdLib::DynamicArray<wchar_t>* Array : TwoDimentionalCharArray)
    {
        std::wcout << *Array << std::endl;
    }

    wprintf(L"Press any button to continue"); getchar();
    return 0;
}