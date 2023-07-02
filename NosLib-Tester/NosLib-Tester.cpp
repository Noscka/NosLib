#include "NosLib/Console.hpp"
#include "NosLib/RGB.hpp"
#include "NosLib/DynamicLoadingScreen.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

void LongRGBFunction(NosLib::LoadingScreen* Object, std::wstring* argue)
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
		Object->UpdateKnownProgressBar((float)i / (float)total, std::vformat((NosLib::RGB::NosRGB(R, G, B).MakeANSICode<wchar_t>() + L"\n{}\nExtra text" + L"\033[0m"), std::make_wformat_args((float)i / (float)total)));
		Sleep(1);
	}
	*argue = L"Loading was completed\n";
}

int main()
{
    NosLib::Console::InitializeModifiers::EnableUnicode();
    NosLib::Console::InitializeModifiers::EnableANSI();
    NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Loading screen");
    NosLib::Console::InitializeModifiers::InitializeEventHandler();

	NosLib::LoadingScreen::InitilizeFont();
	std::wstring logo = LR"(
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

	std::wstring someVar = L"Not set string\n";
	NosLib::LoadingScreen loadingScreen(NosLib::LoadingScreen::LoadType::Known, logo);
	loadingScreen.StartLoading(&LongRGBFunction, &someVar);

    wprintf(L"Press any button to continue"); _getch();
	NosLib::LoadingScreen::TerminateFont();
    return 0;
}