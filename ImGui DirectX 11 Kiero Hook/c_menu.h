#pragma once

#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_win32.h"
#include "merge/byte.h"
#include "merge/custom.h"
#include "c_globals.h"
#include "c_config.h"
#include <string>

#include <D3DX11.h>

enum TABS
{
    TAB_VISUALS,
    TAB_WEAPON,
    TAB_MISC,
    TAB_CONFIG
};

#define ALPHA (ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)
#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)

#define START_LEFT_CHILD ImGui::SetCursorPos({ 191, 11 });
#define START_RIGHT_CHILD ImGui::SetCursorPos({ 424, 11 });

#define OFFSET_LEFT_CHILD(x) ImGui::SetCursorPos({ 191, 19+x });
#define OFFSET_RIGHT_CHILD(x) ImGui::SetCursorPos({ 424, 19+x });

#define MAX_CHILD_SIZE ImVec2(457, 493)
#define CHILD_X_SIZE 224

class c_menu
{
public:

    ImFont* combo_arrow;
    ImFont* medium_bigger;
    ImFont* weapons;
    ImFont* medium;
    ImFont* bold;
    ID3D11ShaderResourceView* justther;
    TABS tab = TAB_VISUALS;

	float anim_speed = 0.05f, m_alpha = 1.f;

    bool opened = true;

	void init()
	{
		
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        ImFontConfig font_config;
        font_config.Density = ImGui_ImplWin32_GetDpiScaleForHwnd(GetCurrentProcess());
        font_config.OversampleH = 1;
        font_config.OversampleV = 1;
        font_config.FontBuilderFlags = 1;

        static const ImWchar ranges[] =
        {
            0x0020, 0x00FF, // Basic Latin + Latin Supplement
            0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
            0x2DE0, 0x2DFF, // Cyrillic Extended-A
            0xA640, 0xA69F, // Cyrillic Extended-B
            0xE000, 0xE226, // icons
            0,
        };

        font_config.GlyphRanges = ranges;

        medium = io.Fonts->AddFontFromMemoryTTF(PTRootUIMedium, sizeof(PTRootUIMedium), 15.0f, &font_config, ranges);
        medium_bigger = io.Fonts->AddFontFromMemoryTTF(PTRootUIMedium, sizeof(PTRootUIMedium), 16.0f, &font_config, ranges);

        bold = io.Fonts->AddFontFromMemoryTTF(PTRootUIBold, sizeof(PTRootUIBold), 15.0f, &font_config, ranges);

        weapons = io.Fonts->AddFontFromMemoryTTF(astriumwep, sizeof(astriumwep), 12.0f, &font_config, ranges);

        combo_arrow = io.Fonts->AddFontFromMemoryTTF(combo, sizeof(combo), 9.0f, &font_config, ranges);

	}

