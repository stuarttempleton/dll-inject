// 
// dll-inject.cpp : Just a standard dll injector. Not reflected.
//

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>


DWORD getProcID(const char* procName)
{
    DWORD PID = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnapshot, &procEntry))
        {
            do
            {
                if (!_stricmp(procEntry.szExeFile, procName))
                {
                    PID = procEntry.th32ProcessID;
                    std::cout << "Process ID found: " << PID << "\n";
                    break;
                }
            } while (Process32Next(hSnapshot, &procEntry));
        }
    }
    else
    {
        std::cout << "Process snapshot failed!\n";
    }

    CloseHandle(hSnapshot);
    return PID;
}


int main(int argc, char ** argv)
{
    const char* ProcessName;
    const char* DLLPath;

    DWORD PID = 0;

    if (argc <= 2)
    {
        std::cout << "Usage: dll-inject.exe <PROCESS_EXE> <FULL_PATH_TO_DLL>\n";
        return 0;
    }

    ProcessName = argv[1];
    DLLPath = argv[2];

    std::cout << "Looking for " << argv[1] << "\n";
    PID = getProcID(ProcessName);

    if (PID == 0) {
        std::cout << "Process not located!\n";
        return 0;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, PID);
    if (hProcess && hProcess != INVALID_HANDLE_VALUE)
    {
        std::cout << "Process handle created.\n";
        void* loc = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (loc)
        {
            WriteProcessMemory(hProcess, loc, DLLPath, strlen(DLLPath) + 1, 0);
        }
        HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

        if (hThread)
        {
            std::cout << "Loaded " << DLLPath << " into process. Closing handle.";
            CloseHandle(hThread);
        }
        else
        {
            DWORD lasterror = GetLastError();
            std::cout << "Error creating remote thread: " << lasterror;
        }
    }
    else
    {
        std::cout << "Process handle NOT created.";
        return 0;
    }

    if (hProcess)
    {
        CloseHandle(hProcess);
    }

    return 1;
}

