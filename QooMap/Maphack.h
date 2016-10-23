#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include "D2Structs.h"
using namespace std;  //vector 사용할때

#define UNIT_MYSELF 0
#define UNIT_NEUTRAL 1
#define UNIT_PARTIED 2
#define UNIT_HOSTILE 3

#define PVP_INVITED_YOU			0x01 // Invited you, but you may not have accepted
#define PVP_INVITED_BY_YOU		0x02 // Invited by you, but he may not have accepted
#define PVP_HOSTILED_YOU		0x04 // Hostiled you, but you may not have hostiled him
#define PVP_HOSTILED_BY_YOU		0x08 // Hostiled by you, but he may not have hostiled you
#define PVP_ALLIED				0x10 // Allied
#define PVP_ALLIED_WITH_YOU		0x20 // Allied with you

extern bool levelRevealed[];
extern bool actRevealed[];
extern bool gameRevealed;
extern bool QooMapRevealOK[5];

extern int unitColors[];
extern int monsterColors[];
extern int missileColors[];

struct AutomapName {        
	char* szLevelName;   
	int levelcolor;
	DWORD dwX;        
	DWORD dwY;       
	DWORD dwLevelId;       
	DWORD dwAct;
	
};

extern bool EnumerateVectors();
extern void DestroyVectors();
	
extern vector< vector<AutomapName*> > AutomapNames;
extern unsigned int viewplayerstate;
extern unsigned int jointime;
extern DWORD selectunitid;
extern DWORD selectunittype;
extern CellFile *BombCellFile;
extern CellFile *CrossCellFile; //몬스터 위치
extern CellFile *ItemCellFile; //아이템 위치
extern CellFile *MissileCellFile; //아이템 위치
extern CellFile *ChickenCellFile; //아이템 위치
extern CellFile *BoardCellFile; //아이템 위치
extern CellFile *EffectViewCellFile; //아이템 위치
extern CellFile *ScrollMapCellFile; //아이템 위치
extern CellFile *MapNameCellFile; //아이템 위치
extern CellFile *DropNoticeCellFile; //아이템 위치
extern CellFile *HighresCellFile; //아이템 위치
extern CellFile *SettingCellFile; //아이템 위치
extern CellFile *ItemConfCellFile; //아이템 위치
extern CellFile *AutoTransmuteCellFile;
void AutoTransmuteItems(); //자동 큐브 돌리기
void DrawAutomapCell2(CellFile *cellfile, int xpos, int ypos, BYTE col);

LPSTR GetShrinesFolder(LPSTR dest, CHAR ch);
void ToAutomap(POINT* pPos);
RosterUnit *FindPartyById(DWORD dwUnitId);
void ScreenToAutomap(POINT* Position, int X, int Y);
wchar_t* AnsiToUnicode(const char* str);
char* UnicodeToAnsi(const wchar_t* str);
DWORD __fastcall D2DrawRectFrame_STUB(RECT* pRect);
void DrawMonsterText(UnitAny *pUnit,wchar_t *monname,int textcolor,bool targetok);
UnitAny* GetSelectedUnit_Intercept(void);
extern wchar_t qoomapwindowtitle[256];

void GameAutomapDraw();  //오른쪽화면에 정보 표시
bool IsValidMonster(UnitAny *pUnit); //필요없는 몬스터
void ClearAutomapNames(); //오토맵에 표시되어있는거 제거
int DrawAutomap_I(void);
void DrawSTUB();
void GameDraw(void);

//맵핵
bool RevealGame(); 
bool RevealAct(DWORD dwAct);
bool RevealLevel(Level* pLevel);
void DrawPresets (Room2 *pRoom2);
void Cube_Action();
Level* GetLevel(Act* pAct, DWORD nLevel);
void __stdcall AutomapCells(CellContext *Context, DWORD X, DWORD Y, RECT * Clip, DWORD Bright);

DWORD __fastcall D2CLIENT_InitAutomapLayer(DWORD nLayerNo); //멥핵
AutomapLayer* InitAutomapLayer(DWORD levelno);
extern string lastGameName;
extern string lastGamePass;
extern int viewtrophy; // automap.cpp , d2state.cpp

VOID __fastcall NextGameNamePatch(Control* box, bool (__stdcall *FunCallBack)(Control*, unsigned, unsigned));
VOID __fastcall NextGamePassPatch(Control* box, bool (__stdcall *FunCallBack)(Control*, unsigned, unsigned));
DWORD  __fastcall GetUnitNameSTUB(DWORD Unit);