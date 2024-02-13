#ifndef _ARRAYPOSITIONTRACK_NosLib_HPP_
#define _ARRAYPOSITIONTRACK_NosLib_HPP_

namespace NosLib
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

			void ModifyArrayPosition(const int& arrayPosition)
			{
				ArrayPosition = arrayPosition;
			}

			int* GetArrayPositionPointer()
			{
				return &ArrayPosition;
			}
		};
	}
}

#endif