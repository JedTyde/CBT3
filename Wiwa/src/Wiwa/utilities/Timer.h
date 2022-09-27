#pragma once
#pragma warning(disable : 4251)

#include <Wiwa/Core.h>

#include <chrono>
#include <iostream>

namespace Wiwa {
	class WI_API Timer
	{
		std::chrono::time_point<std::chrono::steady_clock> start, end;

		std::chrono::duration<float> duration;

	public:
		Timer();
		~Timer();
	};
}
