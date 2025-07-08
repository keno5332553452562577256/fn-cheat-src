#pragma once
#include <fstream>
#include <cstring>
#include <vector>
#include <filesystem>
inline bool get_menu = true;
inline int tab = 0;


namespace updater {

    void run();
    void env_run();
    void CacheLevels();
}
enum boxType : int
{
    normal = 0,
    corner = 1,
};
inline int boxValues[] = {
    boxType::normal,
    boxType::corner,
};
inline const char* getBoxTypeName(int type)
{
    switch (type)
    {
    case boxType::normal: return "Normal";
    case boxType::corner: return "Corner";
    default: return "Unknown";
    }
}
namespace settings
{
    namespace visuals
    {
        inline bool enable = true;
        inline bool radar = false;
        inline bool ignore_teamates = true;
        inline bool box = true;
        inline bool skeleton = false;
        inline bool line = false;
        inline bool name = false;
        inline bool platform = false;
        inline bool distance = false;
        inline bool rank = false;
        inline float renderDistance = 300.0f;
        inline int boxType = boxType::normal;
        inline bool selfesp = false;
        inline bool debug = false;
        inline bool worldesp = false;
        inline bool debugapi = false;
        inline bool chinahat = true;
        inline bool show_health = false;
        inline bool show_spectators = false;
        inline bool show_vehicles = false;
        inline bool show_kills = false;
        inline bool show_season_level = false;
        inline bool highlight_lowhp = false;
    }
    namespace colors
    {
        inline ImColor icCrosshairColor = ImColor(255, 255, 255);
        inline ImColor icFovColor = ImColor(255, 255, 255);
        inline ImColor icBoxColorVisible = ImColor(255, 255, 255);
        inline ImColor icBoxColorInvisible = ImColor(255, 0, 0);
        inline ImColor icSkeletonColorVisible = ImColor(255, 255, 255);
        inline ImColor icSkeletonColorInvisible = ImColor(255, 255, 255);
        inline ImColor icTracerColorVisible = ImColor(255, 255, 255);
        inline ImColor icTracerColorInvisible = ImColor(255, 0, 0);
    }
    namespace aimbot
    {
        inline bool enable = false;
        inline bool show_fov = false;
        inline float fov = 160;
        inline float smoothness = 6;
        inline bool visible_check = false;
        inline static int current_key = VK_RBUTTON;
        static int hitboxtarget = 0;

        // Neue Aimbot-Einstellungen
        inline int aim_type = 0; // 0 = Silent, 1 = Visible
        inline int target_bone = 0; // 0 = Head, 1 = Neck, 2 = Body, 3 = Random
        inline bool trigger_bot = false;
        inline float trigger_delay = 0.1f;
        inline bool prediction = false;
        inline float prediction_strength = 1.0f;
        inline bool recoil_control = false;
        inline float recoil_strength = 1.0f;
        inline bool auto_shoot = false;
        inline bool team_check = true;
        inline bool distance_check = false;
        inline float max_distance = 300.0f;
        inline bool smooth_aim = true;
        inline bool aim_at_visible_only = true;
        inline int aim_priority = 0; // 0 = Distance, 1 = Health, 2 = Threat
    }
    namespace world
    {
        inline bool uncommon_loot = false;
        inline bool common_loot = false;
        inline bool llamas = false;
        inline bool rare_loot = false;
        inline bool foodbox = false;
        inline bool epic_loot = false;
        inline bool legendary_loot = false;
        inline bool mythic_loot = false;
        inline bool chests = false;
        inline bool ammobox = false;
        inline bool vehicle = false;
        inline bool spirits = false;
        inline int render_distance_loot = 350;
        inline int render_distance_ammo = 350;
        inline int render_distance_vehicle = 350;
        inline int render_distance_spirits = 350;
        inline int render_distance_llama = 350;
        inline int render_distance_chest = 350;
        inline bool enableOutlinesContainers = false;
        inline bool enableOutlinesLoot = false;
        inline bool vehicle_esp = false;
    }
}

// Funktionsprototypen für Save/Load
void SaveSettings(const std::string& name);
void LoadSettings(const std::string& name);
std::vector<std::string> ListSettingsPresets();

