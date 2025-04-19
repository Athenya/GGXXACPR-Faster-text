#include <windows.h>
#include "GG.h"

HMODULE exe;
BYTE *target;
BYTE *patchLocation;
BYTE* patchLocation2;



void Patch() {
	DWORD oldProtect;
	int opaque = 255; // Write FF into the value
	DWORDLONG jump2 = 0x9090909090909090; // Fil in a bunch of NOP instructions
	int shorttimer = 37; //Modify how long the text has to move before it gets into place
	VirtualProtect(patchLocation,15,PAGE_EXECUTE_READWRITE,&oldProtect);
	memcpy(target+0x1EB7D1,&opaque,4); // Sets the transparency to always be 255
	memcpy(target+0x1EB7D5,&jump2, 8); // Removes all the other calculations that would make it not 255
	VirtualProtect(patchLocation,15,oldProtect,&oldProtect);

	VirtualProtect(patchLocation2,10, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(target+0x1E8FDB, &shorttimer, 4); // Change the timer for how long the text spends moving
	VirtualProtect(patchLocation2,10, oldProtect, &oldProtect);

	return;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		exe = GetModuleHandle(NULL);
		target = (BYTE*)exe;
		patchLocation = target+0x1EB7D0;
		patchLocation2 = target+0x1E8FD5;
		Patch();
	}
	return TRUE;
}
