#pragma once
#include "../imgui/imgui.h"
#include "../settings/settings.h"
#define DEBUG_DISPLAY(name, value) \
		if (!(value)) ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to read " name "!"); \
		else ImGui::TextColored(ImVec4(0, 1, 0, 1), name ": 0x%llX", value);
void outlinetext(ImFont* font, float fontSize, ImVec2 position, ImColor color, const char* text)
{
	ImGui::GetForegroundDrawList()->AddText(font, fontSize, ImVec2(position.x - 1, position.y - 1), ImColor(0, 0, 0), text);
	ImGui::GetForegroundDrawList()->AddText(font, fontSize, ImVec2(position.x + 1, position.y - 1), ImColor(0, 0, 0), text);
	ImGui::GetForegroundDrawList()->AddText(font, fontSize, ImVec2(position.x - 1, position.y + 1), ImColor(0, 0, 0), text);
	ImGui::GetForegroundDrawList()->AddText(font, fontSize, ImVec2(position.x + 1, position.y + 1), ImColor(0, 0, 0), text);
	ImGui::GetForegroundDrawList()->AddText(font, fontSize, position, color, text);
}
void drawbox(int x, int y, int w, int h, const ImColor color)
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x + 1, y + 1), ImVec2(x + w - 1, y + h - 1), ImColor(0, 0, 0), 0, 0, 1.0f);
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0, 0, 0), 0, 0, 1.0f);
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x + 1, y - 1), ImVec2(x + w - 1, y + h + 1), ImColor(0, 0, 0), 0, 0, 1.0f);
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x - 1, y + 1), ImVec2(x + w + 1, y + h - 1), ImColor(0, 0, 0), 0, 0, 1.0f);
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, 1.0f);
}
static void drawcornerbox(float x, float y, float w, float h, ImColor color)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x - 1, y - 1), ImVec2(x - 1, y + (h / 3) - 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x - 1, y - 1), ImVec2(x + (w / 3) - 1, y - 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - (w / 3) + 1, y - 1), ImVec2(x + w + 1, y - 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w + 1, y - 1), ImVec2(x + w + 1, y + (h / 3) - 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x - 1, y + h - (h / 3) + 1), ImVec2(x - 1, y + h + 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x - 1, y + h + 1), ImVec2(x + (w / 3) - 1, y + h + 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - (w / 3) + 1, y + h + 1), ImVec2(x + w + 1, y + h + 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w + 1, y + h - (h / 3) + 1), ImVec2(x + w + 1, y + h + 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + 1, y + 1), ImVec2(x + 1, y + (h / 3) + 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + 1, y + 1), ImVec2(x + (w / 3) + 1, y + 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - (w / 3) - 1, y + 1), ImVec2(x + w - 1, y + 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - 1, y + 1), ImVec2(x + w - 1, y + (h / 3) + 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + 1, y + h - (h / 3) - 1), ImVec2(x + 1, y + h - 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + 1, y + h - 1), ImVec2(x + (w / 3) + 1, y + h - 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - (w / 3) - 1, y + h - 1), ImVec2(x + w - 1, y + h - 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - 1, y + h - (h / 3) - 1), ImVec2(x + w - 1, y + h - 1), ImColor(0, 0, 0), 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x, y + (h / 3)), color, 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x + (w / 3), y), color, 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - (w / 3), y), ImVec2(x + w, y), color, 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + (h / 3)), color, 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h - (h / 3)), ImVec2(x, y + h), color, 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x + (w / 3), y + h), color, 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - (w / 3), y + h), ImVec2(x + w, y + h), color, 1.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y + h - (h / 3)), ImVec2(x + w, y + h), color, 1.0f);
}

namespace updater {

