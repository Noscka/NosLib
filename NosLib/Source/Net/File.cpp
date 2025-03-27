#include <NosLib/File.hpp>

#ifdef NOSLIB_USE_BOOST
#include <boost/filesystem.hpp>
#endif // NOSLIB_USE_BOOST

#include <filesystem>
#include <algorithm>
#include <string>

/*
#ifdef _WIN32
#include <ShlObj_core.h>
#endif // _WIN32*/

namespace NosLib
{
	NosString File::GetPath() const
	{
		NosString absolutePath = std::filesystem::current_path().string();

		return Path;
	}

	NosString File::GetFilePath() const
	{
		return Path + Filename;
	}

	NosString File::GetFilename() const
	{
		return Filename;
	}

	NosString File::GetExtension() const
	{
		return Filename.substr(Filename.find_last_of(".") + 1);
	}

	#ifdef NOSLIB_USE_BOOST
	uint64_t File::GetFileSize() const
	{
		return boost::filesystem::file_size(GetFilePath());
	}
	#endif // NOSLIB_USE_BOOST

	#ifdef NOSLIB_USE_OPENSSL
	Hash File::GetHash(const EVP_MD* md) const
	{
		return NosLib::GetHash(GetFilePath(), md);
	}
	#endif // NOSLIB_USE_OPENSSL

	NosString File::NormalizePath(NosString path)
	{
		std::replace(path.begin(), path.end(), '\\', '/');
		return path;
	}
}


/*
HRESULT File::CreateFileShortcut(const LPCWSTR& lpszTargetfile, const LPCWSTR& lpszShortcutLocation, const LPCWSTR& lpszIconFile = L"", const int& iIconIndex = 0, const LPCWSTR& lpszDescription = L"", const LPCWSTR& lpszArgument = L"")
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