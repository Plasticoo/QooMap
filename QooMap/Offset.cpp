#include <windows.h>
#include <cstdio>

#include "Offset.h"
#include "Patch.h"

using namespace std;

namespace D2Client {
	/* D2Client Functions */
	PrintGameString_t PrintGameString;
	GetCurrentInteractingNPC_t	GetCurrentInteractingNPC;
	GetCursorItem_t	GetCursorItem;
	RevealAutomapRoom_t RevealAutomapRoom;
	GamePrintString_t GamePrintString;
	DrawAutomap_t DrawAutomap;
	DrawRectFrame_t DrawRectFrame;
	ExitGame_t ExitGame;
	DrawEnteringLevelText_t DrawEnteringLevelText;
	GetUnitX_t GetUnitX;
	GetUnitY_t GetUnitY;
	GetMonsterOwner_t GetMonsterOwner;
	FindClientSideUnit_t FindClientSideUnit;
	FindServerSideUnit_t FindServerSideUnit;
	GetItemNameString_t GetItemNameString;
	GetAutomapSize_t GetAutomapSize;
	CalculateShake_t CalculateShake;
	SetSelectedUnit_I_t SetSelectedUnit_I;
	ClickMap_t ClickMap;
	LeaveParty_t LeaveParty;
	InitAutomapLayer_I_t InitAutomapLayer_I;
	InitLevel_I_t InitLevel;
	NewAutomapCell_t NewAutomapCell;
	AddAutomapCell_t AddAutomapCell;
	GetPlayerUnit_t GetPlayerUnit;
	GetMouseXOffset_t GetMouseXOffset;
	GetMouseYOffset_t GetMouseYOffset;
	GetRosterUnitById_t GetRosterUnitById;
	GetSelectedUnit_t GetSelectedUnit;
	SendGamePacket_I_t SendGamePacket_I;
	GetMonsterText_I_t GetMonsterText_I;
	GetUiState_I_t GetUiState_I;
	SetUIVar_t	SetUIVar;
	GetUIVar_t	GetUIVar;
		//FUNCPTR(D2CLIENT, Transmute, void __fastcall, (void), 0x8CB90)
	
	/* D2Client Vars */
	RosterUnit** PlayerUnitList;
	UnitAny** PlayerUnit;
	unsigned* Ping;
	unsigned* FramesPerSecond;
	unsigned* FramesSkipped;
	AutomapLayer** CurrentAutomapLayer;
	InventoryLayout** TradeLayout;
	InventoryLayout** StashLayout;
	InventoryLayout** StoreLayout;
	InventoryLayout** CubeLayout;
	InventoryLayout** PlayerLayout;
	InventoryLayout** MercLayout;
	unsigned char* Difficulty;
	RosterUnit** UnitTable;
	unsigned* ExpCharFlag;
	unsigned* InGame;
	unsigned* NoPickup;
	unsigned* AutomapOn;
	POINT* PtAutomap;
	unsigned* AutomapMode;
	int *Divisor;
	POINT* Offset;
	int* yPosition;
	int* yShake;
	int* xShake;
	GameStructInfo** GameInfo;
	unsigned* AutomapYDrawPos;
	unsigned* ScreenSizeX_I; 
	unsigned* ScreenSizeX;
	unsigned* ScreenSizeY;
	unsigned* CursorHoverX;
	unsigned* CursorHoverY;
	unsigned* MouseX;
	unsigned* MouseY;
	int* MouseOffsetY;
	int* MouseOffsetX;

	/* Assembly Routines */
	unsigned GetUnitFromId_I;
	unsigned Interact_I;
	unsigned TakeWaypoint_I;
	unsigned ClickShopItem_I;
	unsigned ClickBelt_I;
	unsigned ItemPriceList;
	unsigned ClickBeltRight_I;
	unsigned ClickItemRight_I;
	unsigned MercItemAction_I;
	unsigned TestPvpFlag_I;
	unsigned LoadUIImage_I;
	unsigned GetUnitName_I;
	unsigned GetItemDesc_I;

