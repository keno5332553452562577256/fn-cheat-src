#pragma once
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include <dwmapi.h>
#include <d3d9.h>
#include "../game/actorloop.h"
#include "../settings/settings.h"
#include "../external/gui.h"
#include "../SDK/sdk.h"
#include "../offsets/offsets.h"
#include "../kernel/coms.h"
#include <chrono>
#pragma comment(lib, "d3d9.lib")
#include <dwmapi.h>
#include <windows.h>
#include <psapi.h>
#include <string>
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "dwmapi.lib")
IDirect3D9Ex* p_object = NULL;
IDirect3DDevice9Ex* p_device = NULL;
D3DPRESENT_PARAMETERS p_params = { NULL };
MSG messager = { NULL };
HWND MyWnd = NULL;
DWORD ProcId;

int getFps()
{
	using namespace std::chrono;
	static int count = 0;
	static auto last = high_resolution_clock::now();
	auto now = high_resolution_clock::now();
	static int fps = 0;
	count++;
	if (duration_cast<milliseconds>(now - last).count() > 1000)
	{
		fps = count;
		count = 0;
		last = now;
	}
	return fps;
}
HRESULT directx_init()
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_object))) exit(3);
	ZeroMemory(&p_params, sizeof(p_params));
	p_params.Windowed = TRUE;
	p_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_params.hDeviceWindow = MyWnd;
	p_params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_params.BackBufferWidth = width;
	p_params.BackBufferHeight = height;
	p_params.EnableAutoDepthStencil = TRUE;
	p_params.AutoDepthStencilFormat = D3DFMT_D16;
	p_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	if (FAILED(p_object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, MyWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_params, 0, &p_device)))
	{
		p_object->Release();
		exit(4);
	}
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(MyWnd);
	ImGui_ImplDX9_Init(p_device);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFontAtlas* fontAtlas = io.Fonts;
	ImFontConfig arialConfig;
	arialConfig.FontDataOwnedByAtlas = false;
	ImFont* arialFont = fontAtlas->AddFontFromFileTTF("c:\\Windows\\Fonts\\Bahnschrift.ttf", 14.0f, &arialConfig);
	io.Fonts = fontAtlas;
	io.IniFilename = 0;
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowTitleAlign = { 0.5f, 0.5f };
	style->WindowRounding = 2.0f;
	style->WindowBorderSize = 0.0f;
	style->FrameBorderSize = 0.0f;
	style->FramePadding = { 6.0f, 4.0f };

	ImVec4 dark = ImColor(15, 15, 15);
	ImVec4 darker = ImColor(10, 10, 10);
	ImVec4 accent = ImColor(0, 128, 255);
	ImVec4 transparent = ImColor(0, 0, 0, 0);
	ImVec4 border = ImColor(30, 30, 30);

	style->Colors[ImGuiCol_WindowBg] = darker;
	style->Colors[ImGuiCol_TitleBg] = dark;
	style->Colors[ImGuiCol_TitleBgActive] = dark;
	style->Colors[ImGuiCol_TitleBgCollapsed] = dark;
	style->Colors[ImGuiCol_Border] = border;
	style->Colors[ImGuiCol_ChildBg] = dark;
	style->Colors[ImGuiCol_FrameBg] = dark;
	style->Colors[ImGuiCol_FrameBgHovered] = dark;
	style->Colors[ImGuiCol_FrameBgActive] = dark;
	style->Colors[ImGuiCol_PopupBg] = dark;
	style->Colors[ImGuiCol_Header] = transparent;
	style->Colors[ImGuiCol_HeaderHovered] = transparent;
	style->Colors[ImGuiCol_HeaderActive] = transparent;
	style->Colors[ImGuiCol_CheckMark] = accent;
	style->Colors[ImGuiCol_SliderGrab] = accent;
	style->Colors[ImGuiCol_Button] = dark;
	style->Colors[ImGuiCol_ButtonHovered] = dark;
	style->Colors[ImGuiCol_ButtonActive] = dark;
	style->Colors[ImGuiCol_Separator] = border;
	style->Colors[ImGuiCol_SeparatorHovered] = border;
	style->Colors[ImGuiCol_SeparatorActive] = border;

	p_object->Release();
	return S_OK;
}
static inline bool find_discord_overlay_window(HWND& hwnd_out) {
	HWND hwnd = nullptr;

	while ((hwnd = FindWindowExA(nullptr, hwnd, "Chrome_WidgetWin_1", nullptr)) != nullptr) {
		DWORD pid = 0;
		GetWindowThreadProcessId(hwnd, &pid);

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
		if (hProcess) {
			char exePath[MAX_PATH];
			if (GetModuleFileNameExA(hProcess, nullptr, exePath, MAX_PATH)) {
				std::string path = exePath;
				if (path.find("Discord") != std::string::npos) {
					hwnd_out = hwnd;
					CloseHandle(hProcess);
					return true;
				}
			}
			CloseHandle(hProcess);
		}
	}
	return false;
}
void overlay()
{
	HWND hwnd_out = nullptr;
	if (!find_discord_overlay_window(hwnd_out)) {
		MessageBoxA(0, "Couldn't find Discord overlay window.", "Error", MB_ICONERROR);
		return;
	}
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(hwnd_out, &margin);
	SetWindowLong(hwnd_out, GWL_EXSTYLE, GetWindowLong(hwnd_out, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(hwnd_out, RGB(0, 0, 0), 255, LWA_ALPHA);
	ShowWindow(hwnd_out, SW_SHOW);
	UpdateWindow(hwnd_out);
	MyWnd = hwnd_out;
}
void menu()
{
	char watermarkText[64];
	sprintf_s(watermarkText, "PK-Cheat paid vesion v1 | %.i FPS", getFps());
	ImVec2 wmTextSize = ImGui::CalcTextSize(watermarkText);
	ImVec2 rectSize = ImVec2(wmTextSize.x + 20.0f, wmTextSize.y + 10.0f);
	ImVec2 rectPos = ImVec2(10, 10);

	auto draw = ImGui::GetForegroundDrawList();
	draw->AddRectFilled(rectPos, ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y), ImColor(10, 10, 10), 6.0f);
	draw->AddRectFilled(rectPos, ImVec2(rectPos.x + rectSize.x, rectPos.y + 3), ImColor(0, 128, 255), 6.0f);
	draw->AddText(ImGui::GetFont(), 14.0f, ImVec2(rectPos.x + (rectSize.x - wmTextSize.x) / 2, rectPos.y + (rectSize.y - wmTextSize.y) / 2), ImColor(255, 255, 255), watermarkText);
	if (settings::aimbot::show_fov)
	{
		ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
		float radius = settings::aimbot::fov;
		auto draw = ImGui::GetForegroundDrawList();
		draw->AddCircle(center, radius, ImColor(0, 0, 0, 255), 100, 2.0f);
		draw->AddCircle(center, radius, ImColor(255, 255, 255, 255), 100, 1.0f);
	}


	if (GetAsyncKeyState(VK_INSERT) & 1) get_menu = !get_menu;

	if (get_menu)
	{
		ImGui::SetNextWindowSize({ 640, 400 });
		ImGui::Begin("PK-Cheat paid vesion v1", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		if (ImGui::BeginTabBar("##tabs")) {
			if (ImGui::BeginTabItem("ESP")) {
				ImGui::BeginChild("esp", ImVec2(0, 0), true);
				ImGui::Checkbox("Enable Visuals", &settings::visuals::enable);
				if (settings::visuals::enable) {
					ImGui::Separator();
					ImGui::Text("Allgemein:");
					ImGui::Checkbox("Name", &settings::visuals::name);
					ImGui::Checkbox("Self ESP", &settings::visuals::selfesp);
					ImGui::Checkbox("Platform", &settings::visuals::platform);
					ImGui::Checkbox("Distance", &settings::visuals::distance);
					ImGui::Checkbox("Rank", &settings::visuals::rank);
					ImGui::Checkbox("Skeleton", &settings::visuals::skeleton);
					ImGui::Checkbox("Box", &settings::visuals::box);
					ImGui::Checkbox("World ESP", &settings::visuals::worldesp);
					ImGui::Checkbox("Chinahat ESP", &settings::visuals::chinahat);
					ImGui::Separator();
					ImGui::Text("ESP-Features:");
					ImGui::Checkbox("Health-Balken anzeigen", &settings::visuals::show_health);
					ImGui::Checkbox("Spectator-Warnung", &settings::visuals::show_spectators);
					ImGui::Checkbox("Fahrzeuge anzeigen", &settings::visuals::show_vehicles);
					ImGui::Checkbox("Kills anzeigen", &settings::visuals::show_kills);
					ImGui::Checkbox("Season-Level anzeigen", &settings::visuals::show_season_level);
					ImGui::Checkbox("Low-HP-Highlight", &settings::visuals::highlight_lowhp);
					ImGui::SliderFloat("Render Distance", &settings::visuals::renderDistance, 100, 1000, "%.0fm");
					if (ImGui::BeginCombo("Box Type", getBoxTypeName(settings::visuals::boxType))) {
						for (auto boxType : boxValues) {
							if (ImGui::Selectable(getBoxTypeName(boxType), settings::visuals::boxType == boxType))
								settings::visuals::boxType = boxType;
						}
						ImGui::EndCombo();
					}
					if (ImGui::CollapsingHeader("Debug/Entwickler-Optionen")) {
						ImGui::Checkbox("Debug", &settings::visuals::debug);
						ImGui::Checkbox("API Debug", &settings::visuals::debugapi);
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Aimbot")) {
				ImGui::BeginChild("aimbot", ImVec2(0, 0), true);
				ImGui::Checkbox("Enable Aimbot", &settings::aimbot::enable);
				if (settings::aimbot::enable) {
					ImGui::SliderFloat("Smoothness", &settings::aimbot::smoothness, 1, 10, "Smoothness: %.2f");
					if (ImGui::BeginCombo("Aimbot Type", settings::aimbot::aim_type == 0 ? "Silent" : "Visible")) {
						if (ImGui::Selectable("Silent", settings::aimbot::aim_type == 0))
							settings::aimbot::aim_type = 0;
						if (ImGui::Selectable("Visible", settings::aimbot::aim_type == 1))
							settings::aimbot::aim_type = 1;
						ImGui::EndCombo();
					}
					const char* bone_names[] = { "Head", "Neck", "Body", "Random" };
					if (ImGui::BeginCombo("Target Bone", bone_names[settings::aimbot::target_bone])) {
						for (int i = 0; i < 4; i++) {
							if (ImGui::Selectable(bone_names[i], settings::aimbot::target_bone == i))
								settings::aimbot::target_bone = i;
						}
						ImGui::EndCombo();
					}
					ImGui::Checkbox("Trigger Bot", &settings::aimbot::trigger_bot);
					if (settings::aimbot::trigger_bot) {
						ImGui::SliderFloat("Trigger Delay", &settings::aimbot::trigger_delay, 0.0f, 1.0f, "Delay: %.2f");
					}
					ImGui::Checkbox("Prediction", &settings::aimbot::prediction);
					if (settings::aimbot::prediction) {
						ImGui::SliderFloat("Prediction Strength", &settings::aimbot::prediction_strength, 0.1f, 3.0f, "Strength: %.2f");
					}
					ImGui::Checkbox("Recoil Control", &settings::aimbot::recoil_control);
					if (settings::aimbot::recoil_control) {
						ImGui::SliderFloat("Recoil Strength", &settings::aimbot::recoil_strength, 0.1f, 2.0f, "Strength: %.2f");
					}
					ImGui::Checkbox("Auto Shoot", &settings::aimbot::auto_shoot);
					ImGui::Checkbox("Team Check", &settings::aimbot::team_check);
					ImGui::Checkbox("Distance Check", &settings::aimbot::distance_check);
					if (settings::aimbot::distance_check) {
						ImGui::SliderFloat("Max Distance", &settings::aimbot::max_distance, 50.0f, 500.0f, "Distance: %.0fm");
					}
					ImGui::Checkbox("Smooth Aim", &settings::aimbot::smooth_aim);
					ImGui::Checkbox("Aim at Visible Only", &settings::aimbot::aim_at_visible_only);
					const char* priority_names[] = { "Distance", "Health", "Threat" };
					if (ImGui::BeginCombo("Aim Priority", priority_names[settings::aimbot::aim_priority])) {
						for (int i = 0; i < 3; i++) {
							if (ImGui::Selectable(priority_names[i], settings::aimbot::aim_priority == i))
								settings::aimbot::aim_priority = i;
						}
						ImGui::EndCombo();
					}
				}
				ImGui::Checkbox("Visible Check", &settings::aimbot::visible_check);
				ImGui::Checkbox("Show Fov", &settings::aimbot::show_fov);
				if (settings::aimbot::show_fov) {
					ImGui::SliderFloat("Fov", &settings::aimbot::fov, 50, 300, "Fov: %.2f");
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("World")) {
				ImGui::BeginChild("world", ImVec2(0, 0), true);
				ImGui::Text("World-Features:");
				ImGui::Checkbox("Fahrzeug-ESP (World)", &settings::world::vehicle_esp);
				ImGui::Checkbox("Uncommon Loot", &settings::world::uncommon_loot);
				ImGui::Checkbox("Common Loot", &settings::world::common_loot);
				ImGui::Checkbox("Llamas", &settings::world::llamas);
				ImGui::Checkbox("Rare Loot", &settings::world::rare_loot);
				ImGui::Checkbox("Foodbox", &settings::world::foodbox);
				ImGui::Checkbox("Epic Loot", &settings::world::epic_loot);
				ImGui::Checkbox("Legendary Loot", &settings::world::legendary_loot);
				ImGui::Checkbox("Mythic Loot", &settings::world::mythic_loot);
				ImGui::Checkbox("Chests", &settings::world::chests);
				ImGui::Checkbox("Ammobox", &settings::world::ammobox);
				ImGui::Checkbox("Vehicle", &settings::world::vehicle);
				ImGui::Checkbox("Spirits", &settings::world::spirits);
				ImGui::SliderInt("Loot Render Distance", &settings::world::render_distance_loot, 50, 1000, "%d");
				ImGui::SliderInt("Ammo Render Distance", &settings::world::render_distance_ammo, 50, 1000, "%d");
				ImGui::SliderInt("Vehicle Render Distance", &settings::world::render_distance_vehicle, 50, 1000, "%d");
				ImGui::SliderInt("Spirits Render Distance", &settings::world::render_distance_spirits, 50, 1000, "%d");
				ImGui::SliderInt("Llama Render Distance", &settings::world::render_distance_llama, 50, 1000, "%d");
				ImGui::SliderInt("Chest Render Distance", &settings::world::render_distance_chest, 50, 1000, "%d");
				ImGui::Checkbox("Outlines Containers", &settings::world::enableOutlinesContainers);
				ImGui::Checkbox("Outlines Loot", &settings::world::enableOutlinesLoot);
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc")) {
				ImGui::BeginChild("misc", ImVec2(0, 0), true);
				static char presetName[64] = "default";
				static std::string notification = "";
				static float notificationTime = 0.0f;
				ImGui::InputText("Preset-Name", presetName, 64);
				if (ImGui::Button("Preset speichern")) {
					SaveSettings(presetName);
					notification = std::string("Preset '") + presetName + "' gespeichert!";
					notificationTime = ImGui::GetTime();
				}
				ImGui::SameLine();
				if (ImGui::Button("Preset laden")) {
					LoadSettings(presetName);
					notification = std::string("Preset '") + presetName + "' geladen!";
					notificationTime = ImGui::GetTime();
				}
				// Preset-Liste
				ImGui::Text("Vorhandene Presets:");
				auto presets = ListSettingsPresets();
				for (const auto& p : presets) {
					if (ImGui::Selectable(p.c_str(), false)) {
						strcpy_s(presetName, sizeof(presetName), p.c_str());
					}
				}
				// Benachrichtigung anzeigen
				if (!notification.empty() && ImGui::GetTime() - notificationTime < 2.0f) {
					ImGui::TextColored(ImVec4(0,1,0,1), notification.c_str());
				}
				// --- ESP-Farben ---
				ImGui::Separator();
				ImGui::Text("ESP-Farben:");
				ImGui::ColorEdit4("Box sichtbar", (float*)&settings::colors::icBoxColorVisible, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex);
				ImGui::ColorEdit4("Box unsichtbar", (float*)&settings::colors::icBoxColorInvisible, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex);
				ImGui::ColorEdit4("Skelett sichtbar", (float*)&settings::colors::icSkeletonColorVisible, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex);
				ImGui::ColorEdit4("Skelett unsichtbar", (float*)&settings::colors::icSkeletonColorInvisible, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex);
				ImGui::ColorEdit4("Tracer sichtbar", (float*)&settings::colors::icTracerColorVisible, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex);
				ImGui::ColorEdit4("Tracer unsichtbar", (float*)&settings::colors::icTracerColorInvisible, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex);
				ImGui::ColorEdit4("FOV", (float*)&settings::colors::icFovColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex);
				ImGui::ColorEdit4("Crosshair", (float*)&settings::colors::icCrosshairColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex);
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}
HWND GetProcWindow(uint32_t pid)
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bresult = EnumWindows([](HWND hwnd, LPARAM lparam) -> BOOL
	{
		auto pparams = (std::pair<HWND, uint32_t>*)(lparam);
		uint32_t processid = 0;
		if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processid)) && processid == pparams->second)
		{
			SetLastError((uint32_t)-1);
			pparams->first = hwnd;
			return FALSE;
		}
		return TRUE;
	}, (LPARAM)&params);
	if (!bresult && GetLastError() == -1 && params.first) return params.first;
	return 0;
}

WPARAM RenderLoop()
{
	static RECT old_rc;
	ZeroMemory(&messager, sizeof(MSG));
	while (messager.message != WM_QUIT)
	{
		if (PeekMessage(&messager, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&messager);
			DispatchMessage(&messager);
		}
		if (MyWnd == NULL) exit(0);
		HWND active_wnd = GetForegroundWindow();
		RECT rc;
		POINT xy;
		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		if (MyWnd != NULL) {
			GetWindowRect(MyWnd, &rc);
			xy.x = rc.left;
			xy.y = rc.top;
		}
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = 1.0f / 60.0f;
		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (GetAsyncKeyState(0x1))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].y = io.MousePos.y;
		}
		else
		{
			io.MouseDown[0] = false;
		}
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
			p_params.BackBufferWidth = width;
			p_params.BackBufferHeight = height;
			SetWindowPos(MyWnd, HWND_TOPMOST, rc.left, rc.top, width, height, SWP_NOREDRAW | SWP_NOACTIVATE | SWP_SHOWWINDOW);
			SetWindowLong(MyWnd, GWL_EXSTYLE, GetWindowLong(MyWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
			SetLayeredWindowAttributes(MyWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
			p_device->Reset(&p_params);
		}
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		actor();
		//world_esp();
		menu();
		ImGui::EndFrame();
		p_device->SetRenderState(D3DRS_ZENABLE, false);
		p_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		p_device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		p_device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		if (p_device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			p_device->EndScene();
		}
		HRESULT result = p_device->Present(0, 0, 0, 0);
		if (result == D3DERR_DEVICELOST && p_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_device->Reset(&p_params);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (p_device != 0)
	{
		p_device->EndScene();
		p_device->Release();
	}
	if (p_object != 0) p_object->Release();
	DestroyWindow(MyWnd);
	return messager.wParam;
}



