#ifndef _RGB_NOSLIB_HPP_
#define _RGB_NOSLIB_HPP_

#include <cstdint> 

namespace NosLib
{
	class RGB
	{
	public:
		uint8_t R, G, B;

		inline constexpr RGB() = default;
		inline constexpr RGB(uint8_t r, uint8_t g, uint8_t b) : R(r), G(g), B(b) {}

		inline RGB& Iterate(uint8_t iterateValue = 1)
		{
			if (R > 0 && B == 0)
			{
				R -= iterateValue;
				G += iterateValue;
			}
			if (G > 0 && R == 0)
			{
				G -= iterateValue;
				B += iterateValue;
			}
			if (B > 0 && G == 0)
			{
				R += iterateValue;
				B -= iterateValue;
			}

			return *this;
		}
	};
}
#endif