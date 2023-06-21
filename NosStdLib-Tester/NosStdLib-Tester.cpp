#include "NosStdLib/Console.hpp"
#include "NosStdLib/Chat.hpp"

#include "NosStdLib/Definitions.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

enum class MixedEnums : uint8_t
{
    A = 0b10000000,
    B = 0b01000000,

    C = 0b00000001,
    D = 0b00000010,
};

DEFINE_ENUM_FLAG_OPERATORS(MixedEnums);

////Sets flag to true
//void SetFlag(EFlagValue flag)
//{
//	m_FlagValue |= (int)flag;
//}
//
////Sets flag to false
//void UnsetFlag(EFlagValue flag)
//{
//	m_FlagValue &= ~(int)flag;
//}
//
////Check whether a flag is set to true
//bool HasFlag(EFlagValue flag)
//{
//	return (m_FlagValue & (int)flag) == (int)flag;
//}
//
//bool HasAnyFlag(EFlagValue multiFlag)
//{
//	return (m_FlagValue & (int)multiFlag) != 0;
//}

bool HasFlag(const MixedEnums& original, const MixedEnums& flag)
{
    return ((int)original & (int)flag) == (int)flag;
}

int main()
{
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Enum Testing");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

    MixedEnums example = (MixedEnums::D);

    if (HasFlag(example, MixedEnums::A))
    {
        wprintf(L"Has A\n");
    }
	if (HasFlag(example, MixedEnums::B))
	{
		wprintf(L"Has B\n");
	}
	if (HasFlag(example, MixedEnums::C))
	{
		wprintf(L"Has C\n");
	}
	if (HasFlag(example, MixedEnums::D))
	{
		wprintf(L"Has D\n");
	}

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}