	void render()
	{

        if (GetAsyncKeyState(VK_INSERT) & 1)
            opened = !opened;

        if (!opened)
            return;

        ImGui::SetNextWindowSize({ 659, 515 });
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::Begin("remade.cc", nullptr, ImGuiWindowFlags_NoDecoration);

        auto draw = ImGui::GetWindowDrawList();

        auto pos = ImGui::GetWindowPos();
        auto size = ImGui::GetWindowSize();

        ImGuiStyle style = ImGui::GetStyle();

        draw->AddRectFilled(ImVec2(pos.x + 1, pos.y + 1), ImVec2(pos.x + 181, pos.y + size.y - 1), ImColor(22, 22, 22), style.WindowRounding, ImDrawFlags_RoundCornersLeft);
        draw->AddLine(ImVec2(pos.x + 181, pos.y + 1), ImVec2(pos.x + 181, pos.y + size.y - 1), ImColor(1.0f, 1.0f, 1.0f, 0.03f));

        draw->AddLine(ImVec2(pos.x + 9, pos.y + 44), ImVec2(pos.x + 172, pos.y + 44), ImColor(1.0f, 1.0f, 1.0f, 0.03f));

        draw->AddRect(pos + ImVec2(1, 1), pos + size - ImVec2(1, 1), ImColor(1.0f, 1.0f, 1.0f, 0.03f), style.WindowRounding);

        draw->AddImage(justther, ImVec2(pos.x + 59, pos.y), ImVec2(pos.x + 121, pos.y + 54));
        ImGui::SetCursorPos({ 80, 11 }); {
            if (ImGui::InvisibleButton("##logomenu", ImVec2(18, 23)))
                ImGui::OpenPopup("##logo_menu");

            if (ImGui::BeginPopupModal("##logo_menu", nullptr, ImGuiWindowFlags_NoDecoration)) {
                ImGui::SetCursorPos({ 25, 15 }); {
                    ImGui::BeginChild("##container", ImVec2(265, 155)); {
                        ImGui::Text("Username:"); ImGui::SameLine(205); ImGui::TextColored(ImVec4(ImColor(141, 168, 241)), "0x777");
                        ImGui::Text("Sub expires:"); ImGui::SameLine(192); ImGui::TextColored(ImVec4(ImColor(141, 168, 241)), "21.09.22");
                        ImGui::SliderFloat("Animation speed", &anim_speed, 0.05f, 1.0f, "%.3f", ImGuiSliderFlags_NoInput);
                    } ImGui::EndChild();
                }

                ImGui::SetCursorPos({ 207, 160 }); {
                    if (ImGui::Button("Apply", ImVec2(75, 22))) ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        draw->AddText(bold, 15.0f, ImVec2(pos.x + 20, pos.y + 52), ImColor(1.0f, 1.0f, 1.0f, 0.33f), "Tabs");

        ImGui::SetCursorPos({ 9, 75 });
        ImGui::BeginGroup();
            if (elements::tab("Visuals", tab == TAB_VISUALS)) { tab = TAB_VISUALS; m_alpha = 0.0f; }
            if (elements::tab("Weapons", tab == TAB_WEAPON)) { tab = TAB_WEAPON; m_alpha = 0.0f; }
            if (elements::tab("Misc", tab == TAB_MISC)) { tab = TAB_MISC; m_alpha = 0.0f; }
            if (elements::tab("Configs", tab == TAB_CONFIG)) { tab = TAB_CONFIG; m_alpha = 0.0f; }
        ImGui::EndGroup();
            
        m_alpha = ImLerp(m_alpha, m_alpha < 1.0f ? 1.0f : 1.0f, anim_speed * (1.0f - ImGui::GetIO().DeltaTime));
          
        static int selected_config = 0;

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_alpha);

        switch (tab)
        {

            case TAB_VISUALS:
            {

                START_LEFT_CHILD

                elements::begin_child("Player ESP", MAX_CHILD_SIZE);

                ImGui::Checkbox("Enable Box ESP", &config.box_esp);
                ImGui::SameLine(186);
                ImGui::ColorEdit4("##Box ESP Color", &config.box_esp_color.r);

                ImGui::Checkbox("Enable Name ESP", &config.name_esp);
                ImGui::SameLine(186);
                ImGui::ColorEdit4("##Name ESP Color", &config.name_esp_color.r);

                ImGui::Checkbox("Enable Weapon ESP", &config.weapon_esp);
                ImGui::SameLine(186);
                ImGui::ColorEdit4("##Weapon ESP Color", &config.weapon_esp_color.r);

                ImGui::Checkbox("Enable Health ESP", &config.health_esp);
                if (!config.health_esp_dynamic)
                {

                    ImGui::SameLine(186);
                    ImGui::ColorEdit4("##Health ESP Color", &config.health_esp_color.r);

                }
                ImGui::Checkbox("Health Based Color", &config.health_esp_dynamic);

                ImGui::Checkbox("Enable Skeleton ESP", &config.skeleton_esp);
                ImGui::SameLine(186);
                ImGui::ColorEdit4("##Skeleton ESP Color", &config.skeleton_esp_color.r);

                ImGui::Checkbox("Enable Glow ESP", &config.glow_esp);
                ImGui::SameLine(186);
                ImGui::ColorEdit4("##Glow ESP Color", &config.glow_esp_color.r);

                if (config.glow_esp)
                {

                    ImGui::SliderFloat("Glow ESP Width", &config.glow_esp_width, 0.1f, 10.f);
                    ImGui::Combo("Glow ESP Style", &config.glow_esp_mode, "Outline All\0Outline Visible\0Outline Hidden\0Outline And Silhouette\0Silohouette Only");

                }

                ImGui::Checkbox("Show Teammates", &config.show_teammates_on_esp);

                elements::end_child();
                
                break;

            }

            case TAB_WEAPON:
            {

                START_LEFT_CHILD

                elements::begin_child("Recoil Control", ImVec2(CHILD_X_SIZE, MAX_CHILD_SIZE.y));

                ImGui::Checkbox("Enable No Recoil", &config.enable_recoil_control);
                ImGui::Checkbox("Enable No Spread", &config.enable_no_spread);

                elements::end_child();

                START_RIGHT_CHILD

                elements::begin_child("Weapon Modifications", ImVec2(CHILD_X_SIZE, MAX_CHILD_SIZE.y));

                ImGui::Checkbox("Enable Infinite Ammo", &config.enable_infinite_ammo);
                ImGui::Checkbox("Enable Instant Bullet", &config.enable_instant_bullet);
                ImGui::Checkbox("Enable Instant Kill", &config.enable_instant_kill);
                ImGui::Checkbox("Enable No Pushback", &config.enable_no_pushback);
                ImGui::Checkbox("Enable Rapidfire", &config.enable_rapid_fire);
                ImGui::Checkbox("Enable Infinite Penetration", &config.enable_infinite_penetration);
                ImGui::Checkbox("Enable Silent Aimbot", &config.enable_silent_aimbot);

                elements::end_child();

                break;

            }

            case TAB_MISC:
            {

                START_LEFT_CHILD

                elements::begin_child("Movement", ImVec2(CHILD_X_SIZE, MAX_CHILD_SIZE.y));

                ImGui::Checkbox("Enable Speedhack", &config.enable_speedhack);
                
                if (config.enable_speedhack)
                    ImGui::SliderFloat("Amount", &config.speedhack_amount, 0.1f, 100.f, "%.1f Times Speed");

                ImGui::Checkbox("Enable Infinite Jump", &config.enable_infinite_jump);

                elements::end_child();
                
                START_RIGHT_CHILD

                elements::begin_child("Other", ImVec2(CHILD_X_SIZE, MAX_CHILD_SIZE.y));

                ImGui::Checkbox("Don't Die In Lava", &config.avoid_lava);
                ImGui::Checkbox("Instant Game Start", &config.instant_game_start);
                ImGui::Checkbox("Get All Upgrades (requires 1 kill)", &config.test_mode);
                ImGui::Checkbox("Enable Info Panel", &config.info_panel);

                elements::end_child();

                break;

            }

            case TAB_CONFIG:
            {

                START_LEFT_CHILD

                elements::begin_child("Configs", ImVec2(CHILD_X_SIZE, MAX_CHILD_SIZE.y));

                ImGui::SetNextItemWidth(196);
                elements::ListBox("##Configs", &selected_config, config.configs);

                elements::end_child();

                START_RIGHT_CHILD

                elements::begin_child("Config Manipulation", ImVec2(CHILD_X_SIZE, MAX_CHILD_SIZE.y));

                static char config_name[MAX_PATH];

                ImGui::SetNextItemWidth(196);
                ImGui::InputText("Config Name", config_name, sizeof(config_name));

                if (ImGui::Button("Refresh Configs", ImVec2(196, 37)))
                    config.refresh_configs();

                if (ImGui::Button("Load Config", ImVec2(196, 37)))
                    config.load_config(selected_config);

                if (ImGui::Button("Save Config", ImVec2(196, 37)))
                    config.save_config(selected_config);

                if (ImGui::Button("Create Config", ImVec2(196, 37)))
                    config.create_config(config_name);

                if (ImGui::Button("Delete Config", ImVec2(196, 37)))
                    config.delete_config(selected_config);

                elements::end_child();

                break;

            }

        }

        ImGui::PopStyleVar(2);

        ImGui::End();

	}

}; inline c_menu menu;