// --- Save/Load-Implementierung ---
struct AllSettings {
    // Visuals
    bool visuals_enable;
    bool visuals_radar;
    bool visuals_ignore_teamates;
    bool visuals_box;
    bool visuals_skeleton;
    bool visuals_line;
    bool visuals_name;
    bool visuals_platform;
    bool visuals_distance;
    bool visuals_rank;
    float visuals_renderDistance;
    int visuals_boxType;
    bool visuals_selfesp;
    bool visuals_debug;
    bool visuals_worldesp;
    bool visuals_debugapi;
    bool visuals_chinahat;
    bool visuals_show_health;
    bool visuals_show_spectators;
    bool visuals_show_vehicles;
    bool visuals_show_kills;
    bool visuals_show_season_level;
    bool visuals_highlight_lowhp;
    // ESP-Farben als float[4]
    float icBoxColorVisible[4];
    float icBoxColorInvisible[4];
    float icSkeletonColorVisible[4];
    float icSkeletonColorInvisible[4];
    float icTracerColorVisible[4];
    float icTracerColorInvisible[4];
    float icFovColor[4];
    float icCrosshairColor[4];
    // Aimbot
    bool aimbot_enable;
    bool aimbot_show_fov;
    float aimbot_fov;
    float aimbot_smoothness;
    bool aimbot_visible_check;
    int aimbot_current_key;
    int aimbot_hitboxtarget;
    int aimbot_aim_type;
    int aimbot_target_bone;
    bool aimbot_trigger_bot;
    float aimbot_trigger_delay;
    bool aimbot_prediction;
    float aimbot_prediction_strength;
    bool aimbot_recoil_control;
    float aimbot_recoil_strength;
    bool aimbot_auto_shoot;
    bool aimbot_team_check;
    bool aimbot_distance_check;
    float aimbot_max_distance;
    bool aimbot_smooth_aim;
    bool aimbot_aim_at_visible_only;
    int aimbot_aim_priority;
    // World
    bool world_uncommon_loot;
    bool world_common_loot;
    bool world_llamas;
    bool world_rare_loot;
    bool world_foodbox;
    bool world_epic_loot;
    bool world_legendary_loot;
    bool world_mythic_loot;
    bool world_chests;
    bool world_ammobox;
    bool world_vehicle;
    bool world_spirits;
    int world_render_distance_loot;
    int world_render_distance_ammo;
    int world_render_distance_vehicle;
    int world_render_distance_spirits;
    int world_render_distance_llama;
    int world_render_distance_chest;
    bool world_enableOutlinesContainers;
    bool world_enableOutlinesLoot;
    bool world_vehicle_esp;
};

