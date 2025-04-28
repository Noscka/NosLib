#ifndef _HTTPCLIENT_NOSLIB_HPP_
#define _HTTPCLIENT_NOSLIB_HPP_
#ifdef NOSLIB_USE_HTTPLIB

#include <NosLib/Internal/Export.hpp>

#include <httplib.h>

#include <memory>

namespace NosLib
{
	class NOSLIB_API HttpClient : public httplib::Client
	{
		using Client::Client; // Inherit constructors

	protected:
		std::mutex DownloadMutex;
		inline static std::string UserAgent;

	public:
		using Ptr = std::unique_ptr<HttpClient>;

		static Ptr MakeClient(const std::string& host, const bool& enableServerCertVerification = true);

		bool DownloadFile(const std::string& urlPath, const std::string& filepath);
		bool VerifyResult(const httplib::Result&);

	protected:
		static void LoggingFunction(const httplib::Request& req, const httplib::Response& res);
	};
}

#endif // NOSLIB_USE_HTTPLIB
#endif // _HTTPCLIENT_NOSLIB_HPP_