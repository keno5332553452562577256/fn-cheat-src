#include <iostream>
#include <thread>
#include "../menu/menu.h"
#include "../kernel/coms.h"
#include <Windows.h>
#include "../offset fetching/fetch.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include <vector>
#include <random>
#include <chrono>
#include <conio.h>
#include <atomic>

Kernel* kernel = nullptr;

void ShowInjectPrompt()
{
    bool open = true;
    bool inject = false;
    // Initialisiere Fenster und DirectX wie im directx_init(), aber minimal für das Prompt
    directx_init();
    ImGui_ImplWin32_Init(MyWnd);
    ImGui_ImplDX9_Init(p_device);
    MSG msg = { 0 };
    while (open && !inject)
    {
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);
        ImGui::Begin("Cheat Injection", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
        ImGui::Text("Möchten Sie den Cheat injizieren?");
        if (ImGui::Button("Cheat injizieren", ImVec2(180, 40)))
        {
            inject = true;
            open = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Beenden", ImVec2(180, 40)))
        {
            open = false;
        }
        ImGui::End();
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
        p_device->Present(0, 0, 0, 0);
    }
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    if (!inject)
    {
        exit(0);
    }
}

void SetConsoleTransparency(BYTE alpha = 200)
{
    HWND hwnd = GetConsoleWindow();
    if (hwnd != NULL)
    {
        LONG style = GetWindowLong(hwnd, GWL_EXSTYLE);
        SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
        SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
    }
}

int show_menu_and_snow() {
    const int width = 118;
    const int height = 30;
    const int flakes = 25;
    std::vector<int> snow(flakes);
    std::vector<int> x(flakes);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_x(0, width - 1);
    std::uniform_int_distribution<> dist_y(0, height - 1);
    for (int i = 0; i < flakes; ++i) {
        x[i] = dist_x(gen);
        snow[i] = dist_y(gen) - height;
    }
    int auswahl = 0;
    while (true) {
        system("cls");
        std::vector<std::string> field(height, std::string(width, ' '));
        std::string menu_line1(width, ' ');
        std::string menu_line2(width, ' ');
        std::string opt1 = "[1] Cheat injizieren";
        std::string opt2 = "[2] Beenden";
        menu_line1.replace(2, opt1.size(), opt1);
        menu_line2.replace(2, opt2.size(), opt2);
        field[1] = menu_line1;
        field[2] = menu_line2;
        for (int i = 0; i < flakes; ++i) {
            if (snow[i] >= 3 && snow[i] < height)
                field[snow[i]][x[i]] = '*';
            snow[i]++;
            if (snow[i] >= height) {
                snow[i] = 3;
                x[i] = dist_x(gen);
            }
        }
        for (const auto& line : field)
            std::cout << line << std::endl;
        std::cout.flush();
        if (_kbhit()) {
            int ch = _getch();
            if (ch == '1' || ch == '2') {
                while (_kbhit()) _getch(); // Tastaturpuffer leeren
                return (ch == '1') ? 1 : 2;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main()
{
    SetConsoleTransparency(200);
    int auswahl = show_menu_and_snow();
    if (auswahl == 2) return 0;
    // Nach Auswahl von 1: Meldung anzeigen, dann Cheat injizieren
    system("cls");
    std::cout << "Cheat wird jetzt injectiert..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // Jetzt Cheat-Code wie gewohnt ausführen
    std::cout << "Starte Kernel-Initialisierung..." << std::endl;
    kernel = new Kernel();
    if (!kernel->Init()) {
        std::cout << "Fehler: Kernel konnte nicht initialisiert werden!" << std::endl;
        system("pause");
        FreeConsole();
        return 0;
    }
    std::cout << "Kernel initialisiert!" << std::endl;

    kernel->Attach(L"FortniteClient-Win64-Shipping.exe");
    std::cout << "Kernel an Prozess angehängt!" << std::endl;

    kernel->Base();
    std::cout << "Kernel Base gesetzt!" << std::endl;

    if (!kernel->ProcessBase) {
        std::cout << "Fehler: ProcessBase nicht gefunden! (Ist Fortnite gestartet?)" << std::endl;
        system("pause");
        FreeConsole();
        return 0;
    }
    std::cout << "ProcessBase gefunden!" << std::endl;

    while (MyWnd == 0) {
        Sleep(100);
        kernel->Attach(L"FortniteClient-Win64-Shipping.exe");
        MyWnd = GetProcWindow(kernel->ProcessId);
    }
    std::cout << "Fenster gefunden!" << std::endl;

    if (!MyWnd) {
        std::cout << "Fehler: Fenster nicht gefunden! (Ist Fortnite im Hauptmenü?)" << std::endl;
        system("pause");
        FreeConsole();
        return 0;
    }

    std::cout << "Lade Offsets..." << std::endl;
    offsetsfromapoi("https://raw.githubusercontent.com/Pearlism/website/refs/heads/main/offsets.txt");
    std::cout << "Offsets geladen!" << std::endl;

    if (offsetsfromapoi)
    {
        // Offsets geladen
    }
    else
    {
        std::cout << "Fehler: Offsets konnten nicht geladen werden!" << std::endl;
        system("pause");
        FreeConsole();
        return 0;
    }
    std::cout << "Initialisiere CR3..." << std::endl;
    if (!kernel->Cr3()) {
        std::cout << "Fehler: CR3 Memory Class konnte nicht geladen werden!" << std::endl;
        system("pause");
        // Kein FreeConsole hier, damit du die Meldung siehst
    }
    std::cout << "CR3 initialisiert!" << std::endl;
    uac::init();
    user::init();
    overlay();  
    std::thread(caching).detach(); 
    std::thread(threading).detach(); 
    std::thread(world_reading_work).detach();
    directx_init();
    ImGui_ImplWin32_Init(MyWnd);  
    ImGui_ImplDX9_Init(p_device);
    RenderLoop();
    FreeConsole(); // Konsole am Ende schließen
    system("pause"); // Konsole bleibt offen
    return 0;
}


