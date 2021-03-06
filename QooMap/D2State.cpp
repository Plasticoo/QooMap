#include "Offset.h"
#include "D2Structs.h"
#include "D2State.h"
#include "Maphack.h"
#include "AutoMap.h"
#include "ScreenHooks.h"
#include "BuyAction_Info.h"
#include "EnteringLevelText.h"

#define D2TXTCODE(x) ( ((x)&0xff)<<24 | ((x)>>8&0xff)<<16 | ((x)>>16&0xff)<<8 | ((x)>>24&0xff) )

#pragma warning ( disable: 4996 )

ClientGameState State = ClientStateNull;

WNDPROC OldWndProc = NULL;

UnitAny *viewunit;

char pFile[65535];
bool viewunitstatusok = false;
unsigned int d2res_extend_width = 0;
unsigned int d2res_extend_height = 0;
bool d2res_on = false;
bool OnlyUniqueOK = false;
unsigned int jointime = 0;
unsigned int viewplayerstate = 0; //플레이어 상태보기
bool viewCorpse = false;
bool joingame= false;;
bool FastTownPortal;
bool ScrollMapON =  false;
bool AutoEatCorpse = true;
bool DieOK = false;
bool DrawSetting = false;
bool AllShrine = false;
bool AllGems = false;
bool AlwaysTop = false;
bool Cube_Start = false;
unsigned int AlwaysAltDown= 0;
DWORD Cube_Timer = 0;
RECT rect;
bool fakeselect = false;
unsigned int myportalnum = 0;
bool myportalonoff = false;
bool myitemview = false;
bool d2extend_ok = false;
bool togglelight = true;
bool QooMapNoticeOK = false;
DWORD NoticeTime = 0;
wchar_t QooMapNotice[150] = L"";
bool QooMapChicken = false;
bool QooMapChickenTown = true;
bool QooMapChickenLeftGame = false;
bool PerfectGhostly=false;
unsigned int QooMapChickenLifePercent;
bool BuyOilStart;
bool DropNotify = false;
bool ViewItemBasicStats = false;
HWND WindowTitlehWnd;
bool SkillAutoClickOK;
bool PlayerBuff = false;
bool LockClick = false;
unsigned int belt_potion_id;
DWORD RenewalOilID;
DWORD RenewalOilCost=1000;
DWORD RenewalOilNpc;
bool smallcharm = true;
bool largecharm = true;
bool grandcharm = true;
bool RareRing = false;
bool healingpotion= true;
bool manapotion= true;
bool socket3 = false;
bool socket4 = false;
bool socket5 = false;
bool socket6 = false;
bool inGame;

BYTE MenuAllUniqueItem;
unsigned int MenuAllUnique = 0;
bool EffectView = false;
int playercorpsex;
int playercorpsey;
bool AutoTransmute;
bool Dragon_RareRenwal;
bool SellPriceCheck;

void ChangeStat(BYTE StatID, BYTE Value)
{
	LPBYTE Packet = new BYTE[3];
	Packet[0] = 0x1D;
	Packet[1] = StatID;
	Packet[2] = Value;
	D2Net::ReceivePacket(Packet, 3);
	delete [] Packet;
}


void Delay(DWORD dwMillisecond)
{
  MSG msg;
  DWORD dwStart;
  dwStart = GetTickCount();

 while(GetTickCount() - dwStart < dwMillisecond) 
 {
  while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
   TranslateMessage(&msg);
   DispatchMessage(&msg);
  }
 } 
} 

void QooMap_Notice(wchar_t *text){
	wsprintfW(QooMapNotice,L"%s",text);
	QooMapNoticeOK = true;
	NoticeTime = GetTickCount();
}

LPCSTR BoolToSTring(bool Value){
	if (Value == false)
		return "0";
	else
		return "1";
}

ClientGameState ClientState(void)
{
	UnitAny* pPlayer = D2Client::GetPlayerUnit();

	if(!*D2Win::FirstControl && *D2Client::InGame && pPlayer && pPlayer->pAct)
		return ClientStateInGame;
	else if(*D2Win::FirstControl && !*D2Client::InGame && !pPlayer)
		return ClientStateMenu;

	return ClientStateBusy;
}

void __declspec(naked) ItemBasicStatSTUB()
{
	__asm
	{
		cmp [ViewItemBasicStats], 0;
		JZ CallCode;
		xor EAX, EAX;
		JMP QuitCode;
	CallCode:
		push DWORD PTR [ESP + 4];
		call D2Common::GetFirstItemFromInventory;
QuitCode:
		RET 4;
	}
}

DWORD Addr = 0x6FB6F6C0;
__declspec (naked) CellFile* __fastcall LoadUIDC6(char* szFileName)
{
    __asm
   {
      push esi
      mov esi, ecx
      call Addr
      pop esi
      retn
   }
}

void d2gfx_10060 (DWORD unknown)
{
	typedef void (__stdcall* pd2gfx_10060)(DWORD, DWORD);
	pd2gfx_10060 d2gfx_10060 = (pd2gfx_10060) 0x6FA88360;
	d2gfx_10060(unknown, 0);

}
//6FD8B7D0 // 반 나눈값 창사이즈


//DWORD ChangeWindowCall= 0x6FAC0DF0; // 호출 0x6FAC0DF0

void __declspec(naked) __fastcall NewResolution(DWORD width,DWORD height) {
   __asm {
      MOV DWORD PTR DS:[0x6FB8BC48], ecx//w 
      MOV DWORD PTR DS:[0x6FB8BC4C], edx //h

      MOV DWORD PTR DS:[0x6FBA9E14], ecx// W
      MOV DWORD PTR DS:[0x6FBA9E18], edx // H

	  MOV DWORD PTR DS:[0x6FB8BC50], edx // H

	  ret
   }
}

DWORD __declspec(naked) __fastcall D2CLIENT_GetUnitFromId_STUB(DWORD unitid, DWORD unittype) {
	__asm
	{
		pop eax
		push edx // unittype
		push eax // return address

		shl edx, 9
		mov eax, D2Client::UnitTable
		add edx, eax
		mov eax, ecx // unitid
		and eax, 0x7F
		jmp D2Client::GetUnitFromId_I
	}
}

void FakeClick(int x,int y){
	if (x != 0 && y !=0) SetCursorPos(x,y);
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}

void AutoParty(){

	UnitAny *pPlayer = D2Client::GetPlayerUnit();

	for (RosterUnit* pUnit = *D2Client::PlayerUnitList; pUnit ; pUnit = pUnit->pNext) {
		if (pUnit->dwUnitId == pPlayer->dwUnitId)
			continue;

		switch(pUnit->dwPartyFlags) {
		case PARTY_INVITED_YOU:
			if((*D2Client::PlayerUnitList)->dwPartyFlags == 0) {
				ClickParty_ASM(pUnit, clickParty_Accept);
			}
			break;
		case PARTY_NOT_IN_PARTY:
			if((*D2Client::PlayerUnitList)->dwPartyFlags != 4)
				ClickParty_ASM(pUnit, clickParty_Invite);
			break;
		}
	}
}

__declspec(naked) UnitAny* __fastcall ClickParty_ASM(RosterUnit* RosterUnit, unsigned Mode)
{
		__asm {
			mov eax, ecx
			jmp D2Client::ClickParty_I
		}
}

DWORD __declspec(naked) __fastcall GetUIVarSTUB(DWORD UI)
{
	__asm
	{
		MOV EAX, ECX;
		jmp D2Client::GetUIVar;
	}
}

UnitAny *GetViewingUnit()
{
	if (!viewunit)
		return *D2Client::PlayerUnit;
	return viewunit;
}

BYTE ClickTransmute[7] = {0x4f, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00}; // 조합 클릭

