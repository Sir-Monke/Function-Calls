#include "pch.h"
#include <windows.h>
#include <iostream>
#include "Helper.h"

using namespace std;

typedef __int64(_cdecl* _ChangeGun)(int gun, __int64 fpsent);
_ChangeGun changeGun;

typedef __int64(_cdecl* _HudPlayer)();
_HudPlayer hudPlayer;

typedef __int64(_cdecl* _SndConMsg)(__int64 a1, const char* a2, ...);
_SndConMsg sndConMsg;

DWORD WINAPI MainThread(HMODULE hModule) {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    uintptr_t modBase = (uintptr_t)GetModuleHandle(NULL);

    while (true) {
        if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
            DWORD changeGunAddy = FindPattern("sauerbraten.exe", "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x50\x48\x8B\xDA\x8B", "xxxxxxxxxxxxxx");
            DWORD hudPlayerAddy = FindPattern("sauerbraten.exe", "\x83\x3D\xCC\xCC\xCC\xCC\xCC\x74\x08\x48\x8B\x05", "xx?????xxxxx");
            std::cout << changeGunAddy << endl;
            std::cout << hudPlayerAddy << endl;
            changeGun = (_ChangeGun)(modBase + changeGunAddy);
            hudPlayer = (_HudPlayer)(modBase + hudPlayerAddy);
            __int64 fpsent = hudPlayer();
            std::cout << fpsent << endl;
            changeGun(1,fpsent);
        }
        if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
            DWORD sndConMsgAddy = FindPattern("sauerbraten.exe", "\x48\x89\x54\x24\x10\x4C\x89\x44\x24\x18\x4C\x89\x4C\x24\x20\x48\x83\xEC\x28\x4C", "xxxxxxxxxxxxxxxxxxxx");
            std::cout << sndConMsgAddy << endl;
            sndConMsg = (_SndConMsg)(modBase + sndConMsgAddy);
            sndConMsg(4,"PPAP MAN!!");
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