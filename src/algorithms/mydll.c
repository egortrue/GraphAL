
#include <windows.h>
#include <stdio.h>

#include "graphs.h"
#include "algorithms.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: // ����������� DLL
        printf("DLL was attached:\n");
        printf("|- %s mode\n", lpReserved ? "implicit (compile)" : "explicit (runtime)");
        printf("|- HMODULE = %p\n", hModule);
        break;
    case DLL_PROCESS_DETACH: // ���������� DLL
        printf("DLL was detached\n");
        break;


    case DLL_THREAD_ATTACH: // �������� ������ ������
        break;
    case DLL_THREAD_DETACH: // ���������� ������
        break;
    }
    return TRUE;
}
