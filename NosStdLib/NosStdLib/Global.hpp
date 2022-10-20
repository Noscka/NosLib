#ifndef _GLOBAL_HPP_
#define _GLOBAL_HPP_

#include <string>
#include <filesystem>

namespace NosStdLib
{
    class FilePathStorage
    {
    private:
        std::wstring AbsolutePath;
        std::wstring SubPath;
        std::wstring Filename;

        FilePathStorage() {}
    public:
        FilePathStorage(std::wstring subPath, std::wstring filename)
        {
            SubPath = subPath;
            Filename = filename;
            AbsolutePath = std::filesystem::current_path().wstring();
        }

        std::wstring GetSubPath()
        {
            return AbsolutePath + SubPath;
        }

        std::wstring GetFilePath()
        {
            return GetSubPath() + Filename;
        }

        std::wstring GetFileName()
        {
            return Filename;
        }
    };
}
#endif