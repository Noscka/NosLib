#ifndef _HTTPUTILITIES_NOSLIB_HPP_
#define _HTTPUTILITIES_NOSLIB_HPP_

#include <httplib.h>

#include "HttpClient.hpp"
#include "HostPath.hpp"

#include <fstream>
#include <mutex>

namespace NosLib
{
	namespace HttpUtilities
	{
		inline bool DownloadFile(httplib::Client* client, const std::string& urlPath, const std::string& filepath)
		{
			std::ofstream fileDownloadStream(filepath, std::ios::binary);

			httplib::Result res = client->Get(urlPath, [&](const char* data, size_t data_length)
				{
					fileDownloadStream.write(data, data_length);
					return true;
				});

			if (!res)
			{
				fprintf(stderr, "%s\n", httplib::to_string(res.error()).c_str());

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
				auto result = client->get_openssl_verify_result();
				if (result)
				{
					fprintf(stderr, "verify error: %s\n", X509_verify_cert_error_string(result));
				}
#endif

				throw std::invalid_argument(std::format("Failed to connect to {}. Does the website?", urlPath).c_str());
				return false;
			}

			fileDownloadStream.close();
			return true;
		}

		inline bool DownloadFile(const std::string& url, const std::string& filepath, const bool& enableServerCertVerification = true)
		{
			NosLib::HostPath urlHostPath(url);

			httplib::Client client = NosLib::MakeClient(urlHostPath.Host, enableServerCertVerification);

			return DownloadFile(&client, urlHostPath.Path, filepath);
		}
	}
}

#endif