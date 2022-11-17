#pragma once

#include <cstdint>

#include "il2cpp.h"
#include "c_globals.h"
#include "c_unityengine.h"
#include "c_config.h"

namespace c_offsets
{

	constexpr uintptr_t client_player_update = 0x3A2770;
	constexpr uintptr_t firearms_update = 0x3EEA00;
	constexpr uintptr_t firearms_create_bullet = 0x3E6680;
	constexpr uintptr_t bullet_init = 0x3E3AD0;
	constexpr uintptr_t gamemanager_update = 0x4018B0;
	constexpr uintptr_t networkmanager_cant_play = 0x3B19D0;
	constexpr uintptr_t anticheat_fixed_update = 0x3FBB50;
	constexpr uintptr_t weaponmanager_update = 0x3F9E30;

}

class c_hooking
{

public:

	uintptr_t o_client_player_update = NULL;
	uintptr_t o_client_enemy_fixed_update = NULL;
	uintptr_t o_server_player_update = NULL;
	uintptr_t o_firearms_update = NULL;
	uintptr_t o_firearms_create_bullet = NULL;
	uintptr_t o_bullet_init = NULL;
	uintptr_t o_gamemanager_update = NULL;
	uintptr_t o_chat_update = NULL;
	uintptr_t o_networkmanager_cant_play = NULL;
	uintptr_t o_anticheat_fixed_update = NULL;
	uintptr_t o_random_shit = NULL;
	uintptr_t o_grappling_update = NULL;
	uintptr_t o_weaponmanager_update = NULL;

}; inline c_hooking hooking;

using f_client_player_update = void(__cdecl*)(ClientPlayer_o*);
void __cdecl hk_client_player_update(ClientPlayer_o* self)
{

	if (unityengine.in_game())
	{

		if (!self || !self->fields.m_CachedPtr)
			return reinterpret_cast<f_client_player_update>(hooking.o_client_player_update)(self);

		bool in_list = false;

		for (auto cmp : globals.client_player_list)
			if (cmp == self)
				in_list = true;

		if (!in_list)
		{

			if (self->fields._IsLocal_k__BackingField)
				globals.local_player = self;
			else
			{
				globals.client_player_list_mutex.lock();
				globals.client_player_list.push_back(self);
				globals.client_player_list_mutex.unlock();
			}

		}

	}

	if (globals.local_player)
	{

		if (globals.local_player->fields.playerMovement)
		{

			if (config.enable_infinite_jump)
				globals.local_player->fields.playerMovement->fields.grounded = true;

			if (config.enable_speedhack)
			{

				globals.local_player->fields.playerMovement->fields.moveSpeedFactor = config.speedhack_amount;

			}
			//globals.local_player->fields.playerMovement->fields.surfing = true;

		}

	}

	if (!self)
		return reinterpret_cast<f_client_player_update>(hooking.o_client_player_update)(self);

	reinterpret_cast<f_client_player_update>(hooking.o_client_player_update)(self);

	if (unityengine.get_network_manager() && self->fields.team)
		if (config.glow_esp && !(self->fields.team == globals.local_player->fields.team && !unityengine.teammates_are_enemies()))
			unityengine.outline_display(self, config.glow_esp_color, config.glow_esp_width, config.glow_esp_mode);

	if (unityengine.get_network_manager())
	{

		if (config.instant_game_start)
		{
			unityengine.get_network_manager()->fields.leftTimeToStart = 0.f;
			unityengine.get_network_manager()->fields.gameStarted = true;
		}

		if (config.test_mode)
		{

			unityengine.get_network_manager()->fields.testMode = true;

		}

	}

}

using f_client_enemy_fixed_update = void(__cdecl*)(ClientEnemy_o*);
void __cdecl hk_client_enemy_fixed_update(ClientEnemy_o* self)
{

	return reinterpret_cast<f_client_enemy_fixed_update>(hooking.o_client_enemy_fixed_update)(self);

}

