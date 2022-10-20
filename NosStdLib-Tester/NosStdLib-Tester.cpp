#include <iostream>
#include <NosStdLib/Global.hpp>

int main()
{
    std::cout << (NosStdLib::Global::String::IsNumber(L"+123", true) ? "True" : "False") << std::endl;
    NosStdLib::Global::Console::ClearLine(0);
}