#pragma once

#include "il2cpp.h"
#include "c_types.h"
#include "c_globals.h"

class c_unityengine
{
public:

	UnityEngine_Camera_o* camera = nullptr;

	enum PLAYER_BONES
	{
		EYE_LEFT = 9,
		EYE_RIGHT = 8,
		DICK = 0,
		PELVIS = 1,
		STOMACH = 2,
		LEFT_FOOT = 30,
		RIGHT_FOOT = 35,
		HEAD = 5,
		MOUTH = 7,
	};

	enum GAME_MODES
	{

		ReadyUpScene,
		ShootingRange,
		Brawl,
		TeamDeathMatch,
		Infected,
		KillConfirm,
		Randomizer,
		KingOfTheHill,
		GunGame

	};

	enum GLOW_MODES
	{

		OUTLINE_ALL,
		OUTLINE_VISIBLE,
		OUTLINE_HIDDEN,
		OUTLINE_AND_SILHOUETTE,
		SILHOUETTE_ONLY

	};
	
	UnityEngine_Bounds_o* get_bounds(void* collider)
	{

		if (!collider)
			return nullptr;

		using f_unity_engine_get_bounds = UnityEngine_Bounds_o * (__cdecl*)(void*);
		f_unity_engine_get_bounds get_bounds = nullptr;
		
		if (!get_bounds)
			get_bounds = reinterpret_cast<f_unity_engine_get_bounds>(globals.gameassembly_base + 0x1ADE310);

		return get_bounds(collider);

	}

	bool world_to_screen(vec3 in, vec2& out)
	{

		if (!camera)
			return false;

		if (!(UnityEngine_Camera_o*)camera || in == vec3{0, 0, 0})
			return false;

		vec3 pos = reinterpret_cast<vec3(__cdecl*)(void*, vec3)>(globals.gameassembly_base + 0x1BD4DE0)(camera, in);

		if (pos.z < 0)
			return false;

		ImVec2 screen_size = ImGui::GetIO().DisplaySize;
		out = vec2{ pos.x, screen_size.y - pos.y };
		return true;

	}

	DeveloperConsoleUI_o* get_developer_console()
	{

		// 0x346EA0

		return reinterpret_cast<DeveloperConsoleUI_o*(__cdecl*)()>(globals.gameassembly_base + 0x346EA0)();
		x
	}

	vec3 get_transform_position(UnityEngine_Transform_o* transform)
	{

		if (!transform || sizeof(*transform) != sizeof(UnityEngine_Transform_o))
			return vec3{};

		if (!transform->fields.m_CachedPtr)
			return vec3{};

		return reinterpret_cast<vec3(__cdecl*)(UnityEngine_Transform_o*)>(globals.gameassembly_base + 0x1C22A90)(transform);

	}

	void outline_display(ClientPlayer_o* player, col4 color, float width, int mode)
	{

		if (!player || !player->fields.outline)
			return;

		Outline_o* outline = player->fields.outline;

		if (!outline)
			return;

		outline->fields.outlineWidth = width;
		outline->fields.outlineMode = mode;

		return reinterpret_cast<void(__cdecl*)(ClientPlayer_o*, col4)>(globals.gameassembly_base + 0x393290)(player, color);

	}

	vec3 get_object_position(void* object)
	{

		if (!object)
			return vec3{};

		using f_unity_engine_game_object_get_transform = UnityEngine_Transform_o*(__cdecl*)(void*);
		f_unity_engine_game_object_get_transform unity_gameobject_get_transform = nullptr;

		if (!unity_gameobject_get_transform)
			unity_gameobject_get_transform = reinterpret_cast<f_unity_engine_game_object_get_transform>(globals.gameassembly_base + 0x1A83EB0);

		return get_transform_position(unity_gameobject_get_transform(object));

	}

	vec3 unity_vector_to_vec(UnityEngine_Vector3_o vector)
	{
		return vec3{vector.fields.x, vector.fields.y, vector.fields.z};
	}

	vec3 get_rigidbody_position(UnityEngine_Rigidbody_o* body)
	{

		if (!body)
			return vec3{};

		using f_unity_engine_rigidbody_get_position = vec3(__cdecl*)(UnityEngine_Rigidbody_o*);
		f_unity_engine_rigidbody_get_position unity_rigidbody_get_position = nullptr;

		if (!unity_rigidbody_get_position)
			unity_rigidbody_get_position = reinterpret_cast<f_unity_engine_rigidbody_get_position>(globals.gameassembly_base + 0x1AE3B90);

		return unity_rigidbody_get_position(body);

	}

	vec3 bone_position_at_index(System_Collections_Generic_List_Transform__o* bones, int index)
	{

		if (!bones || !bones->fields._items || !bones->fields._items->m_Items)
			return vec3{};

		if (sizeof(*bones) != sizeof(System_Collections_Generic_List_Transform__o))
			return vec3{};

		UnityEngine_Transform_o* transform = bones->fields._items->m_Items[index];

		if (!transform)
			return vec3{};

		return get_transform_position(transform);

	}

	UnityEngine_Rigidbody_o* get_collider_body(void* collider)
	{

		using f_unity_engine_collider_get_rigidbody = UnityEngine_Rigidbody_o*(__cdecl*)(void*);
		static f_unity_engine_collider_get_rigidbody unity_collider_get_body = nullptr;

		if (!unity_collider_get_body)
			unity_collider_get_body = reinterpret_cast<f_unity_engine_collider_get_rigidbody>(globals.gameassembly_base + 0x1ADE280);

		return unity_collider_get_body(collider);

	}