using f_firearms_update = void(__cdecl*)(Firearms_o*);
void __cdecl hk_firearms_update(Firearms_o* self)
{

	unityengine.camera = self->fields.PlayerCam;

	if (config.enable_infinite_ammo)
		self->fields.currentAmmo->set_value(2);
	if (config.enable_instant_kill)
		self->fields.damage = 9999;
	if (config.enable_rapid_fire)
		self->fields.fireRate = 60.f;
	if (config.enable_no_spread)
	{
		self->fields.spreadAngle = 0.f;
		self->fields.runSpread = 0.f;
		self->fields.normalSpread = 0.f;
	}

	if (config.enable_recoil_control)
	{

		self->fields.recoilX = 0.f;
		self->fields.recoilY = 0.f;
		self->fields.recoilZ = 0.f;

	}

	if (config.enable_no_pushback)
	{

		self->fields.force = 0.f;
		self->fields._multiplayerWeapon->fields.gunDrag = 0.f;
		self->fields.shootBack = 0.f;

	}

	self->fields.createBullet = true;

	return reinterpret_cast<f_firearms_update>(hooking.o_firearms_update)(self);

}

using f_firearms_create_bullet = void(__cdecl*)(Firearms_o*, vec3);
void __cdecl hk_firearms_create_bullet(Firearms_o* self, vec3 pos)
{

	if (unityengine.in_game() && config.enable_silent_aimbot)
	{

		for (auto player : globals.client_player_list)
		{

			if (!player || player->fields.team == globals.local_player->fields.team && !unityengine.teammates_are_enemies())
				continue;

			vec3 new_pos = unityengine.bone_position_at_index(player->fields.bones, unityengine.HEAD);

			reinterpret_cast<f_firearms_create_bullet>(hooking.o_firearms_create_bullet)(self, new_pos);

		}

		return;

	}

	return reinterpret_cast<f_firearms_create_bullet>(hooking.o_firearms_create_bullet)(self, pos);

}

using f_bullet_init = void(__cdecl*)(Bullet_o* self, vec3, float, int, UnityEngine_LayerMask_o*, bool, bool);
void __cdecl hk_bullet_init(Bullet_o* self, vec3 direction, float bullet_speed, int damage, UnityEngine_LayerMask_o* layer_mask, bool local, bool use_gravity)
{

	if (config.enable_infinite_penetration)
		self->fields.penetrationAmount = 9999.f;

	return reinterpret_cast<f_bullet_init>(hooking.o_bullet_init)(self, direction, config.enable_infinite_penetration ? 9999.f : bullet_speed, config.enable_instant_kill ? 9999.f : bullet_speed, layer_mask, local, false);

}

using f_gamemanager_update = void(__cdecl*)(Manager_GameManager_o*);
void __cdecl hk_gamemanager_update(Manager_GameManager_o* self)
{

	if (self->fields.admins->fields._items)
	{

		globals.steam_id = self->fields.admins->fields._items->m_Items[1];
		

	}

	return reinterpret_cast<f_gamemanager_update>(hooking.o_gamemanager_update)(self);

}

using f_chat_update = void(__cdecl*)(Chat_o*);
void __cdecl hk_chat_update(Chat_o* self)
{

	if (globals.steam_id != NULL && unityengine.get_network_manager() && globals.local_player)
	{

		self->fields.steamId = globals.steam_id;
		unityengine.get_network_manager()->fields.steamId.fields.m_SteamID = globals.steam_id;
		reinterpret_cast<void(__cdecl*)(ClientPlayer_o*, uint64_t)>(globals.gameassembly_base + 0x396240)(globals.local_player, globals.steam_id);

	}

	return reinterpret_cast<f_chat_update>(hooking.o_chat_update)(self);

}

using f_networkmanager_cant_play = bool(__cdecl*)(NetworkManager_o*);
bool __cdecl hk_networkmanager_cant_play(NetworkManager_o* self)
{

	return false;

}

using f_anticheat_fixed_update = void(__cdecl*)(Manager_AntiCheatDectect_o*);
void __cdecl hk_anticheat_fixed_update(Manager_AntiCheatDectect_o* self)
{

	self->klass->static_fields->banned = false;

	return;

}

using f_grappling_update = void(__cdecl*)(Lava_o*, void*);
void __cdecl hk_grappling_update(Lava_o* self, void* unused)
{

	return;

}

using f_weaponmanager_update = void(__cdecl*)(WeaponManager_o*);
void __cdecl hk_weaponmanager_update(WeaponManager_o* self)
{

	self->fields.TacticalThrowCount->set_value(999);

	return reinterpret_cast<f_weaponmanager_update>(hooking.o_weaponmanager_update)(self);

}