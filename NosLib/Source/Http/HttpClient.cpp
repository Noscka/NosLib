#ifdef NOSLIB_USE_HTTPLIB
#include <NosLib/Http/HttpClient.hpp>
#include <NosLib/Logging.hpp>

#include <fstream>

namespace NosLib
{
	HttpClient::Ptr HttpClient::MakeClient(const std::string& host, const bool& enableServerCertVerification)
	{
		Ptr ret = std::make_unique<HttpClient>(host);

		ret->enable_server_certificate_verification(enableServerCertVerification);

		ret->set_follow_location(false);
		ret->set_keep_alive(false);
		ret->set_default_headers({ {"User-Agent", std::format("{} (cpp-httplib)", UserAgent).c_str()} });

		ret->set_logger(&LoggingFunction);

		return ret;
	}

	void HttpClient::SetUserAgent(const std::string& userAgent)
	{
		UserAgent = userAgent;
	}

	void HttpClient::SetUserAgent(const std::wstring& userAgent)
	{
		UserAgent = NosLib::String::ToString(userAgent);
	}

	bool HttpClient::DownloadFile(const std::string& urlPath, const std::string& filepath)
	{
		std::ofstream fileDownloadStream(filepath, std::ios::binary);

		httplib::Result res;
		{
			std::lock_guard<std::mutex> lock(DownloadMutex);
			res = Get(urlPath, [&](const char* data, size_t data_length)
			{
				fileDownloadStream.write(data, data_length);
				return true;
			});
		}

		if (!res)
		{
			fprintf(stderr, "%s\n", httplib::to_string(res.error()).c_str());

			#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
			auto result = get_openssl_verify_result();
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

	void HttpClient::LoggingFunction(const httplib::Request& req, const httplib::Response& res)
	{
		/* if is more then Debug */
		if (NosLib::Logging::GetVerboseLevel() > Logging::Verbose::Debug)
		{
			return;
		}

		std::string logOutput;
		logOutput += ("====================================================================================================================\nRequest\n");
		logOutput += (std::format(":METHOD: {}\n", req.method).c_str());
		logOutput += (std::format(":PATH:   {}\n", req.path).c_str());
		logOutput += (std::format(":BODY:   {}\n", req.body).c_str());

		logOutput += ("======HEADERS======\n");

		for (auto itr = req.headers.begin(); itr != req.headers.end(); itr++)
		{
			logOutput += (std::format("{} : {}\n", itr->first, itr->second).c_str());
		}
		logOutput += ("====================================================================================================================\nResponse\n");

		logOutput += (std::format(":STATUS: {}\n", res.status).c_str());
		logOutput += (std::format(":REASON: {}\n", res.reason).c_str());
		logOutput += (std::format(":BODY:   {}\n", res.body).c_str());
		logOutput += (std::format(":LOCATION:   {}\n", res.location).c_str());

		logOutput += ("======HEADERS======\n");

		for (auto itr = res.headers.begin(); itr != res.headers.end(); itr++)
		{
			logOutput += (std::format("{} : {}\n", itr->first, itr->second).c_str());
		}
		logOutput += ("====================================================================================================================\n\n\n");

		NosLib::Logging::CreateLog(NosLib::Logging::Severity::Debug, logOutput);
	}
}

#endif // NOSLIB_USE_HTTPLIB