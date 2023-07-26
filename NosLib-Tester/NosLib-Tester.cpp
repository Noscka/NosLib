#include "NosLib/Console.hpp"
#include "NosLib/String.hpp"

#include "AllHeaders.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

int main()
{
	NosLib::Console::InitializeModifiers::EnableUnicode();
	NosLib::Console::InitializeModifiers::EnableANSI();
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"File Shortcut Testing");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	NosLib::FileManagement::CreateFileShortcut(LR"(C:\Programing Projects\C++\NorzkasGammaInstaller\Build\NorzkasGammaInstaller\x64\Release\NorzkasGammaInstaller.exe)",LR"(D:\Libraries\NosLib\Build\NosLib-Tester\x64\Release\here.lnk)", L"description", L"");
	wprintf(L"Press any button to continue"); _getch();
	return 0;
}