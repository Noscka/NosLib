#ifndef _FILEMANAGEMENT_HPP_
#define _FILEMANAGEMENT_HPP_

#include <string>
#include <filesystem>

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
        private:
            std::wstring AbsolutePath; /* Absolute path to exe (without exe filename) */
            std::wstring RelativePath; /* Subpath from current path */
            std::wstring Filename; /* Filename */

            FilePath() {}
        public:
            /// <summary>
            /// Create FilePath Object
            /// </summary>
            /// <param name="relativePath">- path from current/absolute path</param>
            /// <param name="filename">- filename</param>
            FilePath(std::wstring relativePath, std::wstring filename)
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
        };
    }
}

#endif