#include "NosStdLib/Global.hpp"
#include "NosStdLib/DynamicArray.hpp"

#include <Windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <iostream> 
#include <cstdio> 
#include <conio.h>

#include <format>

template<typename T>
struct is_pointer { static const bool value = false; };

template<typename T>
struct is_pointer<T*> { static const bool value = true; };

template<typename T>
void Release(T& v)
{
    if (is_pointer<T>::value)
    {
        Release<T>(v);
    }
    else
    {
        delete[] v;
    }
}

class destructionTesting
{
private:
    int FirstIndex;
    int SecondIndex;
    std::wstring MemoryHolder;
public:
    destructionTesting()
    {
        wprintf(L"Created Default\n");
    }

    destructionTesting(int firstIndex, int secondIndex)
    {
        FirstIndex = firstIndex;
        SecondIndex = secondIndex;
        MemoryHolder = std::wstring(1000, L'A');
        wprintf(std::format(L"Created: {},{}\n", FirstIndex, SecondIndex).c_str());
    }

    ~destructionTesting()
    {
        wprintf(std::format(L"destroyed: {},{}\n", FirstIndex, SecondIndex).c_str());
    }
};

int main()
{
    NosStdLib::Global::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Global::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Global::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Mouse Tracking tests");
    NosStdLib::Global::Console::InitializeModifiers::InitializeEventHandler();

    //NosStdLib::MouseTracking::InitializeMouseTracking();
    //
    //MSG msg;
    //while (GetMessage(&msg, 0, 0, 0)){}

    wprintf(L"Press any button to start"); _getch();

    int amount = 100;

    destructionTesting*** ptr = new destructionTesting**[amount]();

    for (int i = 0; i <= amount; i++)
    {
        ptr[i] = new destructionTesting*[amount]();

        for (int j = 0; j <= amount; j++)
        {
            ptr[i][j] = new destructionTesting(i, j);
        }
    }

    wprintf(L"Press any button to continue"); _getch();

    Release<destructionTesting***>(ptr);

    //for (int i = 0; i <= amount; i++)
    //{
    //    for (int j = 0; j <= amount; j++)
    //    {
    //        delete ptr[i][j];
    //    }
    //}

    /*NosStdLib::DynamicArray<destructionTesting*> simpleArray(100, 100);

    for (int i = 0; i < 100000000; i++)
    {
        simpleArray.Append(new destructionTesting(i));
    }

    std::wcout << simpleArray.GetArrayIndexPointer() << std::endl;
    std::wcout << simpleArray.GetArraySize() << std::endl;*/

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}

/* ANSI TEST
std::wcout << L"🯰" << std::endl;
std::wcout << NosStdLib::TextColor::NosRGB(255, 255, 255).MakeANSICode<wchar_t>(false) << NosStdLib::TextColor::NosRGB(20, 120, 20).MakeANSICode<wchar_t>() << L"text 1" << L"\033[0m" << std::endl;
std::wcout << NosStdLib::TextColor::NosRGB(214, 48, 124).MakeANSICode<wchar_t>() << L"text 2" << L"\033[0m" << std::endl;
std::wcout << NosStdLib::TextColor::NosRGB(140, 20, 30).MakeANSICode<wchar_t>() << L"abc" << L"\033[0m" << std::endl;
*/

/* MENU TEST
bool SomeBool = false;
int number = 1;

void CheckBool()
{
    wprintf((SomeBool ? L"true\n" : L"false\n"));
    wprintf(L"Press any button to continue"); _getch();
    return;
}

void CheckNumber()
{
    wprintf((std::to_wstring(number) + L"\n").c_str());
    wprintf(L"Press any button to continue"); _getch();
    return;
}

void SomeFunction(int* param1, int* param2)
{
    std::wcout << L"Param1: " << *param1 << L" | Param2: " << *param2 << std::endl;
    wprintf(L"Press any button to continue"); _getch();
    return;
}

====MAIN====
NosStdLib::Global::Console::InitializeModifiers::EnableUnicode();
NosStdLib::Global::Console::InitializeModifiers::EnableANSI();

NosStdLib::Menu::DynamicMenu MainMenu(L"Main Menu", true, true, true);
NosStdLib::Menu::DynamicMenu SecondaryMenu(L"Second Menu", true, true, true);

SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Number", &number));
SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Number", new NosStdLib::Functional::FunctionStore(&CheckNumber)));

int param1, param2;

SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"param1", &param1));
SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"param2", &param2));

SecondaryMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Run Function", new NosStdLib::Functional::FunctionStore(&SomeFunction, &param1, &param2)));

MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Another Menu", &SecondaryMenu));

MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Toggle", &SomeBool));
MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Bool", new NosStdLib::Functional::FunctionStore(&CheckBool)));

MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Number", &number));
MainMenu.AddMenuEntry(new NosStdLib::Menu::MenuEntry(L"Check Number", new NosStdLib::Functional::FunctionStore(&CheckNumber)));

MainMenu.StartMenu();
wprintf(L"Press any button to continue"); _getch();
return 0;
*/

