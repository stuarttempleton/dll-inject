// 
// dllmain.cpp : basic dll structure for whatever.
//

#include "pch.h"


DWORD WINAPI MainThread(LPVOID param)
{
    while (true)
    {
        //TODO: Add injected code here. kinda.
        /*
        // OnKeyPress type thing...
        if (GetAsyncKeyState(VK_F6) & 0x80000)
        {
            MessageBoxA(NULL,"F6 pressed!","Key Pressed!",MB_OK);
        }
        */
        Sleep(100);
    }
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