void CtrlAction(HWND hWnd){
			UnitAny *pOilPotion = NULL;
			UnitAny *pCubeItem = NULL;
			UnitAny *pShrineItem = NULL;
			GetWindowRect(hWnd,&rect);
			DWORD LevelNo = D2Common::GetRoom1LevelNo(D2Client::GetPlayerUnit()->pPath->pRoom1);
	
			if (GetUIVarSTUB(UI_CUBE)){
				/*
				if((pCubeItem = SearchItem(STORAGE_CUBE)) != NULL) {
					if (pCubeItem->pItemData->dwQuality == ITEM_QUALITY_CRAFT){
						if ((pShrineItem = FindShrine(STORAGE_CUBE)) != NULL) {
									SendGamePacket(ClickTransmute, sizeof(ClickTransmute));	//조합 클릭
						}
						else {
							if ((pShrineItem = FindShrine(STORAGE_INVENTORY)) != NULL)
								{
								GetWindowRect(hWnd,&rect);
								InventoryLayout* pLayout = NULL;
								pLayout = (InventoryLayout*)D2Client::PlayerLayout;

								int x = pShrineItem->pItemPath->dwPosX;
								int y = pShrineItem->pItemPath->dwPosY;

								*D2Client::CursorHoverX = x;
								*D2Client::CursorHoverY = y;

								x = pLayout->Left + x * pLayout->SlotPixelWidth + 10; 
								y = pLayout->Top + y * pLayout->SlotPixelHeight + 40;

								SetCursorPos(rect.left+x,rect.top+y); //리뉴오일 위치
								
									Cube_Start = true;
									CubeActionNum = 0;
									Cube_Timer = GetTickCount();
								}
						}
					}
				else // 큐브안에 크래프트 아이템이 아니면 실행
					if((pOilPotion = FindTownPortalScroll(STORAGE_CUBE)) != NULL){
						if (pCubeItem->pItemData->dwQuality == ITEM_QUALITY_RARE){
							InventoryLayout* pLayout = NULL;
							pLayout = (InventoryLayout*)D2Client::CubeLayout;

							int x = pCubeItem->pItemPath->dwPosX;
							int y = pCubeItem->pItemPath->dwPosY;

							*D2Client::CursorHoverX = x;
							*D2Client::CursorHoverY = y;

							x = pLayout->Left + x * pLayout->SlotPixelWidth + 10; 
							y = pLayout->Top + y * pLayout->SlotPixelHeight + 40;

							SetCursorPos(rect.left+x,rect.top+y); //리뉴오일 위치
							Cube_Start = true;

						}
					}
					else if((pOilPotion = FindRenewalOil(STORAGE_INVENTORY)) != NULL) // 리뉴얼 오일일경우 //v5 2409 /v6 = 2410
					{
					InventoryLayout* pLayout = NULL;
					pLayout = (InventoryLayout*)D2Client::PlayerLayout;

					int x = pOilPotion->pItemPath->dwPosX;
					int y = pOilPotion->pItemPath->dwPosY;

					*D2Client::CursorHoverX = x;
					*D2Client::CursorHoverY = y;

					x = pLayout->Left + x * pLayout->SlotPixelWidth + 10; 
					y = pLayout->Top + y * pLayout->SlotPixelHeight + 40;

					SetCursorPos(rect.left+x,rect.top+y); //리뉴오일 위치

					Cube_Timer = GetTickCount();
					Cube_Start = true;
					CubeActionNum = 0;

					}
				}
				*/
			}
			
			else { // 빠른 포탈 

				//하코일경우 강제 종료 
					if (!D2Common::IsLevelATown(LevelNo)){
						if ((*D2Launch::BnData)->nCharFlags & PLAYER_TYPE_HARDCORE)
								D2Client::ExitGame();
						else{
							if((pOilPotion = FindPortal(STORAGE_INVENTORY)) != NULL) {	  // v05 = 1574 /v06 = 1575		
								int nQuantity = D2Common::GetUnitStat(pOilPotion, 70, 0); //Qualntity
								if (nQuantity != 0){
									myportalnum = nQuantity-1;
									myportalonoff = true;
									BYTE aPacket[13] = {0};
									aPacket[0] = 0x20;
									*(LPDWORD)&aPacket[1] = pOilPotion->dwUnitId;
									*(LPDWORD)&aPacket[5] = D2Client::GetPlayerUnit()->pPath->xPos;
									*(LPDWORD)&aPacket[9] = D2Client::GetPlayerUnit()->pPath->yPos;
									SendGamePacket(aPacket,sizeof(aPacket));
									if (FastTownPortal == false ) FastTownPortal = true;
									}
								}
							}
					} //chicken !=false 끝	
			} //else 끝
}

