
#include <windows.h>
#include <stdio.h>

#include "graphs.h"
#include "algorithms.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: // Подключение DLL
        printf("DLL was attached:\n");
        printf("|- %s mode\n", lpReserved ? "implicit (compile)" : "explicit (runtime)");
        printf("|- HMODULE = %p\n", hModule);
        break;
    case DLL_PROCESS_DETACH: // Отключение DLL
        printf("DLL was detached\n");
        break;


    case DLL_THREAD_ATTACH: // Создание нового потока
        break;
    case DLL_THREAD_DETACH: // Завершение потока
        break;
    }
    return TRUE;
}
