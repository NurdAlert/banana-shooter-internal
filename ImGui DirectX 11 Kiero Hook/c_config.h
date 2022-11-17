#pragma once

#include <filesystem>
#include <fstream>

#include "jsoncpp/json.h"
#include "jsoncpp/json-forwards.h"

#include "c_types.h"

struct c_config_item {
	std::string name;
	std::string path;
	int id;
};

class c_config
{
public:

	std::vector<c_config_item> configs{};

	bool name_esp = false;
	col4 name_esp_color{1.f, 1.f, 1.f, 1.f};

	bool box_esp = true;
	col4 box_esp_color{1.f, 1.f, 1.f, 1.f};

	bool skeleton_esp = false;
	col4 skeleton_esp_color{1.f, 1.f, 1.f, 1.f};

	bool weapon_esp = false;
	col4 weapon_esp_color{1.f, 1.f, 1.f, 1.f};

	bool health_esp = false;
	bool health_esp_dynamic = true;
	col4 health_esp_color{1.f, 1.f, 1.f, 1.f};

	bool glow_esp = true;
	col4 glow_esp_color{1.f, 1.f, 1.f, 1.f};
	int glow_esp_mode = 0;
	float glow_esp_width = 1.f;
	bool glow_fill = true;

	bool show_teammates_on_esp = false;

	bool enable_infinite_ammo = false;
	bool enable_rapid_fire = false;
	
	bool enable_recoil_control = true;
	float recoil_control = 0.5f;

	bool enable_silent_aimbot = false;
	bool enable_no_pushback = false;
	bool enable_instant_kill = false;
	bool enable_no_spread = false;
	bool enable_infinite_penetration = false;
	bool enable_instant_bullet = false;

	bool enable_speedhack = false;
	float speedhack_amount = 10.f;

	bool enable_infinite_jump = false;
	bool avoid_lava = true;
	bool instant_game_start = true;
	bool test_mode = false;
	bool info_panel = false;

	void refresh_configs()
	{

		configs.clear();

		int cur_id = -1;
		std::string script_folder = "C:\\remade\\configs\\";

		for (auto& entry : std::filesystem::directory_iterator(script_folder))
		{

			if (entry.path().extension() == ".json") // filters out everything that's not a lua file
			{

				std::filesystem::path file_path = entry.path();
				std::string file_name = file_path.filename().string();
				std::string file_path_string = file_path.string();
				cur_id += 1;

				c_config_item config = { file_name, file_path_string, cur_id };

				configs.emplace_back(config);

			}

		}

	}

	void load_config(int id)
	{

		std::ifstream config_file(configs.at(id).path, std::ifstream::binary);
		Json::Value config_text;
		config_file >> config_text;

		#define gb(a) config_text[a].asBool(); 
		#define gf(a) config_text[a].asFloat();
		#define gi(a) config_text[a].asInt();
		#define gc(a) from_u32(config_text[a].asUInt());

		name_esp = gb("name_esp")
		name_esp_color = gc("name_esp_color")

		box_esp = gb("box_esp")
		box_esp_color = gc("box_esp_color")

		weapon_esp = gb("weapon_esp")
		weapon_esp_color = gc("weapon_esp_color")

		health_esp = gb("health_esp")
		health_esp_dynamic = gb("health_esp_dynamic")
		health_esp_color = gc("health_esp_color")

		skeleton_esp = gb("skeleton_esp")
		skeleton_esp_color = gc("skeleton_esp_color")

		glow_esp = gb("glow_esp")
		glow_esp_color = gc("glow_esp_color")
		glow_esp_mode = gi("glow_esp_mode")
		glow_esp_width = gf("glow_esp_width")

		show_teammates_on_esp = gb("show_teammates_on_esp")

		enable_infinite_ammo = gb("enable_infinite_ammo")
		enable_rapid_fire = gb("enable_rapid_fire")
		enable_recoil_control = gb("enable_recoil_control")

	    enable_silent_aimbot = gb("enable_silent_aimbot")
		enable_no_pushback = gb("enable_no_pushback")
		enable_instant_kill = gb("enable_instant_kill")
		enable_no_spread = gb("enable_no_spread")
		enable_infinite_penetration = gb("enable_infinite_penetration")
		enable_instant_bullet = gb("enable_instant_bullet")

		enable_speedhack = gb("enable_speedhack")
		speedhack_amount = gf("speedhack_amount")

		enable_infinite_jump = gb("enable_infinite_jump")
		avoid_lava = gb("avoid_lava")

	}

	void create_config(const char* config_name)
	{

		std::string path = "C:\\remade\\configs\\";
		path += config_name;
		path += ".json";

		std::ofstream file(path);
		refresh_configs();

	}

	void delete_config(int id)
	{

		remove(configs.at(id).path.c_str());
		refresh_configs();

	}

	void save_config(int id)
	{

		std::ofstream config_file(configs.at(id).path, std::ifstream::binary);
		Json::Value config_data;

		#define tf(a, v) config_data[a] = v; 

		tf("name_esp", name_esp)
		tf("name_esp_color", name_esp_color.to_u32())

		tf("box_esp", box_esp)
		tf("box_esp_color", box_esp_color.to_u32())

		tf("skeleton_esp", skeleton_esp)
		tf("skeleton_esp_color", skeleton_esp_color.to_u32())

		tf("weapon_esp", weapon_esp)
		tf("weapon_esp_color", weapon_esp_color.to_u32())

		tf("health_esp", health_esp)
		tf("health_esp_dynamic", health_esp_dynamic)
		tf("health_esp_color", health_esp_color.to_u32())

		tf("glow_esp", glow_esp)
		tf("glow_esp_color", glow_esp_color.to_u32())
		tf("glow_esp_mode", glow_esp_mode)
		tf("glow_esp_width", glow_esp_width)

		tf("show_teammates_on_esp", show_teammates_on_esp)

		tf("enable_infinite_ammo", enable_infinite_ammo)
		tf("enable_rapid_fire", enable_rapid_fire)

		tf("enable_recoil_control", enable_recoil_control)

		tf("enable_silent_aimbot", enable_silent_aimbot)
		tf("enable_no_pushback", enable_no_pushback)
		tf("enable_instant_kill", enable_instant_kill)
		tf("enable_no_spread", enable_no_spread)
		tf("enable_infinite_penetration", enable_infinite_penetration)
		tf("enable_instant_bullet", enable_instant_bullet)

		tf("enable_speedhack", enable_speedhack)
		tf("speedhack_amount", speedhack_amount)

		tf("enable_infinite_jump", enable_infinite_jump)
		tf("avoid_lava", avoid_lava)

		config_file.clear();

		config_file << config_data;

		config_file.close();

	}

}; inline c_config config;