/* LOADING SCREEN TEST
void LongFunctionTing(NosStdLib::LoadingScreen* Object, std::wstring* argue)
{
    int total = 1000;
    uint8_t R = 255, G = 0, B = 0;
    uint8_t changeValue = 1;
    for (int i = 0; i <= total; i++)
    {
        if (R > 0 && B == 0)
        {
            R -= changeValue;
            G += changeValue;
        }
        if (G > 0 && R == 0)
        {
            G -= changeValue;
            B += changeValue;
        }
        if (B > 0 && G == 0)
        {
            R += changeValue;
            B -= changeValue;
        }

        Object->UpdateKnownProgressBar((float)i / (float)total, std::vformat((NosStdLib::TextColor::MakeANSICode<wchar_t>(NosStdLib::TextColor::NosRGB(R, G, B)) + L"\n{}\nTesting Status\nExtra Line,\n Innit\n{}" + L"\033[0m"), std::make_wformat_args((float)i / (float)total, NosStdLib::FileManagement::GetFileExtension<wchar_t>(L"abc.txt"), true)));
        Sleep(1);
    }
    *argue = L"Completed ting innit fam";
}

void ChangingProgressFunction(NosStdLib::LoadingScreen* Object, std::wstring* argue)
{
    int total = 1000;
    Object->UpdateKnownProgressBar(0.0 / (float)total, L"Testing Status\nExtra Line,\n Innit", true);
    Sleep(1000);
    Object->UpdateKnownProgressBar(250.0 / (float)total, L"Testing Status\nExtra Line,\n Innit", true);
    Sleep(1000);
    Object->UpdateKnownProgressBar(100.0 / (float)total, L"Testing Status\nExtra Line,\n Innit", true);
    Sleep(1000);

    *argue = L"Completed ting innit fam";
}

====MAIN====
NosStdLib::LoadingScreen::InitilizeFont();

std::wstring splash = LR"(
                      ████████                ███████                            
                    ▄██▀    ▀██▄ ▄███████▄  ███▀   ▀████████▄                    
          ▄███████████▌      ██████     ▀█████       ███     ▀▀███▄              
     ▄██▀▀         ██▌        ████       ████▌       ███           ▀▀███▄        
   ██▀            ███         ███▌       ▐███        ▐██▄               ▀▀███▄   
 ██▀       ███    ███         ███▌       ▐███        ▐████▀                  ▀██ 
██▌       ▀███▄▄▄▄███         ███        ▐███        ████▌                     ██
██▌               ██▌         ███        ▐███        ███▌          ████▄▄     ▄██
▀██▄              ██▌         ███        ▐███        ███          ███    ▀█████▀ 
  ▀██████████████▄███         ███        ████       ███          ███             
    ██▀       ████▀██         ███        ▐██▌      ▐██▌          ██▌             
   ███             ██▌        ██▌         ██       ███▌         ███              
   ███             ▐██                            █████▄       ███               
    ▀██▄▄       ▄▄▄████▄                         ███   ▀███▄▄███▀                
       ▀▀▀███▀▀▀▀    ▀██▄         ▄██▄         ▄██▀                              
                       ▀███▄▄▄▄▄███▀████▄▄▄▄▄███▀                                
                           ▀▀▀▀▀        ▀▀▀▀▀                                    )";

std::wstring SomeVar(L"Some Text");

NosStdLib::LoadingScreen LC(NosStdLib::LoadingScreen::LoadType::Known, splash);
LC.StartLoading(&LongFunctionTing, &SomeVar);

wprintf(L"Press any button to continue"); _getch();
NosStdLib::LoadingScreen::TerminateFont();
*/