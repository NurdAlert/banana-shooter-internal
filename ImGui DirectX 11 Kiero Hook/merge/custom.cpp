#include "custom.h"
#include "../c_menu.h"

struct tab_animation {
    float element_opacity, element_opacity_inversed, text_opacity, text_opacity_inversed;
};

static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
    auto& vector = *static_cast<std::vector<c_config_item>*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
    *out_text = vector.at(idx).name.c_str();
    return true;
};

bool elements::Combo(const char* label, int* currIndex, std::vector<c_config_item>& values) {
    if (values.empty()) { return false; }
    return ImGui::Combo(label, currIndex, vector_getter,
        static_cast<void*>(&values), values.size());
}

bool elements::ListBox(const char* label, int* currIndex, std::vector<c_config_item>& values) {
    if (values.empty()) { return false; }
    return ImGui::ListBox(label, currIndex, vector_getter,
        static_cast<void*>(&values), values.size());
}

bool elements::tab(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = menu.medium_bigger->CalcTextSizeA(16.0f, FLT_MAX, 0.0f, name);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + 135, pos.y + 23));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_animation> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f, 0.0f, 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 1.0f : 0.0f), menu.anim_speed * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.element_opacity_inversed = ImLerp(it_anim->second.element_opacity_inversed, (boolean ? 0.0f : 1.0f), menu.anim_speed * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.text_opacity = ImLerp(it_anim->second.text_opacity, (boolean ? 1.0f : 0.0f), menu.anim_speed * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.text_opacity_inversed = ImLerp(it_anim->second.text_opacity_inversed, (boolean ? 0.0f : 0.4f), menu.anim_speed * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilledMultiColor(rect.Min, rect.Max, ImColor(204 / 255.0f, 204 / 255.0f, 204 / 255.0f, it_anim->second.element_opacity_inversed * 0.12f), ImColor(204 / 255.0f, 204 / 255.0f, 204 / 255.0f, 0.0f), ImColor(204 / 255.0f, 204 / 255.0f, 204 / 255.0f, 0.0f), ImColor(204 / 255.0f, 204 / 255.0f, 204 / 255.0f, it_anim->second.element_opacity_inversed * 0.12f));
    window->DrawList->AddRectFilled(rect.Min, ImVec2(rect.Min.x + 2, rect.Max.y), ImColor(121 / 255.0f, 121 / 255.0f, 121 / 255.0f, it_anim->second.element_opacity_inversed));

    window->DrawList->AddRectFilledMultiColor(rect.Min, rect.Max, ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, it_anim->second.element_opacity * 0.12f), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, 0.0f), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, 0.0f), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, it_anim->second.element_opacity * 0.12f));
    window->DrawList->AddRectFilled(rect.Min, ImVec2(rect.Min.x + 2, rect.Max.y), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, it_anim->second.element_opacity));

    window->DrawList->AddText(menu.medium_bigger, 16.0f, ImVec2(rect.Min.x + 11, (rect.Min.y + rect.Max.y) / 2 - label_size.y / 2), ImColor(1.0f, 1.0f, 1.0f, it_anim->second.text_opacity_inversed), name);
    window->DrawList->AddText(menu.medium_bigger, 16.0f, ImVec2(rect.Min.x + 11, (rect.Min.y + rect.Max.y) / 2 - label_size.y / 2), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, it_anim->second.text_opacity), name);

    return pressed;
}

struct subtab_animation {
    float element_opacity, text_opacity, text_opacity_inversed;
};

bool elements::subtab(const char* name, bool boolean, float width)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = menu.weapons->CalcTextSizeA(12.0f, FLT_MAX, 0.0f, name);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + width, pos.y + 35));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x - 8, rect.Max.y), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, subtab_animation> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 1.0f : 0.0f), menu.anim_speed * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.text_opacity = ImLerp(it_anim->second.text_opacity, (boolean ? 1.0f : 0.0f), menu.anim_speed * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.text_opacity_inversed = ImLerp(it_anim->second.text_opacity_inversed, (boolean ? 0.0f : 0.4f), menu.anim_speed * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilledMultiColor(rect.Min, rect.Max, ImColor(141, 168, 241, 0), ImColor(141, 168, 241, 0), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, menu.m_alpha * 0.15f * it_anim->second.element_opacity), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, menu.m_alpha * 0.15f * it_anim->second.element_opacity));
    window->DrawList->AddRectFilled(ImVec2(rect.Min.x, rect.Max.y), ImVec2(rect.Max.x, rect.Max.y - 2), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, menu.m_alpha * it_anim->second.element_opacity));

    window->DrawList->AddText(menu.weapons, 12.0f, (rect.Min + rect.Max) / 2 - label_size / 2, ImColor(1.0f, 1.0f, 1.0f, menu.m_alpha * it_anim->second.text_opacity_inversed), name);
    window->DrawList->AddText(menu.weapons, 12.0f, (rect.Min + rect.Max) / 2 - label_size / 2, ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, menu.m_alpha * it_anim->second.text_opacity), name);

    return pressed;
}

