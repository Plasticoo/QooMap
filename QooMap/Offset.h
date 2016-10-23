#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <windows.h>

#include "D2Structs.h"
typedef CellFile *LPCELLFILE;

using namespace std;

// To GO!
#ifndef ArraySize
#define ArraySize(x) (sizeof((x)) / sizeof((x)[0]))
#endif

enum D2DllNo {
	D2CLIENT, 
	D2COMMON, 
	D2GFX, 
	D2LANG, 
	D2WIN, 
	D2NET, 
	D2GAME, 
	D2LAUNCH, 
	FOG, 
	BNCLIENT, 
	STORM, 
	D2CMP, 
	D2MULTI
};

struct D2DllInfo {
	char* DllName;
	D2DllNo DllNo;
};

static const int MaxModNo = 13;
static const D2DllInfo dll[MaxModNo] = {
	{ "D2Client.DLL", D2CLIENT }, 
	{ "D2Common.DLL", D2COMMON },
	{ "D2Gfx.DLL", D2GFX },
	{ "D2Lang.DLL", D2LANG },
	{ "D2Win.DLL", D2WIN },
	{ "D2Net.DLL", D2NET },
	{ "D2Game.DLL", D2GAME },
	{ "D2Launch.DLL", D2LAUNCH },
	{ "Fog.DLL", FOG },
	{ "BNClient.DLL", BNCLIENT },
	{ "Storm.DLL", STORM },
	{ "D2Cmp.DLL", D2CMP },
	{ "D2Multi.DLL", D2MULTI}
};

struct ModuleInfo {
	string ModuleName;
	int ModuleNo;
	DWORD BaseAddress;
};

struct Offset {
	ModuleInfo* Module;
	DWORD BaseOffset;
};

typedef vector<Offset*> Offsets;
typedef vector<ModuleInfo*> Modules;

namespace D2Client {
	typedef void (__stdcall *PrintGameString_t)(wchar_t *wMessage, int nColor);
	typedef UnitAny* (__fastcall *GetCurrentInteractingNPC_t)(void);
	typedef UnitAny* (__fastcall *GetCursorItem_t)(void);
	typedef void (__stdcall *RevealAutomapRoom_t)(Room1 *pRoom1, DWORD dwClipFlag, AutomapLayer *aLayer);
	typedef DWORD (__stdcall *GetAutomapSize_t)(void);
	typedef void (__stdcall *GamePrintString_t)(wchar_t *wMessage, int nColor);
	typedef int (*DrawAutomap_t)(void);
	typedef void (__fastcall *DrawRectFrame_t)(DWORD Rect);
	typedef DWORD (__fastcall *SetUIVar_t)(DWORD varno, DWORD howset, DWORD unknown1);

	typedef DWORD (__fastcall *GetUIVar_t)(DWORD dwVarNO);
	typedef void (__fastcall *ExitGame_t)(void);
	typedef void (__stdcall *DrawEnteringLevelText_t)(void);
	typedef int (__fastcall *GetUnitX_t)(UnitAny* pUnit);
	typedef int (__fastcall *GetUnitY_t)(UnitAny* pUnit);
	typedef UnitAny* (__fastcall *FindClientSideUnit_t)(DWORD dwId, DWORD dwType);
	typedef UnitAny* (__fastcall *FindServerSideUnit_t)(DWORD dwId, DWORD dwType);
	typedef short (__stdcall *CalculateShake_t)(unsigned xpos, unsigned ypos);
	typedef void (__fastcall *SetSelectedUnit_I_t)(UnitAny *pUnit);
	typedef void (__stdcall *ClickMap_t)(unsigned MouseFlag, unsigned x, unsigned y, unsigned Type);
	typedef void (__fastcall *LeaveParty_t)(void);
	typedef AutomapLayer* (__fastcall *InitAutomapLayer_I_t)(unsigned nLayerNo);
	typedef AutomapLayer* (__fastcall *InitLevel_I_t)(void);
	typedef AutomapCell* (__fastcall *NewAutomapCell_t)(void);
	typedef AutomapCell* (__fastcall *AddAutomapCell_t)(AutomapCell *aCell, AutomapCell **node);
	typedef UnitAny* (__stdcall *GetPlayerUnit_t)(void);
	typedef unsigned (__fastcall *GetMouseXOffset_t)(void);
	typedef unsigned (__fastcall *GetMouseYOffset_t)(void);
	typedef void (__stdcall *GetItemNameString_t)(UnitAny *pItem, wchar_t *wItemName, int nLen);		
	typedef RosterUnit* (__fastcall *GetRosterUnitById_t)(unsigned Unk, unsigned UnitId);
	typedef UnitAny* (__stdcall *GetSelectedUnit_t)(void);
	typedef DWORD (__fastcall *GetMonsterOwner_t)(DWORD nMonsterId);
	typedef size_t (__fastcall *SendGamePacket_I_t)(size_t len, void* pPacket);
	typedef MonsterTxt* (__fastcall *GetMonsterText_I_t)(unsigned LocaleNo);
	typedef unsigned (__fastcall *GetUiState_I_t)(unsigned dwVarNo);

