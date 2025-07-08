#pragma once
#include "../settings/settings.h"
#include "../SDK/sdk.h"
#include "../kernel/coms.h"
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <windows.h> // Für mouse_event

intptr_t NetConnection = 0x520;
uintptr_t RotationInput = NetConnection + 0x8;

// Bone IDs für verschiedene Ziel-Bereiche
// HINWEIS: Prüfe, ob diese IDs für dein Spiel korrekt sind!
const int BONE_HEAD = 66;
const int BONE_NECK = 67;
const int BONE_BODY = 5;

// Trigger-Bot Timer
static auto last_trigger_time = std::chrono::steady_clock::now();

bool memory_event(Vector3 newpos)
{
	tWrite<Vector3>(cache::player_controller + RotationInput, newpos); 
	return true;
}

// Neue Funktion: Get Target Bone basierend auf Einstellung
int get_target_bone() {
	switch (settings::aimbot::target_bone) {
		case 0: return BONE_HEAD;   // Head
		case 1: return BONE_NECK;   // Neck
		case 2: return BONE_BODY;   // Body
		case 3: return (std::rand() % 3 == 0) ? BONE_HEAD : (std::rand() % 2 == 0) ? BONE_NECK : BONE_BODY; // Random
		default: return BONE_HEAD;
	}
}

// Neue Funktion: Team Check
bool is_enemy(uintptr_t player_state) {
	if (!settings::aimbot::team_check) return true;
	
	int target_team = read<int>(player_state + offsets->TEAM_INDEX);
	return target_team != cache::my_team_id;
}

// Neue Funktion: Distance Check
bool is_in_range(Vector3 target_pos) {
	if (!settings::aimbot::distance_check) return true;
	
	Vector3 local_pos = cache::relative_location;
	float distance = local_pos.distance(target_pos);
	return distance <= settings::aimbot::max_distance;
}

// Neue Funktion: Prediction
Vector3 predict_movement(Vector3 current_pos, Vector3 velocity) {
	if (!settings::aimbot::prediction) return current_pos;
	
	// Einfache lineare Vorhersage basierend auf Geschwindigkeit
	return Vector3(
		current_pos.x + velocity.x * settings::aimbot::prediction_strength,
		current_pos.y + velocity.y * settings::aimbot::prediction_strength,
		current_pos.z + velocity.z * settings::aimbot::prediction_strength
	);
}

// Neue Funktion: Trigger Bot
bool should_trigger() {
	if (!settings::aimbot::trigger_bot) return false;
	
	auto now = std::chrono::steady_clock::now();
	auto time_since_last = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_trigger_time).count();
	
	if (time_since_last >= (settings::aimbot::trigger_delay * 1000)) {
		last_trigger_time = now;
		return true;
	}
	return false;
}

// Neue Funktion: Recoil Control
Vector3 apply_recoil_control(Vector3 aim_pos) {
	if (!settings::aimbot::recoil_control) return aim_pos;
	
	// Einfache Recoil-Kompensation (kann erweitert werden)
	Vector3 recoil_offset = Vector3(0, 0, settings::aimbot::recoil_strength * 0.1f);
	return Vector3(aim_pos.x, aim_pos.y, aim_pos.z - recoil_offset.z);
}

void memory(uintptr_t target_mesh)
{
	// Team Check
	uintptr_t player_state = read<uintptr_t>(target_mesh + offsets->PLAYER_STATE);
	if (!is_enemy(player_state)) return;
	
	// Distance Check
	Vector3 target_pos = get_entity_bone(target_mesh, get_target_bone()); // Nutze hier direkt den gewählten Bone
	if (!is_in_range(target_pos)) return;
	
	// Visible Check (erweitert)
	if (settings::aimbot::visible_check || settings::aimbot::aim_at_visible_only)
	{
		if (!target_mesh || !is_visible(target_mesh))
			return;
	}
	
	// Get Target Bone basierend auf Einstellung
	int target_bone = get_target_bone();
	Vector3 bone3d = get_entity_bone(target_mesh, target_bone);
	
	// Prediction anwenden mit echter Velocity
	uintptr_t pawn_private = read<uintptr_t>(player_state + offsets->PAWN_PRIVATE);
	Vector3 velocity = read<Vector3>(pawn_private + 0x180); // Velocity Offset
	bone3d = predict_movement(bone3d, velocity);
	
	// Recoil Control anwenden
	bone3d = apply_recoil_control(bone3d);
	
	Vector2 bone2d = project_world_to_screen(bone3d);
	Vector2 target{};
	
	// FOV Check
	if (settings::aimbot::show_fov) {
		float distance_from_center = std::sqrt(std::pow(bone2d.x - screen_center_x, 2) + std::pow(bone2d.y - screen_center_y, 2));
		if (distance_from_center > settings::aimbot::fov) return;
	}
	
	if (bone2d.x != 0)
	{
		if (bone2d.x > screen_center_x)
		{
			target.x = -(screen_center_x - bone2d.x);
			target.x /= (settings::aimbot::smooth_aim ? settings::aimbot::smoothness : 1.0f);
			if (target.x + screen_center_x > screen_center_x * 2) target.x = 0;
		}
		if (bone2d.x < screen_center_x)
		{
			target.x = bone2d.x - screen_center_x;
			target.x /= (settings::aimbot::smooth_aim ? settings::aimbot::smoothness : 1.0f);
			if (target.x + screen_center_x < 0) target.x = 0;
		}
	}
	if (bone2d.y != 0)
	{
		if (bone2d.y > screen_center_y)
		{
			target.y = -(screen_center_y - bone2d.y);
			target.y /= (settings::aimbot::smooth_aim ? settings::aimbot::smoothness : 1.0f);
			if (target.y + screen_center_y > screen_center_y * 2) target.y = 0;
		}
		if (bone2d.y < screen_center_y)
		{
			target.y = bone2d.y - screen_center_y;
			target.y /= (settings::aimbot::smooth_aim ? settings::aimbot::smoothness : 1.0f);
			if (target.y + screen_center_y < 0) target.y = 0;
		}
	}
	
	// Aimbot-Typ: Silent oder Visible
	if (settings::aimbot::aim_type == 0) {
		// Silent Aimbot (unsichtbar)
		memory_event(Vector3(-target.y / 5, target.x / 5, 0));
	} else {
		// Visible Aimbot (sichtbar - bewegt Maus)
		// Hier könnte die Mausbewegung implementiert werden
		memory_event(Vector3(-target.y / 5, target.x / 5, 0));
	}
	
	// Auto Shoot (Trigger Bot)
	if (settings::aimbot::auto_shoot && should_trigger()) {
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		Sleep(10);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
}

