#ifndef _UNICODETEXTGENERATOR_HPP_
#define _UNICODETEXTGENERATOR_HPP_

#include <string>
#include <Windows.h>

namespace NosStdLib
{
	/// <summary>
	/// namespace for functions/classes for unicode text generation
	/// </summary>
	namespace UnicodeTextGenerator
	{
	#pragma region BasicUnicodeTextGenerate
		/// <summary>
		/// a Unicode text generator which will create big text from normal wstring with custom Console Handle
		/// </summary>
		/// <param name="consoleHandle">- custom Console Handle</param>
		/// <param name="input">- The text to convert into big text</param>
		/// <param name="center">- if the output should be centered or not</param>
		/// <returns></returns>
		std::wstring BasicUnicodeTextGenerate(const HANDLE& consoleHandle, const std::wstring& input, const bool center)
		{
			/*
			Program: Unicode Text Generator (Modified by Noscka)
			Description: Design text with unicode art. Modification: Made function return string instead of printing it straight to console
			======= ORIGINAL CREATOR =======
			Author: Lord Hypersonic
			Email: lordhypersonic.522@gmail.com
			Website: www.lordhypersonic.blogspot.com
			*/

			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(consoleHandle, &csbi);

			std::wstring output;
			int SpaceLenght = (((csbi.srWindow.Right - csbi.srWindow.Left + 1) / 2) - ((input.length() * 8) / 2));

			if (center)
				output += std::wstring(SpaceLenght, ' ');

			//loop will print first layer
			for (int i = 0; i < input.size(); i++)
			{
				if (input[i] == 'A' || input[i] == 'a')
					output += L"  ___   ";
				if (input[i] == 'B' || input[i] == 'b')
					output += L" _____  ";
				if (input[i] == 'C' || input[i] == 'c')
					output += L" _____  ";
				if (input[i] == 'D' || input[i] == 'd')
					output += L" _____  ";
				if (input[i] == 'E' || input[i] == 'e')
					output += L" _____  ";
				if (input[i] == 'F' || input[i] == 'f')
					output += L" _____  ";
				if (input[i] == 'G' || input[i] == 'g')
					output += L" _____  ";
				if (input[i] == 'H' || input[i] == 'h')
					output += L" _   _  ";
				if (input[i] == 'I' || input[i] == 'i')
					output += L" _____  ";
				if (input[i] == 'J' || input[i] == 'j')
					output += L"   ___  ";
				if (input[i] == 'K' || input[i] == 'k')
					output += L" _   __ ";
				if (input[i] == 'L' || input[i] == 'l')
					output += L" _      ";
				if (input[i] == 'M' || input[i] == 'm')
					output += L" __  __  ";
				if (input[i] == 'N' || input[i] == 'n')
					output += L" _   _  ";
				if (input[i] == 'O' || input[i] == 'o')
					output += L" _____  ";
				if (input[i] == 'P' || input[i] == 'p')
					output += L" _____  ";
				if (input[i] == 'Q' || input[i] == 'q')
					output += L" _____  ";
				if (input[i] == 'R' || input[i] == 'r')
					output += L" _____  ";
				if (input[i] == 'S' || input[i] == 's')
					output += L" _____  ";
				if (input[i] == 'T' || input[i] == 't')
					output += L" _____  ";
				if (input[i] == 'U' || input[i] == 'u')
					output += L" _   _  ";
				if (input[i] == 'V' || input[i] == 'v')
					output += L" _   _  ";
				if (input[i] == 'W' || input[i] == 'w')
					output += L" _    _  ";
				if (input[i] == 'X' || input[i] == 'x')
					output += L"__   __ ";
				if (input[i] == 'Y' || input[i] == 'y')
					output += L"__   __ ";
				if (input[i] == 'Z' || input[i] == 'z')
					output += L" ______ ";
				if (input[i] == ' ')
					output += L"  ";
				if (input[i] == '`')
					output += L" _  ";
				if (input[i] == '~')
					output += L"      ";
				if (input[i] == '1')
					output += L" __   ";
				if (input[i] == '2')
					output += L" _____  ";
				if (input[i] == '3')
					output += L" _____  ";
				if (input[i] == '4')
					output += L"   ___  ";
				if (input[i] == '5')
					output += L" _____  ";
				if (input[i] == '6')
					output += L"  ____  ";
				if (input[i] == '7')
					output += L" ______ ";
				if (input[i] == '.')
					output += L"    ";
				if (input[i] == '8')
					output += L" _____  ";
				if (input[i] == '9')
					output += L" _____  ";
				if (input[i] == '0')
					output += L" _____  ";
				if (input[i] == '!')
					output += L" _  ";
				if (input[i] == '@')
					output += L"   ____   ";
				if (input[i] == '#')
					output += L"   _  _    ";
				if (input[i] == '$')
					output += L"  _   ";
				if (input[i] == '%')
					output += L" _   __ ";
				if (input[i] == '^')
					output += L" /\\  ";
				if (input[i] == '&')
					output += L"         ";
				if (input[i] == '*')
					output += L"    _     ";
				if (input[i] == '(')
					output += L"  __ ";
				if (input[i] == ')')
					output += L"__   ";
				if (input[i] == '-')
					output += L"         ";
				if (input[i] == '_')
					output += L"         ";
				if (input[i] == '=')
					output += L"         ";
				if (input[i] == '+')
					output += L"        ";
				if (input[i] == '[')
					output += L" ___  ";
				if (input[i] == '{')
					output += L"   __ ";
				if (input[i] == ']')
					output += L" ___  ";
				if (input[i] == '}')
					output += L"__    ";
				if (input[i] == '|')
					output += L" _  ";
				if (input[i] == '\\')
					output += L"__      ";
				if (input[i] == ';')
					output += L" _  ";
				if (input[i] == ':')
					output += L"    ";
				if (input[i] == '\'')
					output += L" _  ";
				if (input[i] == '"')
					output += L" _ _  ";
				if (input[i] == '<')
					output += L"   __ ";
				if (input[i] == ',')
					output += L"    ";
				if (input[i] == '>')
					output += L"__    ";
				if (input[i] == '/')
					output += L"     __ ";
				if (input[i] == '?')
					output += L" ___   ";
			}
			output += L"\n";
			if (center)
				output += std::wstring(SpaceLenght, ' ');
			//loop will print second layer
			for (int i = 0; i < input.size(); i++)
			{
				if (input[i] == 'A' || input[i] == 'a')
					output += L" / _ \\  ";
				if (input[i] == 'B' || input[i] == 'b')
					output += L"| ___ \\ ";
				if (input[i] == 'C' || input[i] == 'c')
					output += L"/  __ \\ ";
				if (input[i] == 'D' || input[i] == 'd')
					output += L"|  _  \\ ";
				if (input[i] == 'E' || input[i] == 'e')
					output += L"|  ___| ";
				if (input[i] == 'F' || input[i] == 'f')
					output += L"|  ___| ";
				if (input[i] == 'G' || input[i] == 'g')
					output += L"|  __ \\ ";
				if (input[i] == 'H' || input[i] == 'h')
					output += L"| | | | ";
				if (input[i] == 'I' || input[i] == 'i')
					output += L"|_   _| ";
				if (input[i] == 'J' || input[i] == 'j')
					output += L"  |_  | ";
				if (input[i] == 'K' || input[i] == 'k')
					output += L"| | / / ";
				if (input[i] == 'L' || input[i] == 'l')
					output += L"| |     ";
				if (input[i] == 'M' || input[i] == 'm')
					output += L"|  \\/  | ";
				if (input[i] == 'N' || input[i] == 'n')
					output += L"| \\ | | ";
				if (input[i] == 'O' || input[i] == 'o')
					output += L"|  _  | ";
				if (input[i] == 'P' || input[i] == 'p')
					output += L"| ___ \\ ";
				if (input[i] == 'Q' || input[i] == 'q')
					output += L"|  _  | ";
				if (input[i] == 'R' || input[i] == 'r')
					output += L"| ___ \\ ";
				if (input[i] == 'S' || input[i] == 's')
					output += L"/  ___| ";
				if (input[i] == 'T' || input[i] == 't')
					output += L"|_   _| ";
				if (input[i] == 'U' || input[i] == 'u')
					output += L"| | | | ";
				if (input[i] == 'V' || input[i] == 'v')
					output += L"| | | | ";
				if (input[i] == 'W' || input[i] == 'w')
					output += L"| |  | | ";
				if (input[i] == 'X' || input[i] == 'x')
					output += L"\\ \\ / / ";
				if (input[i] == 'Y' || input[i] == 'y')
					output += L"\\ \\ / / ";
				if (input[i] == 'Z' || input[i] == 'z')
					output += L"|___  / ";
				if (input[i] == ' ')
					output += L"  ";
				if (input[i] == '`')
					output += L"( ) ";
				if (input[i] == '~')
					output += L"      ";
				if (input[i] == '1')
					output += L"/  |  ";
				if (input[i] == '2')
					output += L"/ __  \\ ";
				if (input[i] == '3')
					output += L"|____ | ";
				if (input[i] == '4')
					output += L"  /   | ";
				if (input[i] == '5')
					output += L"|  ___| ";
				if (input[i] == '6')
					output += L" / ___| ";
				if (input[i] == '7')
					output += L"|___  / ";
				if (input[i] == '.')
					output += L"    ";
				if (input[i] == '8')
					output += L"|  _  | ";
				if (input[i] == '9')
					output += L"|  _  | ";
				if (input[i] == '0')
					output += L"|  _  | ";
				if (input[i] == '!')
					output += L"| | ";
				if (input[i] == '@')
					output += L"  / __ \\  ";
				if (input[i] == '#')
					output += L" _| || |_  ";
				if (input[i] == '$')
					output += L" | |  ";
				if (input[i] == '%')
					output += L"(_) / / ";
				if (input[i] == '^')
					output += L"|/\\| ";
				if (input[i] == '&')
					output += L"  ___    ";
				if (input[i] == '*')
					output += L" /\\| |/\\  ";
				if (input[i] == '(')
					output += L" / / ";
				if (input[i] == ')')
					output += L"\\ \\  ";
				if (input[i] == '-')
					output += L"         ";
				if (input[i] == '_')
					output += L"         ";
				if (input[i] == '=')
					output += L" ______  ";
				if (input[i] == '+')
					output += L"   _    ";
				if (input[i] == '[')
					output += L"|  _| ";
				if (input[i] == '{')
					output += L"  / / ";
				if (input[i] == ']')
					output += L"|_  | ";
				if (input[i] == '}')
					output += L"\\ \\   ";
				if (input[i] == '|')
					output += L"| | ";
				if (input[i] == '\\')
					output += L"\\ \\     ";
				if (input[i] == ';')
					output += L"(_) ";
				if (input[i] == ':')
					output += L" _  ";
				if (input[i] == '\'')
					output += L"( ) ";
				if (input[i] == '"')
					output += L"( | ) ";
				if (input[i] == '<')
					output += L"  / / ";
				if (input[i] == ',')
					output += L"    ";
				if (input[i] == '>')
					output += L"\\ \\   ";
				if (input[i] == '/')
					output += L"    / / ";
				if (input[i] == '?')
					output += L"|__ \\  ";
			}
			output += L"\n";
			if (center)
				output += std::wstring(SpaceLenght, ' ');
			//loop will print third layer
			for (int i = 0; i < input.size(); i++)
			{
				if (input[i] == 'A' || input[i] == 'a')
					output += L"/ /_\\ \\ ";
				if (input[i] == 'B' || input[i] == 'b')
					output += L"| |_/ / ";
				if (input[i] == 'C' || input[i] == 'c')
					output += L"| /  \\/ ";
				if (input[i] == 'D' || input[i] == 'd')
					output += L"| | | | ";
				if (input[i] == 'E' || input[i] == 'e')
					output += L"| |__   ";
				if (input[i] == 'F' || input[i] == 'f')
					output += L"| |_    ";
				if (input[i] == 'G' || input[i] == 'g')
					output += L"| |  \\/ ";
				if (input[i] == 'H' || input[i] == 'h')
					output += L"| |_| | ";
				if (input[i] == 'I' || input[i] == 'i')
					output += L"  | |   ";
				if (input[i] == 'J' || input[i] == 'j')
					output += L"    | | ";
				if (input[i] == 'K' || input[i] == 'k')
					output += L"| |/ /  ";
				if (input[i] == 'L' || input[i] == 'l')
					output += L"| |     ";
				if (input[i] == 'M' || input[i] == 'm')
					output += L"| .  . | ";
				if (input[i] == 'N' || input[i] == 'n')
					output += L"|  \\| | ";
				if (input[i] == 'O' || input[i] == 'o')
					output += L"| | | | ";
				if (input[i] == 'P' || input[i] == 'p')
					output += L"| |_/ / ";
				if (input[i] == 'Q' || input[i] == 'q')
					output += L"| | | | ";
				if (input[i] == 'R' || input[i] == 'r')
					output += L"| |_/ / ";
				if (input[i] == 'S' || input[i] == 's')
					output += L"\\ `--.  ";
				if (input[i] == 'T' || input[i] == 't')
					output += L"  | |   ";
				if (input[i] == 'U' || input[i] == 'u')
					output += L"| | | | ";
				if (input[i] == 'V' || input[i] == 'v')
					output += L"| | | | ";
				if (input[i] == 'W' || input[i] == 'w')
					output += L"| |  | | ";
				if (input[i] == 'X' || input[i] == 'x')
					output += L" \\ V /  ";
				if (input[i] == 'Y' || input[i] == 'y')
					output += L" \\ V /  ";
				if (input[i] == 'Z' || input[i] == 'z')
					output += L"   / /  ";
				if (input[i] == ' ')
					output += L"  ";
				if (input[i] == '`')
					output += L" \\| ";
				if (input[i] == '~')
					output += L" /\\/| ";
				if (input[i] == '1')
					output += L"`| |  ";
				if (input[i] == '2')
					output += L"`' / /' ";
				if (input[i] == '3')
					output += L"    / / ";
				if (input[i] == '4')
					output += L" / /| | ";
				if (input[i] == '5')
					output += L"|___ \\  ";
				if (input[i] == '6')
					output += L"/ /___  ";
				if (input[i] == '7')
					output += L"   / /  ";
				if (input[i] == '.')
					output += L"    ";
				if (input[i] == '8')
					output += L" \\ V /  ";
				if (input[i] == '9')
					output += L"| |_| | ";
				if (input[i] == '0')
					output += L"| |/' | ";
				if (input[i] == '!')
					output += L"| | ";
				if (input[i] == '@')
					output += L" / / _` | ";
				if (input[i] == '#')
					output += L"|_  __  _| ";
				if (input[i] == '$')
					output += L"/ __) ";
				if (input[i] == '%')
					output += L"   / /  ";
				if (input[i] == '^')
					output += L"     ";
				if (input[i] == '&')
					output += L" ( _ )   ";
				if (input[i] == '*')
					output += L" \\ ` ' /  ";
				if (input[i] == '(')
					output += L"| |  ";
				if (input[i] == ')')
					output += L" | | ";
				if (input[i] == '-')
					output += L" ______  ";
				if (input[i] == '_')
					output += L"         ";
				if (input[i] == '=')
					output += L"|______| ";
				if (input[i] == '+')
					output += L" _| |_  ";
				if (input[i] == '[')
					output += L"| |   ";
				if (input[i] == '{')
					output += L" | |  ";
				if (input[i] == ']')
					output += L"  | | ";
				if (input[i] == '}')
					output += L" | |  ";
				if (input[i] == '|')
					output += L"| | ";
				if (input[i] == '\\')
					output += L" \\ \\    ";
				if (input[i] == ';')
					output += L"    ";
				if (input[i] == ':')
					output += L"(_) ";
				if (input[i] == '\'')
					output += L"|/  ";
				if (input[i] == '"')
					output += L" V V  ";
				if (input[i] == '<')
					output += L" / /  ";
				if (input[i] == ',')
					output += L"    ";
				if (input[i] == '>')
					output += L" \\ \\  ";
				if (input[i] == '/')
					output += L"   / /  ";
				if (input[i] == '?')
					output += L"   ) | ";
			}
			output += L"\n";
			if (center)
				output += std::wstring(SpaceLenght, ' ');
			//loop will print fourth layer
			for (int i = 0; i < input.size(); i++)
			{
				if (input[i] == 'A' || input[i] == 'a')
					output += L"|  _  | ";
				if (input[i] == 'B' || input[i] == 'b')
					output += L"| ___ \\ ";
				if (input[i] == 'C' || input[i] == 'c')
					output += L"| |     ";
				if (input[i] == 'D' || input[i] == 'd')
					output += L"| | | | ";
				if (input[i] == 'E' || input[i] == 'e')
					output += L"|  __|  ";
				if (input[i] == 'F' || input[i] == 'f')
					output += L"|  _|   ";
				if (input[i] == 'G' || input[i] == 'g')
					output += L"| | __  ";
				if (input[i] == 'H' || input[i] == 'h')
					output += L"|  _  | ";
				if (input[i] == 'I' || input[i] == 'i')
					output += L"  | |   ";
				if (input[i] == 'J' || input[i] == 'j')
					output += L"    | | ";
				if (input[i] == 'K' || input[i] == 'k')
					output += L"|    \\  ";
				if (input[i] == 'L' || input[i] == 'l')
					output += L"| |     ";
				if (input[i] == 'M' || input[i] == 'm')
					output += L"| |\\/| | ";
				if (input[i] == 'N' || input[i] == 'n')
					output += L"| . ` | ";
				if (input[i] == 'O' || input[i] == 'o')
					output += L"| | | | ";
				if (input[i] == 'P' || input[i] == 'p')
					output += L"|  __/  ";
				if (input[i] == 'Q' || input[i] == 'q')
					output += L"| | | | ";
				if (input[i] == 'R' || input[i] == 'r')
					output += L"|    /  ";
				if (input[i] == 'S' || input[i] == 's')
					output += L" `--. \\ ";
				if (input[i] == 'T' || input[i] == 't')
					output += L"  | |   ";
				if (input[i] == 'U' || input[i] == 'u')
					output += L"| | | | ";
				if (input[i] == 'V' || input[i] == 'v')
					output += L"| | | | ";
				if (input[i] == 'W' || input[i] == 'w')
					output += L"| |/\\| | ";
				if (input[i] == 'X' || input[i] == 'x')
					output += L" / ^ \\  ";
				if (input[i] == 'Y' || input[i] == 'y')
					output += L"  \\ /   ";
				if (input[i] == 'Z' || input[i] == 'z')
					output += L"  / /   ";
				if (input[i] == ' ')
					output += L"  ";
				if (input[i] == '`')
					output += L"    ";
				if (input[i] == '~')
					output += L"|/\\/  ";
				if (input[i] == '1')
					output += L" | |  ";
				if (input[i] == '2')
					output += L"  / /   ";
				if (input[i] == '3')
					output += L"    \\ \\ ";
				if (input[i] == '4')
					output += L"/ /_| | ";
				if (input[i] == '5')
					output += L"    \\ \\ ";
				if (input[i] == '6')
					output += L"| ___ \\ ";
				if (input[i] == '7')
					output += L"  / /   ";
				if (input[i] == '.')
					output += L"    ";
				if (input[i] == '8')
					output += L" / _ \\  ";
				if (input[i] == '9')
					output += L"\\____ | ";
				if (input[i] == '0')
					output += L"|  /| | ";
				if (input[i] == '!')
					output += L"| | ";
				if (input[i] == '@')
					output += L"| | (_| | ";
				if (input[i] == '#')
					output += L" _| || |_  ";
				if (input[i] == '$')
					output += L"\\__ \\ ";
				if (input[i] == '%')
					output += L"  / /   ";
				if (input[i] == '^')
					output += L"     ";
				if (input[i] == '&')
					output += L" / _ \\/\\ ";
				if (input[i] == '*')
					output += L"|_     _| ";
				if (input[i] == '(')
					output += L"| |  ";
				if (input[i] == ')')
					output += L" | | ";
				if (input[i] == '-')
					output += L"|______| ";
				if (input[i] == '_')
					output += L"         ";
				if (input[i] == '=')
					output += L" ______  ";
				if (input[i] == '+')
					output += L"|_   _| ";
				if (input[i] == '[')
					output += L"| |   ";
				if (input[i] == '{')
					output += L"< <   ";
				if (input[i] == ']')
					output += L"  | | ";
				if (input[i] == '}')
					output += L"  > > ";
				if (input[i] == '|')
					output += L"| | ";
				if (input[i] == '\\')
					output += L"  \\ \\   ";
				if (input[i] == ';')
					output += L" _  ";
				if (input[i] == ':')
					output += L"    ";
				if (input[i] == '\'')
					output += L"    ";
				if (input[i] == '"')
					output += L"      ";
				if (input[i] == '<')
					output += L"< <   ";
				if (input[i] == ',')
					output += L" _  ";
				if (input[i] == '>')
					output += L"  > > ";
				if (input[i] == '/')
					output += L"  / /   ";
				if (input[i] == '?')
					output += L"  / /  ";
			}
			output += L"\n";
			if (center)
				output += std::wstring(SpaceLenght, ' ');
			//loop will print fifth layer
			for (int i = 0; i < input.size(); i++)
			{
				if (input[i] == 'A' || input[i] == 'a')
					output += L"| | | | ";
				if (input[i] == 'B' || input[i] == 'b')
					output += L"| |_/ / ";
				if (input[i] == 'C' || input[i] == 'c')
					output += L"| \\__/\\ ";
				if (input[i] == 'D' || input[i] == 'd')
					output += L"| |/ /  ";
				if (input[i] == 'E' || input[i] == 'e')
					output += L"| |___  ";
				if (input[i] == 'F' || input[i] == 'f')
					output += L"| |     ";
				if (input[i] == 'G' || input[i] == 'g')
					output += L"| |_\\ \\ ";
				if (input[i] == 'H' || input[i] == 'h')
					output += L"| | | | ";
				if (input[i] == 'I' || input[i] == 'i')
					output += L" _| |_  ";
				if (input[i] == 'J' || input[i] == 'j')
					output += L"/\\__/ / ";
				if (input[i] == 'K' || input[i] == 'k')
					output += L"| |\\  \\ ";
				if (input[i] == 'L' || input[i] == 'l')
					output += L"| |____ ";
				if (input[i] == 'M' || input[i] == 'm')
					output += L"| |  | | ";
				if (input[i] == 'N' || input[i] == 'n')
					output += L"| |\\  | ";
				if (input[i] == 'O' || input[i] == 'o')
					output += L"\\ \\_/ / ";
				if (input[i] == 'P' || input[i] == 'p')
					output += L"| |     ";
				if (input[i] == 'Q' || input[i] == 'q')
					output += L"\\ \\/' / ";
				if (input[i] == 'R' || input[i] == 'r')
					output += L"| |\\ \\  ";
				if (input[i] == 'S' || input[i] == 's')
					output += L"/\\__/ / ";
				if (input[i] == 'T' || input[i] == 't')
					output += L"  | |   ";
				if (input[i] == 'U' || input[i] == 'u')
					output += L"| |_| | ";
				if (input[i] == 'V' || input[i] == 'v')
					output += L"\\ \\_/ / ";
				if (input[i] == 'W' || input[i] == 'w')
					output += L"\\  /\\  / ";
				if (input[i] == 'X' || input[i] == 'x')
					output += L"/ / \\ \\ ";
				if (input[i] == 'Y' || input[i] == 'y')
					output += L"  | |   ";
				if (input[i] == 'Z' || input[i] == 'z')
					output += L"./ /___ ";
				if (input[i] == ' ')
					output += L"  ";
				if (input[i] == '`')
					output += L"    ";
				if (input[i] == '~')
					output += L"      ";
				if (input[i] == '1')
					output += L"_| |_ ";
				if (input[i] == '2')
					output += L"./ /___ ";
				if (input[i] == '3')
					output += L".___/ / ";
				if (input[i] == '4')
					output += L"\\___  | ";
				if (input[i] == '5')
					output += L"/\\__/ / ";
				if (input[i] == '6')
					output += L"| \\_/ | ";
				if (input[i] == '7')
					output += L"./ /    ";
				if (input[i] == '.')
					output += L" _  ";
				if (input[i] == '8')
					output += L"| |_| | ";
				if (input[i] == '9')
					output += L".___/ / ";
				if (input[i] == '0')
					output += L"\\ |_/ / ";
				if (input[i] == '!')
					output += L"|_| ";
				if (input[i] == '@')
					output += L" \\ \\__,_| ";
				if (input[i] == '#')
					output += L"|_  __  _| ";
				if (input[i] == '$')
					output += L"(   / ";
				if (input[i] == '%')
					output += L" / / _  ";
				if (input[i] == '^')
					output += L"     ";
				if (input[i] == '&')
					output += L"| (_>  < ";
				if (input[i] == '*')
					output += L" / , . \\  ";
				if (input[i] == '(')
					output += L"| |  ";
				if (input[i] == ')')
					output += L" | | ";
				if (input[i] == '-')
					output += L"         ";
				if (input[i] == '_')
					output += L" ______  ";
				if (input[i] == '=')
					output += L"|______| ";
				if (input[i] == '+')
					output += L"  |_|   ";
				if (input[i] == '[')
					output += L"| |_  ";
				if (input[i] == '{')
					output += L" | |  ";
				if (input[i] == ']')
					output += L" _| | ";
				if (input[i] == '}')
					output += L" | |  ";
				if (input[i] == '|')
					output += L"| | ";
				if (input[i] == '\\')
					output += L"   \\ \\  ";
				if (input[i] == ';')
					output += L"( ) ";
				if (input[i] == ':')
					output += L" _  ";
				if (input[i] == '\'')
					output += L"    ";
				if (input[i] == '"')
					output += L"      ";
				if (input[i] == '<')
					output += L" \\ \\  ";
				if (input[i] == ',')
					output += L"( ) ";
				if (input[i] == '>')
					output += L" / /  ";
				if (input[i] == '/')
					output += L" / /    ";
				if (input[i] == '?')
					output += L" |_|   ";
			}
			output += L"\n";
			if (center)
				output += std::wstring(SpaceLenght, ' ');
			//loop will print sixth layer
			for (int i = 0; i < input.size(); i++)
			{
				if (input[i] == 'A' || input[i] == 'a')
					output += L"\\_| |_/ ";
				if (input[i] == 'B' || input[i] == 'b')
					output += L"\\____/  ";
				if (input[i] == 'C' || input[i] == 'c')
					output += L" \\____/ ";
				if (input[i] == 'D' || input[i] == 'd')
					output += L"|___/   ";
				if (input[i] == 'E' || input[i] == 'e')
					output += L"\\____/  ";
				if (input[i] == 'F' || input[i] == 'f')
					output += L"\\_|     ";
				if (input[i] == 'G' || input[i] == 'g')
					output += L" \\____/ ";
				if (input[i] == 'H' || input[i] == 'h')
					output += L"\\_| |_/ ";
				if (input[i] == 'I' || input[i] == 'i')
					output += L" \\___/  ";
				if (input[i] == 'J' || input[i] == 'j')
					output += L"\\____/  ";
				if (input[i] == 'K' || input[i] == 'k')
					output += L"\\_| \\_/ ";
				if (input[i] == 'L' || input[i] == 'l')
					output += L"\\_____/ ";
				if (input[i] == 'M' || input[i] == 'm')
					output += L"\\_|  |_/ ";
				if (input[i] == 'N' || input[i] == 'n')
					output += L"\\_| \\_/ ";
				if (input[i] == 'O' || input[i] == 'o')
					output += L" \\___/  ";
				if (input[i] == 'P' || input[i] == 'p')
					output += L"\\_|     ";
				if (input[i] == 'Q' || input[i] == 'q')
					output += L" \\_/\\_\\ ";
				if (input[i] == 'R' || input[i] == 'r')
					output += L"\\_| \\_| ";
				if (input[i] == 'S' || input[i] == 's')
					output += L"\\____/  ";
				if (input[i] == 'T' || input[i] == 't')
					output += L"  \\_/   ";
				if (input[i] == 'U' || input[i] == 'u')
					output += L" \\___/  ";
				if (input[i] == 'V' || input[i] == 'v')
					output += L" \\___/  ";
				if (input[i] == 'W' || input[i] == 'w')
					output += L" \\/  \\/  ";
				if (input[i] == 'X' || input[i] == 'x')
					output += L"\\/   \\/ ";
				if (input[i] == 'Y' || input[i] == 'y')
					output += L"  \\_/   ";
				if (input[i] == 'Z' || input[i] == 'z')
					output += L"\\_____/ ";
				if (input[i] == ' ')
					output += L"  ";
				if (input[i] == '`')
					output += L"    ";
				if (input[i] == '~')
					output += L"      ";
				if (input[i] == '1')
					output += L"\\___/ ";
				if (input[i] == '2')
					output += L"\\_____/ ";
				if (input[i] == '3')
					output += L"\\____/  ";
				if (input[i] == '4')
					output += L"    |_/ ";
				if (input[i] == '5')
					output += L"\\____/  ";
				if (input[i] == '6')
					output += L"\\_____/ ";
				if (input[i] == '7')
					output += L"\\_/     ";
				if (input[i] == '.')
					output += L"(_) ";
				if (input[i] == '8')
					output += L"\\_____/ ";
				if (input[i] == '9')
					output += L"\\____/  ";
				if (input[i] == '0')
					output += L" \\___/  ";
				if (input[i] == '!')
					output += L"(_) ";
				if (input[i] == '@')
					output += L"  \\____/  ";
				if (input[i] == '#')
					output += L"  |_||_|   ";
				if (input[i] == '$')
					output += L" |_|  ";
				if (input[i] == '%')
					output += L"/_/ (_) ";
				if (input[i] == '^')
					output += L"     ";
				if (input[i] == '&')
					output += L" \\___/\\/ ";
				if (input[i] == '*')
					output += L" \\/|_|\\/  ";
				if (input[i] == '(')
					output += L" \\_\\ ";
				if (input[i] == ')')
					output += L"/_/  ";
				if (input[i] == '-')
					output += L"         ";
				if (input[i] == '_')
					output += L"|______| ";
				if (input[i] == '=')
					output += L"         ";
				if (input[i] == '+')
					output += L"        ";
				if (input[i] == '[')
					output += L"|___| ";
				if (input[i] == '{')
					output += L"  \\_\\ ";
				if (input[i] == ']')
					output += L"|___| ";
				if (input[i] == '}')
					output += L"/_/   ";
				if (input[i] == '|')
					output += L"|_| ";
				if (input[i] == '\\')
					output += L"    \\_\\ ";
				if (input[i] == ';')
					output += L"|/  ";
				if (input[i] == ':')
					output += L"(_) ";
				if (input[i] == '\'')
					output += L"    ";
				if (input[i] == '"')
					output += L"      ";
				if (input[i] == '<')
					output += L"  \\_\\ ";
				if (input[i] == ',')
					output += L"|/  ";
				if (input[i] == '>')
					output += L"/_/   ";
				if (input[i] == '/')
					output += L"/_/     ";
				if (input[i] == '?')
					output += L" (_)   ";
			}
			output += L"\n";
			return output;
		}

		/// <summary>
		/// a Unicode text generator which will create big text from normal wstring
		/// </summary>
		/// <param name="input">- The text to convert into big text</param>
		/// <param name="center">- if the output should be centered or not</param>
		/// <returns></returns>
		std::wstring BasicUnicodeTextGenerate(const std::wstring& input, bool center)
		{
			BasicUnicodeTextGenerate(GetStdHandle(STD_OUTPUT_HANDLE), input, center);
		}
	#pragma endregion
	}
}

#endif