inline void SaveSettings(const std::string& name) {
    AllSettings s;
    // Visuals
    s.visuals_enable = settings::visuals::enable;
    s.visuals_radar = settings::visuals::radar;
    s.visuals_ignore_teamates = settings::visuals::ignore_teamates;
    s.visuals_box = settings::visuals::box;
    s.visuals_skeleton = settings::visuals::skeleton;
    s.visuals_line = settings::visuals::line;
    s.visuals_name = settings::visuals::name;
    s.visuals_platform = settings::visuals::platform;
    s.visuals_distance = settings::visuals::distance;
    s.visuals_rank = settings::visuals::rank;
    s.visuals_renderDistance = settings::visuals::renderDistance;
    s.visuals_boxType = settings::visuals::boxType;
    s.visuals_selfesp = settings::visuals::selfesp;
    s.visuals_debug = settings::visuals::debug;
    s.visuals_worldesp = settings::visuals::worldesp;
    s.visuals_debugapi = settings::visuals::debugapi;
    s.visuals_chinahat = settings::visuals::chinahat;
    s.visuals_show_health = settings::visuals::show_health;
    s.visuals_show_spectators = settings::visuals::show_spectators;
    s.visuals_show_vehicles = settings::visuals::show_vehicles;
    s.visuals_show_kills = settings::visuals::show_kills;
    s.visuals_show_season_level = settings::visuals::show_season_level;
    s.visuals_highlight_lowhp = settings::visuals::highlight_lowhp;
    // ESP-Farben als float[4]
    for (int i = 0; i < 4; ++i) {
        s.icBoxColorVisible[i] = settings::colors::icBoxColorVisible.Value[i];
        s.icBoxColorInvisible[i] = settings::colors::icBoxColorInvisible.Value[i];
        s.icSkeletonColorVisible[i] = settings::colors::icSkeletonColorVisible.Value[i];
        s.icSkeletonColorInvisible[i] = settings::colors::icSkeletonColorInvisible.Value[i];
        s.icTracerColorVisible[i] = settings::colors::icTracerColorVisible.Value[i];
        s.icTracerColorInvisible[i] = settings::colors::icTracerColorInvisible.Value[i];
        s.icFovColor[i] = settings::colors::icFovColor.Value[i];
        s.icCrosshairColor[i] = settings::colors::icCrosshairColor.Value[i];
    }
    // Aimbot
    s.aimbot_enable = settings::aimbot::enable;
    s.aimbot_show_fov = settings::aimbot::show_fov;
    s.aimbot_fov = settings::aimbot::fov;
    s.aimbot_smoothness = settings::aimbot::smoothness;
    s.aimbot_visible_check = settings::aimbot::visible_check;
    s.aimbot_current_key = settings::aimbot::current_key;
    s.aimbot_hitboxtarget = settings::aimbot::hitboxtarget;
    s.aimbot_aim_type = settings::aimbot::aim_type;
    s.aimbot_target_bone = settings::aimbot::target_bone;
    s.aimbot_trigger_bot = settings::aimbot::trigger_bot;
    s.aimbot_trigger_delay = settings::aimbot::trigger_delay;
    s.aimbot_prediction = settings::aimbot::prediction;
    s.aimbot_prediction_strength = settings::aimbot::prediction_strength;
    s.aimbot_recoil_control = settings::aimbot::recoil_control;
    s.aimbot_recoil_strength = settings::aimbot::recoil_strength;
    s.aimbot_auto_shoot = settings::aimbot::auto_shoot;
    s.aimbot_team_check = settings::aimbot::team_check;
    s.aimbot_distance_check = settings::aimbot::distance_check;
    s.aimbot_max_distance = settings::aimbot::max_distance;
    s.aimbot_smooth_aim = settings::aimbot::smooth_aim;
    s.aimbot_aim_at_visible_only = settings::aimbot::aim_at_visible_only;
    s.aimbot_aim_priority = settings::aimbot::aim_priority;
    // World
    s.world_uncommon_loot = settings::world::uncommon_loot;
    s.world_common_loot = settings::world::common_loot;
    s.world_llamas = settings::world::llamas;
    s.world_rare_loot = settings::world::rare_loot;
    s.world_foodbox = settings::world::foodbox;
    s.world_epic_loot = settings::world::epic_loot;
    s.world_legendary_loot = settings::world::legendary_loot;
    s.world_mythic_loot = settings::world::mythic_loot;
    s.world_chests = settings::world::chests;
    s.world_ammobox = settings::world::ammobox;
    s.world_vehicle = settings::world::vehicle;
    s.world_spirits = settings::world::spirits;
    s.world_render_distance_loot = settings::world::render_distance_loot;
    s.world_render_distance_ammo = settings::world::render_distance_ammo;
    s.world_render_distance_vehicle = settings::world::render_distance_vehicle;
    s.world_render_distance_spirits = settings::world::render_distance_spirits;
    s.world_render_distance_llama = settings::world::render_distance_llama;
    s.world_render_distance_chest = settings::world::render_distance_chest;
    s.world_enableOutlinesContainers = settings::world::enableOutlinesContainers;
    s.world_enableOutlinesLoot = settings::world::enableOutlinesLoot;
    s.world_vehicle_esp = settings::world::vehicle_esp;
    std::filesystem::create_directories("settings/presets");
    std::string path = "settings/presets/" + name + ".bin";
    std::ofstream f(path, std::ios::binary);
    f.write((char*)&s, sizeof(s));
    f.close();
}

