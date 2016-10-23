#pragma once
#include "D2Structs.h"

void DrawAutomap_II(void);
void InfravisionPatch_ASM();
void GameFailToJoin_STUB();
void GameShake1_STUB();
void __stdcall GameShakePatch(LPDWORD lpX, LPDWORD lpY);
bool StringToBool(const char* str);

bool Item_Shrine(UnitAny *item);
bool Item_Gem(UnitAny *item);
bool Item_SacredOnly(UnitAny *item);
bool Item_Ultimative(UnitAny *item);
bool Item_MagicJewel(UnitAny *item);
bool Item_RareItem(UnitAny *item);
bool Item_Charm(UnitAny *item);

//char *Mon_NameUber(int dwact,int TextNo);
bool Mon_NameUber(int dwact,int TextNo);

extern int runenumber; //ini에서 읽어오는 룬 숫자
extern unsigned int monstercolor; //ini에서 읽어오는 몬스터 색상
extern unsigned int xMapShake;
extern unsigned int yMapShake;
void ScrollMap(char x,char y);
extern bool UserAutomapname;

extern bool ViewJewelMagic;
extern bool ViewJewelRare;
extern bool ItemViewAll;
extern bool RareAmulet;

extern bool CreepyShrine;
extern bool AbandonedShrine;
extern bool QuietShrine;
extern bool EerieShrine;
extern bool WeirdShrine;
extern bool GloomyShrine;
extern bool TaintedShrine;
extern bool FascinatingShrine;
extern bool OrnateShrine;
extern bool SacredShrine;
extern bool ShimmeringShrine;
extern bool SpiritualShrine;
extern bool MagicalShrine;
extern bool HiddenShrine;
extern bool StoneShrine;

extern bool PerfectAmethyst;
extern bool PerfectTopaz;
extern bool PerfectSapphire;
extern bool PerfectEmerald;
extern bool PerfectRuby;
extern bool PerfectDiamond;
extern bool PerfectSkull;
extern bool PerfectRainbowStone;
extern bool PerfectOnyx;
extern bool PerfectBloodStone;
extern bool PerfectTurquoise;
extern bool PerfectAmber;
extern bool PerfectGhostly;
extern unsigned int ViewGold_Value;

CellFile *LoadBMPCellFile(LPSTR FileName);
LPBYTE AllocReadFile(LPSTR FileName);
CellFile *InitCellFile(CellFile *File);