LONG WINAPI WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	ClientGameState gameState = ClientState();

		WORD repeatCount = LOWORD(lParam);
		//bool altState = !!(HIWORD(lParam) & KF_ALTDOWN);
		bool previousState = !!(HIWORD(lParam) & KF_REPEAT);
		bool transitionState = !!(HIWORD(lParam) & KF_UP);
		//bool isRepeat = !transitionState && repeatCount != 1;
		bool isDown = !(previousState && transitionState);
		//bool isUp = previousState && transitionState;

	if(uMsg == WM_KEYDOWN && gameState == ClientStateMenu) {
		if(isDown && !previousState && repeatCount == 1 && wParam == VK_CONTROL) {
				GetWindowRect(hWnd,&rect);
				FakeClick(rect.left+700,rect.top + 480); //게임 이름
				Delay(10);
				FakeClick(rect.left+700,rect.top + 450); //게임 이름
			}
	}
	
	if(uMsg == WM_KEYDOWN && gameState == ClientStateInGame) {
		

		if(isDown && !previousState && repeatCount == 1 && wParam == 0x30) { //숫자 0
			UnitAny *pUnit = D2Client::GetSelectedUnit();

			if (pUnit && pUnit->pInventory && pUnit->dwType == UNIT_TYPE_PLAYER){
				if (pUnit->dwMode != PLAYER_MODE_DEAD && pUnit->dwMode != PLAYER_MODE_DEATH)
					viewunit = pUnit;
					if (!GetUIVarSTUB(UI_INVENTORY))
						D2Client::SetUIVar(UI_INVENTORY,0,0);
			}

			return 1;
		}

		if(isDown && !previousState && repeatCount == 1 && wParam == 0x6A) { //키패드 *
			 if (!GetUIVarSTUB(UI_CHAT)) {
				if (togglelight == true) togglelight = false; //정보보기 1페이지
				else togglelight = true; // 정보보기 2페이지
			 }
			
			return 1;
		}

		if(isDown && !previousState && repeatCount == 1 && wParam == VK_DELETE) { //숫자38  = 8
				if (AlwaysAltDown == 0) {
					QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0AltKey Down Type 1 (Always Press) ÿc1ON");
					AlwaysAltDown = 1;
				}
				else if (AlwaysAltDown == 1){ 
					QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0AltKey Down Type 2 (Selective Press) ÿc1ON");
					AlwaysAltDown = 2;
				}
				else {
					QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Altkey Down ÿc1Off");
					D2Client::SetUIVar(UI_ALTDOWN,1,0);
					AlwaysAltDown = 0;
				}
		}

		if(isDown && !previousState && repeatCount == 1 && wParam == 0xc0) { //숫자38  = `표션빨때키
			 if (!GetUIVarSTUB(UI_CHAT)) {
				if (fakeselect == false){
					QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Wtf (Boss Auto Target) ÿc1On");
					//D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Wtf On",1);
					fakeselect = true;}
					else {
						QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Wtf (Boss Auto Target) ÿc1Off");
						//D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Wtf Off",1);
						fakeselect = false;
					}
			 }
				return 1;
		}

		if(isDown && !previousState && repeatCount == 1 && wParam == 0x4A) { //  J키
			 if (!GetUIVarSTUB(UI_CHAT)) {
				GetWindowRect(hWnd,&rect);
				if (!GetUIVarSTUB(UI_CHAT)) {
					if (AlwaysTop == false){
						SetWindowPos(hWnd,HWND_TOPMOST,rect.left,rect.top,0,0,SWP_NOSIZE);
						AlwaysTop = true;
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0항상 위 ÿc1ON") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Always Top ÿc1ON");
						//D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0항상 위 On",1);
					}
					else {
						SetWindowPos(hWnd,HWND_NOTOPMOST,rect.left,rect.top,0,0,SWP_NOSIZE);
						AlwaysTop = false;
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0항상 위 ÿc1Off") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Always Top ÿc1Off");
						//D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0항상 위 Off",1);
					}
				}		
			 }
			return 1;
		}

		if (ScrollMapON == true){
				if(isDown && !previousState && repeatCount == 1 && wParam == VK_LEFT ) {//넘버패드9
					ScrollMap(-2,0);}
				if(isDown && !previousState && repeatCount == 1 && wParam == VK_RIGHT ) {//넘버패드9
					ScrollMap(2,0); }
				if(isDown && !previousState && repeatCount == 1 && wParam == VK_UP ) {//넘버패드9
					ScrollMap(0,-2);}
				if(isDown && !previousState && repeatCount == 1 && wParam == VK_DOWN ) {//넘버패드9
					ScrollMap(0,2);}
		}

		if(isDown && !previousState && repeatCount == 1 && wParam == VK_CONTROL) {
			/*
			if (GetUIVarSTUB(UI_NPCSHOP) && *D2Client::MouseX < 400) {
				GetWindowRect(hWnd,&rect);

				if (D2Client::GetPlayerUnit()->dwAct ==0)SetCursorPos(rect.left +370,rect.top + 250);
				if (D2Client::GetPlayerUnit()->dwAct ==1)SetCursorPos(rect.left +370,rect.top + 250);
				if (D2Client::GetPlayerUnit()->dwAct ==2)SetCursorPos(rect.left +370,rect.top + 280);
				if (D2Client::GetPlayerUnit()->dwAct ==3)SetCursorPos(rect.left +340,rect.top + 340);
				if (D2Client::GetPlayerUnit()->dwAct ==4)SetCursorPos(rect.left +160,rect.top + 250);

				BuyOilStart = true;
				if (BuyoilThread == NULL) BuyoilThread = CreateThread( NULL , 0 , BuyoilThreadFunction , NULL , 0 , NULL );
				
				d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0리뉴얼 오일 자동으로 사기") : QooMap_Notice(L"ÿc9Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc9Renewal Oil ÿc0Items To Buy...");
			}
			

			else */ CtrlAction(hWnd); // 액션임!
		}
	}

	if(uMsg == WM_LBUTTONDOWN && gameState == ClientStateInGame) {
		selectunitid=0; // 가짜 클릭 할때 사용
		myportalonoff = false;
		if (DrawSetting == true && *D2Client::MouseX < 400) return false;
		if (*D2Client::MouseX > 170 && *D2Client::MouseX < 390 &&*D2Client::MouseY < 600 && *D2Client::MouseY > 555) return false;


		if (LockClick == true){
			DWORD LevelNo = D2Common::GetRoom1LevelNo(D2Client::GetPlayerUnit()->pPath->pRoom1);

			if (!D2Common::IsLevelATown(LevelNo)) 
				if (*D2Client::MouseX > 400 && GetUIVarSTUB(UI_INVENTORY)) 
					return false;

		}
	}


	if(uMsg == WM_RBUTTONDOWN && gameState == ClientStateInGame) {
		ViewItemBasicStats = true;
		myitemview = true;//누르고 있으면 창고/큐브안에 잇는 템 정보 보여줌
	}

	if(uMsg == WM_RBUTTONUP && gameState == ClientStateInGame) {
		ViewItemBasicStats = false;
		myitemview = false; 
	
		UnitAny *pUnit = D2Client::GetPlayerUnit();

			if (pUnit->pInventory->pCursorItem != NULL && pUnit->pInventory->pCursorItem->dwTxtFileNo != 1606) { //인벤토리 10x6 , 큐브 10x8 , 창고 , 10x10

				int pInv[10][6] = {0}; //울티메티브 v5버전 인벤이 넖음
				//int pInv[10][6] = {0};
				int pCube[10][8]  = {0};
				int pStash[10][10]  = {0};
				int GotoItem_Loc = 0;
				int sizeinventory = 0;

				if (GetUIVarSTUB(UI_CUBE) || GetUIVarSTUB(UI_STASH)){
							if (*D2Client::MouseX < 400) {
								GotoItem_Loc = STORAGE_INVENTORY; //마우스가 큐브위치에 있으면  인벤토리로 템이동
								sizeinventory = 6; //울티메티브 v5버전은 8
							}
							else {
								if (GetUIVarSTUB(UI_CUBE)) {// 마우스가  인벤토리위에 잇으면 큐브로 템이동
										GotoItem_Loc = STORAGE_CUBE;
										sizeinventory = 8;
								}
								if (GetUIVarSTUB(UI_STASH)){// 마우스가 인벤토리위에 잇으면 창고로 템이동
										GotoItem_Loc = STORAGE_STASH; 
										sizeinventory = 10;
								}
							}
						
							for (UnitAny *pItem = pUnit->pInventory->pFirstItem; pItem; pItem = pItem->pItemData->pNextInvItem) {
							//for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
								if (pItem->pItemData->InventoryLocation == GotoItem_Loc){		
								
								ItemTxt *pTxt = D2Common::GetItemText(pItem->dwTxtFileNo);
								if (pItem == pUnit->pInventory->pCursorItem)
									continue;

								for (INT i = 0; i < pTxt->xSize; i++)
									for (INT j = 0; j < pTxt->ySize; j++) {
										if (GotoItem_Loc == STORAGE_INVENTORY) pInv[pItem->pItemPath->dwPosX + i][pItem->pItemPath->dwPosY + j] = 1;
										if (GotoItem_Loc == STORAGE_CUBE) pCube[pItem->pItemPath->dwPosX + i][pItem->pItemPath->dwPosY + j] = 1;
										if (GotoItem_Loc == STORAGE_STASH) pStash[pItem->pItemPath->dwPosX + i][pItem->pItemPath->dwPosY + j] = 1;
									}
						}
					}
					ItemTxt *ppickTxt = D2Common::GetItemText(pUnit->pInventory->pCursorItem->dwTxtFileNo);

						for (INT i = 0; i < 11; i++) //인벤토리 가로
						{
							for (INT j = 0; j < (sizeinventory+1); j++) //인벤토리 세로
							{
								BOOL bWorks = TRUE;

								if (GotoItem_Loc == STORAGE_INVENTORY) {if (pInv[i][j] == 1) continue;}
								if (GotoItem_Loc == STORAGE_CUBE){ if (pCube[i][j] == 1) continue;}
								if (GotoItem_Loc == STORAGE_STASH)  {if (pStash[i][j] == 1) continue;}

								for (INT x = 0; x < ppickTxt->xSize; x++)
									for (INT y = 0; y < ppickTxt->ySize; y++) {
										if (GotoItem_Loc == STORAGE_INVENTORY) if (pInv[i + x][j + y]) bWorks = FALSE;
										if (GotoItem_Loc == STORAGE_CUBE) if (pCube[i + x][j + y]) bWorks = FALSE;
										if (GotoItem_Loc == STORAGE_STASH) if (pStash[i + x][j + y]) bWorks = FALSE;								
									}

								if (!bWorks)
									continue;

								BYTE aPacket[17] = {0};
								aPacket[0] = 0x18;
								*(LPDWORD)&aPacket[1] = pUnit->pInventory->pCursorItem->dwUnitId;
								*(LPDWORD)&aPacket[5] = i;
								*(LPDWORD)&aPacket[9] = j;
								*(LPDWORD)&aPacket[13] = GotoItem_Loc; // 인벤토리
								SendGamePacket(aPacket,sizeof(aPacket));
						}
					}	
				}	
				if (GetUIVarSTUB(UI_INVENTORY)){
					if (pUnit->pInventory->pCursorItem) {
						BYTE aPacket[5];
						aPacket[0] = 0x17;
						*(LPDWORD)&aPacket[1] = pUnit->pInventory->pCursorItem->dwUnitId;
						SendGamePacket(aPacket,sizeof(aPacket));
						}
					}
			}
	}

	if(uMsg == WM_LBUTTONUP && gameState == ClientStateInGame) {
		if (DrawSetting == true && *D2Client::MouseX < 400){
			if (*D2Client::MouseX > 90 && *D2Client::MouseX < 200 && *D2Client::MouseY < 55 && *D2Client::MouseY > 40) {
				ShellExecute(NULL, "open", "iexplore.exe", "http://l0veshoo.pusku.com", "", SW_SHOW);
			}

			if (*D2Client::MouseX > 65+85 && *D2Client::MouseX < 65+95 &&*D2Client::MouseY < 95 && *D2Client::MouseY > 80) if (runenumber > 1) runenumber--;
			if (*D2Client::MouseX > 65+115 && *D2Client::MouseX < 65+130 &&*D2Client::MouseY < 95 && *D2Client::MouseY > 80) if (runenumber <33) runenumber++;
			if (*D2Client::MouseX > 65+70 && *D2Client::MouseX < 65+85 &&*D2Client::MouseY < 95 && *D2Client::MouseY > 80) {  runenumber-= 5; if (runenumber < 1) runenumber = 1;}
			if (*D2Client::MouseX > 65+130 && *D2Client::MouseX < 65+145 &&*D2Client::MouseY < 95 && *D2Client::MouseY > 80) { runenumber+= 5; if (runenumber > 33) runenumber = 33;}


			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+115 &&*D2Client::MouseY < 125 && *D2Client::MouseY > 115) if (smallcharm==false)smallcharm =  true; else smallcharm = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+135 &&*D2Client::MouseY < 135 && *D2Client::MouseY > 125) if (largecharm==false)largecharm =  true; else largecharm = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+105 &&*D2Client::MouseY < 145 && *D2Client::MouseY > 135) if (grandcharm==false)grandcharm =  true; else grandcharm = false;
			
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+120 &&*D2Client::MouseY < 175 && *D2Client::MouseY > 165) if (healingpotion==false)healingpotion =  true; else healingpotion = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+140 &&*D2Client::MouseY < 185 && *D2Client::MouseY > 175) if (manapotion==false)manapotion =  true; else manapotion = false;

			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+120 &&*D2Client::MouseY < 215 && *D2Client::MouseY > 205) if (socket3==false)socket3 =  true; else socket3 = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+140 &&*D2Client::MouseY < 225 && *D2Client::MouseY > 215) if (socket4==false)socket4 =  true; else socket4 = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+120 &&*D2Client::MouseY < 235 && *D2Client::MouseY > 225) if (socket5==false)socket5 =  true; else socket5 = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+140 &&*D2Client::MouseY < 245 && *D2Client::MouseY > 235) if (socket6==false)socket6 =  true; else socket6 = false;
			/*
			if (*D2Client::MouseX > 65+25 && *D2Client::MouseX < 65+60 &&*D2Client::MouseY < 110 && *D2Client::MouseY > 100) if (AllShrine==false) {
				CreepyShrine =  true;AbandonedShrine =  true;QuietShrine =  true; EerieShrine =  true; WeirdShrine =  true;  GloomyShrine =  true;
				TaintedShrine =  true; FascinatingShrine =  true; OrnateShrine =  true; SacredShrine =  true;ShimmeringShrine =  true;SpiritualShrine =  true;
				MagicalShrine =  true;HiddenShrine =  true; StoneShrine =  true; AllShrine = true;} 
			else {CreepyShrine =  false;AbandonedShrine =  false;QuietShrine =  false; EerieShrine =  false; WeirdShrine =  false;  GloomyShrine =  false;
				TaintedShrine =  false; FascinatingShrine = false; OrnateShrine =  false; SacredShrine =  false;ShimmeringShrine =  false;SpiritualShrine =  false;
				MagicalShrine =  false;HiddenShrine =  false; StoneShrine =  false; AllShrine = false;} 
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+115 &&*D2Client::MouseY < 120 && *D2Client::MouseY > 110) if (CreepyShrine==false)CreepyShrine =  true; else CreepyShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+135 &&*D2Client::MouseY < 130 && *D2Client::MouseY > 120) if (AbandonedShrine==false)AbandonedShrine =  true; else AbandonedShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+105 &&*D2Client::MouseY < 140 && *D2Client::MouseY > 130) if (QuietShrine==false)QuietShrine =  true; else QuietShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+105 &&*D2Client::MouseY < 150 && *D2Client::MouseY > 140) if (EerieShrine==false)EerieShrine =  true; else EerieShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+110 &&*D2Client::MouseY < 160 && *D2Client::MouseY > 150) if (WeirdShrine==false)WeirdShrine =  true; else WeirdShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+115 &&*D2Client::MouseY < 170 && *D2Client::MouseY > 160) if (GloomyShrine==false)GloomyShrine =  true; else GloomyShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+120 &&*D2Client::MouseY < 180 && *D2Client::MouseY > 170) if (TaintedShrine==false)TaintedShrine =  true; else TaintedShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+140 &&*D2Client::MouseY < 190 && *D2Client::MouseY > 180) if (FascinatingShrine==false)FascinatingShrine =  true; else FascinatingShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+115 &&*D2Client::MouseY < 200 && *D2Client::MouseY > 190) if (OrnateShrine==false)OrnateShrine =  true; else OrnateShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+110 &&*D2Client::MouseY < 210 && *D2Client::MouseY > 200) if (SacredShrine==false)SacredShrine =  true; else SacredShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+140 &&*D2Client::MouseY < 220 && *D2Client::MouseY > 210) if (ShimmeringShrine==false)ShimmeringShrine =  true; else ShimmeringShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+125 &&*D2Client::MouseY < 230 && *D2Client::MouseY > 220) if (SpiritualShrine==false)SpiritualShrine =  true; else SpiritualShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+120 &&*D2Client::MouseY < 240 && *D2Client::MouseY > 230) if (MagicalShrine==false)MagicalShrine =  true; else MagicalShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+115 &&*D2Client::MouseY < 250 && *D2Client::MouseY > 240) if (HiddenShrine==false)HiddenShrine =  true; else HiddenShrine = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+110 &&*D2Client::MouseY < 260 && *D2Client::MouseY > 250) if (StoneShrine==false)StoneShrine =  true; else StoneShrine = false;

			if (*D2Client::MouseX > 65+25 && *D2Client::MouseX < 65+110 &&*D2Client::MouseY < 280 && *D2Client::MouseY > 270) {if (AllGems == false) {
				 PerfectAmethyst = true;PerfectTopaz = true;PerfectSapphire = true;PerfectEmerald = true;PerfectRuby = true;PerfectDiamond = true;
				 PerfectSkull = true;PerfectRainbowStone = true;PerfectOnyx = true;PerfectBloodStone = true;PerfectTurquoise = true;PerfectAmber = true; AllGems = true;} 
				else {PerfectAmethyst = false;PerfectTopaz = false;PerfectSaphire = false;PerfectEmerald = false;PerfectRuby = false;PerfectDiamond = false;
									PerfectSkull = false;PerfectRainbowStone = false;PerfectOnyx = false;PerfectBloodStone = false;PerfectTurquoise = false;PerfectAmber = false; AllGems = false;}
			}
			*/

			if (*D2Client::MouseX > 65+25 && *D2Client::MouseX < 65+110 &&*D2Client::MouseY < 260 && *D2Client::MouseY > 250) {if (AllGems == false) {
				 PerfectAmethyst = true;PerfectTopaz = true;PerfectSapphire = true;PerfectEmerald = true;PerfectRuby = true;PerfectDiamond = true;
				 PerfectSkull = true; PerfectGhostly=true; AllGems = true;} 
				else {PerfectAmethyst = false;PerfectTopaz = false;PerfectSapphire = false;PerfectEmerald = false;PerfectRuby = false;PerfectDiamond = false;
									PerfectSkull = false;PerfectGhostly = false; AllGems = false;}
			}
			
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+135 &&*D2Client::MouseY < 275 && *D2Client::MouseY > 265) if (PerfectAmethyst==false)PerfectAmethyst =  true; else PerfectAmethyst = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+115 &&*D2Client::MouseY < 285 && *D2Client::MouseY > 275) if (PerfectTopaz==false)PerfectTopaz =  true; else PerfectTopaz = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+125 &&*D2Client::MouseY < 295 && *D2Client::MouseY > 285) if (PerfectSapphire==false)PerfectSapphire =  true; else PerfectSapphire = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+130 &&*D2Client::MouseY < 305 && *D2Client::MouseY > 295) if (PerfectDiamond==false)PerfectDiamond =  true; else PerfectDiamond = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+130 &&*D2Client::MouseY < 315 && *D2Client::MouseY > 305) if (PerfectEmerald==false)PerfectEmerald =  true; else PerfectEmerald = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+110 &&*D2Client::MouseY < 325 && *D2Client::MouseY > 315) if (PerfectRuby==false)PerfectRuby =  true; else PerfectRuby = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+110 &&*D2Client::MouseY < 335 && *D2Client::MouseY > 325) if (PerfectSkull==false)PerfectSkull =  true; else PerfectSkull = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+160 &&*D2Client::MouseY < 345 && *D2Client::MouseY > 335) if (PerfectGhostly==false)PerfectGhostly =  true; else PerfectGhostly = false;
			
			/*
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+160 &&*D2Client::MouseY < 360 && *D2Client::MouseY > 350) if (PerfectRainbowStone==false)PerfectRainbowStone =  true; else PerfectRainbowStone = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+110 &&*D2Client::MouseY < 370 && *D2Client::MouseY > 360) if (PerfectOnyx==false)PerfectOnyx =  true; else PerfectOnyx = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+140 &&*D2Client::MouseY < 380 && *D2Client::MouseY > 370) if (PerfectBloodStone==false)PerfectBloodStone =  true; else PerfectBloodStone = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+135 &&*D2Client::MouseY < 390 && *D2Client::MouseY > 380) if (PerfectTurquoise==false)PerfectTurquoise =  true; else PerfectTurquoise = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+115 &&*D2Client::MouseY < 400 && *D2Client::MouseY > 390) if (PerfectAmber==false)PerfectAmber =  true; else PerfectAmber = false;
			*/
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+145 &&*D2Client::MouseY < 375 && *D2Client::MouseY > 365) if (viewCorpse == false) viewCorpse= true; else viewCorpse = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+145 &&*D2Client::MouseY < 385 && *D2Client::MouseY > 375) if (SellPriceCheck == false) SellPriceCheck= true; else SellPriceCheck = false;

			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+110 &&*D2Client::MouseY < 430 && *D2Client::MouseY > 420) if (ViewJewelMagic==false)ViewJewelMagic =  true; else ViewJewelMagic = false;
			if (*D2Client::MouseX > 65+115 && *D2Client::MouseX < 65+180 &&*D2Client::MouseY < 430 && *D2Client::MouseY > 420)  if (ViewJewelRare==false)ViewJewelRare =  true; else ViewJewelRare = false;

			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 65+110 &&*D2Client::MouseY < 440 && *D2Client::MouseY > 430) if (RareAmulet==false)RareAmulet =  true; else RareAmulet = false;
			if (*D2Client::MouseX > 65+115 && *D2Client::MouseX < 65+180 &&*D2Client::MouseY < 440 && *D2Client::MouseY > 430) if (RareRing==false)RareRing =  true; else RareRing = false;
			if (*D2Client::MouseX > 105 && *D2Client::MouseX < 185 &&*D2Client::MouseY < 480 && *D2Client::MouseY > 470) if (QooMapChicken==true){QooMapChickenTown =  true; QooMapChickenLeftGame = false;}
			if (*D2Client::MouseX > 188 && *D2Client::MouseX < 320 &&*D2Client::MouseY < 480 && *D2Client::MouseY > 470) if (QooMapChicken==true){QooMapChickenLeftGame = true; QooMapChickenTown =  false;}
			if (*D2Client::MouseX > 190 && *D2Client::MouseX < 200 &&*D2Client::MouseY < 470 && *D2Client::MouseY > 460) if (QooMapChickenLifePercent> 20) QooMapChickenLifePercent -= 10;
			if (*D2Client::MouseX > 230 && *D2Client::MouseX < 240 &&*D2Client::MouseY < 470 && *D2Client::MouseY > 460) if (QooMapChickenLifePercent< 90) QooMapChickenLifePercent += 10;

			if (*D2Client::MouseX > 345 && *D2Client::MouseX < 395 &&*D2Client::MouseY < 520 && *D2Client::MouseY > 500) {
				D2Client::SetUIVar(UI_QUEST,1,0);
				DrawSetting = false;
				
				char mytemp[10] = ""; 
				WritePrivateProfileString("MapHack","RuneNumber",itoa(runenumber,mytemp,10),pFile);
				WritePrivateProfileString("MapHack","Rare Amulet",BoolToSTring(RareAmulet),pFile);
				WritePrivateProfileString("MapHack","Magic Jewel",BoolToSTring(ViewJewelMagic),pFile);
				WritePrivateProfileString("MapHack","Rare Jewel",BoolToSTring(ViewJewelRare),pFile);
				WritePrivateProfileString("MapHack","RareRing",BoolToSTring(RareRing),pFile);
				WritePrivateProfileString("MapHack","HealingPotion",BoolToSTring(healingpotion),pFile);
				WritePrivateProfileString("MapHack","ManaPotion",BoolToSTring(manapotion),pFile);
				WritePrivateProfileString("MapHack","Perfect Amethyst",BoolToSTring(PerfectAmethyst),pFile);
				WritePrivateProfileString("MapHack","Perfect Topaz",BoolToSTring(PerfectTopaz),pFile);
				WritePrivateProfileString("MapHack","Perfect Sapphire",BoolToSTring(PerfectSapphire),pFile);
				WritePrivateProfileString("MapHack","Perfect Diamond",BoolToSTring(PerfectDiamond),pFile);
				WritePrivateProfileString("MapHack","Perfect Emerald",BoolToSTring(PerfectEmerald),pFile);
				WritePrivateProfileString("MapHack","Perfect Ruby",BoolToSTring(PerfectRuby),pFile);
				WritePrivateProfileString("MapHack","SellPrice",BoolToSTring(SellPriceCheck),pFile);
				WritePrivateProfileString("MapHack","Socket3",BoolToSTring(socket3),pFile);
				WritePrivateProfileString("MapHack","Socket4",BoolToSTring(socket4),pFile);
				WritePrivateProfileString("MapHack","Socket5",BoolToSTring(socket5),pFile);
				WritePrivateProfileString("MapHack","Socket6",BoolToSTring(socket6),pFile);
				WritePrivateProfileString("MapHack","Perfect Skull",BoolToSTring(PerfectSkull),pFile);
				WritePrivateProfileString("MapHack","Perfect Ghostly",BoolToSTring(PerfectGhostly),pFile);
				/*
				WritePrivateProfileString("MapHack","Perfect Rainbowstone",BoolToSTring(PerfectRainbowStone),pFile);
				WritePrivateProfileString("MapHack","Perfect Onyx",BoolToSTring(PerfectOnyx),pFile);
				WritePrivateProfileString("MapHack","Perfect Bloodstone",BoolToSTring(PerfectBloodStone),pFile);
				WritePrivateProfileString("MapHack","Perfect Turquoise",BoolToSTring(PerfectTurquoise),pFile);
				WritePrivateProfileString("MapHack","Perfect Amber",BoolToSTring(PerfectAmber),pFile);

				WritePrivateProfileString("MapHack","Creepy shrine",BoolToSTring(CreepyShrine),pFile);
				WritePrivateProfileString("MapHack","Abandoned Shrine",BoolToSTring(AbandonedShrine),pFile);
				WritePrivateProfileString("MapHack","Quiet Shrine",BoolToSTring(QuietShrine),pFile);
				WritePrivateProfileString("MapHack","Eerie Shrine",BoolToSTring(EerieShrine),pFile);
				WritePrivateProfileString("MapHack","Weird Shrine",BoolToSTring(WeirdShrine),pFile);
				WritePrivateProfileString("MapHack","Gloomy Shrine",BoolToSTring(GloomyShrine),pFile);
				WritePrivateProfileString("MapHack","Tainted Shrine",BoolToSTring(TaintedShrine),pFile);
				WritePrivateProfileString("MapHack","Fascinating Shrine",BoolToSTring(FascinatingShrine),pFile);
				WritePrivateProfileString("MapHack","Ornate Shrine",BoolToSTring(OrnateShrine),pFile);
				WritePrivateProfileString("MapHack","Sacred Shrine",BoolToSTring(SacredShrine),pFile);
				WritePrivateProfileString("MapHack","Shimmering Shrine",BoolToSTring(ShimmeringShrine),pFile);
				WritePrivateProfileString("MapHack","Spiritual Shrine",BoolToSTring(SpiritualShrine),pFile);
				WritePrivateProfileString("MapHack","Magical Shrine",BoolToSTring(MagicalShrine),pFile);
				WritePrivateProfileString("MapHack","Hidden Shrine",BoolToSTring(HiddenShrine),pFile);
				WritePrivateProfileString("MapHack","Stone Shrine",BoolToSTring(StoneShrine),pFile);
				*/
				WritePrivateProfileString("MapHack","EnemyCorpse",BoolToSTring(viewCorpse),pFile);

				WritePrivateProfileString("MapHack","ChickenTown",BoolToSTring(QooMapChickenTown),pFile);
				WritePrivateProfileString("MapHack","ChickenLeftGame",BoolToSTring(QooMapChickenLeftGame),pFile);
				WritePrivateProfileString("MapHack","ChickenLifePercent",itoa(QooMapChickenLifePercent,mytemp,10),pFile);

				d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0세팅 저장 ÿc1완료") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Setting Save ÿc1OK");
			}
		}

		if(GetUIVarSTUB(UI_CHARACTER)){

				if (*D2Client::MouseX > 125 && *D2Client::MouseX < 210 &&*D2Client::MouseY < 520 && *D2Client::MouseY > 500) {
					if (PlayerBuff == false) {
						ChangeStat(STAT_IAS, 255);
						ChangeStat(STAT_FASTERBLOCK,255);
						ChangeStat(STAT_FASTERCAST,255);
						ChangeStat(STAT_FASTERHITRECOVERY,255);
						QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Buff (increase ias/fhr/fcr/fbr) ÿc1On");
						PlayerBuff = true;
					}
					else{
						ChangeStat(STAT_IAS, 0);
						ChangeStat(STAT_FASTERBLOCK,0);
						ChangeStat(STAT_FASTERCAST,0);
						ChangeStat(STAT_FASTERHITRECOVERY,0);
						//ChangeStat(STAT_FASTERRUNWALK,0);
						QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Buff ÿc1Off");
						PlayerBuff = false;
					}
				}
				
				if (*D2Client::MouseX > 270 && *D2Client::MouseX < 395 && *D2Client::MouseY > 495 &&*D2Client::MouseY < (unsigned)(d2res_extend_height+ 515))
					if(viewplayerstate == 0){ //첫페이지 
						viewplayerstate = 1;
						return 0;
					}
				if (*D2Client::MouseX > 345 && *D2Client::MouseX < 395 && *D2Client::MouseY > 495 &&*D2Client::MouseY < (unsigned)(d2res_extend_height+ 520)){
					if(viewplayerstate == 1) // 두번째페이지
						viewplayerstate = 2;
					//else if (viewplayerstate == 2) viewplayerstate =3;
					else viewplayerstate = 0;
				}
		}

		if(GetUIVarSTUB(UI_INVENTORY)){
			if (*D2Client::MouseX > 610 && *D2Client::MouseX < 710 && *D2Client::MouseY > 480 &&*D2Client::MouseY < (unsigned)(d2res_extend_height+ 490)) {
					if (AutoEatCorpse == false) {
						AutoEatCorpse = true; 
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0시체 자동 회수 ÿc1ON") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Auto Eat Corpse ÿc1ON");
						//D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0시체 자동 회수 ON", 1);
					}
					else {
					   AutoEatCorpse = false;  
					   d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0시체 자동 회수 ÿc1Off") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Auto Eat Corpse ÿc1Off");
					   //D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0시체 자동 회수 OFF", 1);
					}

					WritePrivateProfileString("MapHack","AutoEatCorpse",BoolToSTring(AutoEatCorpse),pFile);
				}

			if (*D2Client::MouseX > 610 && *D2Client::MouseX < 710 && *D2Client::MouseY > 460 &&*D2Client::MouseY < (unsigned)(d2res_extend_height+ 475)){
					if (LockClick == false)  LockClick = true;
					else LockClick= false;
			}
		}


		if(GetUIVarSTUB(UI_PARTY)){
			if (*D2Client::MouseX > 90 && *D2Client::MouseX < 150 && *D2Client::MouseY > 460 &&*D2Client::MouseY < (unsigned)(d2res_extend_height+ 480)){
				 if (pAutoParty == false){
					 pAutoParty = true;
					 d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0자동 파티 ÿc1ON") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Auto Party ÿc1ON");
				 }
				 else{
					 d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0자동 파티 ÿc1Off") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Auto Party ÿc1OFF");
					 pAutoParty = false;

				 }
				 WritePrivateProfileString("MapHack","AutoParty",BoolToSTring(pAutoParty),pFile);
			}
		}

		/*
		if(GetUIVarSTUB(UI_CUBE)){
			if (*D2Client::MouseX > 240 && *D2Client::MouseX < 350 && *D2Client::MouseY > 500 &&*D2Client::MouseY < (unsigned)(d2res_extend_height+ 515)){
			   if (AutoTransmute == false) {
				   AutoTransmute = true;
				   QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Auto Transmute ÿc1ON");
			   }
			   else {
				   AutoTransmute = false;
			       QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Auto Transmute ÿc1OFF");
			   }

				WritePrivateProfileString("MapHack","AutoTransmute",BoolToSTring(AutoTransmute),pFile);
			}
		}
		

		if(GetUIVarSTUB(UI_STASH)){ // save html file
			if (*D2Client::MouseX > 150 && *D2Client::MouseX < 320 && *D2Client::MouseY > 500 &&*D2Client::MouseY < (unsigned)(d2res_extend_height+ 515)){
			   			if (SaveItemThread == NULL) SaveItemThread = CreateThread( NULL , 0 , SaveItemThreadFunction , NULL , 0 , NULL );
			}
		}
		*/
		if (DrawSetting == false){
			if(GetUIVarSTUB(UI_QUEST)){
				if (*D2Client::MouseX > 90 && *D2Client::MouseX < 180 && *D2Client::MouseY > 460 &&*D2Client::MouseY < (unsigned)(d2res_extend_height+ 480)){
					 if (UberQuest == true){
						  d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0우버 퀘스트 정보 ÿc1Off") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0UberQuest Info ÿc1OFF");
						 UberQuest = false;
					 }
					 else{
						 UberQuest = true;
						 d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0우버 퀘스트 정보 ÿc1ON") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0UberQuest Info ÿc1ON");
					 }
					 WritePrivateProfileString("MapHack","UberQuest",BoolToSTring(UberQuest),pFile);
				}
			}
		}

		if (*D2Client::MouseY > unsigned(d2res_extend_height + 550)) {
			if (!GetUIVarSTUB(UI_CUBE) || !GetUIVarSTUB(UI_STASH)) {
				if (*D2Client::MouseX > 170 && *D2Client::MouseX < 270 && *D2Client::MouseY < (unsigned)(d2res_extend_height+ 575) && *D2Client::MouseY > (unsigned)(d2res_extend_height+ 555)) { 
					CHAR iniFileName[256];

					if (MenuAllUnique == 0) {
						OnlyUniqueOK = true;
						wsprintfA(GetShrinesFolder(iniFileName, '\\'), "allunique.bmp");
						MenuAllUniqueItem = 0x53;
						//wsprintfW(MenuAllUniqueItem,L"4 All Unique ");
						char mytemp[10] = "";
						MenuAllUnique = 1;
						WritePrivateProfileString("MapHack","UniqueOnly",itoa(MenuAllUnique,mytemp,10),pFile);

					}
					else {
						wsprintfA(GetShrinesFolder(iniFileName, '\\'), "allitem.bmp");
						MenuAllUniqueItem = 0x13; //0x13 회색
						OnlyUniqueOK = false;
						char mytemp[10] = "";
						MenuAllUnique = 0; //초기화
						WritePrivateProfileString("MapHack","UniqueOnly",itoa(MenuAllUnique,mytemp,10),pFile);
	
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0모든 아이템 보기") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0All Item");
						//D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0모든 아이템 보기", 1);
					}

					/*
					if (MenuAllUnique == 0) {
						wsprintfA(GetShrinesFolder(iniFileName, '\\'), "sacred.bmp");
						MenuAllUniqueItem = 0x53; // 앞에 숫자는 색코드 maphack.cpp에서 사용
						UniqueItem_Off_TierItem = true;
						OnlyUniqueOK = true;
						char mytemp[10] = "";
						WritePrivateProfileString("MapHack","UniqueOnly",itoa(MenuAllUnique,mytemp,10),pFile);
						
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc4유니크ÿc0/ÿc2세트ÿc0/ÿc8미스틱 오브 (유닉)ÿc0/ÿc;룬 (그레이트 룬)ÿc0/ÿc8트로피ÿc0 보기") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc4Uniqueÿc0/ÿc2Setÿc0/ÿc8Mystic Orb (Unique)ÿc0/ÿc;Rune (Great Rune)ÿc0/ÿc8Tropyÿc0 ÿc1ON");
						//D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc4유니크ÿc0/ÿc2세트ÿc0/ÿc8미스틱 오브 (유닉)ÿc0/ÿc;룬 (그레이트 룬)ÿc0/ÿc8트로피ÿc0 보기", 1);
						MenuAllUnique = 1;
					}
					else if (MenuAllUnique == 1) {
						wsprintfA(GetShrinesFolder(iniFileName, '\\'), "allunique.bmp");
						MenuAllUniqueItem = 0x53;
						//wsprintfW(MenuAllUniqueItem,L"4 All Unique ");
						char mytemp[10] = "";
						WritePrivateProfileString("MapHack","UniqueOnly",itoa(MenuAllUnique,mytemp,10),pFile);
						MenuAllUnique = 2;
						}
					else if (MenuAllUnique == 2) {
						wsprintfA(GetShrinesFolder(iniFileName, '\\'), "tieroff.bmp");
						MenuAllUniqueItem = 0x6c; //노랑색
						//wsprintfW(MenuAllUniqueItem,L"9 TierItem Off ");
						OnlyUniqueOK = false;
						UniqueItem_Off_TierItem = false;
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0티어ÿc9[ÿc3템이름(1) ~ 템이름(6)ÿc9]ÿc0 아이템 ÿc1끄기") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Tierÿc9[ÿc3ItemName(1) ~ ItemName(6)ÿc9]ÿc0 Item ÿc1Clear");
						char mytemp[10] = "";
						WritePrivateProfileString("MapHack","UniqueOnly",itoa(MenuAllUnique,mytemp,10),pFile);
						MenuAllUnique = 3;
						}	
					else {
						wsprintfA(GetShrinesFolder(iniFileName, '\\'), "allitem.bmp");
						MenuAllUniqueItem = 0x13; //0x13 회색
						OnlyUniqueOK = false;
						UniqueItem_Off_TierItem = false;
						char mytemp[10] = "";
						WritePrivateProfileString("MapHack","UniqueOnly",itoa(MenuAllUnique,mytemp,10),pFile);
						MenuAllUnique = 0; //초기화
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0모든 아이템 보기") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0All Item");
						//D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0모든 아이템 보기", 1);
					}
					*/

					ItemConfCellFile = InitCellFile(LoadBMPCellFile(iniFileName)); //아이템 위치
				}
				if (*D2Client::MouseX > 320 && *D2Client::MouseX < 410 && *D2Client::MouseY < (unsigned)(d2res_extend_height+ 575) && *D2Client::MouseY > (unsigned)(d2res_extend_height+ 565)) { 
					if (EffectView == false) {
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0미사일 안보이게 ÿc1설정") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Hide Missile Effect ÿc1ON");
						EffectView = true; //D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0시체 안보이게 설정 ", 1);
					}
					else {
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0미사일 보이게 ÿc1설정") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Hide Missile Effect ÿc1Off");
						EffectView = false; //D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0시체 보이게 설정 ", 1);
					}

					WritePrivateProfileString("MapHack","Effect",BoolToSTring(EffectView),pFile);
				}

				if (*D2Client::MouseX > 170 && *D2Client::MouseX < 232 && *D2Client::MouseY < (unsigned)(d2res_extend_height+ 585) && *D2Client::MouseY > (unsigned)(d2res_extend_height+ 575)) {
					if (ScrollMapON == false) {
						QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Scroll Map (Beta) ÿc1ON") ;
						ScrollMapON = true; //D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0ScrollMap ÿc1Beta ÿc0ON! ÿc9(지진이 일어 나면 정상 작동)",1);
					}
					else {
						QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Scroll Map ÿc1Off");
						ScrollMapON = false; //D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0ScrollMap OFF",1); ScrollMap(0,0);
					}
				}
				if (*D2Client::MouseX > 245 && *D2Client::MouseX < 300 && *D2Client::MouseY < (unsigned)(d2res_extend_height+ 585) && *D2Client::MouseY > (unsigned)(d2res_extend_height+ 575)) { 
					if (QooMapChicken == false) {
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0치킨 ÿc1설정") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Chicken ÿc1ON");
						QooMapChicken = true; //D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0시체 안보이게 설정 ", 1);
						QooMapChickenTown =  true;
					}
					else {
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0치킨 ÿc1해제") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Chicken ÿc1Off");
						QooMapChicken = false; //D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0시체 보이게 설정 ", 1);
						QooMapChickenTown =  false;
					}

					WritePrivateProfileString("MapHack","Chicken",BoolToSTring(QooMapChicken),pFile);
				}
				if (*D2Client::MouseX > 312 && *D2Client::MouseX < 360 && *D2Client::MouseY < (unsigned)(d2res_extend_height+ 585) && *D2Client::MouseY > (unsigned)(d2res_extend_height+ 575)) { 
					if (UserAutomapname == false) {
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0오토맵 이름 ÿc1보기") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Automap Name ÿc1ON");
						UserAutomapname = true; //D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0오토맵 이름 보기",1);
					}
					else {
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0오토맵 이름 ÿc1끄기") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Automap Name ÿc1Off");
						UserAutomapname = false; //D2Client::PrintGameString(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0오토맵 이름 끄기",1);
					}

					WritePrivateProfileString("MapHack","MapName",BoolToSTring(UserAutomapname),pFile);
				}

				if (*D2Client::MouseX > 170 && *D2Client::MouseX < 250 && *D2Client::MouseY < (unsigned)(d2res_extend_height+ 595) && *D2Client::MouseY > (unsigned)(d2res_extend_height+ 585))  {
					if (DropNotify == false){ 
						DropNotify = true;
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0아이템 드랍 표시 ÿc1켜기") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Drop Notify ÿc1ON");
					}
					else {
						DropNotify = false;
						d2locale_kor? QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0아이템 드랍 표시 ÿc1끄기") : QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Drop Notify ÿc1Off");
					}
					WritePrivateProfileString("MapHack","DropNotify",BoolToSTring(DropNotify),pFile);
				}

				if (*D2Client::MouseX > 255 && *D2Client::MouseX < 290 && *D2Client::MouseY < (unsigned)(d2res_extend_height+ 595) && *D2Client::MouseY > (unsigned)(d2res_extend_height+ 585))  {
					if (GetSystemMetrics(SM_CXSCREEN) > 1000){// 폴화면이 아닐경우
						if (d2extend_ok ==false) { 
						QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0High Resolution Mode (1344x700) ÿc1On");
						NewResolution(1344,700); //	
						d2gfx_10060(3);
						d2res_extend_width = 1344 - 800;
						d2res_extend_height = 100;
						D2Client::SetUIVar(UI_CHARACTER,0,0);
						D2Client::SetUIVar(UI_CHARACTER,1,0);
						d2extend_ok = true;
					  }
					  else {
						QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0High Resolution Mode ÿc1Off");
						NewResolution(800,600); //	
						d2gfx_10060(2);
						d2res_extend_width = 0;
						d2res_extend_height = 0;
						D2Client::SetUIVar(UI_CHARACTER,0,0);
						D2Client::SetUIVar(UI_CHARACTER,1,0);
						d2extend_ok = false;
					  }
					}else {
						QooMap_Notice(L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : ÿc0Run after switching to window mode");
					}
				}

				if (*D2Client::MouseX > 430 && *D2Client::MouseX < 470 && *D2Client::MouseY < (unsigned)(d2res_extend_height+ 600) && *D2Client::MouseY > (unsigned)(d2res_extend_height+ 590)) { 
						if (ClickGotoBeltThread == NULL) ClickGotoBeltThread = CreateThread( NULL , 0 , ClickGotoBeltThreadFunction , NULL , 0 , NULL );
				}

				if (*D2Client::MouseX > 300 && *D2Client::MouseX < 360 && *D2Client::MouseY < (unsigned)(d2res_extend_height+ 595) && *D2Client::MouseY > (unsigned)(d2res_extend_height+ 585))  {
					DrawSetting = true;
					//ShowWindow(WindowTitlehWnd,SW_SHOW);
				}
			}
		}

		}
	
	return (LONG)CallWindowProcA(OldWndProc, hWnd, uMsg, wParam, lParam);
}

