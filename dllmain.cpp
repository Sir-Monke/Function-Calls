#include "pch.h"
#include <windows.h>
#include <iostream>
#include "Helper.h"

using namespace std;

typedef void(_cdecl* _ForceGamePause)(bool state);
_ForceGamePause forceGamePause;

DWORD WINAPI MainThread(HMODULE hModule) {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    uintptr_t modBase = (uintptr_t)GetModuleHandle(NULL);

    while (true) {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
            DWORD forceGamePauseAddress = FindPattern("sauerbraten.exe", "\x48\x83\xec\x00\x38\x0d\x00\x00\x00\x00\x74\x00\x33\xc0", "xxx?xx????x?xx");
            forceGamePause = (_ForceGamePause)(modBase + forceGamePauseAddress);
            std::cout << forceGamePauseAddress << endl;
            forceGamePause(1);
        }
        if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
            forceGamePause = (_ForceGamePause)(modBase + 0XAB950);
            forceGamePause(1);
        }
    }
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, NULL, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }
	return true;
}