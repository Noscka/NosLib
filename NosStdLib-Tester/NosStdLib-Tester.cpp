#include <iostream>
#include <NosStdLib/Global.hpp>

int main()
{
    std::cout << (NosStdLib::Global::String::IsNumber("abc") ? "True" : "False") << std::endl;
}