#ifndef _ERRORCODES_NOSLIB_HPP_
#define _ERRORCODES_NOSLIB_HPP_

#include <NosLib/Internal/Export.hpp>
#include <system_error>
#include <string>

namespace NosLib
{
	enum class NOSLIB_API GenericErrors : uint8_t
	{
		Successful,
		Casting,
		NullPointer
	};

	class NOSLIB_API GenericErrorCategory : public std::error_category
	{
	public:
		static const GenericErrorCategory& instance()
		{
			static GenericErrorCategory inst; // Guaranteed to be instantiated once.
			return inst;
		}

		const char* name() const noexcept override
		{
			return "GenericErrorCategory";
		}

		std::string message(int ev) const override
		{
			switch (static_cast<GenericErrors>(ev))
			{
			case GenericErrors::Successful:
				return "Successful";

			case GenericErrors::Casting:
				return "Couldn't cast";

			case GenericErrors::NullPointer:
				return "Null Pointer";
			}

			return "Unknown error";
		}
	};

	inline std::error_code NOSLIB_API make_error_code(NosLib::GenericErrors e) noexcept
	{
		return { static_cast<int>(e), NosLib::GenericErrorCategory::instance() };
	}
}

namespace std
{
	template <>
	struct is_error_code_enum<NosLib::GenericErrors> : true_type {};
}

#endif // _ERRORCODES_NOSLIB_HPP_ 