	/* D2Client Functions */
	extern PrintGameString_t PrintGameString;
	extern GetCurrentInteractingNPC_t	GetCurrentInteractingNPC;
	extern GetCursorItem_t	GetCursorItem;
	extern RevealAutomapRoom_t RevealAutomapRoom;
	extern GamePrintString_t GamePrintString;
	extern DrawAutomap_t DrawAutomap;
	extern DrawRectFrame_t DrawRectFrame;
	extern ExitGame_t ExitGame;
	extern DrawEnteringLevelText_t DrawEnteringLevelText;
	extern GetUnitX_t GetUnitX;
	extern GetUnitY_t GetUnitY;
	extern GetMonsterOwner_t GetMonsterOwner;
	extern FindClientSideUnit_t FindClientSideUnit;
	extern FindServerSideUnit_t FindServerSideUnit;
	extern GetAutomapSize_t GetAutomapSize;
	extern CalculateShake_t CalculateShake;
	extern SetSelectedUnit_I_t SetSelectedUnit_I;
	extern SetUIVar_t SetUIVar;
	extern GetUIVar_t GetUIVar;
	extern ClickMap_t ClickMap;
	extern LeaveParty_t LeaveParty;
	extern InitAutomapLayer_I_t InitAutomapLayer_I;
	extern InitLevel_I_t InitLevel;
	extern NewAutomapCell_t NewAutomapCell;
	extern AddAutomapCell_t AddAutomapCell;
	extern GetItemNameString_t GetItemNameString;
	extern GetPlayerUnit_t GetPlayerUnit;
	extern GetMouseXOffset_t GetMouseXOffset;
	extern GetMouseYOffset_t GetMouseYOffset;
	extern GetRosterUnitById_t GetRosterUnitById;
	extern GetSelectedUnit_t GetSelectedUnit;
	extern SendGamePacket_I_t SendGamePacket_I;
	extern GetMonsterText_I_t GetMonsterText_I;
	extern GetUiState_I_t GetUiState_I;

	/* D2Client Vars */
	extern RosterUnit** PlayerUnitList;
	extern UnitAny** PlayerUnit;
	extern unsigned* Ping;
	extern unsigned* FramesPerSecond;
	extern unsigned* FramesSkipped;
	extern AutomapLayer** CurrentAutomapLayer;
	extern InventoryLayout** TradeLayout;
	extern InventoryLayout** StashLayout;
	extern InventoryLayout** StoreLayout;
	extern InventoryLayout** CubeLayout;
	extern InventoryLayout** PlayerLayout;
	extern InventoryLayout** MercLayout;
	extern unsigned char* Difficulty;
	extern unsigned int *GameType;
	extern RosterUnit** UnitTable;
	extern unsigned* ExpCharFlag;
	extern unsigned* InGame;
	extern unsigned* NoPickup;
	extern unsigned* AutomapOn;
	extern unsigned* AutomapMode;
	extern POINT* PtAutomap;
	extern int* Divisor;
	extern POINT* Offset;
	extern int* yPosition;
	extern int* yShake;
	extern int* xShake;
	extern GameStructInfo** GameInfo;
	extern unsigned* AutomapYDrawPos;
	extern unsigned* ScreenSizeX_I; 
	extern unsigned* ScreenSizeX;
	extern unsigned* ScreenSizeY;
	extern unsigned* CursorHoverX;
	extern unsigned* CursorHoverY;
	extern unsigned* MouseX;
	extern unsigned* MouseY;
	extern int* MouseOffsetY;
	extern int* MouseOffsetX;

