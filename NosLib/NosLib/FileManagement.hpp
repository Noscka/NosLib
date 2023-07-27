#ifndef _FILEMANAGEMENT_NosLib_HPP_
#define _FILEMANAGEMENT_NosLib_HPP_

#include "String.hpp"

#include <string>
#include <filesystem>
#include <ShlObj_core.h>

namespace NosLib
{
    /// <summary>
    /// namespace for file management
    /// </summary>
    namespace FileManagement
    {
        /// <summary>
        /// Class which allows for storing file paths
        /// </summary>
        class FilePath
        {
        protected:
            std::wstring AbsolutePath; /* Absolute path to exe (without exe filename) */
            std::wstring RelativePath; /* Subpath from current path */
            std::wstring Filename; /* Filename */

        public:
            /// <summary>
            /// Create FilePath Object with black members
            /// </summary>
            FilePath() {}

            /// <summary>
            /// Create FilePath Object
            /// </summary>
            /// <param name="relativePath">- path from current/absolute path</param>
            /// <param name="filename">- filename</param>
            FilePath(const std::wstring& relativePath, const std::wstring& filename)
            {
                RelativePath = relativePath;
                Filename = filename;
                AbsolutePath = std::filesystem::current_path().wstring();
            }

            /// <summary>
            /// Returns Absolute path without filename
            /// </summary>
            /// <returns>Absolute path without filename</returns>
            std::wstring GetAbsolutePath()
            {
                return AbsolutePath + RelativePath;
            }

            /// <summary>
            /// Returns Absolute path to file
            /// </summary>
            /// <returns>Absolute path to file</returns>
            std::wstring GetFilePath()
            {
                return GetAbsolutePath() + Filename;
            }

            /// <summary>
            /// Return just name of file
            /// </summary>
            /// <returns>filename</returns>
            std::wstring GetFilename()
            {
                return Filename;
            }

            /// <summary>
            /// Change the filename
            /// </summary>
            /// <param name="newFilename">- the new filename</param>
            /// <returns>filename</returns>
            std::wstring SetFilename(const std::wstring& newFilename)
            {
                Filename = newFilename;
                return Filename;
            }
        };

    #pragma region GetFileExtension
        /// <summary>
        /// Get file extention from filename
        /// </summary>
        /// <typeparam name="CharT">- string type</typeparam>
        /// <param name="filename">- filename to get extension from</param>
        /// <returns>filename extension</returns>
        template <typename CharT>
        std::basic_string<CharT> GetFileExtension(const std::basic_string<CharT>& filename)
        {
            return filename.substr(filename.find_last_of(L".") + 1);
        }
    #pragma endregion

    #pragma region GetFileSize
        /// <summary>
        /// Gets file size
        /// </summary>
        /// <typeparam name="CharT">- string type</typeparam>
        /// <param name="filename">- path to file</param>
        /// <returns>size as int</returns>
        template <typename CharT>
        int GetFileSize(const std::basic_string<CharT>& filePath)
        {
            struct stat stat_buf;
            int rc = stat(NosLib::String::ConvertString<char, CharT>(filePath).c_str(), &stat_buf);
            return rc == 0 ? stat_buf.st_size : -1;
        }
    #pragma endregion

        HRESULT CreateFileShortcut(const LPCWSTR& lpszTargetfile, const LPCWSTR& lpszShortcutLocation, const LPCWSTR& lpszIconFile = L"", const int& iIconIndex = 0, const LPCWSTR& lpszDescription = L"", const LPCWSTR& lpszArgument = L"")
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
		}
    }
}
#endif