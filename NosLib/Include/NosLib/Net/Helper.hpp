#ifndef _BOOSTHELPER_NOSLIB_HPP_
#define _BOOSTHELPER_NOSLIB_HPP_
#ifdef NOSLIB_USE_BOOST

#include "Definitions.hpp"
#include <NosLib/Pointer.hpp>

#include <boost/asio.hpp>

namespace NosLib::Net
{
	inline std::string EndpointAsString(boost::asio::ip::tcp::endpoint Endpoint)
	{
		return std::format("{}:{}", Endpoint.address().to_v4().to_string(), std::to_string(Endpoint.port()));
	}

	inline std::string StreamBufferToString(const boost::asio::streambuf& streamBuffer, const size_t& bytesReceived)
	{
		auto bufSeqStart = boost::asio::buffers_begin(streamBuffer.data());
		auto bufSeqEnd = boost::asio::buffers_begin(streamBuffer.data()) + bytesReceived;
		return std::string(bufSeqStart, bufSeqEnd);
	}

	template<typename SyncWriteStream, typename BufferSequence>
	inline void SimpleWrite(SyncWriteStream& socket, BufferSequence& streamBuf)
	{
		uint64_t dataSize = static_cast<uint64_t>(streamBuf.size());

		Definitions::Byte dataSizeBytes[sizeof(dataSize)];
		std::copy(static_cast<const char*>(static_cast<const void*>(&dataSize)),
				  static_cast<const char*>(static_cast<const void*>(&dataSize)) + sizeof(dataSize),
				  dataSizeBytes);

		boost::asio::write(socket, boost::asio::buffer(dataSizeBytes));
		boost::asio::write(socket, streamBuf);
	}

	template<typename SyncWriteStream, typename BufferSequence>
	inline void SimpleWrite(SyncWriteStream& socket, BufferSequence&& streamBuf)
	{
		SimpleWrite(socket, streamBuf);
	}

	template<typename SyncWriteStream, typename BufferSequence>
	inline void SimpleRead(SyncWriteStream& socket, BufferSequence& streamBuf)
	{
		uint64_t dataSize = 0;
		Definitions::Byte dataSizeBytes[sizeof(dataSize)];

		boost::asio::streambuf headerBuf;
		boost::asio::read(socket, headerBuf, boost::asio::transfer_exactly(sizeof(dataSize)));
		std::istream is(&headerBuf);
		is.read(reinterpret_cast<char*>(dataSizeBytes), sizeof(dataSize));

		std::copy(dataSizeBytes,
				  dataSizeBytes + sizeof(dataSize),
				  reinterpret_cast<char*>(&dataSize));

		boost::asio::read(socket, streamBuf, boost::asio::transfer_exactly(dataSize));
	}

	template <class Class, class ... ClassArgs, typename SyncWriteStream>
	void SerializeSend(SyncWriteStream& socket, ClassArgs&& ... args)
	{
		typename Class::Base::Ptr outObject = std::make_unique<Class>(std::forward<ClassArgs>(args)...);

		boost::asio::streambuf reqBuf;
		Class::Base::Serialize(outObject, &reqBuf);
		SimpleWrite(socket, reqBuf);
	}

	template <class Class, typename SyncWriteStream>
	void SerializeSendPre(SyncWriteStream& socket, const typename Class::Ptr& classObject)
	{
		boost::asio::streambuf reqBuf;
		Class::Base::Serialize(static_cast<const typename Class::Base*>(classObject.get()), &reqBuf);
		SimpleWrite(socket, reqBuf);
	}

	template <class Class, class ... ClassArgs, typename SyncWriteStream>
	Class::Ptr DeserializeRead(SyncWriteStream& socket)
	{
		typename Class::Base::Ptr inObject;

		boost::asio::streambuf reqBuf;
		SimpleRead(socket, reqBuf);
		inObject = Class::Base::Deserialize(&reqBuf);

		return NosLib::Pointer::DynamicUniquePtrCast<Class, typename Class::Base>(std::move(inObject));
	}
}

#endif // NOSLIB_USE_BOOST
#endif // _BOOSTHELPER_NOSLIB_HPP_