#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

void monitorForegroundWindow() {
    while (true) {
        HWND hWnd = GetForegroundWindow();
        if (hWnd == NULL) {
            std::cout << "[*] No foreground window" << std::endl;
        }
        else {
            char title[256] = { 0 };
            if (GetWindowTextA(hWnd, title, sizeof(title))) {
                std::cout << "[*] Foreground window title: " << title << std::endl;
            }
            else {
                std::cout << "[!] Failed to get window title." << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main() {
    std::cout << "[*] Starting foreground window monitor..." << std::endl;
    monitorForegroundWindow();
    return 0;
}
