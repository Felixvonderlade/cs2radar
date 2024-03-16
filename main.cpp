#include "memory.h"

#include <thread>
#include <iostream>

// https://github.com/frk1/hazedumper/blob/master/csgo.hpp
namespace offsets
{
	constexpr auto localPlayer = 0x17371A8;
	constexpr auto entityList = 0x18C2D58;
	constexpr auto teamNum = 0x3CB;
	constexpr auto spotted = 0x8;
}


int main()
{
	const auto mem = Memory("cs2.exe");
	const auto client = mem.GetModuleAddress("client.dll");
	std::cout << "cs2 found!" << std::endl;
	std::cout << "client.dll -> " << "0x" << std::hex << client << std::dec << std::endl;

	//hack loop
	while (true) {
		const auto localPlayer = mem.Read<std::uintptr_t>(client + offsets::localPlayer);
		const auto LocalTeam = mem.Read<std::uintptr_t>(client + offsets::teamNum);

		for (auto i = 1; i <= 64; ++i) {
			const auto entity = mem.Read<std::uintptr_t>(client + offsets::entityList + i * 0x10);

			if (mem.Read<std::uintptr_t>(entity + offsets::teamNum) == LocalTeam)
				continue;

			mem.Write<bool>(entity + offsets::spotted, true);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}
