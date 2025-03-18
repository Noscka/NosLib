#ifdef NOSLIB_USE_OPENSSL
#include <NosLib/Hash.hpp>

#include <fstream>
#include <sstream>
#include <iomanip>

namespace NosLib
{
	Hash GetHash(const NosString& filePath, const EVP_MD* hashAlgo)
	{
		// Create a new digest context
		EVP_MD_CTX* ctx = EVP_MD_CTX_new();
		if (!ctx)
		{
			throw std::runtime_error("Failed to create digest context");
		}

		// Initialize the digest context with the provided algorithm
		if (!EVP_DigestInit_ex(ctx, hashAlgo, nullptr))
		{
			EVP_MD_CTX_free(ctx);
			throw std::runtime_error("EVP_DigestInit_ex failed");
		}

		// Open the file in binary mode
		std::ifstream file(filePath, std::ios::binary);
		if (!file)
		{
			EVP_MD_CTX_free(ctx);
			throw std::runtime_error("Cannot open file: " + filePath);
		}

		// Read the file in chunks and update the digest
		const size_t bufferSize = 4096;
		char buffer[bufferSize];
		while (file.good())
		{
			file.read(buffer, bufferSize);
			std::streamsize bytesRead = file.gcount();
			if (bytesRead > 0)
			{
				if (!EVP_DigestUpdate(ctx, buffer, bytesRead))
				{
					EVP_MD_CTX_free(ctx);
					throw std::runtime_error("EVP_DigestUpdate failed");
				}
			}
		}

		Hash returnHash{};
		if (!EVP_DigestFinal_ex(ctx, returnHash.HashBinary.data(), &returnHash.HashLenght))
		{
			EVP_MD_CTX_free(ctx);
			throw std::runtime_error("EVP_DigestFinal_ex failed");
		}
		EVP_MD_CTX_free(ctx);

		return returnHash;
	}

	NosString GetHashString(const Hash& hash)
	{
		std::ostringstream oss;
		for (unsigned int i = 0; i < hash.HashLenght; i++)
		{
			oss << std::hex << std::setw(2) << std::setfill('0')
				<< static_cast<int>(hash.HashBinary[i]);
		}
		return oss.str();
	}
}

#endif // NOSLIB_USE_OPENSSL