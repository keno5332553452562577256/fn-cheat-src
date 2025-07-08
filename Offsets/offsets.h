#pragma once
#include <memory>
#include <winperf.h>

struct Offsets {
    DWORD UWORLD;
    DWORD GAME_INSTANCE;
    DWORD LOCAL_PLAYERS;
    DWORD PLAYER_CONTROLLER;
    DWORD LOCAL_PAWN;
    DWORD PAWN_PRIVATE;
    DWORD ROOT_COMPONENT;
    DWORD RELATIVE_LOCATION;
    DWORD PLAYER_STATE;
    DWORD TEAM_INDEX;
    DWORD GAME_STATE;
    DWORD PLAYER_ARRAY;
    DWORD MESH;
    DWORD COMPONENT_TO_WORLD;
    DWORD BONE_ARRAY;
    DWORD BONE_ARRAY_CACHE;
    DWORD HABANERO_COMPONENT;
    DWORD RANKED_PROGRESS;
    DWORD PLATFORM;
    DWORD PLAYERNAME;
    DWORD CURRENT_WEAPON;
    DWORD WEAPON_DATA;
    DWORD LEVELS;
    DWORD actors2;
    DWORD rot;
    DWORD loc;
    DWORD AcknowledgedPawn;
    DWORD current_vehicle;
    DWORD b_is_dying;
    DWORD LastSubmitTimeOnScreen;
    DWORD b_has_thanked_bus_driver;
    DWORD kill_score;
    DWORD season_level_ui_display;
    DWORD SpectatorCount;
    DWORD Health;
    DWORD MaxHealth;
    Offsets()
        : UWORLD(0), GAME_INSTANCE(0), LOCAL_PLAYERS(0), PLAYER_CONTROLLER(0),
        LOCAL_PAWN(0), PAWN_PRIVATE(0), ROOT_COMPONENT(0), RELATIVE_LOCATION(0),
        PLAYER_STATE(0), TEAM_INDEX(0), GAME_STATE(0), PLAYER_ARRAY(0), MESH(0),
        COMPONENT_TO_WORLD(0), BONE_ARRAY(0), BONE_ARRAY_CACHE(0), HABANERO_COMPONENT(0),
        RANKED_PROGRESS(0), PLATFORM(0), PLAYERNAME(0), CURRENT_WEAPON(0), WEAPON_DATA(0),
        LEVELS(0), actors2(0), rot(0), loc(0),
        AcknowledgedPawn(0), current_vehicle(0), b_is_dying(0), LastSubmitTimeOnScreen(0),
        b_has_thanked_bus_driver(0), kill_score(0), season_level_ui_display(0), SpectatorCount(0),
        Health(0), MaxHealth(0) {
    }
    DWORD getUWorld() const { return UWORLD; }
    DWORD getGameInstance() const { return GAME_INSTANCE; }
    DWORD getLocalPlayers() const { return LOCAL_PLAYERS; }
    DWORD getPlayerController() const { return PLAYER_CONTROLLER; }
    DWORD getLocalPawn() const { return LOCAL_PAWN; }
    DWORD getPawnPrivate() const { return PAWN_PRIVATE; }
    DWORD getRootComponent() const { return ROOT_COMPONENT; }
    DWORD getRelativeLocation() const { return RELATIVE_LOCATION; }
    DWORD getPlayerState() const { return PLAYER_STATE; }
    DWORD getTeamIndex() const { return TEAM_INDEX; }
    DWORD getGameState() const { return GAME_STATE; }
    DWORD getPlayerArray() const { return PLAYER_ARRAY; }
    DWORD getMesh() const { return MESH; }
    DWORD getComponentToWorld() const { return COMPONENT_TO_WORLD; }
    DWORD getBoneArray() const { return BONE_ARRAY; }
    DWORD getBoneArrayCache() const { return BONE_ARRAY_CACHE; }
    DWORD getHabaneroComponent() const { return HABANERO_COMPONENT; }
    DWORD getRankedProgress() const { return RANKED_PROGRESS; }
    DWORD getPlatform() const { return PLATFORM; }
    DWORD getPlayerName() const { return PLAYERNAME; }
    DWORD getCurrentWeapon() const { return CURRENT_WEAPON; }
    DWORD getWeaponData() const { return WEAPON_DATA; }
    DWORD getLevels() const { return LEVELS; }
    DWORD getActors2() const { return actors2; }
    DWORD getRot() const { return rot; }
    DWORD getLoc() const { return loc; }
    DWORD getAcknowledgedPawn() const { return AcknowledgedPawn; }
    DWORD getCurrentVehicle() const { return current_vehicle; }
    DWORD getIsDying() const { return b_is_dying; }
    DWORD getLastSubmitTimeOnScreen() const { return LastSubmitTimeOnScreen; }
    DWORD getHasThankedBusDriver() const { return b_has_thanked_bus_driver; }
    DWORD getKillScore() const { return kill_score; }
    DWORD getSeasonLevelUiDisplay() const { return season_level_ui_display; }
    DWORD getSpectatorCount() const { return SpectatorCount; }
    DWORD getHealth() const { return Health; }
    DWORD getMaxHealth() const { return MaxHealth; }
};
inline std::shared_ptr<Offsets> offsets = std::make_shared<Offsets>();