	UnityEngine_Renderer_o* get_renderer_component(UnityEngine_GameObject_o* object)
	{

		// https://www.unknowncheats.me/forum/unity/422949-findanyobject-il2cpp.html

		using get_type_from_handle = System_Type_o*(__stdcall*)(System_RuntimeTypeHandle_o);
		using get_type_handle = System_RuntimeTypeHandle_o(__stdcall*)(Il2CppObject*);

		using unity_renderer_constructor = UnityEngine_Renderer_o*(__cdecl*)();
		UnityEngine_Renderer_o* dummy = reinterpret_cast<unity_renderer_constructor>(globals.gameassembly_base + 0x326E40)();

		System_RuntimeTypeHandle_o handle = reinterpret_cast<get_type_handle>(globals.gameassembly_base + 0xE70790)((Il2CppObject*)dummy);
		System_Type_o* type = reinterpret_cast<get_type_from_handle>(globals.gameassembly_base + 0xE70720)(handle);

		using gameobject_get_component = UnityEngine_Renderer_o*(__cdecl*)(UnityEngine_GameObject_o*, System_Type_o*);
		return reinterpret_cast<gameobject_get_component>(globals.gameassembly_base + 0x1A838F0)(object, type);

	}

	boundingbox get_player_box(ClientPlayer_o* player)
	{

		if (!player)
			return boundingbox{};

		boundingbox box{};

		vec2 screen_origin;
		vec3 dick_pos = bone_position_at_index(player->fields.bones, DICK);

		if (!world_to_screen(dick_pos, screen_origin))
			return boundingbox{};

		vec2 screen_head;
		vec3 head_pos = get_transform_position(player->fields.head);
		if (!world_to_screen(head_pos, screen_head))
			return boundingbox{};

		vec2 screen_left_foot, screen_right_foot;
		vec3 left_foot = bone_position_at_index(player->fields.bones, LEFT_FOOT);
		vec3 right_foot = bone_position_at_index(player->fields.bones, RIGHT_FOOT);
		if (!world_to_screen(left_foot, screen_left_foot))
			return boundingbox{};
		if (!world_to_screen(right_foot, screen_right_foot))
			return boundingbox{};

		float height = screen_left_foot.y - screen_head.y;

		int size = 1;
		int width = 9;

		auto left_x = screen_origin.x - width - size;
		auto right_x = screen_origin.x + width + size;
		auto top_y = screen_head.y - size;
		auto bottom_y = screen_left_foot.y + size;

		box.top_left = { left_x, top_y };
		box.bottom_right = { right_x, bottom_y };
		box.valid = true;

		return box;

	}

	UnityEngine_GameObject_o* get_object_from_comp(void* transform)
	{

		using f_unity_engine_get_object_from_transform = UnityEngine_GameObject_o*(__cdecl*)(void*);
		static f_unity_engine_get_object_from_transform get_object_from_transform = nullptr;

		if (!get_object_from_transform)
			get_object_from_transform = reinterpret_cast<f_unity_engine_get_object_from_transform>(globals.gameassembly_base + 0x1A82590);

		return get_object_from_transform(transform);

	}

	vec3 get_transform_angles(UnityEngine_Transform_o* transform)
	{

		using f_unity_engine_get_angles_from_transform = vec3(__cdecl*)(UnityEngine_Transform_o*);
		static f_unity_engine_get_angles_from_transform get_angles_from_transform = nullptr;

		if (!get_angles_from_transform)
			get_angles_from_transform = reinterpret_cast<f_unity_engine_get_angles_from_transform>(globals.gameassembly_base + 0x1AAE2B0);

		return get_angles_from_transform(transform);

	}

	float to_radians(float degrees)
	{
		constexpr auto pi_radians = static_cast<float>(3.1415926535897932);
		constexpr auto pi_degrees = static_cast<float>(180.0);
		return (degrees * (pi_radians / pi_degrees));
	}

	NetworkManager_o* get_network_manager()
	{

		using network_manager_get_instance = NetworkManager_o * (__cdecl*)();
		NetworkManager_o* network_manager = reinterpret_cast<network_manager_get_instance>(globals.gameassembly_base + 0x3BA0C0)();

		return network_manager;

	}

	LobbyManager_o* get_lobby_manager()
	{

		return reinterpret_cast<LobbyManager_o*(__cdecl*)()>(globals.gameassembly_base + 0x3B10E0)();

	}

	bool in_game()
	{

		NetworkManager_o* network_manager = get_network_manager();

		if (!get_network_manager())
			return false;

		if (network_manager->fields.connecting || !network_manager->fields.game)
			return false;

		if (!camera || !camera->fields.m_CachedPtr)
			return false;

		if (!get_lobby_manager())
			return false;

		return true;

	}

	bool teammates_are_enemies()
	{

		if (!globals.local_player)
			return false;

		if (!get_network_manager())
			return false;

		uint16_t game_mode = get_network_manager()->fields.gameMode;

		if (game_mode == Brawl || game_mode == KillConfirm || game_mode == Randomizer || game_mode == GunGame)
			return true;

		return false;

	}

	WeaponManager_o* get_weapon_manager()
	{

		WeaponManager_c* klass = reinterpret_cast<WeaponManager_c*>(globals.gameassembly_base + 39425552);
		return klass->static_fields->Instance;

	}

	Riptide_Message_o* create_message(uint32_t send_mode, short id)
	{

		return reinterpret_cast<Riptide_Message_o * (__fastcall*)(uint32_t, short)>(globals.gameassembly_base + 0x111C250)(send_mode, id);

	}

}; inline c_unityengine unityengine;