#ifndef _HTTPCLIENT_NOSLIB_HPP_
#define _HTTPCLIENT_NOSLIB_HPP_

#include <httplib.h>

#include <format>
#include <string>

namespace NosLib
{
	inline void LoggingFunction(const httplib::Request& req, const httplib::Response& res)
	{
		printf("====================================================================================================================\nRequest\n");
		printf(std::format(":METHOD: {}\n", req.method).c_str());
		printf(std::format(":PATH:   {}\n", req.path).c_str());
		printf(std::format(":BODY:   {}\n", req.body).c_str());

		printf("======HEADERS======\n");

		for (auto itr = req.headers.begin(); itr != req.headers.end(); itr++)
		{
			printf(std::format("{} : {}\n", itr->first, itr->second).c_str());
		}
		printf("====================================================================================================================\nResponse\n");

		printf(std::format(":STATUS: {}\n", res.status).c_str());
		printf(std::format(":REASON: {}\n", res.reason).c_str());
		printf(std::format(":BODY:   {}\n", res.body).c_str());
		printf(std::format(":LOCATION:   {}\n", res.location).c_str());

		printf("======HEADERS======\n");

		for (auto itr = res.headers.begin(); itr != res.headers.end(); itr++)
		{
			printf(std::format("{} : {}\n", itr->first, itr->second).c_str());
		}
		printf("====================================================================================================================\n\n\n");
	}

	inline httplib::Client MakeClient(const std::string& host, const std::string& userAgent)
	{
		httplib::Client ret(host);

		//ret.enable_server_certificate_verification(false);

		ret.set_follow_location(false);
		ret.set_keep_alive(false);
		ret.set_default_headers({ {"User-Agent", std::format("{}  (cpp-httplib)", userAgent).c_str()} });

#ifdef HeaderDebug
		ret.set_logger(&LoggingFunction);
#endif

		return ret;
	}
}
#endif