	unsigned ClickParty_I;
	unsigned ClickParty_II;
	unsigned LoadAct_1;
	unsigned LoadAct_2;
	unsigned InputCall_I;
}

namespace D2Win {
	/* D2Win Functions */
	DrawSprites_t DrawSprites;
	DrawText_t DrawText;
	CreateEditBox_t CreateEditBox;
	GetTextWidthFileNo_t GetTextWidthFileNo;
	SetTextSize_t SetTextSize;
	SelectEditBoxText_t SelectEditBoxText;
	SetControlText_t SetControlText;
	SetEditBoxProc_t SetEditBoxProc;
	GetTextWidth_I_t GetTextWidth_I;
	InitMPQ_t InitMPQ;

	/* D2Win Vars */
	Control** FirstControl;
	Control** FocusedControl;
	Control** UnkControl;
	wchar_t *ItemDescBuffer;
}

namespace D2Gfx {
	/* D2Gfx Functions */
	DrawAutomapCell_t DrawAutomapCell;
	DrawAutomapCell2_t DrawAutomapCell2;
	GetHwnd_t GetHwnd;
	DrawRectangle_t DrawRectangle;
	DrawLine_t DrawLine;
	GetScreenSize_t GetScreenSize;
}

namespace D2Common {
	/* D2Common Functions */
	GetUnitByBeltSlot_t GetUnitByBeltSlot;
	GetUnitStat_t GetUnitStat;
	GetRoomFromUnit_t GetRoomFromUnit;
	AbsScreenToMap_t AbsScreenToMap;
	MapToAbsScreen_t MapToAbsScreen;
	GetExpForLevel_t GetExpForLevel;
	GetUnitMaxHealth_t GetUnitMaxHealth;
	IsLevelATown_t IsLevelATown;
	AddRoomData_t AddRoomData;
	GetItemMagicalMods_t	GetItemMagicalMods;
	RemoveRoomData_t RemoveRoomData;
	GetLevelText_t GetLevelText;
	GetObjectText_t GetObjectText;
	GetBaseStat_t	GetBaseStat;
	GetItemText_t GetItemText;
	GetItemPrice_t GetItemPrice;
	GetLayer_t GetLayer;
	GetLevel_t GetLevel;
	LoadLevel_t LoadLevel;
	InitLevel_t InitLevel;
	GetRoom1LevelNo_t GetRoom1LevelNo;
	LoadAct_t LoadAct;
	UnloadAct_t UnloadAct;
	GetSkillLevel_t GetSkillLevel;
	GetUnitDistance_t GetUnitDistance;
	GetFirstItemFromInventory_t GetFirstItemFromInventory;
	GetNextItemFromInventory_t GetNextItemFromInventory;
	Unk10651_t Unk10651;
	Unk11142_t Unk11142;
	Unk11087_t Unk11087;

	GetRoomNear_t GetRoomNear;
	ExpRel_t ExpRel;

	/* D2Common Vars */
	unsigned* MaxObjId;
}

namespace D2Launch {
	/* D2Launch Vars*/
	BnetData** BnData;
}

namespace D2Net {
	/* D2Net Functions */
	SendPacket_t SendPacket;
	ReceivePacket_t ReceivePacket;
	ReceivePacket_I_t ReceivePacket_I;
}

namespace D2Cmp {
	/* D2Cmp Functions */
	InitCellFile_t InitCellFile;
	DeleteCellFile_t DeleteCellFile;
}

namespace D2Lang {\
	/* D2Lang Functions */
	GetLocaleText_t GetLocaleText;
}

namespace Fog {

}

namespace Storm {

}

OffsetManager* OffsetManager::instance = NULL;

OffsetManager* OffsetManager::Instance() {
	return instance ? instance : ((instance = new OffsetManager));
}