	/* Assembly Routines */
	extern unsigned GetUnitFromId_I;
	extern unsigned Interact_I;
	extern unsigned TakeWaypoint_I;
	extern unsigned ClickShopItem_I;
	extern unsigned ClickBelt_I;
	extern unsigned ClickBeltRight_I;
	extern unsigned ClickItemRight_I;
	extern unsigned MercItemAction_I;
	extern unsigned TestPvpFlag_I;
	extern unsigned LoadUIImage_I;
	extern unsigned GetUnitName_I;
	extern unsigned GetItemDesc_I;


	extern unsigned ClickParty_I;
	extern unsigned ClickParty_II;
	extern unsigned ItemPriceList;
	extern unsigned LoadAct_1;
	extern unsigned LoadAct_2;
	extern unsigned InputCall_I;
}
//#define D2CLIENT_GetUnitName(x)	(wchar_t*)D2CLIENT_GetUnitName_STUB((DWORD)x)
struct cStylePrefs
{
   DWORD dwFontType;   //0x00
   DWORD dwUnk;      //0x04
};


namespace D2Win {
	typedef void (__fastcall *DrawSprites_t)(void);
	typedef void (__fastcall *DrawText_t)(wchar_t *wStr, int xPos, int yPos, unsigned Colour, unsigned Unk);
	typedef unsigned (__fastcall *GetTextWidthFileNo_t)(wchar_t *wStr, DWORD * Width, DWORD * FileNo);
	typedef unsigned (__fastcall *SetTextSize_t)(unsigned Size);
	typedef void (__fastcall *SelectEditBoxText_t)(Control* box); // wchar_t* arg 2 seems to be gone
	typedef void* (__fastcall *SetControlText_t)(Control* box, wchar_t* txt); // Updated 
	typedef void (__fastcall *SetEditBoxProc_t)(Control* box, bool (__stdcall *FunCallBack)(Control*,unsigned,unsigned));
	typedef int (__fastcall *GetTextWidth_I_t)(wchar_t* wText, int Len);
	typedef DWORD (__stdcall *InitMPQ_t)(char *dll,const char *mpqfile, char *mpqname, int v4, int v5);//, 0x7E60)
	//typedef wchar_t * (__stdcall *ItemDescBuffer_t)(void);

	typedef Control (__fastcall *CreateEditBox_t)(DWORD xPos, DWORD yPos, DWORD SizeX, DWORD SizeY, DWORD LeftOffset, DWORD TopOffset, CellFile *ptCellFile, bool (__stdcall *OnAccept)(Control), bool (__stdcall *OnClick)(Control), DWORD isCloaked, cStylePrefs *ptPrefs);
	
	extern DrawSprites_t DrawSprites;
	extern CreateEditBox_t CreateEditBox;
	extern DrawText_t DrawText;
	extern GetTextWidthFileNo_t GetTextWidthFileNo;
	extern SetTextSize_t SetTextSize;
	extern SelectEditBoxText_t SelectEditBoxText; // wchar_t* arg 2 seems to be gone
	extern SetControlText_t SetControlText;
	extern SetEditBoxProc_t SetEditBoxProc;
	extern GetTextWidth_I_t GetTextWidth_I;
	extern InitMPQ_t InitMPQ;

	extern Control** FirstControl;
	extern Control** FocusedControl;
	extern Control** UnkControl;
	extern wchar_t *ItemDescBuffer;
}

namespace D2Gfx {
	typedef void (__stdcall *DrawAutomapCell_t)(CellContext *context, DWORD xpos, DWORD ypos, RECT *cliprect, DWORD bright);//-10079) //Updated 1.13c
	typedef void (__stdcall *DrawAutomapCell2_t)(CellContext *context, DWORD xpos, DWORD ypos, DWORD bright2, DWORD bright, BYTE *coltab);//, -10041) 

	typedef HWND (__stdcall *GetHwnd_t)(void);
	typedef void (__stdcall *DrawRectangle_t)(int X1, int Y1, int X2, int Y2, unsigned Color, unsigned Trans);
	typedef void (__stdcall *DrawLine_t)(int X1, int Y1, int X2, int Y2, unsigned Color, unsigned Unk);
	typedef unsigned (__stdcall *GetScreenSize_t)(void); 

	extern DrawAutomapCell_t DrawAutomapCell;
	extern DrawAutomapCell2_t DrawAutomapCell2;
	extern GetHwnd_t GetHwnd;
	extern DrawRectangle_t DrawRectangle;
	extern DrawLine_t DrawLine;
	extern GetScreenSize_t GetScreenSize;
}

