#include "NosStdLib/Console.hpp"
//#include "NosStdLib/String.hpp"
//#include "NosStdLib/DynamicMenuSystem.hpp"
//#include "NosStdLib/DynamicLoadingScreen.hpp"

#include "AllHeaders.hpp"

#include <Windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <iostream> 
#include <cstdio> 
#include <conio.h>
#include <oleidl.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

class TestDropDrag : public IDropTarget
{
private:
public:
	TestDropDrag()
	{

	}

	~TestDropDrag()
	{

	}

	// basic IUnknown stuff
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject)
	{
		return S_OK;
	}

	ULONG STDMETHODCALLTYPE AddRef(void)
	{
		return 0;
	}

	ULONG STDMETHODCALLTYPE Release(void)
	{
		return 0;
	}

	// IDropTarget stuff
	virtual HRESULT DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		return S_OK;
	}

	virtual HRESULT DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		return S_OK;
	}

	virtual HRESULT DragLeave() override
	{
		return S_OK;
	}

	virtual HRESULT Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		wprintf(L"Dropped file\n");
		return S_OK;
	}
};

int main()
{
    NosStdLib::Console::InitializeModifiers::EnableUnicode();
    NosStdLib::Console::InitializeModifiers::EnableANSI();
    NosStdLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Drag and Drop Testing");
    NosStdLib::Console::InitializeModifiers::InitializeEventHandler();

    RegisterDragDrop(GetConsoleWindow(), new TestDropDrag());

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

    wprintf(L"Press any button to continue"); _getch();
    return 0;
}