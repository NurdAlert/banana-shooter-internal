#pragma once

#include <Windows.h>
#include <cstdint>
#include <mutex>
#include <vector>

#include "il2cpp.h"

class c_globals
{

public:
	uintptr_t unityplayer_base = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");
	uintptr_t gameassembly_base = (uintptr_t)GetModuleHandleA("GameAssembly.dll");

	std::mutex client_player_list_mutex{};
	std::vector<ClientPlayer_o*> client_player_list{};
	ClientPlayer_o* local_player;

	std::mutex client_enemy_list_mutex{};
	std::vector<ClientEnemy_o*> client_enemy_list{};

	uint64_t steam_id = NULL;

}; inline c_globals globals;