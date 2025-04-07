#ifndef _VERSION_NOSLIB_HPP_
#define _VERSION_NOSLIB_HPP_

#include <NosLib/Internal/Export.hpp>
#include <NosLib/Internal/String.hpp>

#include <boost/serialization/access.hpp>

#include <format>
#include <cstdint>

namespace NosLib
{
	class Version
	{
	private:
		#ifdef NOSLIB_USE_BOOST
		friend class boost::serialization::access;
		template<class Archive>
		inline void serialize(Archive& archive, const unsigned int)
		{
			archive& Major;
			archive& Minor;
			archive& Patch;
		}
		#endif // NOSLIB_USE_BOOST
	protected:
		uint16_t Major, Minor, Patch;

	public:
		Version() = default;
		Version(const uint16_t& major, const uint16_t& minor, const uint16_t& patch) :
			Major(major),
			Minor(minor),
			Patch(patch)
		{ }
		virtual ~Version() = default;

		inline uint16_t GetMajor() { return Major; }
		inline uint16_t GetMinor() { return Minor; }
		inline uint16_t GetPatch() { return Patch; }

		inline NosString GetVersion()
		{
			return std::format("{}.{}.{}", Major, Minor, Patch);
		}
	};
}
#endif // _VERSION_NOSLIB_HPP_