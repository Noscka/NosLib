#include "NosLib/Console.hpp"
#include "NosLib/Functional.hpp"
#include "NosLib/DynamicMenuSystem.hpp"

#include <Windows.h>
#include <conio.h>

/* TODO: Figure out if it is worth it to change calling convention from default (__cdelc) to __fastcall */

// Custom implementation of the IDropTarget interface
class CustomDropTarget : public IDropTarget
{

private:
	ULONG m_refCount = 0; // Reference count

public:

	// Constructor
	CustomDropTarget() : m_refCount(1)
	{

	}

	// IDropTarget interface methods
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override
	{
		if (riid == IID_IUnknown || riid == IID_IDropTarget)
		{
			*ppvObject = static_cast<IDropTarget*>(this);
			AddRef();
			return S_OK;
		}

		*ppvObject = nullptr;
		return E_NOINTERFACE;
	}

	ULONG STDMETHODCALLTYPE AddRef() override
	{
		return InterlockedIncrement(&m_refCount);
	}

	ULONG STDMETHODCALLTYPE Release() override
	{
		ULONG refCount = InterlockedDecrement(&m_refCount);
		if (refCount == 0)
		{
			delete this;
		}
		return refCount;
	}

	HRESULT STDMETHODCALLTYPE DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		wprintf(L"Drag Enter\n");

		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		wprintf(L"Drag Over\n");

		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DragLeave() override
	{
		wprintf(L"Drag Leave\n");

		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect) override
	{
		wprintf(L"Drop\n");

		return S_OK;
	}
};

// Global instance of the IDropTarget interface
CustomDropTarget* g_pDropTarget = nullptr;

// Window procedure for the main window
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		// Register the custom window as a drop target
		if (g_pDropTarget)
		{
			RegisterDragDrop(hWnd, g_pDropTarget);
		}
		break;
	}
	case WM_DESTROY:
	{
		// Unregister the drop target before destroying the window
		if (g_pDropTarget)
		{
			RevokeDragDrop(hWnd);
		}
		PostQuitMessage(0);
		break;
	}
	// Handle other window messages as needed
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int main()
{
	NosLib::Console::InitializeModifiers::EnableUnicode();
	NosLib::Console::InitializeModifiers::EnableANSI();
	NosLib::Console::InitializeModifiers::BeatifyConsole<wchar_t>(L"Dynamic Array Updating");
	NosLib::Console::InitializeModifiers::InitializeEventHandler();

	//SetWindowLongPtr();

	// Create the main window
	HWND hWnd = CreateWindowEx(0, L"STATIC", nullptr, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, nullptr, nullptr);
	if (hWnd == nullptr)
	{
		// Handle window creation error
		return -1;
	}

	// Create an instance of the custom IDropTarget implementation
	g_pDropTarget = new CustomDropTarget();

	// Register the window class and show the window
	ShowWindow(hWnd, SW_SHOWNORMAL);

	// Register the custom window as a drop target
	if (g_pDropTarget)
	{
		RegisterDragDrop(hWnd, g_pDropTarget);
	}

	// Run the message loop
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Cleanup
	delete g_pDropTarget;

	wprintf(L"Press any button to continue"); _getch();
	return 0;
}