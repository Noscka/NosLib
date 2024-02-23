#ifndef _HOSTPATH_NOSLIB_HPP_
#define _HOSTPATH_NOSLIB_HPP_

#include <httplib.h>

#include <string>

namespace NosLib
{
	struct HostPath
	{
		std::string Host;
		std::string Path;

		HostPath() {}

		HostPath(const std::string& host, const std::string& path)
		{
			Host = host;
			Path = path;
		}

		HostPath(const std::string& link)
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

		bool operator==(HostPath right)
		{
			return (this->Host == right.Host) && (this->Path == right.Path);
		}
	};
}
#endif