RosterUnit *FindPartyByName(LPSTR szName)
{
	for(RosterUnit *pUnit = *D2Client::PlayerUnitList; pUnit; pUnit = pUnit->pNext)
		if(!_stricmp(szName, pUnit->szName))
			return pUnit;

	return NULL;
}

BYTE TakeCorpse[9] = {0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 시체 자동 회수
DWORD __fastcall HandleGamePacketReceived(BYTE* pPacket, DWORD dwSize)
	{
		if (!pPacket) return TRUE;
		UnitAny *pPlayer= D2Client::GetPlayerUnit();

		switch(pPacket[0]) {
			case 0x2A: { //NPC Transaction   2a [BYTE Trade Type] [BYTE Result? - 0x00 =  Purchased || 0x01 = Sold || 0x0c = Insuffecient Gold] [DWORD Unknown] [DWORD  Merchandise Id] [DWORD Gold in Inventory]
				//if (pPacket[1] == 0x04) { // [BYTE Trade Type]  4는 buy
				//		D2Client::PrintGameString(L"insert",1);
				//	}

				if (pPacket[1] == 0x00) { // 0은 공간없음
						BuyOilStart = false;
				}
		
				break;
			 }

			case 0x1D: { //캐릭 정보 얻기 값 255 밑
				if (pPacket[2] < 256) {
					if (pPacket[1] == 0) baseStr = pPacket[2]; //0은 힘
					if (pPacket[1] == 2) baseDex = pPacket[2];
					if (pPacket[1] == 3) baseVit = pPacket[2];
					if (pPacket[1] == 1) baseEne = pPacket[2]; //0은 힘
				}
				break;
			 }

			case 0x1E: { //캐릭 정보 얻기
				if (*(WORD*)&pPacket[2] > 255){
					if (pPacket[1] == 0) baseStr = *(WORD*)&pPacket[2]; //0은 힘
					if (pPacket[1] == 2) baseDex = *(WORD*)&pPacket[2];
					if (pPacket[1] == 3) baseVit = *(WORD*)&pPacket[2];
					if (pPacket[1] == 1) baseEne = *(WORD*)&pPacket[2]; //0은 힘
				}
				break;
			 }

			case 0x60: { //60 [BYTE State] [BYTE Area Id] [DWORD Unit Id]

				if (FastTownPortal == true) {
					FastTownPortal = false;
					
				if (!D2Common::IsLevelATown(pPlayer->pAct->pRoom1->pRoom2->pLevel->dwLevelNo)){
						BYTE TakeTP[9] = {0x13, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
						*(DWORD*)&TakeTP[5] = *(DWORD*)&pPacket[3];
						SendGamePacket(TakeTP, sizeof(TakeTP));			
					}
				}
				break;
					   }
			case 0x06: {
					inGame = false;

				break;
				}
			case 0x5A:{
				if (pPacket[1] == 0x02){ // 조인 메세지
					RosterUnit *Unit = FindPartyByName((LPSTR)&pPacket[8]);

					if (!Unit || Unit->dwUnitId == D2Client::GetPlayerUnit()->dwUnitId)
						return TRUE;
					
					wchar_t joinmessage[256] = L"";
					char *chartype[7] = {"Amazon","Sorceress","NecroMancer","Paladin","BarBarian","Druid","Assasin"};

					//%s 와 %S 주의   %s는 글자 그대로 입력받고 %S는 반대 wchar_t 라면 char로 입력받음
					wsprintfW(joinmessage,L"ÿc4Lv%d - ÿc0%Sÿc4 [ÿc9%Sÿc4] (%S) has joined our world.", Unit->wLevel,(LPSTR)&pPacket[8],chartype[Unit->dwClassId] ,(LPSTR)&pPacket[24]);
					D2Client::PrintGameString(joinmessage,0);
					return FALSE;}

				if (pPacket[1] == 0x03){ // leftgame 메세지
					wchar_t leftmessage[256] = L"";
					
					wsprintfW(leftmessage,L"ÿc0%S ÿc4(%S) left our world.", (LPSTR)&pPacket[8],(LPSTR)&pPacket[24]);
					D2Client::PrintGameString(leftmessage,0);
					return FALSE;}

				if (pPacket[1] == 0x08){ // busy 메세지
					wchar_t busymessage[256] = L"";
					//%s 와 %S 주의   %s는 글자 그대로 입력받고 %S는 반대 wchar_t 라면 char로 입력받음
					wsprintfW(busymessage,L"ÿc0%S ÿc4is busy.", (LPSTR)&pPacket[8]);
					D2Client::PrintGameString(busymessage,0);
					return FALSE;}

				break;

			}
			case 0x74:{   // 74 [BYTE Assign 0x00 = False || 0x01 True] [DWORD Owner Id] [DWORD Corpse Id]
				
					if (DieOK == true && D2Common::IsLevelATown(D2Common::GetRoom1LevelNo(D2Client::GetPlayerUnit()->pPath->pRoom1))){
								*(DWORD*)&TakeCorpse[5] = *(DWORD*)&pPacket[6];
								playercorpsex = pPlayer->pPath->xPos;
								playercorpsey = pPlayer->pPath->yPos;
					}

					if (DieOK == false && !D2Common::IsLevelATown(D2Common::GetRoom1LevelNo(D2Client::GetPlayerUnit()->pPath->pRoom1))){
						*(DWORD*)&TakeCorpse[5] = *(DWORD*)&pPacket[6];	
						playercorpsex = pPlayer->pPath->xPos;
						playercorpsey = pPlayer->pPath->yPos;
						DieOK = true;
					} //이렇게 하는이유// 재접시 시체 찾지 못함

					break;
				 }
			case 0x5B: {// player in game  = 5b [WORD Packet Length] [DWORD Player Id] [BYTE Char Type]
					if(pPacket[1] == 0x24 && *(DWORD*)&pPacket[3] == D2Client::GetPlayerUnit()->dwUnitId) {
						joingame = true;

						if (PlayerBuff == true) {
						ChangeStat(STAT_IAS, 255);
						ChangeStat(STAT_FASTERBLOCK,255);
						ChangeStat(STAT_FASTERCAST,255);
						ChangeStat(STAT_FASTERHITRECOVERY,255);
						//ChangeStat(STAT_FASTERRUNWALK,255);
						}

						if (d2extend_ok) {
							d2extend_ok = true;
							d2extend_test = true;
						}
					}


					break;
				}
			case 0xA7: {
				if(pPacket[6] == 102) return FALSE; // 포탈 딜레이 없애기   102 = AFFECT_JUST_PORTALED 
				if(pPacket[6] == 92) return FALSE; // 데스 딜레이   102 = AFFECT_JUST_PORTALED
				break;
				}
			case 0xA8: { // Set STATE
				if(pPacket[7] == 25) {
					//if (Dragon_RareRenwal == false) Dragon_RareRenwal = true;
				}
				break;
				}
			case 0xA9: {  // END STATE
				if(pPacket[6] == 25) {
					//if (Dragon_RareRenwal == true) Dragon_RareRenwal = false;
				}
				break;
				}
	} //switch(pPacket[0]) end
	return TRUE;
}

void SendGamePacket(unsigned char* Packet, unsigned PacketSize)
	{
		unsigned size1 = PacketSize;
		__asm {
			pushad
				mov eax, Packet 
				push Packet		
				mov ebx, size1
				mov edx, D2Client::SendGamePacket_I
				call edx
				popad
		}
	}

__declspec(naked) void GamePacketReceived_I(void)
	{
		__asm {
			pop ebp
			pushad

			call HandleGamePacketReceived
			test eax, eax

			popad
			jnz OldCode

			mov edx, 0

OldCode:
			call D2Net::ReceivePacket_I

			push ebp
			ret
		}
	}


void __declspec(naked) ViewInventorySTUB1()
{
	__asm
	{
		PUSH EAX;
		CALL GetViewingUnit;
		MOV ESI, EAX;
		POP EAX;
		RET;
	}
}

void __declspec(naked) ViewInventorySTUB2()
{
	__asm
	{
		PUSH EAX;
		CALL GetViewingUnit;
		MOV EBX, EAX;
		POP EAX;
		RET;
	}
}

void __declspec(naked) ViewInventorySTUB3()
{
	__asm
	{
		PUSH EAX;
		PUSH EBX;
		CALL GetViewingUnit;

		MOV EBX, [EDI];
		CMP EBX, 1;
		JE OldCode;

		MOV EDI, EAX;

OldCode:
		POP EBX;
		POP EAX;
		TEST EAX, EAX;
		MOV ECX, DWORD PTR [EDI + 0x60];
		RET;
	}
}

UnitAny* FindItem(DWORD Code, int nLoc)
{
	for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
		if (pItem->pItemData->InventoryLocation == nLoc){		
				if(pItem->dwTxtFileNo == Code)
				return pItem;
		
		}
	}
	return NULL;
}

UnitAny* SearchItem(int Storage){
	for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
		if (pItem->pItemData->InventoryLocation == Storage){
			if (pItem->pItemData->dwQuality == ITEM_QUALITY_RARE){
				 return pItem;
			}
			if (pItem->pItemData->dwQuality == ITEM_QUALITY_SUPERIOR)
				return pItem;
			if (pItem->pItemData->dwQuality == ITEM_QUALITY_CRAFT)
				return pItem;
		}

	}
	return NULL;
}


