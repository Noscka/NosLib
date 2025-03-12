#include <NosLib/File.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <ShlObj_core.h>
#endif // _WIN32

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
NosLib::NosString NosLib::File::GetHash(const EVP_MD* md) const
{
	return GetHash(GetFilePath(), md);
}

NosLib::NosString NosLib::File::GetHash(const NosLib::NosString& filePath, const EVP_MD* md)
{
	// Create a new digest context
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
	if (!ctx)
	{
		throw std::runtime_error("Failed to create digest context");
	}

	// Initialize the digest context with the provided algorithm
	if (!EVP_DigestInit_ex(ctx, md, nullptr))
	{
		EVP_MD_CTX_free(ctx);
		throw std::runtime_error("EVP_DigestInit_ex failed");
	}

	// Open the file in binary mode
	std::ifstream file(filePath, std::ios::binary);
	if (!file)
	{
		EVP_MD_CTX_free(ctx);
		throw std::runtime_error("Cannot open file: " + filePath);
	}

	// Read the file in chunks and update the digest
	const size_t bufferSize = 4096;
	char buffer[bufferSize];
	while (file.good())
	{
		file.read(buffer, bufferSize);
		std::streamsize bytesRead = file.gcount();
		if (bytesRead > 0)
		{
			if (!EVP_DigestUpdate(ctx, buffer, bytesRead))
			{
				EVP_MD_CTX_free(ctx);
				throw std::runtime_error("EVP_DigestUpdate failed");
			}
		}
	}

	// Finalize the digest
	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hashLen = 0;
	if (!EVP_DigestFinal_ex(ctx, hash, &hashLen))
	{
		EVP_MD_CTX_free(ctx);
		throw std::runtime_error("EVP_DigestFinal_ex failed");
	}
	EVP_MD_CTX_free(ctx);

	// Convert the binary digest to a hex string
	std::ostringstream oss;
	for (unsigned int i = 0; i < hashLen; i++)
	{
		oss << std::hex << std::setw(2) << std::setfill('0')
			<< static_cast<int>(hash[i]);
	}
	return oss.str();
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