#pragma once

#include "c_types.h"
#include "c_unityengine.h"
#include "c_config.h"
#include "c_globals.h"

class c_esp
{
public:
	void run(ImDrawList* draw)
	{

        for (int idx = 0; idx < globals.client_player_list.size(); idx++)
        {

            ClientPlayer_o* client_player = globals.client_player_list.at(idx);

            if (!client_player || client_player->fields._Dead_k__BackingField || !client_player->fields.currentWeapon || !client_player->fields._Username_k__BackingField)
            {
                globals.client_player_list_mutex.lock();
                globals.client_player_list.erase(globals.client_player_list.begin() + idx);
                globals.client_player_list_mutex.unlock();
                continue;
            }

            if (client_player->fields.team == globals.local_player->fields.team)
                if (!config.show_teammates_on_esp && !unityengine.teammates_are_enemies())
                    continue;

            boundingbox player_box = unityengine.get_player_box(client_player);

            if (player_box.valid == false)
                continue;

            if (config.box_esp)
            {

                draw->AddRect(player_box.bottom_right, player_box.top_left, config.box_esp_color.to_u32());

            }

            if (config.skeleton_esp)
            {

                for (int i = 0; i < client_player->fields.bones->fields._size; i++)
                {

                    UnityEngine_Transform_o* transform = client_player->fields.bones->fields._items->m_Items[i];
                    UnityEngine_Transform_o* last_transform;
                    if (i == 0)
                        last_transform = client_player->fields.bones->fields._items->m_Items[i + 1];
                    else
                        last_transform = client_player->fields.bones->fields._items->m_Items[i - 1];

                    if (!transform || !last_transform)
                        continue;

                    vec2 screen_pos;
                    vec2 last_bone;

                    if (unityengine.world_to_screen(unityengine.get_transform_position(transform), screen_pos) && unityengine.world_to_screen(unityengine.get_transform_position(last_transform), last_bone))
                    {

                        int y_diff = screen_pos.y - last_bone.y;
                        int x_diff = screen_pos.x - last_bone.x;

                        if (y_diff > 50 || x_diff > 50)
                            continue;

                        draw->AddLine(last_bone, screen_pos, config.skeleton_esp_color.to_u32());

                    }

                }

            }

            if (config.name_esp)
            {

                const char* name = client_player->fields._Username_k__BackingField->fields.to_string().c_str();
                draw->AddText(ImVec2(player_box.top_left.x, player_box.top_left.y - (ImGui::CalcTextSize(name).y + 1)), config.name_esp_color.to_u32(), "player name");

            }

            if (config.health_esp)
            {

                if (config.health_esp_dynamic)
                    draw->AddText(ImVec2(player_box.top_left.x - 30, player_box.top_left.y), IM_COL32(100, (int)min(255.f, client_player->fields.health * 225.f / client_player->fields.maxHealth), 0, 255), std::to_string(client_player->fields.health).c_str());
                else
                    draw->AddText(ImVec2(player_box.top_left.x - 30, player_box.top_left.y), config.health_esp_color.to_u32(), std::to_string(client_player->fields.health).c_str());

            }

            if (config.weapon_esp)
            {

                const char* weapon_name = client_player->fields.currentWeapon->fields.name->fields.to_string().c_str();
                draw->AddText(ImVec2(player_box.top_left.x, player_box.bottom_right.y + (ImGui::CalcTextSize(weapon_name).y + 1)), config.weapon_esp_color.to_u32(), "weapon name");

            }

        }

	}
};

class c_features
{

public:
	c_esp esp;

}; inline c_features features;