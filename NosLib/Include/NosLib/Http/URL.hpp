#ifndef _URL_NOSLIB_HPP_
#define _URL_NOSLIB_HPP_
#ifdef NOSLIB_USE_HTTPLIB

#include <NosLib/Internal/Export.hpp>
#include <NosLib/String.hpp>

#include <httplib.h>

namespace NosLib
{
	class URL
	{
	protected: /* How a URL would be separated. e.g. https://BmwChadCentral.com/login */
		std::string Scheme; /* https:// */
		std::string Domain;	/* BmwChadCentral.com */
		std::string Subdir; /* /login */

	public:
		URL() {}

		/* Constructor when Scheme, Domain and Subdir are passed in separately */
		inline URL(const std::string& scheme, const std::string& domain, const std::string& subdir) :
			Scheme(scheme),
			Domain(domain),
			Subdir(subdir)
		{}

		/* constructor when Scheme and Domain are passed as Domain, and Subdir is separate */
		inline URL(const std::string& domain, const std::string& subdir) :
			Subdir(subdir)
		{
			ParseDomain(domain);
		}

		/* constructor when Scheme, Domain and Subdir are passed as 1 URL */
		inline URL(const std::string& url)
		{
			ParseURL(url);
		}

		std::string GetScheme() const; /* https:// */
		std::string GetDomain() const; /* BmwChadCentral.com */
		std::string GetSubdir() const; /* /login */
		std::string GetFullDomain() const; /* https://BmwChadCentral.com */
		std::string GetFullURL() const; /* https://BmwChadCentral.com/login */
		bool operator==(const URL&) const;

	protected:
		void ParseDomain(const std::string&); /* Parses host (https://domain.com) into scheme (https://) and domain (domain.com) */
		void ParseURL(const std::string&); /* Parses whole URL (https://domain.com/path) into scheme (https://), domain (domain.com), and subdir (/path) */
	};
}

#endif // NOSLIB_USE_HTTPLIB
#endif // _URL_NOSLIB_HPP_