unsigned OffsetManager::GetDllOffset(int DllNo, int Offset)
{
	HMODULE hMod = GetModuleHandle(moduleList[DllNo]->ModuleName.c_str());
	if(!hMod)
		hMod = LoadLibrary(moduleList[DllNo]->ModuleName.c_str());

	if(!hMod)
		throw;

	DWORD Addr = 0;
	if(Offset < 0) {
		Addr = (DWORD)GetProcAddress(hMod, (LPCSTR)(-Offset));
		printf("Addr %#08X for %s Ordinal %lu\n", Addr, moduleList[DllNo]->ModuleName.c_str(), -Offset);
	} else {
		Addr = ((DWORD)hMod)+Offset;
		printf("Addr %#08X for %s Offset %#08X\n", Addr, moduleList[DllNo]->ModuleName.c_str(), Offset);
	}

	if(Addr == 0)
		throw;

	return Addr;
}

void OffsetManager::CleanUp() {
	if(moduleList.size()) {
		for(unsigned i = 0; i < moduleList.size(); i++) {
			delete moduleList[i];
		}
	}
}

bool OffsetManager::DefineBases() {
	if(basesDefined)
		return false;

	if(moduleList.size())
		throw;

	for(int i = 0; i < MaxModNo; i++) {
		ModuleInfo* modInfo = new ModuleInfo;
		modInfo->ModuleName = dll[i].DllName;
		modInfo->ModuleNo = dll[i].DllNo;
		modInfo->BaseAddress = 0;
		moduleList.push_back(modInfo);

		unsigned BaseAddress = GetDllOffset(i, 0);
		moduleList[i]->BaseAddress = BaseAddress;
	}

	basesDefined = true;

	return true;
}

