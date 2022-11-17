#include "includes.h"
#include "kiero/minhook/include/MinHook.h"
#include "c_hooking.h"
#include "c_globals.h"
#include "c_unityengine.h"
#include "c_menu.h"
#include "c_features.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

using fIl2cppThreadAttach = void* (__fastcall*)(void*);
using fIl2cppDomainGet = void* (__fastcall*)();

fIl2cppDomainGet il2cpp_domain_get;
fIl2cppThreadAttach il2cpp_thread_attach;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
	menu.init();
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	ImGuiIO& io = ImGui::GetIO();
	POINT mPos;
	GetCursorPos(&mPos);
	ScreenToClient(window, &mPos);
	ImGui::GetIO().MousePos.x = mPos.x;
	ImGui::GetIO().MousePos.y = mPos.y;

	if (menu.opened)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

int one, two, three, four;

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{

			il2cpp_thread_attach(il2cpp_domain_get());

			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	float dpi_scale = ImGui_ImplWin32_GetDpiScaleForHwnd(GetCurrentProcess());
	io.DisplayFramebufferScale = ImVec2(dpi_scale, dpi_scale);
	io.DisplaySize.x /= dpi_scale;
	io.DisplaySize.y /= dpi_scale;

	if (io.MousePos.x != -FLT_MAX && io.MousePos.y != -FLT_MAX)
	{
		io.MousePos.x /= dpi_scale;
		io.MousePos.y /= dpi_scale;
	}

	ImGui::NewFrame();

	if (!unityengine.in_game())
	{

		globals.client_player_list_mutex.lock();
		globals.client_player_list.clear();
		globals.client_player_list_mutex.unlock();

	}

	if (unityengine.in_game())
	{
		features.esp.run(ImGui::GetForegroundDrawList());
	}

	menu.render();

	if (config.info_panel)
	{

		ImGui::Begin("##InfoPanel", nullptr, ImGuiWindowFlags_NoDecoration);

		ImGui::Text("Number Of Entities: %s", std::to_string(globals.client_player_list.size()).c_str());
		
		if (globals.local_player)
		{

			ImGui::Text("Local Player Health: %s", std::to_string(globals.local_player->fields.health).c_str());
			ImGui::Text("Local Player Max Health: %s", std::to_string(globals.local_player->fields.maxHealth).c_str());
			ImGui::Text("Local Player Weapon: %s", globals.local_player->fields.currentWeapon->fields.name->fields.to_string().c_str());
			ImGui::Text("Local Player Name: %s", globals.local_player->fields._Username_k__BackingField->fields.to_string().c_str());

		}

		ImGui::End();

	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

#define MAKE_HOOK(x, y, z) MH_CreateHook((uintptr_t*)globals.gameassembly_base + x, y, (LPVOID*)z); // doesn't work, idk why

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{

			il2cpp_domain_get = reinterpret_cast<fIl2cppDomainGet>(GetProcAddress(GetModuleHandleA("GameAssembly.dll"), "il2cpp_domain_get"));
			il2cpp_thread_attach = reinterpret_cast<fIl2cppThreadAttach>(GetProcAddress(GetModuleHandleA("GameAssembly.dll"), "il2cpp_thread_attach"));

			kiero::bind(8, (void**)& oPresent, hkPresent);

			MH_CreateHook((uintptr_t*)(globals.gameassembly_base + c_offsets::client_player_update), hk_client_player_update, (LPVOID*)&hooking.o_client_player_update);
			MH_CreateHook((uintptr_t*)(globals.gameassembly_base + c_offsets::firearms_update), hk_firearms_update, (LPVOID*)&hooking.o_firearms_update);
			MH_CreateHook((uintptr_t*)(globals.gameassembly_base + c_offsets::firearms_create_bullet), hk_firearms_create_bullet, (LPVOID*)&hooking.o_firearms_create_bullet);
			MH_CreateHook((uintptr_t*)(globals.gameassembly_base + c_offsets::bullet_init), hk_bullet_init, (LPVOID*)&hooking.o_bullet_init);
			MH_CreateHook((uintptr_t*)(globals.gameassembly_base + c_offsets::networkmanager_cant_play), hk_networkmanager_cant_play, (LPVOID*)&hooking.o_networkmanager_cant_play);
			MH_CreateHook((uintptr_t*)(globals.gameassembly_base + c_offsets::anticheat_fixed_update), hk_anticheat_fixed_update, (LPVOID*)&hooking.o_anticheat_fixed_update);
			//MH_CreateHook((uintptr_t*)(globals.gameassembly_base + c_offsets::grappling_update), hk_grappling_update, (LPVOID*)&hooking.o_grappling_update);
			MH_CreateHook((uintptr_t*)(globals.gameassembly_base + c_offsets::weaponmanager_update), hk_weaponmanager_update, (LPVOID*)&hooking.o_weaponmanager_update);

			if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
			{

				MessageBoxA(NULL, "Hooking failed.", "ERROR", NULL);
				exit(0);

			}

			init_hook = true;

			CreateDirectoryA("C:\\remade\\", NULL);
			CreateDirectoryA("C:\\remade\\configs\\", NULL);

			config.refresh_configs();

		}

	} while (!init_hook);

	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}