bool elements::begin_child_ex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* parent_window = g.CurrentWindow;

    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
    flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag

    // Size
    const ImVec2 content_avail = GetContentRegionAvail();
    ImVec2 size = ImFloor(size_arg);
    const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
    if (size.x <= 0.0f)
        size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too much issues)
    if (size.y <= 0.0f)
        size.y = ImMax(content_avail.y + size.y, 4.0f);

    SetNextWindowPos(ImVec2(parent_window->DC.CursorPos.x, parent_window->DC.CursorPos.y + 34.0f));
    SetNextWindowSize(size - ImVec2(0, 36));

    parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(22 / 255.0f, 22 / 255.0f, 22 / 255.0f, menu.m_alpha), 2.0f);
    parent_window->DrawList->AddRect(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(1.0f, 1.0f, 1.0f, menu.m_alpha * 0.03f), 2.0f);
    parent_window->DrawList->AddLine(parent_window->DC.CursorPos + ImVec2(1, 25), parent_window->DC.CursorPos + ImVec2(size.x - 1, 25), ImColor(1.0f, 1.0f, 1.0f, menu.m_alpha * 0.03f));
    parent_window->DrawList->AddRectFilledMultiColorRounded(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x - 115, 25), ImColor(22 / 255.0f, 22 / 255.0f, 22 / 255.0f, menu.m_alpha), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, menu.m_alpha * 0.13f), ImColor(147, 190, 66, 0), ImColor(147, 190, 66, 0), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, menu.m_alpha * 0.13f), 2.0f, ImDrawCornerFlags_TopLeft);
    parent_window->DrawList->AddText(menu.bold, 15.0f, parent_window->DC.CursorPos + ImVec2(14, 5), ImColor(141 / 255.0f, 168 / 255.0f, 241 / 255.0f, menu.m_alpha), name);

    // Build up name. If you need to append to a same child from multiple location in the ID stack, use BeginChild(ImGuiID id) with a stable value.
    const char* temp_window_name;
    if (name)
        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
    else
        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

    const float backup_border_size = g.Style.ChildBorderSize;
    if (!border)
        g.Style.ChildBorderSize = 0.0f;
    bool ret = Begin(temp_window_name, NULL, flags);
    g.Style.ChildBorderSize = backup_border_size;

    ImGuiWindow* child_window = g.CurrentWindow;
    child_window->ChildId = id;
    child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

    // Set the cursor to handle case where the user called SetNextWindowPos()+BeginChild() manually.
    // While this is not really documented/defined, it seems that the expected thing to do.
    if (child_window->BeginCount == 1)
        parent_window->DC.CursorPos = child_window->Pos;

    // Process navigation-in immediately so NavInit can run on first frame
    if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavHasScroll))
    {
        FocusWindow(child_window);
        NavInitWindow(child_window, false);
        SetActiveID(id + 1, child_window); // Steal ActiveId with another arbitrary id so that key-press won't activate child item
        g.ActiveIdSource = ImGuiInputSource_Nav;
    }
    return ret;
}

bool elements::begin_child(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14, 0));
    return begin_child_ex(str_id, window->GetID(str_id), size_arg, border, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove);
}

bool elements::begin_child(ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    IM_ASSERT(id != 0);
    return begin_child_ex(NULL, id, size_arg, border, extra_flags);
}

