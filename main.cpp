#include <iostream>
#include <Windows.h>
#include <psapi.h> // Windows Base Address
using namespace std;
//Get Process ID
DWORD get_ProcID(string x){
    DWORD procID;
	HWND hwnd = FindWindowA(NULL, x.c_str());
	if (hwnd == NULL) {return 0;}
	else {
		GetWindowThreadProcessId(hwnd, &procID);
		return procID;
	}
}
//Get Game Handle
HANDLE get_GameHandle(DWORD procID){
		HANDLE hProcess(NULL);
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
        return handle;
}
//Get Game Base Address
DWORD get_BaseAddress(DWORD procID, HANDLE hProcess){
    DWORD BaseAddress(NULL);

    //Get Process ID
	if (hProcess == NULL)
		return NULL; // No access to the process

	HMODULE lphModule[1024]; // Array that receives the list of module handles
	DWORD lpcbNeeded(NULL); // Output of EnumProcessModules, giving the number of bytes requires to store all modules handles in the lphModule array

	if (!EnumProcessModules(hProcess, lphModule, sizeof(lphModule), &lpcbNeeded))
		return NULL; // Impossible to read modules

	TCHAR szModName[MAX_PATH];
	if (!GetModuleFileNameEx(hProcess, lphModule[0], szModName, sizeof(szModName) / sizeof(TCHAR)))
		return NULL; // Impossible to get module info


    BaseAddress = (DWORD)lphModule[0]; // Module 0 is apparently always the EXE itself, returning its address
    return BaseAddress;
}
int main(DWORD procID,HANDLE gameH,DWORD baseAddress){
    string x = "assaultcube";                          // Nom de la fenetre qu'on vx modifier
	procID = get_ProcID(x);
	gameH = get_GameHandle(procID);
	baseAddress = get_BaseAddress(procID, gameH);
	//-----------------------------------
	int newValue = 5000;                               // Valeur qu'on vx modifier
    DWORD pointer=0x100E90;                            // Premier pointer a partir de la baseAddress
    DWORD offsets[] = {0xA8,0x830,0x69C,0x34,0xA8,0x4E0};                          // Pointer offsets
//---
    baseAddress += pointer;
    for (int i = 0; i < sizeof(offsets)/sizeof(offsets[0]); i++) {
        ReadProcessMemory(gameH, (void*)baseAddress, &baseAddress, sizeof(baseAddress), 0);
        baseAddress += offsets[i];
    }
    WriteProcessMemory(gameH, (void*)baseAddress, &newValue, sizeof(newValue), 0);
//---
    return 0;

}