inline void LoadSettings(const std::string& name) {
    AllSettings s;
    std::string path = "settings/presets/" + name + ".bin";
    std::ifstream f(path, std::ios::binary);
    if (!f.read((char*)&s, sizeof(s))) return;
    f.close();
    // Visuals
    settings::visuals::enable = s.visuals_enable;
    settings::visuals::radar = s.visuals_radar;
    settings::visuals::ignore_teamates = s.visuals_ignore_teamates;
    settings::visuals::box = s.visuals_box;
    settings::visuals::skeleton = s.visuals_skeleton;
    settings::visuals::line = s.visuals_line;
    settings::visuals::name = s.visuals_name;
    settings::visuals::platform = s.visuals_platform;
    settings::visuals::distance = s.visuals_distance;
    settings::visuals::rank = s.visuals_rank;
    settings::visuals::renderDistance = s.visuals_renderDistance;
    settings::visuals::boxType = s.visuals_boxType;
    settings::visuals::selfesp = s.visuals_selfesp;
    settings::visuals::debug = s.visuals_debug;
    settings::visuals::worldesp = s.visuals_worldesp;
    settings::visuals::debugapi = s.visuals_debugapi;
    settings::visuals::chinahat = s.visuals_chinahat;
    settings::visuals::show_health = s.visuals_show_health;
    settings::visuals::show_spectators = s.visuals_show_spectators;
    settings::visuals::show_vehicles = s.visuals_show_vehicles;
    settings::visuals::show_kills = s.visuals_show_kills;
    settings::visuals::show_season_level = s.visuals_show_season_level;
    settings::visuals::highlight_lowhp = s.visuals_highlight_lowhp;
    // ESP-Farben als float[4]
    settings::colors::icBoxColorVisible = ImColor(s.icBoxColorVisible[0], s.icBoxColorVisible[1], s.icBoxColorVisible[2], s.icBoxColorVisible[3]);
    settings::colors::icBoxColorInvisible = ImColor(s.icBoxColorInvisible[0], s.icBoxColorInvisible[1], s.icBoxColorInvisible[2], s.icBoxColorInvisible[3]);
    settings::colors::icSkeletonColorVisible = ImColor(s.icSkeletonColorVisible[0], s.icSkeletonColorVisible[1], s.icSkeletonColorVisible[2], s.icSkeletonColorVisible[3]);
    settings::colors::icSkeletonColorInvisible = ImColor(s.icSkeletonColorInvisible[0], s.icSkeletonColorInvisible[1], s.icSkeletonColorInvisible[2], s.icSkeletonColorInvisible[3]);
    settings::colors::icTracerColorVisible = ImColor(s.icTracerColorVisible[0], s.icTracerColorVisible[1], s.icTracerColorVisible[2], s.icTracerColorVisible[3]);
    settings::colors::icTracerColorInvisible = ImColor(s.icTracerColorInvisible[0], s.icTracerColorInvisible[1], s.icTracerColorInvisible[2], s.icTracerColorInvisible[3]);
    settings::colors::icFovColor = ImColor(s.icFovColor[0], s.icFovColor[1], s.icFovColor[2], s.icFovColor[3]);
    settings::colors::icCrosshairColor = ImColor(s.icCrosshairColor[0], s.icCrosshairColor[1], s.icCrosshairColor[2], s.icCrosshairColor[3]);
    // Aimbot
    settings::aimbot::enable = s.aimbot_enable;
    settings::aimbot::show_fov = s.aimbot_show_fov;
    settings::aimbot::fov = s.aimbot_fov;
    settings::aimbot::smoothness = s.aimbot_smoothness;
    settings::aimbot::visible_check = s.aimbot_visible_check;
    settings::aimbot::current_key = s.aimbot_current_key;
    settings::aimbot::hitboxtarget = s.aimbot_hitboxtarget;
    settings::aimbot::aim_type = s.aimbot_aim_type;
    settings::aimbot::target_bone = s.aimbot_target_bone;
    settings::aimbot::trigger_bot = s.aimbot_trigger_bot;
    settings::aimbot::trigger_delay = s.aimbot_trigger_delay;
    settings::aimbot::prediction = s.aimbot_prediction;
    settings::aimbot::prediction_strength = s.aimbot_prediction_strength;
    settings::aimbot::recoil_control = s.aimbot_recoil_control;
    settings::aimbot::recoil_strength = s.aimbot_recoil_strength;
    settings::aimbot::auto_shoot = s.aimbot_auto_shoot;
    settings::aimbot::team_check = s.aimbot_team_check;
    settings::aimbot::distance_check = s.aimbot_distance_check;
    settings::aimbot::max_distance = s.aimbot_max_distance;
    settings::aimbot::smooth_aim = s.aimbot_smooth_aim;
    settings::aimbot::aim_at_visible_only = s.aimbot_aim_at_visible_only;
    settings::aimbot::aim_priority = s.aimbot_aim_priority;
    // World
    settings::world::uncommon_loot = s.world_uncommon_loot;
    settings::world::common_loot = s.world_common_loot;
    settings::world::llamas = s.world_llamas;
    settings::world::rare_loot = s.world_rare_loot;
    settings::world::foodbox = s.world_foodbox;
    settings::world::epic_loot = s.world_epic_loot;
    settings::world::legendary_loot = s.world_legendary_loot;
    settings::world::mythic_loot = s.world_mythic_loot;
    settings::world::chests = s.world_chests;
    settings::world::ammobox = s.world_ammobox;
    settings::world::vehicle = s.world_vehicle;
    settings::world::spirits = s.world_spirits;
    settings::world::render_distance_loot = s.world_render_distance_loot;
    settings::world::render_distance_ammo = s.world_render_distance_ammo;
    settings::world::render_distance_vehicle = s.world_render_distance_vehicle;
    settings::world::render_distance_spirits = s.world_render_distance_spirits;
    settings::world::render_distance_llama = s.world_render_distance_llama;
    settings::world::render_distance_chest = s.world_render_distance_chest;
    settings::world::enableOutlinesContainers = s.world_enableOutlinesContainers;
    settings::world::enableOutlinesLoot = s.world_enableOutlinesLoot;
    settings::world::vehicle_esp = s.world_vehicle_esp;
}

inline std::vector<std::string> ListSettingsPresets() {
    std::vector<std::string> result;
    for (const auto& entry : std::filesystem::directory_iterator("settings/presets")) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".bin") {
                result.push_back(filename.substr(0, filename.size() - 4));
            }
        }
    }
    return result;
}
