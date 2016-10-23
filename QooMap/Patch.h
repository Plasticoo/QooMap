#pragma once

#include "Weather.h" //LightingPatch
#include "ItemPatch.h" //itempatch_asm();
#include "Maphack.h"
#include "Offset.h"
#include "AutoMap.h"
#include "D2State.h" //gamepacketrecievd
#include "AutoMapCell.h"
#include "EnteringLevelText.h"

void GameLoop_Interception();
#define WallsInside         INST_NOP

PatchHook pHooks[] = {
	{CALL, D2CLIENT, 0xC3A9C, (DWORD)DrawAutomap_I, 5}, //몬스터 그리기 -include maphack.h
	{CALL, D2CLIENT, 0xA9A37,	(DWORD)LightingPatch, 6}, //시야 없애
	{CALL,D2CLIENT,	0x604EA,(DWORD)AutomapCells,5},
	{JMP, D2CLIENT, 0xC3DB5,	(DWORD)DrawSTUB,5},
	{CALL, D2CLIENT, 0x4363E, (DWORD)GameFailToJoin_STUB, 6},
	{CALL, D2MULTI, 0x14D29, (DWORD)NextGameNamePatch, 5},
	{CALL, D2MULTI, 0x14A0B, (DWORD)NextGameNamePatch, 5},
	{CALL, D2MULTI, 0x14A46, (DWORD)NextGamePassPatch, 5},
	{CALL, D2MULTI, 0x14D64, (DWORD)NextGamePassPatch, 5},
	{CALL, D2CLIENT, 0x442A2, (DWORD)GameShakePatch, 5},
	{CALL,	D2CLIENT, 0x1BF94,	(DWORD)GetSelectedUnit_Intercept, 5},
	{CALL, D2CLIENT,0x8B096, (DWORD)GameShake1_STUB, 6},
	{CALL,	D2CLIENT, 0xACE61, (DWORD)GamePacketReceived_I, 5},
	{CALL, D2CLIENT, 0x92366, (DWORD)ItemNamePatch_ASM, 6}, //아이템 이름 패치
	{JMP, D2CLIENT, 0x626C9, (DWORD)DrawAutomap_II, 5}, //화면 오른편에 정보 표시하기.
	{CALL, D2CLIENT, 0x66623, (DWORD)InfravisionPatch_ASM, 7}, //벽뒤  보기
	{CALL, D2CLIENT, 0x44230,	(DWORD)GameLoop_Interception, 7},
	{CALL, D2CLIENT, 0x953E2,	(DWORD)ViewInventorySTUB1,6},
	{CALL, D2CLIENT, 0x94AB4,	(DWORD)ViewInventorySTUB2,6},
	{CALL, D2CLIENT, 0x93A6F,	(DWORD)ViewInventorySTUB3,5},
	{CALL,D2WIN,0x118F0,(DWORD)MonsterLifeBarNameSTUB,6},
	{CALL,D2CLIENT,0x55C1D,   (DWORD)ItemBasicStatSTUB,5},
	{CALL,D2CLIENT,0xC3AA1,	(DWORD)DrawEnteringLevelTextSTUB,5},

};