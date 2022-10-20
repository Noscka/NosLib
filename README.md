# NosStdLib / Noscka Standard Library
NosStdLib or Noscka Standard Library is a library (like boost) made to expand on existing std C++ features, add new functions and classes for easier use and more. I have made this for personal use and so it won't be designed for "mass usage" but anyone is more then welcome to use, upgrade or add to the library. There is a [RULES.md](RULES.md) file for rules on naming, comments and anything else so I remember on how I should do certain things.

## library structers, functions and more.
this section will show the library structer, mostly for me when I want to search if I got a function or not. <br />
For example it will be shown like this. <br />
```
ExampleFolder
├ HeaderFileOne.hpp
└ HeaderFileTwo.hpp
	├ {Class} SomeClass
	│	│
	│	└ {Function} std::wstring SimpleClassFunction(std::wstring ReturnWstring)
	│
	└ {Function} std::wstring SimpleNoneClassFunction(std::wstring ReturnWstring)
```
<br />
(Will need to create a another program to automatically parse library structure for me) <br />
Here is the real structure <br />

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
	│
	└ {Namespace} Global
		├ {Namespace} String
		│	├ {Function} std::wstring ToWstring(std::string str)
		│	└ {Function} std::wstring ToString(std::wstring wstr)
		└ {Namespace} Console
			├ {Function} void ClearRange(int position, int range, wchar_t fillChar = L' ')
			└ {Function} void ClearScreen(wchar_t fillChar = L' ')
```