UnitAny* FindPotion(int Storage)
{
	BnetData* pData = (*D2Launch::BnData);	
	for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
		if (pItem->pItemData->InventoryLocation == Storage){		
					wchar_t wItemName[256] = L"";
					D2Client::GetItemNameString(pItem,wItemName,sizeof(wItemName));

					if (wcsstr(wItemName,L"Healing Potion [L") != NULL) return pItem;
					if (wcsstr(wItemName,L"Full Rejuvenation Potion") != NULL) return pItem;

		}
	}
	return NULL;
	
}

UnitAny* FindPortal(int Storage)
{
	for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
		if (pItem->pItemData->InventoryLocation == Storage){		
					wchar_t wItemName[256] = L"";
					D2Client::GetItemNameString(pItem,wItemName,sizeof(wItemName));
					if (wcsstr(wItemName,L"Tome of Town Portal") != NULL) 
						return pItem;
		}
	}
	return NULL;
	
}

UnitAny* FindTownPortalScroll(int Storage)
{
	for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
		if (pItem->pItemData->InventoryLocation == Storage){		
					wchar_t wItemName[256] = L"";
					D2Client::GetItemNameString(pItem,wItemName,sizeof(wItemName));
					if (wcsstr(wItemName,L"Scroll of Town Portal") != NULL) 
						return pItem;
		}
	}
	return NULL;
	
}

UnitAny* FindCube()
{
	for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
		if (pItem->pItemData->InventoryLocation == STORAGE_INVENTORY){		
					wchar_t wItemName[256] = L"";
					D2Client::GetItemNameString(pItem,wItemName,sizeof(wItemName));
					if (wcsstr(wItemName,L"Horadric Cube") != NULL) 
						return pItem;
		}
	}
	return NULL;
	
}

UnitAny* FindRenewalOil(int Storage)
{
	for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
		if (pItem->pItemData->InventoryLocation == Storage){		
					wchar_t wItemName[256] = L"";
					D2Client::GetItemNameString(pItem,wItemName,sizeof(wItemName));
					if (wcsstr(wItemName,L"Oil of Renewal") != NULL) 
						return pItem;
		}
	}
	return NULL;
	
}

UnitAny* FindShrine(int Storage)
{
	for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
		if (pItem->pItemData->InventoryLocation == Storage){		
					wchar_t wItemName[256] = L"";
					D2Client::GetItemNameString(pItem,wItemName,sizeof(wItemName));
					if (wcsstr(wItemName,L"Shrine") != NULL) 
						return pItem;
		}
	}
	return NULL;
	
}