	void run()
	{
		while (true)
		{
			auto uworld = read<uintptr_t>(kernel->ProcessBase + offsets->UWORLD);
			auto owning_game_instance = read<uintptr_t>(uworld + offsets->GAME_INSTANCE);
			auto local_player = read<uintptr_t>(read<uintptr_t>(owning_game_instance + offsets->LOCAL_PLAYERS));
			auto player_controller = read<uintptr_t>(local_player + offsets->PLAYER_CONTROLLER);
			auto local_pawn = read<uintptr_t>(player_controller + offsets->LOCAL_PAWN);
			auto player_state = read<uintptr_t>(local_pawn + offsets->PLAYER_STATE);
			auto root_component = read<uintptr_t>(local_pawn + offsets->ROOT_COMPONENT);

			// Store local data
			cache::uworld = uworld;
			cache::local_pawn = local_pawn;
			cache::my_team_id = read<int>(player_state + offsets->TEAM_INDEX);

			auto game_state = read<uintptr_t>(uworld + offsets->GAME_STATE);
			tarray<uintptr_t> player_array(game_state + offsets->PLAYER_ARRAY);
			auto player_list = player_array.get_all();

			udata::world_t.game_state = game_state;
			udata::world_t.player_array = player_list;

			std::vector<udata::actor> actors;
			actors.reserve(player_list.size());
			udata::world_t.mesh = NULL;

			for (const auto& player_state : player_list)
			{
				if (!player_state) continue;

				auto team_id = read<int>(player_state + offsets->TEAM_INDEX);
				if (team_id == cache::my_team_id) continue;

				auto pawn_private = read<uintptr_t>(player_state + offsets->PAWN_PRIVATE);
				if (!pawn_private || pawn_private == cache::local_pawn) continue;

				auto mesh = read<uintptr_t>(pawn_private + offsets->MESH);
				if (!mesh) continue;

				actors.emplace_back(player_state, pawn_private, mesh, team_id);
			}

			udata::actor_t = std::move(actors);

			Sleep(1200);
		}
	}

	void CacheLevels()
	{
	}

	uintptr_t RootComponent(uintptr_t actor)
	{
		return read<uintptr_t>(actor + offsets->ROOT_COMPONENT);
	}

	Vector3 GetLocation(uintptr_t actor)
	{
		return read<Vector3>(RootComponent(actor) + offsets->RELATIVE_LOCATION);
	}

	
}
void chinahat(ImVec2 position, ImColor color, float size)
{
	ImGui::GetForegroundDrawList()->AddCircleFilled(position, size, color, 16);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(position.x - size, position.y), ImVec2(position.x + size, position.y), color, 2.0f);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(position.x - size / 2, position.y - size / 2), ImVec2(position.x + size / 2, position.y - size / 2), color, 2.0f);
}
void RiceHat(const Vector3& head, float radius, float height, int segments, float tick) {
	Vector3 tip = { head.x, head.y, head.z + height };

	std::vector<ImVec2> basePoints2D;

	for (int i = 0; i < segments; ++i) {
		float angle = (2 * M_PI / segments) * i;
		float x = cosf(angle) * radius;
		float y = sinf(angle) * radius;

		Vector3 basePoint3D = { head.x + x, head.y + y, head.z + 5 };
		Vector2 screenBase = project_world_to_screen(basePoint3D);
		basePoints2D.push_back(ImVec2(screenBase.x, screenBase.y));
	}

	Vector2 screenTip = project_world_to_screen(tip);
	ImVec2 tip2D = ImVec2(screenTip.x, screenTip.y);

	auto draw_list = ImGui::GetForegroundDrawList();

	ImU32 cyan = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));  // Cyan
	ImU32 black = ImGui::GetColorU32(ImVec4(0, 0, 0, 1.0f));           // Black
	float outlineThickness = tick + 2.0f;
	for (int i = 0; i < segments; ++i) {
		draw_list->AddLine(tip2D, basePoints2D[i], black, outlineThickness);
		draw_list->AddLine(basePoints2D[i], basePoints2D[(i + 1) % segments], black, outlineThickness);
	}
	for (int i = 0; i < segments; ++i) {
		draw_list->AddLine(tip2D, basePoints2D[i], cyan, tick);
		draw_list->AddLine(basePoints2D[i], basePoints2D[(i + 1) % segments], cyan, tick);
	}
}








