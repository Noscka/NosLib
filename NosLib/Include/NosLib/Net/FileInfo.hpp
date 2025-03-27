#ifndef _FILEINFO_NOSLIB_HPP_
#define _FILEINFO_NOSLIB_HPP_
#ifdef NOSLIB_USE_BOOST

#include <NosLib/Boost/WinVersion.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/filesystem.hpp>

#include <NosLib/File.hpp>

#include <string>
#include <array>

namespace NosLib::Net
{
	class NOSLIB_API FileInfo
	{
	public:
		enum class FileActions : uint8_t
		{
			Update,
			Delete
		};

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& archive, const unsigned int)
		{
			archive& FileName;
			archive& FileHash;
			archive& FileAction;
			archive& FileSize;
		}

	protected:
		NosString FileName;
		NosLib::Hash FileHash;
		FileActions FileAction;
		uint64_t FileSize;

	public:
		FileInfo() = default;
		FileInfo(const NosString& fileName, const FileActions& fileAction = FileActions::Update) :
			FileName(fileName),
			FileHash(NosLib::GetHash(fileName, EVP_sha256())),
			FileAction(fileAction),
			FileSize(boost::filesystem::file_size(boost::filesystem::path(fileName)))
		{}

		FileInfo(const FileInfo& other, const FileActions& fileAction) :
			FileName(other.FileName),
			FileHash(other.FileHash),
			FileAction(fileAction),
			FileSize(other.FileSize)
		{}

		NosString GetName() const;
		NosLib::Hash GetHash() const;
		NosString GetHashString() const;
		FileActions GetAction() const;
		NosString GetActionName() const;
		uint64_t GetSize() const;
	};
}

#endif // NOSLIB_USE_BOOST
#endif // _FILEINFO_NOSLIB_HPP_