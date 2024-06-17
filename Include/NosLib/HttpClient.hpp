#ifndef _HTTPCLIENT_NOSLIB_HPP_
#define _HTTPCLIENT_NOSLIB_HPP_

#include "Logging.hpp"
#include "String.hpp"

#include <httplib.h>

#include <format>
#include <string>

namespace NosLib
{
	inline static std::string UserAgent = "";

	inline void LoggingFunction(const httplib::Request& req, const httplib::Response& res)
	{
		/* if is more then Debug */
		if (NosLib::Logging::GetVerboseLevel() > Logging::Verbose::Debug)
		{
			return;
		}

		std::string logOutput;
		logOutput+=("====================================================================================================================\nRequest\n");
		logOutput+=(std::format(":METHOD: {}\n", req.method).c_str());
		logOutput+=(std::format(":PATH:   {}\n", req.path).c_str());
		logOutput+=(std::format(":BODY:   {}\n", req.body).c_str());

		logOutput+=("======HEADERS======\n");

		for (auto itr = req.headers.begin(); itr != req.headers.end(); itr++)
		{
			logOutput+=(std::format("{} : {}\n", itr->first, itr->second).c_str());
		}
		logOutput+=("====================================================================================================================\nResponse\n");

		logOutput+=(std::format(":STATUS: {}\n", res.status).c_str());
		logOutput+=(std::format(":REASON: {}\n", res.reason).c_str());
		logOutput+=(std::format(":BODY:   {}\n", res.body).c_str());
		logOutput+=(std::format(":LOCATION:   {}\n", res.location).c_str());

		logOutput+=("======HEADERS======\n");

		for (auto itr = res.headers.begin(); itr != res.headers.end(); itr++)
		{
			logOutput+=(std::format("{} : {}\n", itr->first, itr->second).c_str());
		}
		logOutput+=("====================================================================================================================\n\n\n");

		NosLib::Logging::CreateLog<char>(logOutput, NosLib::Logging::Severity::Debug);
	}

	inline httplib::Client MakeClient(const std::string& host, const bool& enableServerCertVerification = true)
	{
		httplib::Client ret(host);

		ret.enable_server_certificate_verification(enableServerCertVerification);

		ret.set_follow_location(false);
		ret.set_keep_alive(false);
		ret.set_default_headers({ {"User-Agent", std::format("{} (cpp-httplib)", UserAgent).c_str()} });

		ret.set_logger(&LoggingFunction);

		return ret;
	}

	inline void SetUserAgent(const std::string& userAgent)
	{
		UserAgent = userAgent;
	}

	inline void SetUserAgent(const std::wstring& userAgent)
	{
		UserAgent = NosLib::String::ToString(userAgent);
	}
}
#endif