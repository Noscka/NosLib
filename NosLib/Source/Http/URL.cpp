#include <NosLib/Http/URL.hpp>

#include <NosLib/Logging.hpp>

using NosLog = NosLib::Logging;

namespace NosLib
{
	std::string URL::GetScheme() const
	{
		return Scheme;
	}

	std::string URL::GetDomain() const
	{
		return Domain;
	}

	std::string URL::GetSubdir() const
	{
		return Subdir;
	}

	std::string URL::GetFullDomain() const
	{
		return Scheme + Domain;
	}

	std::string URL::GetFullURL() const
	{
		return Scheme + Domain + Subdir;
	}

	bool URL::operator==(const URL& right) const
	{
		return (this->Scheme == right.Scheme) &&
			   (this->Domain == right.Domain) &&
			   (this->Subdir == right.Subdir);
	}

	void URL::ParseDomain(const std::string& domain)
	{
		/* https://domain.com
		 * [scheme][domain  ]
		 * Separated with ://
		 * */

		constexpr std::string_view schemeSeperator("://");

		std::size_t pos = domain.find(schemeSeperator);

		if (pos == std::string::npos)
		{
			NosLog::CreateLog(NosLog::Severity::Error, "Unable to find scheme/domain separator for {}", domain);
			Domain = domain;
			return;
		}
		
		std::size_t shemeSubstrSize = pos + schemeSeperator.size();
		Scheme = domain.substr(0, shemeSubstrSize);
		Domain = domain.substr(shemeSubstrSize);
	}

	void URL::ParseURL(const std::string& url)
	{
		/* https://domain.com/account/management
		 * [scheme][domain  ][subdir           ]
		 * [ParseDomain     ]
		 * */

		constexpr std::string_view subdirSeperator("/");

		/* Will Separate scheme and domain. but not domain and subdir */
		ParseDomain(url);
		std::string combinedDomainSubdir = Domain;

		std::size_t pos = combinedDomainSubdir.find(subdirSeperator);

		if (pos == std::string::npos)
		{
			NosLog::CreateLog(NosLog::Severity::Warning, "Unable to find domain/subdir separator (first /) for {}", combinedDomainSubdir);
			Domain = combinedDomainSubdir;
			return;
		}

		std::size_t domainSubstrSize = (pos + subdirSeperator.size()) - 1; /* -1 because the / should be with path path instead of domain */
		Domain = combinedDomainSubdir.substr(0, domainSubstrSize);
		Subdir = combinedDomainSubdir.substr(domainSubstrSize);
	}
}