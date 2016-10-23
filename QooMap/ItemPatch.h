#pragma once

#include "D2Structs.h"
#include <windows.h>
#include <string>

void ItemNamePatch_ASM();
void __fastcall ItemNamePatch(wchar_t *name, UnitAny *item);
void MonsterLifeBarNameSTUB();
wchar_t *  __fastcall MonsterLifeBarName(WCHAR * Old, LPDWORD TextSize, LPDWORD Size, DWORD Flag);
