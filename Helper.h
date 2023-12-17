#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <vector>

MODULEINFO getModuleInfo(const char* szModule) {
    MODULEINFO modInfo = { 0 };
    wchar_t wszModule[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, szModule, -1, wszModule, MAX_PATH);
    HMODULE hModule = GetModuleHandle(wszModule);
    if (hModule != NULL) {
        GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
    }
    return modInfo;
}

DWORD FindPattern(const char* module, const char* pattern, const char* mask) {
    MODULEINFO mInfo = getModuleInfo(module);
    uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
    DWORD size = mInfo.SizeOfImage;
    DWORD patternLength = static_cast<DWORD>(strlen(mask));
    for (DWORD i = 0; i < size - patternLength; i++) {
        bool found = true;
        for (DWORD j = 0; j < patternLength; j++) {
            if (mask[j] != '?' && pattern[j] != *reinterpret_cast<char*>(base + i + j)) {
                found = false;
                break;
            }
        }
        if (found) {
            return static_cast<DWORD>(i);
        }
    }
    return NULL;
}
