#include "NosLib/Console.hpp"
#include "NosLib/String.hpp"

#include "AllHeaders.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

const std::wstring Splash = LR"(


                                               ▄▄▄▄▄                 ▄▄▄▄▄▄▄     ▄      ▄▄▄▄  
             ███████       ███▀ ▄█        ▄▄██████████▀ ▄█       ▄██████████▀▀▄▄▀      ▄██▀ ▄█
             ████████     █▀ ▄██▀       ▄█████▀▀▀▀█▀ ▄███      ▄█████▀▀▀█▀▀▄▄███      █▀ ▄███ 
            █████████▌    ▄█████       █████▀       █████▌   ▄█████▀      ▐█████      ▄█████  
           ▐█████████▀   ▐█████       █████▌                ▐█████                    █████▌  
           ██████ ▀ ▄██  █████▌      █████▀                 ████▀▀    ▄▄▄▄▄▄▄  ▄     ▐█████   
          ▐████▀  ▀█████▐█████      ▄██▀ ▄                 ██▀ ▄▄    █████▀ ▄██      ████▀    
          ██▀ ▄█   ▐█████████▌     █▀ ▄███        ▄▀ ▄█▀ ▄▀ ▄▄███      ▀ ▄████      ██▀▀▄▄▀   
           ▄███     █████████       ▐█████▄       ▄███▀    ██████▄      ██████     ▀▀▄▄██▌    
        ▄█████▌      ███████▌        ████████▀ ▄████▀       █████▀ ▄█████████      ▄█████     
     ▄████████        ██████           ▀██▀ ▄███▀▀            ▀ ▄███████▀▀        ▐█████▌     

)";

const std::wstring Overlay = LR"(
                                       ▄                                                           
                                    ▄███                    ▄██▌                   ███▀         ██▄
             ███████       ██████████▌      ▒█████████▌  ▄███▌     ███████████  ████   ██████████▀ 
            █▀      █     █   ██▌ █▌     █▀▀          ██▀ █▀   ▄█▀░         ▄██▀██    █▌  ██▀ █▀   
            █        █   █ ██▌  ▄█     █▀     ▄██▄ ██▌   █    █▀     ▄██ ▄██▌   █   ▄█▄██▀   █     
           █         ▓▌███▌     ▌     █     ██░██▀▌█     █  █▌     ██▒██▀░▌     █ ▄████     █      
           ▌         ██▌▐▌     █     █     █▌  ▌   ███████ ▐▌     █▀      █████████▀ █     ▐▌      
          █      ███▌  ██     ▐▌    █     ██              ▄█    ▄█▌  █▌▌▌▌▌▌▌████▀  █▌     █ █     
          █    ███▌     ▌     █    █▀  ████          ██████  ▄██▌█  █░    ███░ █    █    ▄███▀     
         █  ██▌▐█ █▌         ▐▌   █░███   █      █▀███▀███▄██▌   █  ▀▀████▀   █    █▀ ▄██▌█▌       
        ▐██▌   █   █         █  ██▀█▌     ██▄▄█████▀  ███▀█      ▀█▌████      ▌  ▄█▌██▌  █▌        
      ▄█▀     ▐▌    █       █▌  ▌   █        ▐██▀   ▒█▀    █     ▒██░        █  ███▌     █         
   ▄█▀        █      █      █        ██▌  ▄██▀  ▄█▀▀        █████▀      ▄██▀▀     █     █░         
▄██████▌▌▀▀▀▀▀        ▀▀▀▀▀▀            ███▀█▌▌░            ███▀█▌▌▌▌▌▀           ▀▀▀▀▀▀░          

)";

int main()
{
	NosLib::Console::InitializeModifiers::EnableUnicode();
	NosLib::Console::InitializeModifiers::EnableANSI();
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"File Shortcut Testing");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	wprintf((NosLib::RGB::NosRGB(0,0,0).MakeANSICode<wchar_t>() + Splash + NosLib::RGB::NosRGB::ANSIResetCode<wchar_t>()).c_str());
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0,0});

	wprintf(NosLib::RGB::NosRGB(130, 0, 0).MakeANSICode<wchar_t>().c_str());
    for (wchar_t ch : Overlay)
    {
        if (ch == L' ')
        {
            wprintf(L"\033[1C");
            continue;
        }

        wprintf(L"%c", ch);
    }
    wprintf(NosLib::RGB::NosRGB::ANSIResetCode<wchar_t>().c_str());

	wprintf(L"Press any button to continue"); _getch();
	return 0;
}