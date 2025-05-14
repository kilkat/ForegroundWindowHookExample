#include "pch.h"

#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

typedef HWND(WINAPI* GetForegroundWindow_t)();
GetForegroundWindow_t fpGetForegroundWindow = nullptr;

// 후킹 함수 정의: 항상 NULL 반환 + 디버깅 MessageBox
HWND WINAPI HookedGetForegroundWindow() {
    return NULL;
}

// 후킹 초기화 함수
DWORD WINAPI InitHook(LPVOID) {
    // MinHook 초기화
    if (MH_Initialize() != MH_OK) {
        return 1;
    }

    // user32.dll에서 GetForegroundWindow 함수 주소 가져오기
    HMODULE hUser32 = GetModuleHandleA("user32.dll");
    if (!hUser32) {
        return 1;
    }

    void* pTarget = GetProcAddress(hUser32, "GetForegroundWindow");
    if (!pTarget) {
        return 1;
    }

    // 후킹 생성
    if (MH_CreateHook(pTarget, &HookedGetForegroundWindow, reinterpret_cast<void**>(&fpGetForegroundWindow)) != MH_OK) {
        return 1;
    }

    // 후킹 활성화
    if (MH_EnableHook(pTarget) != MH_OK) {
        return 1;
    }

    return 0;
}

// DllMain 엔트리포인트
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, InitHook, NULL, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
        break;
    }
    return TRUE;
}