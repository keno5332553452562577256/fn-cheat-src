#pragma once
#include "../SDK/sdk.h"
#include "../offsets/offsets.h"
#include "../settings/settings.h"
#include "drawing.h"
#include <shared_mutex>
#include "aimbot.h"
std::string getRank(int tier) {
	switch (tier) {
	case 0:  return "Unranked";
	case 1:  return "Bronze 2";
	case 2:  return "Bronze 3";
	case 3:  return "Silver 1";
	case 4:  return "Silver 2";
	case 5:  return "Silver 3";
	case 6:  return "Gold 1";
	case 7:  return "Gold 2";
	case 8:  return "Gold 3";
	case 9:  return "Platinum 1";
	case 10: return "Platinum 2";
	case 11: return "Platinum 3";
	case 12: return "Diamond 1";
	case 13: return "Diamond 2";
	case 14: return "Diamond 3";
	case 15: return "Elite";
	case 16: return "Champion";
	case 17: return "Unreal";
	default: return "Unranked";
	}
}
ImVec4 getRankColor(int tier) {
	switch (tier) {
	case 0:  return ImVec4(0.6f, 0.0f, 0.6f, 1.0f);
	case 3:  return ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	case 2:  return ImVec4(0.8f, 0.5f, 0.2f, 1.0f);
	case 1:  return ImVec4(0.8f, 0.5f, 0.2f, 1.0f);
	case 5:  return ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	case 6:  return ImVec4(1.0f, 0.843f, 0.0f, 1.0f);
	case 4:  return ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	case 8:  return ImVec4(1.0f, 0.843f, 0.0f, 1.0f);
	case 7:  return ImVec4(1.0f, 0.843f, 0.0f, 1.0f);
	case 9:  return ImVec4(0.4f, 0.6f, 1.0f, 1.0f);
	case 10: return ImVec4(0.4f, 0.6f, 1.0f, 1.0f);
	case 11: return ImVec4(0.4f, 0.6f, 1.0f, 1.0f);
	case 14: return ImVec4(0.7f, 0.4f, 1.0f, 1.0f);
	case 12: return ImVec4(0.7f, 0.4f, 1.0f, 1.0f);
	case 15: return ImVec4(0.9f, 0.85f, 0.6f, 1.0f);
	case 13: return ImVec4(0.7f, 0.4f, 1.0f, 1.0f);
	case 16: return ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
	case 17: return ImVec4(0.6f, 0.0f, 0.6f, 1.0f);
	default: return ImVec4(0.6f, 0.0f, 0.6f, 1.0f);
	}
}

void world_esp()
{
	auto entities = get_loot_cache_snapshot();
	for (const auto& entity : entities)
	{
		if (!entity.is_valid())
			continue;
		if (entity.distance > 150) // max distance here
			continue;
		Vector2 screen = project_world_to_screen(entity.pos);

		if (screen.x == 0 && screen.y == 0)
			continue;

		float dist_m = entity.distance;

		// Loot-Filter
		if (settings::world::uncommon_loot && entity.name.find("Uncommon") != std::string::npos) goto draw_loot;
		if (settings::world::common_loot && entity.name.find("Common") != std::string::npos) goto draw_loot;
		if (settings::world::rare_loot && entity.name.find("Rare") != std::string::npos) goto draw_loot;
		if (settings::world::epic_loot && entity.name.find("Epic") != std::string::npos) goto draw_loot;
		if (settings::world::legendary_loot && entity.name.find("Legendary") != std::string::npos) goto draw_loot;
		if (settings::world::mythic_loot && entity.name.find("Mythic") != std::string::npos) goto draw_loot;
		if (settings::world::llamas && entity.name.find("Llama") != std::string::npos) goto draw_loot;
		if (settings::world::chests && entity.name.find("Chest") != std::string::npos) goto draw_loot;
		if (settings::world::ammobox && entity.name.find("Ammo") != std::string::npos) goto draw_loot;
		if (settings::world::foodbox && entity.name.find("Food") != std::string::npos) goto draw_loot;
		if (settings::world::vehicle && entity.name.find("Vehicle") != std::string::npos) goto draw_loot;
		if (settings::world::spirits && entity.name.find("Spirit") != std::string::npos) goto draw_loot;
		continue;

draw_loot:
		ImVec2 text_size = ImGui::CalcTextSize(entity.name.c_str());
		ImVec2 pos_text = ImVec2(screen.x - text_size.x / 2, screen.y - 18);
		ImGui::GetForegroundDrawList()->AddText(
			ImGui::GetFont(),
			13.0f,
			(pos_text),
			ImColor(255, 255, 255),
			entity.name.c_str()
		);
	}
}

