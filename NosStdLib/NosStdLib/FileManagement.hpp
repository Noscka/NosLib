#ifndef _FILEMANAGEMENT_NOSSTDLIB_HPP_
#define _FILEMANAGEMENT_NOSSTDLIB_HPP_

#include <string>
#include <filesystem>

#include "Global.hpp"

namespace NosStdLib
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
            FilePath(const std::wstring relativePath, const std::wstring filename)
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
            std::wstring SetFilename(std::wstring newFilename)
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
    }
}
#endif