void elements::end_child()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    IM_ASSERT(g.WithinEndChild == false);
    IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() calls

    g.WithinEndChild = true;
    if (window->BeginCount > 1)
    {
        End();
    }
    else
    {
        ImVec2 sz = window->Size;
        if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) // Arbitrary minimum zero-ish child size of 4.0f causes less trouble than a 0.0f
            sz.x = ImMax(4.0f, sz.x);
        if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
            sz.y = ImMax(4.0f, sz.y);
        End();

        ImGuiWindow* parent_window = g.CurrentWindow;
        ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
        ItemSize(sz);
        if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavHasScroll) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
        {
            ItemAdd(bb, window->ChildId);
            RenderNavHighlight(bb, window->ChildId);

            // When browsing a window that has no activable items (scroll only) we keep a highlight on the child (pass g.NavId to trick into always displaying)
            if (window->DC.NavLayersActiveMask == 0 && window == g.NavWindow)
                RenderNavHighlight(ImRect(bb.Min - ImVec2(2, 2), bb.Max + ImVec2(2, 2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
        }
        else
        {
            // Not navigable into
            ItemAdd(bb, 0);
        }
        if (g.HoveredWindow == window)
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
    }
    ImGui::PopStyleVar(2);
    g.WithinEndChild = false;
    g.LogLinePosY = -FLT_MAX; // To enforce a carriage return
}

bool elements::key_bind(const char* label, const char* unique_id, const ImVec2& size_arg, bool clicked, ImGuiButtonFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;
    SetCursorPosX(window->Size.x - 14 - size_arg.x);
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(unique_id);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    if (g.CurrentItemFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    static std::map<ImGuiID, float> hover_animation;
    auto it_hover = hover_animation.find(id);
    if (it_hover == hover_animation.end()) {
        hover_animation.insert({ id, 0.0f });
        it_hover = hover_animation.find(id);
    }

    it_hover->second = ImClamp(it_hover->second + (3.0f * ImGui::GetIO().DeltaTime * ((hovered || clicked || IsPopupOpen(unique_id)) ? 1.0f : -1.0f)), 0.0f, 1.0f);

    static std::map<ImGuiID, float> open_animation;
    auto it_open = open_animation.find(id);
    if (it_open == open_animation.end())
    {
        open_animation.insert({ id, 0.f });
        it_open = open_animation.find(id);
    }
    it_open->second = ImClamp(it_open->second + (4.0f * ImGui::GetIO().DeltaTime * ((clicked || IsPopupOpen(unique_id)) ? 1.0f : -1.0f)), 0.01f, 1.0f);

    if (ImGui::GetIO().MouseClicked[1] && hovered) {
        OpenPopup(unique_id);
    }

    // Render
    ImVec4 textcolor = ImLerp(ImVec4(201 / 255.f, 204 / 255.f, 210 / 255.f, menu.m_alpha), ImVec4(1.0f, 1.0f, 1.0f, menu.m_alpha), it_hover->second);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(16 / 255.0f, 16 / 255.0f, 16 / 255.0f, menu.m_alpha), 2.0f, IsPopupOpen(unique_id) ? ImDrawCornerFlags_Top : 0);
    window->DrawList->AddRect(bb.Min, bb.Max, ImColor(1.0f, 1.0f, 1.0f, menu.m_alpha * 0.03f), 2.0f, IsPopupOpen(unique_id) ? ImDrawCornerFlags_Top : 0);

    window->DrawList->AddText(bb.Min + ImVec2(size_arg.x / 2 - CalcTextSize(label).x / 2, size_arg.y / 2 - CalcTextSize(label).y / 2), ColorConvertFloat4ToU32(textcolor), label);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar;
    SetNextWindowPos(pos + ImVec2(0, size_arg.y - 1));
    SetNextWindowSize(ImVec2(size_arg.x, 63 * it_open->second));
    PushStyleColor(ImGuiCol_WindowBg, ImVec4(16 / 255.0f, 16 / 255.0f, 16 / 255.0f, menu.m_alpha));
    PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, menu.m_alpha * 0.02f));
    {
        static bool toggle = true, always, hold;

        if (BeginPopup(unique_id, window_flags)) {
            PushStyleVar(ImGuiStyleVar_Alpha, it_open->second); {
                SetCursorPos(ImVec2(7, 2)); {
                    BeginGroup(); {
                        if (Selectable("Always", always))toggle = false, hold = false, always = true;
                        if (Selectable("Hold", hold))toggle = false, hold = true, always = false;
                        if (Selectable("Toggle", toggle))toggle = true, hold = false, always = false;
                    } EndGroup();
                }
            } PopStyleVar();
            EndPopup();
        }
    } PopStyleColor(2);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
    return pressed;
}