namespace D2Common {
	typedef UnitAny* (__stdcall *GetUnitByBeltSlot_t)(Inventory* pInventory, unsigned BeltLocation);
	typedef DWORD (__stdcall *GetBaseStat_t)(UnitAny *Unit, DWORD dwStat, DWORD dwUkn);
	typedef DWORD (__stdcall *GetUnitStat_t)(UnitAny* pUnit, unsigned dwStat, unsigned dwStat2);
	typedef char* (__stdcall *GetItemMagicalMods_t)(WORD wPrefixNum);
	typedef Room1* (__stdcall *GetRoomFromUnit_t)(UnitAny * ptUnit);
	typedef void (__stdcall *AbsScreenToMap_t)(long *pX, long *pY);
	typedef void (__stdcall *MapToAbsScreen_t)(long *pX, long *pY);
	typedef DWORD (__stdcall *GetExpForLevel_t)(int ClassId, int CharLevel);
	typedef DWORD (__stdcall *GetUnitMaxHealth_t)(UnitAny* pUnit);
	typedef bool (__stdcall *IsLevelATown_t)(unsigned LevelNo);
	typedef void (__stdcall *AddRoomData_t)(Act* ptAct, int LevelId, int Xpos, int Ypos, Room1 * pRoom);
	typedef void (__stdcall *RemoveRoomData_t)(Act* ptAct, int LevelId, int Xpos, int Ypos, Room1 * pRoom);
	typedef LevelTxt* (__stdcall *GetLevelText_t)(unsigned LevelNo);
	typedef ObjectTxt* (__stdcall *GetObjectText_t)(unsigned ObjNo);
	typedef ItemTxt* (__stdcall *GetItemText_t)(unsigned ItemNo);
	typedef DWORD (__stdcall *GetItemPrice_t)(UnitAny *MyUnit,UnitAny *pItem, DWORD U1_,DWORD U2_,DWORD U3_,DWORD U4_);
	typedef AutomapLayer2* (__fastcall *GetLayer_t)(unsigned LevelNo);
	typedef Level* (__stdcall *GetLevel_t)(Act* pAct, unsigned LevelNo);
	typedef Level* (__fastcall *LoadLevel_t)(ActMisc *pMisc, unsigned LevelNo);
	typedef void (__stdcall *InitLevel_t)(Level *pLevel);
	typedef unsigned (__stdcall *GetRoom1LevelNo_t)(Room1* pRoom1);
	typedef Act* (__stdcall *LoadAct_t)(unsigned ActNo, unsigned MapId, unsigned Unk, unsigned Unk_2, unsigned Unk_3, unsigned Unk_4, unsigned TownLevelNo, unsigned Func_1, unsigned Func_2);
	typedef unsigned (__stdcall *UnloadAct_t)(Act* pAct);
	typedef int (__stdcall *GetSkillLevel_t)(UnitAny* pUnit, Skill* pSkill, bool bTotal); // Check bool type
	typedef int (__stdcall *GetUnitDistance_t)(UnitAny* pUnit1, UnitAny* pUnit2);
	typedef UnitAny* (__stdcall *GetFirstItemFromInventory_t)(Inventory* inv);
	typedef UnitAny* (__stdcall *GetNextItemFromInventory_t)(UnitAny *pItem);


	typedef unsigned (__stdcall *Unk10651_t)(UnitAny* pUnit);
	typedef unsigned (__stdcall *Unk11142_t)(UnitAny* pUnit);
	typedef unsigned (__stdcall *Unk11087_t)(int* pXpos, int* pYpos);

	typedef Room2* (__stdcall *GetRoomNear_t)(Room1* pRoom1, int X, int Y);
	typedef int (__stdcall *ExpRel_t)(signed int Class);

