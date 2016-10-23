#include "Weather.h"
#include "Offset.h"
#include "D2State.h"


void __declspec(naked) LightingPatch()
{ 
	__asm {
		cmp [togglelight],0
		je lightold
		mov eax,0xff
		mov byte ptr [esp+4+0], al
		mov byte ptr [esp+4+1], al
		mov byte ptr [esp+4+2], al
		add dword ptr [esp], 0x72;
		ret
	lightold:
		push esi
		call D2Common::GetRoom1LevelNo;
		ret
	}
}