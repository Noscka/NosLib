#ifndef _FILESEND_NOSLIB_HPP_
#define _FILESEND_NOSLIB_HPP_
#ifdef NOSLIB_USE_BOOST

#include <NosLib/Net/Definitions.hpp>

#include <boost/asio/ssl.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include <fstream>

namespace NosLib::Net
{
	template<typename SyncWriteStream>
	inline void SendFile(SyncWriteStream& socket, const std::string& filename, const uint64_t& startPos = 0)
	{
		std::ifstream iFStream(filename, std::ios::binary);

		uint64_t fileSize = boost::filesystem::file_size(boost::filesystem::path(filename));
		uint64_t sendSize = fileSize - startPos; /* file size taking into account amount already received by client */
		uint64_t segmentCount = sendSize / NosLib::Net::Definitions::SegmentSize; /* Amount of segments server has to send */
		uint64_t leftOver = sendSize % NosLib::Net::Definitions::SegmentSize; /* Amount of bytes left which will get sent separately */

		uint64_t CurrentOperationCount = 0; /* Storing current operation count */

		while (sendSize != 0)
		{
			/* Vector for sending the data gotten from file, is a pointer to not put the object on stack */
			std::vector<NosLib::Net::Definitions::Byte> DividedFileContents;
			uint64_t readPosition = -1;
			uint64_t readAmount = -1;

			if (CurrentOperationCount < segmentCount)
			{
				DividedFileContents = std::vector<NosLib::Net::Definitions::Byte>(NosLib::Net::Definitions::SegmentSize);

				readPosition = (CurrentOperationCount * NosLib::Net::Definitions::SegmentSize) + startPos;
				readAmount = NosLib::Net::Definitions::SegmentSize;

				CurrentOperationCount++;
			}
			else
			{
				DividedFileContents = std::vector<NosLib::Net::Definitions::Byte>(leftOver);

				readPosition = (segmentCount * NosLib::Net::Definitions::SegmentSize) + startPos;
				readAmount = leftOver;
			}

			iFStream.seekg(readPosition);
			iFStream.read(reinterpret_cast<char*>(DividedFileContents.data()), readAmount);

			/* write the vector into the socket stream for the client. also minus the amount of bytes sent from total */
			sendSize -= boost::asio::write(socket, boost::asio::buffer(DividedFileContents));
		}
	}
}

#endif // NOSLIB_USE_BOOST
#endif // _FILESEND_NOSLIB_HPP_