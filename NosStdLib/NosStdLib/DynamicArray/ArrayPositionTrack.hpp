#ifndef _ARRAYPOSITIONTRACK_NOSSTDLIB_HPP_
#define _ARRAYPOSITIONTRACK_NOSSTDLIB_HPP_

namespace NosStdLib
{
	namespace ArrayPositionTrack
	{
		/// <summary>
		/// Has to be a inherited. allows for tracking current position in DynamicArray array
		/// </summary>
		class PositionTrack
		{
		protected:
			int ArrayPosition = -1;
		public:
			PositionTrack(){}

			PositionTrack(const int& arrayPosition)
			{
				ArrayPosition = arrayPosition;
			}

			void ChangePosition(const int& arrayPosition)
			{
				ArrayPosition = arrayPosition;
			}

			int GetPosition()
			{
				return ArrayPosition;
			}
		};
	}
}

#endif