	extern GetItemPrice_t GetItemPrice;
	extern GetBaseStat_t	GetBaseStat;
	extern GetItemMagicalMods_t	GetItemMagicalMods;
	extern GetUnitByBeltSlot_t GetUnitByBeltSlot;
	extern GetUnitStat_t GetUnitStat;
	extern GetRoomFromUnit_t GetRoomFromUnit;
	extern AbsScreenToMap_t AbsScreenToMap;
	extern MapToAbsScreen_t MapToAbsScreen;
	extern GetExpForLevel_t GetExpForLevel;
	extern GetUnitMaxHealth_t GetUnitMaxHealth;
	extern IsLevelATown_t IsLevelATown;
	extern AddRoomData_t AddRoomData;
	extern RemoveRoomData_t RemoveRoomData;
	extern GetLevelText_t GetLevelText;
	extern GetObjectText_t GetObjectText;
	extern GetItemText_t GetItemText;
	extern GetLayer_t GetLayer;
	extern GetLevel_t GetLevel;
	extern LoadLevel_t LoadLevel;
	extern InitLevel_t InitLevel;
	extern GetRoom1LevelNo_t GetRoom1LevelNo;
	extern LoadAct_t LoadAct;
	extern UnloadAct_t UnloadAct;
	extern GetSkillLevel_t GetSkillLevel;
	extern GetUnitDistance_t GetUnitDistance;
	extern GetFirstItemFromInventory_t GetFirstItemFromInventory;
	extern GetNextItemFromInventory_t GetNextItemFromInventory;
	extern Unk10651_t Unk10651;
	extern Unk11142_t Unk11142;
	extern Unk11087_t Unk11087;

	extern GetRoomNear_t GetRoomNear;
	extern ExpRel_t ExpRel;

	extern unsigned* MaxObjId;
}

namespace D2Launch {
	extern BnetData** BnData;
}

namespace D2Net {
	typedef void (__stdcall *SendPacket_t)(size_t aLen, unsigned arg1, unsigned char* aPacket);
	typedef void (__stdcall *ReceivePacket_t)(unsigned char* aPacket, unsigned aLen);
	typedef void (__stdcall *ReceivePacket_I_t)(unsigned char* aPacket, unsigned aLen);

	extern SendPacket_t SendPacket;
	extern ReceivePacket_t ReceivePacket;
	extern ReceivePacket_I_t ReceivePacket_I;
}

namespace D2Cmp {
	typedef void (__stdcall *InitCellFile_t)(void *cellfile, CellFile **outptr, char *srcfile, INT lineno, INT filever, char *filename);
	typedef void (__stdcall *DeleteCellFile_t)(CellFile *cellfile);

	extern InitCellFile_t InitCellFile;
	extern DeleteCellFile_t DeleteCellFile;
}

namespace D2Lang {
	typedef wchar_t* (__fastcall *GetLocaleText_t)(WORD nLocaleTxtNo);

	extern GetLocaleText_t GetLocaleText;
}

namespace Fog {

}

namespace Storm {

}

class OffsetManager {
private:
	static OffsetManager* instance;

	Offsets offsetList;
	Modules moduleList;
	bool basesDefined;
	bool offsetsDefined;

	//ModuleInfo* GetModuleInfo(int num);

	template<typename S>
	void InitPtr(S & Symbol, int Dllno, int Offset) {
		Symbol = reinterpret_cast<S>(GetDllOffset(Dllno, Offset));
	}
	template<>
	void InitPtr<unsigned>(unsigned & Symbol, int Dllno, int Offset) {
		Symbol = GetDllOffset(Dllno, Offset);
	}

	OffsetManager() : basesDefined(false), offsetsDefined(false) {}
	~OffsetManager() { CleanUp(); }
	OffsetManager(const OffsetManager &);
	OffsetManager & operator= (OffsetManager const &);

public:
	static OffsetManager * Instance();
	unsigned GetDllOffset(int DllNo, int Offset);

	bool DefineOffsets();
	bool DefineBases();

	void CleanUp();
};

#define INST_INT3	0xCC
#define INST_CALL	0xE8
#define INST_NOP	0x90
#define INST_JMP	0xE9
#define INST_RET	0xC3

enum PatchType {
	CALL,
	JMP,
	Fill
};

typedef struct PatchHook_t
{
	PatchType Type;
	DWORD dwDllNo;
	DWORD dwAddr;
	DWORD dwFunc;
	DWORD dwLen;
	BYTE *bOldCode;
} PatchHook;

class PatchManager {
private:
	PatchHook *RetrievePatchHooks(PINT pBuffer);
	void PatchBytes(DWORD dwAddr, DWORD dwValue, DWORD dwLen);
	void PatchJmp(DWORD dwAddr, DWORD dwFunc, DWORD dwLen);
	void PatchCall(DWORD dwAddr, DWORD dwFunc, DWORD dwLen);
	bool InterceptLocalCode(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen);
	bool FillBytes(void *pAddr, BYTE bFill, DWORD dwLen);
	bool WriteBytes(void *pAddr, void *pData, DWORD dwLen);

public:
	PatchManager() {}
	~PatchManager() {}

	void RemovePatches();
	bool InstallPatches();
};