bool OffsetManager::DefineOffsets() {
	try {
		if(!DefineBases())
			throw;
	} catch(const exception &ex) {
		throw(ex);
	}

	// I need to figure out a clean way to automate this
	InitPtr(D2Client::PrintGameString, D2CLIENT, 0x7D850);
	InitPtr(D2Client::GetCurrentInteractingNPC,D2CLIENT,0x46150);
	InitPtr(D2Client::GetCursorItem,D2CLIENT,0x16020);
	InitPtr(D2Client::GetAutomapSize, D2CLIENT, 0x5F080);
	InitPtr(D2Client::RevealAutomapRoom, D2CLIENT, 0x62580);
	InitPtr(D2Client::GamePrintString, D2CLIENT, 0x7D850);
	InitPtr(D2Client::DrawAutomap, D2CLIENT, 0x62680);
	InitPtr(D2Client::DrawRectFrame, D2CLIENT, 0xBE4C0);
	InitPtr(D2Client::GetMonsterOwner,D2CLIENT,0x216A0);
	//FUNCPTR(D2CLIENT, DrawRectFrame, VOID __fastcall, (DWORD Rect), 0xBE4C0) // Updated 1.13c

	InitPtr(D2Client::ExitGame, D2CLIENT, 0x42850);
	InitPtr(D2Client::GetUnitX, D2CLIENT, 0x1630);
	InitPtr(D2Client::GetUnitY, D2CLIENT, 0x1660);
	InitPtr(D2Client::FindClientSideUnit, D2CLIENT, 0xA5B20);
	InitPtr(D2Client::FindServerSideUnit, D2CLIENT, 0xA5B40);
	InitPtr(D2Client::GetAutomapSize, D2CLIENT, 0x5F080);
	InitPtr(D2Client::CalculateShake, D2CLIENT, 0x8AFD0);
	InitPtr(D2Client::SetSelectedUnit_I, D2CLIENT, 0x51860);
	InitPtr(D2Client::GetUIVar,D2CLIENT,0xBE400);
	InitPtr(D2Client::SetUIVar,D2CLIENT,0xC2790);
	InitPtr(D2Client::ClickMap, D2CLIENT, 0x1BF20);
	InitPtr(D2Client::LeaveParty, D2CLIENT, 0x9E5D0);
	InitPtr(D2Client::InitAutomapLayer_I, D2CLIENT, 0x62710);
	InitPtr(D2Client::GetItemNameString,D2CLIENT,0x914F0); //아이템네임
	//InitPtr(D2Client::InitLevel_I, D2CLIENT,0x62710); // Dup ?????
	InitPtr(D2Client::NewAutomapCell, D2CLIENT, 0x5F6B0);
	InitPtr(D2Client::AddAutomapCell, D2CLIENT, 0x61320);
	InitPtr(D2Client::PtAutomap,D2CLIENT, 0x11C23C);
	InitPtr(D2Client::GetPlayerUnit, D2CLIENT, 0xA4D60);
	InitPtr(D2Client::PlayerUnit, D2CLIENT, 0x11BBFC);
	InitPtr(D2Client::Divisor, D2CLIENT, 0xF16B0);
	InitPtr(D2Client::GetMouseXOffset, D2CLIENT, 0x3F6C0);
	InitPtr(D2Client::GetMouseYOffset, D2CLIENT, 0x3F6D0);
	InitPtr(D2Client::GetRosterUnitById, D2CLIENT, 0x9D630);
	InitPtr(D2Client::GetSelectedUnit, D2CLIENT, 0x51A80);
	InitPtr(D2Client::SendGamePacket_I, D2CLIENT, 0x143E0);
	InitPtr(D2Client::GetMonsterText_I, D2CLIENT, 0x1230);

	InitPtr(D2Client::LoadUIImage_I,D2CLIENT, 0x2B380); // Updated 1.13c
	InitPtr(D2Client::GetUiState_I, D2CLIENT, 0xBE400);
	InitPtr(D2Client::PlayerUnitList, D2CLIENT, 0x11BC14);
	InitPtr(D2Client::Ping, D2CLIENT, 0x119804);
	InitPtr(D2Client::FramesPerSecond, D2CLIENT, 0x11C2AC);
	InitPtr(D2Client::FramesSkipped, D2CLIENT, 0x119810);
	InitPtr(D2Client::CurrentAutomapLayer, D2CLIENT, 0x11C1C4);
	InitPtr(D2Client::TradeLayout, D2CLIENT, 0x10B288);
	InitPtr(D2Client::StashLayout, D2CLIENT, 0x10B2D0);
	InitPtr(D2Client::StoreLayout, D2CLIENT, 0x10B3B0);
	InitPtr(D2Client::CubeLayout, D2CLIENT, 0x10B3C8);
	InitPtr(D2Client::PlayerLayout, D2CLIENT, 0x10B3E0);
	InitPtr(D2Client::MercLayout, D2CLIENT, 0x11BD94);
	InitPtr(D2Client::Difficulty, D2CLIENT, 0x11C390);
	InitPtr(D2Client::UnitTable, D2CLIENT, 0x10A608);
	InitPtr(D2Client::ExpCharFlag, D2CLIENT, 0x119854);
	InitPtr(D2Client::InGame, D2CLIENT, 0x11C39C);
	InitPtr(D2Client::NoPickup, D2CLIENT, 0x11C2F0);
	InitPtr(D2Client::AutomapOn, D2CLIENT, 0xFADA8);
	InitPtr(D2Client::AutomapMode, D2CLIENT, 0xF16B0);
	InitPtr(D2Client::Offset, D2CLIENT, 0x11C1F8);
	InitPtr(D2Client::yPosition, D2CLIENT, 0xBC21C);
	InitPtr(D2Client::yShake, D2CLIENT, 0x10B9DC);
	InitPtr(D2Client::GameInfo, D2CLIENT, 0x11B980);
	InitPtr(D2Client::xShake, D2CLIENT, 0x11BF00);
	InitPtr(D2Client::AutomapYDrawPos, D2CLIENT, 0x11C21C);
	InitPtr(D2Client::ScreenSizeX_I, D2CLIENT, 0xF9E14) ;
	InitPtr(D2Client::ScreenSizeX, D2CLIENT, 0xDBC48);
	InitPtr(D2Client::ScreenSizeY, D2CLIENT, 0xDBC4C);
	InitPtr(D2Client::CursorHoverX, D2CLIENT, 0xE0EB8);
	InitPtr(D2Client::CursorHoverY, D2CLIENT, 0xE0EBC);
	InitPtr(D2Client::MouseX, D2CLIENT, 0x11B828);
	InitPtr(D2Client::MouseY, D2CLIENT, 0x11B824);
	InitPtr(D2Client::MouseOffsetY, D2CLIENT, 0x11995C);
	InitPtr(D2Client::MouseOffsetX, D2CLIENT, 0x119960);
	InitPtr(D2Client::DrawEnteringLevelText,D2CLIENT,0xBF0D0);

	InitPtr(D2Client::GetUnitFromId_I, D2CLIENT, 0xA4E20);
	InitPtr(D2Client::Interact_I, D2CLIENT, 0x1BDE0);
	InitPtr(D2Client::TakeWaypoint_I, D2CLIENT, 0xAA8B3);
	InitPtr(D2Client::ClickShopItem_I, D2CLIENT, 0x46EE0);
	InitPtr(D2Client::ClickBelt_I, D2CLIENT, 0x28260);
	InitPtr(D2Client::ClickBeltRight_I, D2CLIENT, 0x29990);
	InitPtr(D2Client::ClickItemRight_I, D2CLIENT, 0x98B60);
	InitPtr(D2Client::MercItemAction_I, D2CLIENT, 0x14A10);
	InitPtr(D2Client::TestPvpFlag_I, D2CLIENT, 0x4FD90);
	InitPtr(D2Client::GetUnitName_I, D2CLIENT, 0xA5D90);
	InitPtr(D2Client::GetItemDesc_I,D2CLIENT, 0x560B0); // Updated 1.13c
	InitPtr(D2Client::ClickParty_I, D2CLIENT, 0x9E180);
	InitPtr(D2Client::ClickParty_II, D2CLIENT, 0x773A0);
	InitPtr(D2Client::ItemPriceList,D2CLIENT,0x11973B);
	InitPtr(D2Client::LoadAct_1, D2CLIENT, 0x62AA0);
	InitPtr(D2Client::LoadAct_2, D2CLIENT, 0x62760);
	InitPtr(D2Client::InputCall_I, D2CLIENT, 0x147A0);

	InitPtr(D2Common::GetUnitByBeltSlot, D2COMMON, -10455);
	InitPtr(D2Common::GetUnitStat, D2COMMON, 0x38B70);
	InitPtr(D2Common::GetRoomFromUnit, D2COMMON, 0x2FE10);
	InitPtr(D2Common::AbsScreenToMap, D2COMMON, -10474);
	InitPtr(D2Common::MapToAbsScreen, D2COMMON, -11087);
	InitPtr(D2Common::GetExpForLevel, D2COMMON, -10399);
	InitPtr(D2Common::GetUnitMaxHealth, D2COMMON, -10913);
	InitPtr(D2Common::IsLevelATown, D2COMMON, -10416);
	InitPtr(D2Common::AddRoomData, D2COMMON, -10401);//1.13bPTR
	InitPtr(D2Common::GetItemMagicalMods,D2COMMON, -10248);
	InitPtr(D2Common::RemoveRoomData, D2COMMON, -11099);//1.13bPTR
	InitPtr(D2Common::GetLevelText, D2COMMON, -10014);
	InitPtr(D2Common::GetObjectText, D2COMMON, -10688);
	InitPtr(D2Common::GetItemText, D2COMMON, 0x719A0); //원본 -10695
	//InitPtr(D2Common::GetBaseStat,D2COMMON,-10550);
	InitPtr(D2Common::GetItemPrice,D2COMMON, -10107);
	InitPtr(D2Common::GetLayer, D2COMMON, -10749);
	InitPtr(D2Common::GetLevel, D2COMMON, -10207);
	InitPtr(D2Common::LoadLevel, D2COMMON, -10207);
	InitPtr(D2Common::GetRoom1LevelNo, D2COMMON, -10826);
	InitPtr(D2Common::LoadAct, D2COMMON, 0x3CB30); //1.13bPTR
	InitPtr(D2Common::UnloadAct, D2COMMON, -10868);
	InitPtr(D2Common::GetSkillLevel, D2COMMON, -10306);
	InitPtr(D2Common::GetUnitDistance, D2COMMON, -10522);
	InitPtr(D2Common::GetFirstItemFromInventory, D2COMMON, -10460);
	InitPtr(D2Common::GetNextItemFromInventory, D2COMMON, -10464);
	InitPtr(D2Common::Unk10651, D2COMMON, -10651);
	InitPtr(D2Common::Unk11142, D2COMMON, -11142);
	InitPtr(D2Common::Unk11087, D2COMMON, -11087);
	InitPtr(D2Common::InitLevel, D2COMMON, -10322);//1.1
	InitPtr(D2Common::MaxObjId, D2COMMON, 0xA0B98);

	InitPtr(D2Common::GetRoomNear, D2COMMON, -10916);
	InitPtr(D2Common::ExpRel, D2COMMON, -10066);

	InitPtr(D2Win::DrawSprites, D2WIN, 0x18750);
	InitPtr(D2Win::CreateEditBox, D2WIN, -10057);
	InitPtr(D2Win::DrawText, D2WIN, 0x12FA0);
	InitPtr(D2Win::GetTextWidthFileNo, D2WIN, 0x12700);
	InitPtr(D2Win::SetTextSize, D2WIN, 0x12FE0);
	InitPtr(D2Win::SelectEditBoxText, D2WIN, -10021); // Updated  wchar_t* arg 2 seems to be gone
	InitPtr(D2Win::SetControlText, D2WIN, -10042);
	InitPtr(D2Win::SetEditBoxProc, D2WIN, 0x13970);
	InitPtr(D2Win::GetTextWidth_I, D2WIN, -10028);
	InitPtr(D2Win::InitMPQ, D2WIN,0x7E60);

	InitPtr(D2Win::FirstControl, D2WIN, 0x214A0);
	InitPtr(D2Win::FocusedControl, D2WIN, 0x214B0);
	InitPtr(D2Win::UnkControl, D2WIN, 0xC8D04);
	InitPtr(D2Win::ItemDescBuffer,D2WIN,0xC9E58);

	InitPtr(D2Gfx::DrawAutomapCell,D2GFX,-10079);
	InitPtr(D2Gfx::DrawAutomapCell2,D2GFX,-10041);
	InitPtr(D2Gfx::GetHwnd, D2GFX, 0x7FB0);
	InitPtr(D2Gfx::DrawRectangle, D2GFX, 0xB9F0);
	InitPtr(D2Gfx::DrawLine, D2GFX, 0xB9C0);
	InitPtr(D2Gfx::GetScreenSize, D2GFX, -10031); 

	InitPtr(D2Launch::BnData, D2LAUNCH, 0x25ABC);

	InitPtr(D2Cmp::InitCellFile, D2CMP, -10006);
	InitPtr(D2Cmp::DeleteCellFile, D2CMP, -10065); // 10106

	InitPtr(D2Net::SendPacket, D2NET, -10024);
	InitPtr(D2Net::ReceivePacket, D2NET, 0x6BD0);
	InitPtr(D2Net::ReceivePacket_I, D2NET, -10033);

	InitPtr(D2Lang::GetLocaleText, D2LANG, -10003);

	return true;
}

