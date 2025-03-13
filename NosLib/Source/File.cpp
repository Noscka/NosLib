#include <NosLib/File.hpp>

#include <filesystem>

/*
#ifdef _WIN32
#include <ShlObj_core.h>
#endif // _WIN32*/

NosLib::NosString NosLib::File::GetPath() const
{
	NosString absolutePath = std::filesystem::current_path().string();

	return Path;
}

NosLib::NosString NosLib::File::GetFilePath() const
{
	return Path + Filename;
}

NosLib::NosString NosLib::File::GetFilename() const
{
	return Filename;
}

NosLib::NosString NosLib::File::GetExtension() const
{
	return Filename.substr(Filename.find_last_of(".") + 1);
}

uint64_t NosLib::File::GetFileSize() const
{
	struct stat stat_buf;
	int rc = stat(GetFilePath().c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

#ifdef NOSLIB_USE_OPENSSL
NosLib::Hash NosLib::File::GetHash(const EVP_MD* md) const
{
	return NosLib::GetHash(GetFilePath(), md);
}
#endif // NOSLIB_USE_OPENSSL

/*
HRESULT NosLib::File::CreateFileShortcut(const LPCWSTR& lpszTargetfile, const LPCWSTR& lpszShortcutLocation, const LPCWSTR& lpszIconFile = L"", const int& iIconIndex = 0, const LPCWSTR& lpszDescription = L"", const LPCWSTR& lpszArgument = L"")
{
	(void)CoInitialize(NULL);
	HRESULT hres;
	IShellLinkW* psl;

	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&psl));
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		std::filesystem::path resolvedFilePath = std::filesystem::canonical(lpszTargetfile).wstring();

		psl->SetPath(resolvedFilePath.c_str());
		psl->SetWorkingDirectory(resolvedFilePath.remove_filename().c_str());
		psl->SetIconLocation(std::filesystem::canonical(lpszIconFile).c_str(), iIconIndex);
		psl->SetArguments(lpszArgument);
		psl->SetDescription(lpszDescription);

		// Query IShellLink for the IPersistFile interface, used for saving the
		// shortcut in persistent storage.
		hres = psl->QueryInterface(IID_PPV_ARGS(&ppf));
		if (SUCCEEDED(hres))
		{
			// Save the link by calling IPersistFile::Save.
			hres = ppf->Save(lpszShortcutLocation, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
	(void)CoUninitialize();
	return hres;
}*/