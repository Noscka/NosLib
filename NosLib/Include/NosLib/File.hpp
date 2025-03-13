#ifndef _FILE_NOSLIB_HPP_
#define _FILE_NOSLIB_HPP_

#include "String.hpp"
#include "Internal/String.hpp"
#include "Internal/Export.hpp"

#include <NosLib/Hash.hpp>

namespace NosLib
{
	class NOSLIB_API File
	{
	protected:
		NosString Path; /* Relative Path  */
		NosString Filename; /* Filename */

	public:
		File() = default;
		inline File(const NosString& path, const NosString& filename) :
			Path(path),
			Filename(filename)
		{}

		NosString GetPath() const;
		NosString GetFilePath() const;
		NosString GetFilename() const;
		NosString GetExtension() const;

		#ifdef NOSLIB_USE_BOOST
		uint64_t GetFileSize() const;
		#endif // NOSLIB_USE_BOOST

		#ifdef NOSLIB_USE_OPENSSL
		Hash GetHash(const EVP_MD*) const;
		#endif // NOSLIB_USE_OPENSSL

		/*#ifdef _WIN32
		HRESULT CreateFileShortcut(const LPCWSTR& lpszTargetfile, const LPCWSTR& lpszShortcutLocation, const LPCWSTR& lpszIconFile = L"", const int& iIconIndex = 0, const LPCWSTR& lpszDescription = L"", const LPCWSTR& lpszArgument = L"");
		#endif // _WIN32*/
	};
}
#endif