#include <init.h>
#include <windows.h>
#include <fstream>
#include <iostream>

static std::streambuf *CoutBuffer, *CerrBuffer;
static std::fstream ConsoleOutput, ConsoleError;

static void RedirectIO() {
    CoutBuffer = std::cout.rdbuf();
    CerrBuffer = std::cerr.rdbuf();
    
    ConsoleOutput.open("UTT_O.txt", std::ios::out);
    ConsoleError.open("UTT_E.txt", std::ios::out);

    std::cout.rdbuf(ConsoleOutput.rdbuf());
    std::cerr.rdbuf(ConsoleError.rdbuf());
}

static void ResetIO() {
    ConsoleOutput.close();
    ConsoleError.close();
}

static void StartWin(LPVOID hModule) {
    RedirectIO();
    {
        std::string path;
        path.resize(FILENAME_MAX, 0);
        auto path_size(GetModuleFileName(static_cast<HMODULE>(hModule), &path.front(), FILENAME_MAX));
        path.resize(path_size);

        init(path);
    }
    ResetIO();
    std::exit(0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(StartWin), hModule, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
