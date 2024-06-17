#ifndef _HOSTPATH_NOSLIB_HPP_
#define _HOSTPATH_NOSLIB_HPP_

#include <NosLib/String.hpp>

#include <httplib.h>

namespace NosLib
{
	struct HostPath
	{
		std::wstring Host;
		std::wstring Path;

		HostPath() {}

		inline HostPath(const std::wstring& host, const std::wstring& path)
		{
			Host = host;
			Path = path;
		}

		inline HostPath(const std::wstring& link)
		{
			int slashCount = 0;

			for (int i = 0; i < link.length(); i++)
			{
				if (slashCount == 3)
				{
					Host = link.substr(0, i - 1);
					Path = link.substr(i - 1);
					break;
				}

				if (link[i] == L'/')
				{
					slashCount++;
				}
			}
		}

		inline HostPath(const std::string& host, const std::string& path)
			: HostPath(NosLib::String::ToWstring(host), NosLib::String::ToWstring(path))
		{}

		inline HostPath(const std::string& link)
			: HostPath(NosLib::String::ToWstring(link))
		{}

		inline std::wstring Full() const
		{
			return Host + Path;
		}

		inline bool operator==(HostPath right)
		{
			return (this->Host == right.Host) && (this->Path == right.Path);
		}
	};
}
#endif