bool PatchManager::InstallPatches(void) {
	for(int x = 0; x < ArraySize(pHooks); x++)
	{
		DWORD Offset = OffsetManager::Instance()->GetDllOffset(pHooks[x].dwDllNo, pHooks[x].dwAddr);
		pHooks[x].dwAddr = Offset;
		pHooks[x].bOldCode = new BYTE[pHooks[x].dwLen];
		if(::ReadProcessMemory(GetCurrentProcess(), (void*)Offset, pHooks[x].bOldCode, pHooks[x].dwLen, NULL) == 0) {
			printf("ReadProcessMemory: Offset='%#08X' Err'%d'\n", Offset, GetLastError());
			return false;
		}

		// Need better handling
		if(pHooks[x].Type == CALL)
			PatchCall(Offset, pHooks[x].dwFunc, pHooks[x].dwLen);
		else if(pHooks[x].Type == JMP)
			PatchJmp(Offset, pHooks[x].dwFunc, pHooks[x].dwLen);
		else if(pHooks[x].Type == Fill )
			FillBytes((LPVOID)Offset, (BYTE)pHooks[x].dwFunc, pHooks[x].dwLen);
		}
	return true;
}

void PatchManager::RemovePatches()
{
	for(int x = 0; x < ArraySize(pHooks); x++) {
		if(!WriteBytes((void*)pHooks[x].dwAddr, pHooks[x].bOldCode, pHooks[x].dwLen)) {
			delete[] pHooks[x].bOldCode;
			throw;
		}
		delete[] pHooks[x].bOldCode;
	}	
}

