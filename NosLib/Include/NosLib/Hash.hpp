#ifndef _HASH_NOSLIB_HPP_
#define _HASH_NOSLIB_HPP_
#ifdef NOSLIB_USE_OPENSSL

#include "Internal/String.hpp"
#include "Internal/Export.hpp"

#ifdef NOSLIB_USE_BOOST
#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>
#endif // NOSLIB_USE_BOOST

#include <openssl/evp.h>
#include <array>

namespace NosLib
{
	struct Hash
	{
		std::array<uint8_t, EVP_MAX_MD_SIZE> HashBinary;
		uint32_t HashLenght;

		#ifdef NOSLIB_USE_BOOST
		template <typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& HashBinary;
			ar& HashLenght;
		}
		#endif // NOSLIB_USE_BOOST
	};

	Hash NOSLIB_API GetHash(const NosString&, const EVP_MD*);
	NosString NOSLIB_API GetHashString(const Hash&);
}

#endif // NOSLIB_USE_OPENSSL
#endif