struct players {
	uintptr_t player_state;
	uintptr_t pawn_private;
	uintptr_t Mesh;
	uintptr_t actorRootComponent;
	Vector3 location;
};

std::vector<players> EntityListA;
std::vector<players> templist;
std::shared_mutex EntityListMutex;
void caching( )
{
	while (true)
	{
		templist.clear();

		cache::uworld = read<uintptr_t>(kernel->ProcessBase + offsets->UWORLD);
		cache::game_instance = read<uintptr_t>(cache::uworld + offsets->GAME_INSTANCE);
		cache::local_players = read<uintptr_t>(read<uintptr_t>(cache::game_instance + offsets->LOCAL_PLAYERS));
		cache::player_controller = read<uintptr_t>(cache::local_players + offsets->PLAYER_CONTROLLER);
		cache::local_pawn = read<uintptr_t>(cache::player_controller + offsets->LOCAL_PAWN);
		cache::localRelativeLocation = read<Vector3>(cache::root_component + offsets->RELATIVE_LOCATION);
		if (cache::local_pawn != 0)
		{
			cache::root_component = read<uintptr_t>(cache::local_pawn + offsets->ROOT_COMPONENT);
			cache::relative_location = read<Vector3>(cache::root_component + offsets->RELATIVE_LOCATION);
			cache::player_state = read<uintptr_t>(cache::local_pawn + offsets->PLAYER_STATE);
			cache::my_team_id = read<int>(cache::player_state + offsets->TEAM_INDEX);
		}
		cache::game_state = read<uintptr_t>(cache::uworld + offsets->GAME_STATE);
		cache::player_array = read<uintptr_t>(cache::game_state + offsets->PLAYER_ARRAY);
		cache::player_count = read<int>(cache::game_state + (offsets->PLAYER_ARRAY + sizeof(uintptr_t)));
		cache::closest_distance = FLT_MAX;
		cache::closest_mesh = NULL;
		for (int i = 0; i < cache::player_count; i++)
		{
			uintptr_t player_state = read<uintptr_t>(cache::player_array + (i * sizeof(uintptr_t)));
			if (!player_state) continue;
			//int player_team_id = read<int>(player_state + TEAM_INDEX); //self esp
			//if (player_team_id == cache::my_team_id) continue; //self esp
			uintptr_t pawn_private = read<uintptr_t>(player_state + offsets->PAWN_PRIVATE);
			//if (!pawn_private || pawn_private == cache::local_pawn) continue; //self esp
			if (!pawn_private) continue;
			if (pawn_private == cache::local_pawn && !settings::visuals::selfesp) continue;
			uintptr_t mesh = read<uintptr_t>(pawn_private + offsets->MESH);
			if (!mesh) continue;
			uintptr_t actorRootComponent = read<uintptr_t>(pawn_private + offsets->ROOT_COMPONENT);
			Vector3 actorRelativeLocation = read<Vector3>(actorRootComponent + offsets->RELATIVE_LOCATION);

			players entity{};

			entity.actorRootComponent = actorRootComponent;
			entity.Mesh = mesh;
			entity.pawn_private = pawn_private;
			entity.location = actorRelativeLocation;
			entity.player_state = player_state;
			templist.push_back(entity);
		}
		EntityListA.swap(templist);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}
void actor()
{
	for (auto player : EntityListA)
	{
		auto mesh = player.Mesh;

		Vector3 head3d = get_entity_bone(mesh, 110);
		Vector2 head2d = project_world_to_screen(Vector3(head3d.x, head3d.y, head3d.z + 15));
		Vector3 bottom3d = get_entity_bone(mesh, 0);
		Vector2 bottom2d = project_world_to_screen(bottom3d);
		float box_height = abs(head2d.y - bottom2d.y);
		float box_width = box_height * 0.30f;
		uintptr_t actorRootComponent = player.actorRootComponent;
		Vector3 actorRelativeLocation = player.location;
		float distance = cache::localRelativeLocation.distance(actorRelativeLocation) / 100.0f;
		double dx = head2d.x - screen_center_x;
		double dy = head2d.y - screen_center_y;
		auto draw_list = ImGui::GetForegroundDrawList();
		if (settings::visuals::box)
		{
			auto color = is_visible ? settings::colors::icBoxColorVisible : settings::colors::icBoxColorInvisible;
			if (settings::visuals::boxType == boxType::normal)
				drawbox(head2d.x - box_width / 2, head2d.y, box_width, box_height, color);
			else if (settings::visuals::boxType == boxType::corner)
				drawcornerbox(head2d.x - box_width / 2, head2d.y, box_width, box_height, color);
		}
		if (settings::visuals::name)
		{
			std::string playerUsername = GetPlayerName(player.player_state);
			ImVec2 textSize = ImGui::CalcTextSize(playerUsername.c_str());
			ImVec2 rectPos = ImVec2(head2d.x - textSize.x / 2 - 10.0f, head2d.y - textSize.y - 20.0f);
			ImVec2 rectSize = ImVec2(textSize.x + 2 * 10.0f, textSize.y + 2 * 5.0f);
			ImGui::GetBackgroundDrawList()->AddRectFilled(
				rectPos,
				ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y),
				ImColor(10, 10, 10),
				0.0f
			);
			ImGui::GetBackgroundDrawList()->AddRectFilled(
				rectPos,
				ImVec2(rectPos.x + rectSize.x, rectPos.y + 2.0f),
				ImColor(0, 128, 255)
			);
			ImGui::GetBackgroundDrawList()->AddText(
				ImGui::GetFont(),
				13.0f,
				ImVec2(rectPos.x + (rectSize.x - textSize.x) / 2, rectPos.y + (rectSize.y - textSize.y) / 2),
				ImColor(255, 255, 255),
				playerUsername.c_str()
			);
		}
		if (settings::visuals::rank)
		{
			uintptr_t habaneroComponent = read<uintptr_t>(player.player_state + offsets->HABANERO_COMPONENT);
			uint32_t rank = read<uint32_t>(habaneroComponent + offsets->RANKED_PROGRESS + 0x10);
			std::string rankName = getRank(rank);
			ImVec2 textSize = ImGui::CalcTextSize(rankName.c_str());
			float offsetY = settings::visuals::distance ? 25.0f : 10.0f;
			ImVec2 rectPos = ImVec2(bottom2d.x - textSize.x / 2 - 10.0f, bottom2d.y + offsetY - textSize.y / 2 - 5.0f);
			ImVec2 rectSize = ImVec2(textSize.x + 2 * 10.0f, textSize.y + 2 * 5.0f);
			ImGui::GetBackgroundDrawList()->AddRectFilled(
				rectPos,
				ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y),
				ImColor(10, 10, 10),
				0.0f
			);
			ImGui::GetBackgroundDrawList()->AddRectFilled(
				rectPos,
				ImVec2(rectPos.x + rectSize.x, rectPos.y + 2.0f),
				ImColor(0, 128, 255)
			);
			ImGui::GetBackgroundDrawList()->AddText(
				ImGui::GetFont(),
				13.0f,
				ImVec2(rectPos.x + (rectSize.x - textSize.x) / 2, rectPos.y + (rectSize.y - textSize.y) / 2),
				ImColor(255, 255, 255),
				rankName.c_str()
			);
		}
		if (settings::visuals::distance)
		{
			char dist[64];
			sprintf_s(dist, "%.fm", distance);
			ImVec2 textSize = ImGui::CalcTextSize(dist);
			ImVec2 rectPos = ImVec2(bottom2d.x - textSize.x / 2 - 6.0f, bottom2d.y + 10 - textSize.y / 2 - 3.0f);
			ImVec2 rectSize = ImVec2(textSize.x + 2 * 6.0f, textSize.y + 2 * 3.0f);
			ImGui::GetBackgroundDrawList()->AddRectFilled(rectPos, ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y), ImColor(10, 10, 10), 4.0f);
			ImGui::GetBackgroundDrawList()->AddRectFilled(rectPos, ImVec2(rectPos.x + rectSize.x, rectPos.y + 2), ImColor(0, 128, 255), 4.0f);
			ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 12.0f, ImVec2(rectPos.x + (rectSize.x - textSize.x) / 2, rectPos.y + (rectSize.y - textSize.y) / 2), ImColor(255, 255, 255), dist);
		}
		if (settings::visuals::platform)
		{
			uintptr_t platformPtr = read<uintptr_t>(player.player_state + offsets->PLATFORM);
			wchar_t platformChar[64] = { 0 };
			kernel->ReadPhysical(reinterpret_cast<PVOID>(platformPtr), reinterpret_cast<uint8_t*>(platformChar), sizeof(platformChar));
			std::wstring platformWstr(platformChar);
			std::string platformStr(platformWstr.begin(), platformWstr.end());
			ImVec2 textSize = ImGui::CalcTextSize(platformStr.c_str());
			float offsetY = settings::visuals::name ? 25.0f : 10.0f;
			ImVec2 rectPos = ImVec2(bottom2d.x - textSize.x / 2 - 6.0f, bottom2d.y - offsetY - textSize.y / 2 - 3.0f);
			ImVec2 rectSize = ImVec2(textSize.x + 2 * 6.0f, textSize.y + 2 * 3.0f);
			ImGui::GetBackgroundDrawList()->AddRectFilled(rectPos, ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y), ImColor(10, 10, 10), 4.0f);
			ImGui::GetBackgroundDrawList()->AddRectFilled(rectPos, ImVec2(rectPos.x + rectSize.x, rectPos.y + 2), ImColor(0, 128, 255), 4.0f);
			ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 12.0f, ImVec2(rectPos.x + (rectSize.x - textSize.x) / 2, rectPos.y + (rectSize.y - textSize.y) / 2), ImColor(255, 255, 255), platformStr.c_str());
		}
		if (settings::visuals::skeleton) {
			std::vector<Vector3> bones = {
				get_entity_bone(mesh, 66),  // neck
				get_entity_bone(mesh, 9),   // left shoulder
				get_entity_bone(mesh, 10),  // left elbow
				get_entity_bone(mesh, 11),  // left hand
				get_entity_bone(mesh, 38),  // right shoulder
				get_entity_bone(mesh, 39),  // right elbow
				get_entity_bone(mesh, 40),  // right hand
				get_entity_bone(mesh, 2),   // pelvis
				get_entity_bone(mesh, 71),  // left hip
				get_entity_bone(mesh, 72),  // left knee
				get_entity_bone(mesh, 78),  // right hip
				get_entity_bone(mesh, 79),  // right knee
				get_entity_bone(mesh, 75),  // left foot
				get_entity_bone(mesh, 82),  // right foot
				get_entity_bone(mesh, 67)   // head
			};
			std::vector<Vector2> screenPositions(bones.size());
			for (size_t i = 0; i < bones.size(); ++i) {
				screenPositions[i] = project_world_to_screen(bones[i]);
			}
			ImU32 color = is_visible(mesh) ? settings::colors::icSkeletonColorVisible : settings::colors::icSkeletonColorInvisible;
			ImU32 outline = IM_COL32(0, 0, 0, 255); //outline col
			auto draw_line = [&](int idx1, int idx2) {
				ImVec2 p1(screenPositions[idx1].x, screenPositions[idx1].y);
				ImVec2 p2(screenPositions[idx2].x, screenPositions[idx2].y);
				ImGui::GetBackgroundDrawList()->AddLine(p1, p2, outline, 3.5f);
				ImGui::GetBackgroundDrawList()->AddLine(p1, p2, color, 2.0f);
				};
			draw_line(1, 4);   
			draw_line(1, 2);   
			draw_line(2, 3);  
			draw_line(4, 5);   
			draw_line(5, 6);   
			draw_line(0, 7);   
			draw_line(7, 8);   
			draw_line(8, 9);  
			draw_line(9, 12);  
			draw_line(7, 10);  
			draw_line(10, 11); 
			draw_line(11, 13); 
		}
		if (settings::visuals::debug)
		{
			char debugLines[10][128];
			sprintf_s(debugLines[0], "UWorld: 0x%llX", cache::uworld);
			sprintf_s(debugLines[1], "GameInstance: 0x%llX", cache::game_instance);
			sprintf_s(debugLines[2], "GameState: 0x%llX", cache::game_state);
			sprintf_s(debugLines[3], "PlayerArray: 0x%llX", cache::player_array);
			sprintf_s(debugLines[4], "Driver Base: 0x%llX", kernel->ProcessBase);

			ImFont* font = ImGui::GetFont();
			float fontSize = 14.0f;
			float padding = 10.0f;
			float spacing = 2.0f;
			float maxWidth = 0.0f;
			float totalHeight = 0.0f;
			ImVec2 textSizes[10];
			for (int i = 0; i < 10; ++i)
			{
				textSizes[i] = ImGui::CalcTextSize(debugLines[i]);
				if (textSizes[i].x > maxWidth)
					maxWidth = textSizes[i].x;
				totalHeight += textSizes[i].y + spacing;
			}
			ImVec2 rectPos = ImVec2(150, 25);
			ImVec2 rectSize = ImVec2(maxWidth + 2 * padding, totalHeight + 2 * padding);
			ImGui::GetBackgroundDrawList()->AddRectFilled(
				rectPos,
				ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y),
				ImColor(10, 10, 10)
			);
			ImGui::GetBackgroundDrawList()->AddRectFilled(
				rectPos,
				ImVec2(rectPos.x + rectSize.x, rectPos.y + 2),
				ImColor(0, 128, 255)
			);
			float y = rectPos.y + padding;
			for (int i = 0; i < 10; ++i)
			{
				float x = rectPos.x + (rectSize.x - textSizes[i].x) / 2;
				ImGui::GetBackgroundDrawList()->AddText(
					font, fontSize, ImVec2(x, y), ImColor(255, 255, 255), debugLines[i]
				);
				y += textSizes[i].y + spacing;
			}
		}
		if (settings::visuals::debugapi)
		{
			char debugLines[10][128];
			sprintf_s(debugLines[1], "API UWorld: 0x%X", offsets->UWORLD);
			sprintf_s(debugLines[2], "API GameInstance: 0x%X", offsets->GAME_INSTANCE);
			sprintf_s(debugLines[3], "API GameState: 0x%X", offsets->GAME_STATE);
			sprintf_s(debugLines[4], "API PlayerArray: 0x%X", offsets->PLAYER_ARRAY);
			sprintf_s(debugLines[5], "API Mesh: 0x%X", offsets->MESH);
			ImFont* font = ImGui::GetFont();
			float fontSize = 14.0f;
			float padding = 10.0f;
			float spacing = 2.0f;
			float maxWidth = 0.0f;
			float totalHeight = 0.0f;
			ImVec2 textSizes[10];
			for (int i = 0; i < 10; ++i)
			{
				textSizes[i] = ImGui::CalcTextSize(debugLines[i]);
				if (textSizes[i].x > maxWidth)
					maxWidth = textSizes[i].x;
				totalHeight += textSizes[i].y + spacing;
			}
			ImVec2 rectPos = ImVec2(150, 25);
			ImVec2 rectSize = ImVec2(maxWidth + 2 * padding, totalHeight + 2 * padding);
			ImGui::GetBackgroundDrawList()->AddRectFilled(
				rectPos,
				ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y),
				ImColor(10, 10, 10)
			);
			ImGui::GetBackgroundDrawList()->AddRectFilled(
				rectPos,
				ImVec2(rectPos.x + rectSize.x, rectPos.y + 2),
				ImColor(0, 128, 255)
			);
			float y = rectPos.y + padding;
			for (int i = 0; i < 10; ++i)
			{
				float x = rectPos.x + (rectSize.x - textSizes[i].x) / 2;
				ImGui::GetBackgroundDrawList()->AddText(
					font, fontSize, ImVec2(x, y), ImColor(255, 255, 255), debugLines[i]
				);
				y += textSizes[i].y + spacing;
			}
		}
		if (settings::visuals::worldesp)
		{
			world_esp();
		}
		
		float dist = sqrtf(dx * dx + dy * dy);
		if (dist <= settings::aimbot::fov && dist < cache::closest_distance)
		{
			cache::closest_distance = dist;
			cache::closest_mesh = mesh;
		}
		if (settings::aimbot::enable && GetAsyncKeyState(settings::aimbot::current_key))
		{
			memory(cache::closest_mesh);
		}
		if (settings::visuals::chinahat)
		{
			RiceHat(head3d, 15.0f, 15.0f, 20, 0.7f);
		}
		// Low-HP-Highlight (Rahmenfarbe ändern)
		bool isLowHP = false;
		if (settings::visuals::highlight_lowhp) {
			float health = read<float>(player.pawn_private + OFFSET_Health());
			float maxHealth = read<float>(player.pawn_private + OFFSET_MaxHealth());
			if (maxHealth > 0 && health / maxHealth < 0.3f) {
				isLowHP = true;
			}
		}

		// Health-Balken
		if (settings::visuals::show_health) {
			float health = read<float>(player.pawn_private + OFFSET_Health());
			float maxHealth = read<float>(player.pawn_private + OFFSET_MaxHealth());
			bool isDying = read<bool>(player.pawn_private + OFFSET_b_is_dying());
			if (maxHealth > 0 && health > 0 && !isDying) {
				float healthPerc = health / maxHealth;
				ImVec2 barPos = ImVec2(head2d.x - box_width / 2 - 8, head2d.y);
				ImVec2 barSize = ImVec2(6, box_height);
				ImGui::GetForegroundDrawList()->AddRectFilled(barPos, ImVec2(barPos.x + barSize.x, barPos.y + barSize.y), ImColor(30, 30, 30));
				ImGui::GetForegroundDrawList()->AddRectFilled(barPos, ImVec2(barPos.x + barSize.x, barPos.y + barSize.y * healthPerc), ImColor(0, 255, 0));
			}
		} 
		// Kills anzeigen
		if (settings::visuals::show_kills) {
			int kills = read<int>(player.player_state + OFFSET_kill_score());
			char killsText[16];
			sprintf_s(killsText, "Kills: %d", kills);
			ImVec2 textSize = ImGui::CalcTextSize(killsText);
			ImVec2 pos = ImVec2(head2d.x - textSize.x / 2, head2d.y - 35);
			ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), 13.0f, pos, ImColor(255, 255, 0), killsText);
		}

		// Season-Level anzeigen
		if (settings::visuals::show_season_level) {
			int level = read<int>(player.player_state + OFFSET_season_level_ui_display());
			char levelText[32];
			sprintf_s(levelText, "Level: %d", level);
			ImVec2 textSize = ImGui::CalcTextSize(levelText);
			ImVec2 pos = ImVec2(head2d.x - textSize.x / 2, head2d.y - 50);
			ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), 13.0f, pos, ImColor(0, 200, 255), levelText);
		}

		// Spectator-Warnung
		if (settings::visuals::show_spectators) {
			int spectators = read<int>(player.player_state + OFFSET_SpectatorCount());
			if (spectators > 0) {
				char specText[32];
				sprintf_s(specText, "Spectators: %d", spectators);
				ImVec2 textSize = ImGui::CalcTextSize(specText);
				ImVec2 pos = ImVec2(head2d.x - textSize.x / 2, head2d.y - 65);
				ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), 13.0f, pos, ImColor(255, 0, 0), specText);
			}
		}

		// Fahrzeuge anzeigen (ESP)
		if (settings::visuals::show_vehicles || settings::world::vehicle_esp) {
			uintptr_t vehicle = read<uintptr_t>(player.pawn_private + OFFSET_current_vehicle());
			if (vehicle) {
				ImVec2 pos = ImVec2(bottom2d.x, bottom2d.y + 20);
				ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), 13.0f, pos, ImColor(0, 255, 255), "Fahrzeug");
			}
		}

		// Box-Farbe für Low-HP-Highlight anpassen
		if (settings::visuals::box && isLowHP) {
			ImU32 highlightColor = IM_COL32(255, 0, 0, 255);
			if (settings::visuals::boxType == boxType::normal)
				drawbox(head2d.x - box_width / 2, head2d.y, box_width, box_height, highlightColor);
			else if (settings::visuals::boxType == boxType::corner)
				drawcornerbox(head2d.x - box_width / 2, head2d.y, box_width, box_height, highlightColor);
		}
		//more options
	}
}