bool PatchManager::WriteBytes(void *pAddr, void *pData, DWORD dwLen)
{
	DWORD dwOld;

	if(!VirtualProtect(pAddr, dwLen, PAGE_READWRITE, &dwOld))
		return false;

	::memcpy(pAddr, pData, dwLen);
	return VirtualProtect(pAddr, dwLen, dwOld, &dwOld) != FALSE;
}

bool PatchManager::FillBytes(void *pAddr, BYTE bFill, DWORD dwLen)
{
	BYTE *bCode = new BYTE[dwLen];
	::memset(bCode, bFill, dwLen);

	if(!WriteBytes(pAddr, bCode, dwLen)) {
		delete [] bCode;
		return false;
	}

	delete[] bCode;
	return true;
}

bool PatchManager::InterceptLocalCode(BYTE bInst, DWORD pAddr, DWORD pFunc, DWORD dwLen)
{
	BYTE *bCode = new BYTE[dwLen];
	::memset(bCode, 0x90, dwLen);
	DWORD dwFunc = pFunc - (pAddr + 5);

	bCode[0] = bInst;
	*(DWORD *)&bCode[1] = dwFunc;
	if(!WriteBytes((void*)pAddr, bCode, dwLen)) {
		delete [] bCode;
		return false;
	}

	delete [] bCode;
	return true;
}

void PatchManager::PatchCall(DWORD dwAddr, DWORD dwFunc, DWORD dwLen)
{
	if(!InterceptLocalCode(INST_CALL, dwAddr, dwFunc, dwLen))
		throw;
}

void PatchManager::PatchJmp(DWORD dwAddr, DWORD dwFunc, DWORD dwLen)
{
	if(!InterceptLocalCode(INST_JMP, dwAddr, dwFunc, dwLen))
		throw;
}

void PatchManager::PatchBytes(DWORD dwAddr, DWORD dwValue, DWORD dwLen)
{
	BYTE *bCode = new BYTE[dwLen];
	::memset(bCode, (BYTE)dwValue, dwLen);

	if(!WriteBytes((LPVOID)dwAddr, bCode, dwLen)) {
		delete [] bCode;
		throw;
	}

	delete [] bCode;
}

PatchHook *PatchManager::RetrievePatchHooks(PINT pBuffer)
{
	*pBuffer = ArraySize(pHooks);
	return &pHooks[0];
}

