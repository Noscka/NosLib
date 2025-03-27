#include <NosLib/Net/FileInfo.hpp>

#include <openssl/evp.h>
#include <openssl/err.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace NosLib::Net
{
	NosString FileInfo::GetName() const
	{
		return FileName;
	}

	NosLib::Hash FileInfo::GetHash() const
	{
		return FileHash;
	}

	NosString FileInfo::GetHashString() const
	{
		return NosLib::GetHashString(FileHash);
	}

	FileInfo::FileActions FileInfo::GetAction() const
	{
		return FileAction;
	}

	NosString FileInfo::GetActionName() const
	{
		switch (GetAction())
		{
		case FileActions::Update:
			return "Update";
		case FileActions::Delete:
			return "Delete";
		}
		return "UNDEFINED";
	}

	uint64_t FileInfo::GetSize() const
	{
		return FileSize;
	}
}