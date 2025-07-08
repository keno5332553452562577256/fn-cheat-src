#pragma once
#include <windows.h>
#include <wininet.h>
#include <string>
#include <sstream>
#include <map>
#include "../offsets/offsets.h"
// DO NOT TOUCH THIS IF YOU DONT UNDERSTAND!!!! - lANDEN419 LOVES YOU
#pragma comment(lib, "wininet.lib")

std::string DownloadOffsetText(const std::string& url) {
    HINTERNET hInternet = InternetOpenA("OffsetLoader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return "";

    HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0,
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID,
        0);
    if (!hUrl) {
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[4096];
    DWORD bytesRead;
    std::stringstream ss;

    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0)
        ss.write(buffer, bytesRead);

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);
    return ss.str();
}

std::map<std::string, DWORD> ParseOffsetsFromText(const std::string& text) {
    std::map<std::string, DWORD> offsetsMap;
    std::istringstream stream(text);
    std::string line;

    while (std::getline(stream, line)) {
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        offsetsMap[key] = static_cast<DWORD>(std::stoul(value, nullptr, 16));
    }

    return offsetsMap;
}

bool offsetsfromapoi(const std::string& url) {    // if you add more offsets an want to auto update them, add them here aswell as offsets->h file!
    std::string text = DownloadOffsetText(url); 
    if (text.empty()) return false;

    auto map = ParseOffsetsFromText(text);

    offsets->UWORLD = map["UWORLD"];
    offsets->GAME_INSTANCE = map["GAME_INSTANCE"];
    offsets->LOCAL_PLAYERS = map["LOCAL_PLAYERS"];
    offsets->PLAYER_CONTROLLER = map["PLAYER_CONTROLLER"];
    offsets->LOCAL_PAWN = map["LOCAL_PAWN"];
    offsets->PAWN_PRIVATE = map["PAWN_PRIVATE"];
    offsets->ROOT_COMPONENT = map["ROOT_COMPONENT"];
    offsets->RELATIVE_LOCATION = map["RELATIVE_LOCATION"];
    offsets->PLAYER_STATE = map["PLAYER_STATE"];
    offsets->TEAM_INDEX = map["TEAM_INDEX"];
    offsets->GAME_STATE = map["GAME_STATE"];
    offsets->PLAYER_ARRAY = map["PLAYER_ARRAY"];
    offsets->MESH = map["MESH"];
    offsets->COMPONENT_TO_WORLD = map["COMPONENT_TO_WORLD"];
    offsets->BONE_ARRAY = map["bonearray"];
    offsets->BONE_ARRAY_CACHE = map["bonearraycache"];
    offsets->HABANERO_COMPONENT = map["HABANERO_COMPONENT"];
    offsets->RANKED_PROGRESS = map["RANKED_PROGRESS"];
    offsets->PLATFORM = map["PLATFORM"];
    offsets->PLAYERNAME = map["PLAYERNAME"];
    offsets->CURRENT_WEAPON = map["CURRENT_WEAPON"];
    offsets->WEAPON_DATA = map["WEAPON_DATA"];
    offsets->LEVELS = map["LEVELS"];
    offsets->actors2 = map["actors2"];

    return true;
}
