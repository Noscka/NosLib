# NosStdLib / Noscka Standard Library
NosStdLib or Noscka Standard Library is a library (like boost) made to expand on existing std C++ features, add new functions and classes for easier use and more. I have made this for personal use and so it won't be designed for "mass usage" but anyone is more then welcome to use, upgrade or add to the library. There is a [RULES.md](RULES.md) file for rules on naming, comments and anything else so I remember on how I should do certain things.

## Sub-projects included
 - [Console Dynamic Menu](Sub-Project-Markdown/DynamicMenuSystem.md)
 - [Console Dynamic Loading Screen](Sub-Project-Markdown/DynamicLoadingScreen.md)
 - [Dynamic Array](Sub-Project-Markdown/DynamicArray.md)

## library structers, functions and more.
this section will show the library structer, mostly for me when I want to search if I got a function or not. <br />
For example it will be shown like this. <br />
```
ExampleFolder
├ HeaderFileOne.hpp
└ HeaderFileTwo.hpp
	├ {Class} SomeClass
	│	└ {Function} std::wstring SimpleClassFunction(std::wstring ReturnWstring)
	└ {Function} std::wstring SimpleNoneClassFunction(std::wstring ReturnWstring)
```
<br />
(Will need to create a another program to automatically parse library structure for me) <br />
Here is the real structure <br />
NOT UP TO DATE. NEED TO CREATE PARSER IN ORDER TO PARSE AND OUTPUT A STRUCTER TREE
<br />

```
NosStdLib
├ FileManagement.hpp
│	└ {Namespace} FileManagement
│		└ {Class} FilePath
│			├ {Function} FilePath(std::wstring relativePath, std::wstring filename)
│			├ {Function} std::wstring GetAbsolutePath()
│			├ {Function} std::wstring GetFilePath()
│			└ {Function} std::wstring GetFilename()
└ Global.hpp
	└ {Namespace} Global
		├ {Namespace} String
		│	├ {Function} std::wstring ToWstring(std::string str)
		│	├ {Function} std::wstring ToString(std::wstring wstr)
		│	├ {Function} bool IsNumber(const std::wstring& wstr, bool allowSigns = false)
		│	└ {Function} bool IsNumber(const std::string& str, bool allowSigns = false) !overload!
		└ {Namespace} Console
			├ {Function} COORD GetConsoleCaretPosition(HANDLE ConsoleHandle)
			├ {Function} COORD GetConsoleCaretPosition() !overload!
			├ {Function} void ClearRange(HANDLE ConsoleHandle, int position, int range, wchar_t fillChar = L' ')
			├ {Function} void ClearRange(int position, int range, wchar_t fillChar = L' ') !overload!
			├ {Function} void ClearScreen(HANDLE ConsoleHandle, wchar_t fillChar = L' ')
			├ {Function} void ClearScreen(wchar_t fillChar = L' ') !overload!
			├ {Function} void ShowCaret(HANDLE ConsoleHandle, bool showFlag)
			└ {Function} void ShowCaret(bool showFlag) !overload!
```
