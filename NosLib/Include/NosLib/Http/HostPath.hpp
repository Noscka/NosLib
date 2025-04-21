#ifndef _HOSTPATH_NOSLIB_HPP_
#define _HOSTPATH_NOSLIB_HPP_
#ifdef NOSLIB_USE_HTTPLIB

#include <NosLib/Internal/Export.hpp>
#include <NosLib/String.hpp>

#include <httplib.h>

namespace NosLib
{
	class HostPath
	{
	protected:
		std::string Host;
		std::string Path;

	public:
		HostPath() {}

		inline HostPath(const std::string& host, const std::string& path) :
			Host(host),
			Path(path)
		{}

		inline HostPath(const std::string& link)
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

		inline std::string GetHost() const
		{
			return Host;
		}

		inline std::string GetPath() const
		{
			return Path;
		}

		inline std::string GetFull() const
		{
			return Host + Path;
		}

		inline bool operator==(HostPath right)
		{
			return (this->Host == right.Host) && (this->Path == right.Path);
		}
	};
}

#endif // NOSLIB_USE_HTTPLIB
#endif // _HOSTPATH_NOSLIB_HPP_