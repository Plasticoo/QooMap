#include <ctime>
#include <math.h>
#include "AutoMap.h"
#include "ScreenHooks.h"
#include "Vector.h"
#include "BuyAction_Info.h"
#include "AutoMapCell.h"
#include "EnteringLevelText.h"

#define D2TXTCODE(x) ( ((x)&0xff)<<24 | ((x)>>8&0xff)<<16 | ((x)>>16&0xff)<<8 | ((x)>>24&0xff) )

int unitColors[] = {0xFF, 0x0A, 0x84, 0x1C,0xA2};  //1C = 기본몹  //0B메디안몹
int monsterColors[] = {0x91,0x60,0x62};
int missileColors[] = {0x20, 0x0A, 0x84, 0x4B};
bool levelRevealed[255], actRevealed[6], gameRevealed;
bool QooMapRevealOK[5];
vector< vector<AutomapName*> > AutomapNames;
CArrayEx<mVECTOR, mVECTOR> V_CurrentVectors;

DWORD selectunitid;
DWORD selectunittype;

char* szText[3] = {"Normal", "Nightmare", "Hell"};
//DWORD LastUpdate = 0;
string gamename = "";
string gamepass = "";
wstring levelname = L"";

int NextLevelVectorColor = 0x9F;  //9f=블루
int PreviousLevelVectorColor = 0xAC; //ac = 블랙
int QuestVectorColor = 0x67;  // ab = 흰색 6f = 노란색
int WaypointVectorColor = 0x5E; // 5e = 금색
int UberQuestVectorColor = 0x0A; // 0a적색
int HeroicAreaVectorColor = 0x7f; //7f 연녹색
int UberQuestVectorColor2 = 0x62; //주황 


CellFile *BombCellFile;
CellFile *CrossCellFile;
CellFile *ItemCellFile;
CellFile *MissileCellFile;
CellFile *ItemConfCellFile; //아이템 위치
CellFile *ChickenCellFile; //아이템 위치
CellFile *EffectViewCellFile; //아이템 위치
CellFile *ScrollMapCellFile; //아이템 위치
CellFile *MapNameCellFile; //아이템 위치
CellFile *DropNoticeCellFile; //아이템 위치
CellFile *HighresCellFile; //아이템 위치
CellFile *SettingCellFile; //아이템 위치
CellFile *BoardCellFile; //아이템 위치
CellFile *AutoTransmuteCellFile;
int MonID_Uber[39] = {961,1076,1567,1573,1612,952,1253,1004,1083,1084,1085,2048,1095,1096,1097,1098,1099,969,1059,1629,2044,2085,2086,2087,2084,2083,2062,1030,1032,1038,1227,1246,1596,1833,1832,1829,1831,1830,1834};
int MonID_Veteran[10] = {1699,1706,1702,1709,1701,1708,1700,1707,1703,1710};
int MonID_Egg[4] = {1652,1653,1656,1657};

wchar_t* AnsiToUnicode(const char* str)
{
	wchar_t* buf = NULL;
	int len = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str, -1, buf, 0);
	buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str, -1, buf, len);
	return buf;
}

char* UnicodeToAnsi(const wchar_t* str)
{
	char* buf = NULL;
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, buf, 0, "?", NULL);
	buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, str, -1, buf, len, "?", NULL);
	return buf;
}

void ScreenToAutomap(POINT* Position, int X, int Y)
{
	Position->x = ((X - Y) / 2 / (*(INT*)D2Client::Divisor)) - (*D2Client::Offset).x + 8;
	Position->y = ((X + Y) / 4 / (*(INT*)D2Client::Divisor)) - (*D2Client::Offset).y - 8;

	if (D2Client::GetAutomapSize())
	{
		--Position->x;
		Position->y += 5;
	}
}

RosterUnit *FindPartyById(DWORD dwUnitId)
{
	for(RosterUnit *pUnit = (*D2Client::PlayerUnitList); pUnit; pUnit = pUnit->pNext)
		if(pUnit->dwUnitId == dwUnitId)
			return pUnit;

	return NULL;
}

void ToAutomap(POINT* ptPos) { 
	int x = ptPos->x * 32, y = ptPos->y * 32;
	ptPos->x = ((x - y) / 2 / (*(INT*)D2Client::AutomapMode)) - D2Client::Offset->x + 8; 
	ptPos->y = ((x + y) / 4 / (*(INT*)D2Client::AutomapMode)) - D2Client::Offset->y - 8; 
	if(D2Client::GetAutomapSize()) { 
		--ptPos->x; 
		ptPos->y += 5; 
	}
}

void AddRoomCell(INT xPos, INT yPos, INT nCell, Room2 *pRoom)
{
	AutomapCell *pCell = D2Client::NewAutomapCell();

	xPos += pRoom->dwPosX * 5;
	yPos += pRoom->dwPosY * 5;

	pCell->nCellNo = nCell;
	pCell->xPixel = (((xPos - yPos) * 16) / 10) + 1;
	pCell->yPixel = (((xPos + yPos) * 8) / 10) - 3;
	D2Client::AddAutomapCell(pCell, &((*D2Client::CurrentAutomapLayer)->pObjects));
}

void DrawAutomapCell2(CellFile *cellfile, int xpos, int ypos, BYTE col)
{
	CellContext ct;
	memset(&ct, 0, sizeof(ct));
	if (!(ct.pCellFile = cellfile)) return;

	static BYTE coltab[2][256];//, tabno = 0, lastcol = 0;
	if (!coltab[0][1]) for (int k = 0; k < 255; k++) coltab[0][k] = coltab[1][k] = k;
	cellfile->mylastcol = coltab[cellfile->mytabno ^= (col != cellfile->mylastcol)][255] = col;

	D2Gfx::DrawAutomapCell2(&ct, xpos, ypos, -1, 5, coltab[cellfile->mytabno]);
}

void __stdcall AutomapCells(CellContext *Context, DWORD X, DWORD Y, RECT * Clip, DWORD Bright)
{	
	D2Gfx::DrawAutomapCell(Context, X, Y, Clip, Bright);
}


int DrawAutomap_I(void){
	UnitAny* pPlayer = D2Client::GetPlayerUnit();

	BYTE missilecolor = 0;
	POINT myPoint,targetPosition = {0,0};
	ScreenToAutomap(&myPoint,pPlayer->pPath->xPos*32,pPlayer->pPath->yPos*32);

	for (Room1* pRoom = pPlayer->pAct->pRoom1; pRoom; pRoom = pRoom->pRoomNext) {
			for(UnitAny* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pListNext) {

				//Check to see if it is a valid monster.
				if (!IsValidMonster(pUnit))
					continue;

				POINT pPoint = {pUnit->pPath->xPos, pUnit->pPath->yPos};
				ToAutomap(&pPoint);

				if (pUnit->dwType == UNIT_TYPE_MONSTER ) {
							if (pUnit->dwType == UNIT_TYPE_PLAYER) continue;
							
							// 이뮨 표시
							wchar_t MonImmune[64] = L"";
							wsprintfW(MonImmune,L"%s%s%s%s%s%s",(int)D2Common::GetUnitStat(pUnit, 36, 0) >= 100 ? L"ÿc4i" : L"",
								(int)D2Common::GetUnitStat(pUnit, 43, 0) >= 100 ? L"ÿc3i" : L"" ,
								(int)D2Common::GetUnitStat(pUnit, 39, 0) >= 100 ? L"ÿc1i" : L"" ,
								(int)D2Common::GetUnitStat(pUnit, 41, 0) >= 100 ? L"ÿc9i" : L"",
								(int)D2Common::GetUnitStat(pUnit, 45, 0) >= 100 ? L"ÿc2i" : L"",
								(int)D2Common::GetUnitStat(pUnit, 37, 0) >= 100 ? L"ÿc8i" : L"");
						    
							BYTE nColor = 0x1C; //unitColors[GetRelations(pUnit)]; // 기본 몹 색상

							// 우버 보스
							
							if (Mon_NameUber(pPlayer->dwAct,pUnit->dwTxtFileNo) == true) { 
								
								wchar_t *monnameboss = (wchar_t *)GetUnitNameSTUB((DWORD)pUnit);

								bool target_ok = false;
								WideTexthookDraw(Automap, pUnit->pPath->xPos-10, pUnit->pPath->yPos-10, 0, 6, CenterFromX, L"%s",monnameboss);
								nColor = monsterColors[2];
									if(fakeselect == true) {
										nColor = 0x9b;
										selectunitid = pUnit->dwUnitId;
										selectunittype = pUnit->dwType;
										target_ok = true;
									}
									
									D2Gfx::DrawLine(myPoint.x,myPoint.y,pPoint.x,pPoint.y, nColor, -1);
									DrawMonsterText(pUnit,monnameboss,4,target_ok);
								} else selectunitid=0; //가짜클릭 끄기

						WideTexthookDraw(Automap,pUnit->pPath->xPos, pUnit->pPath->yPos, 0, 4, CenterFromX, L"%s",MonImmune);
						
							if (pUnit->pMonsterData->fChamp) nColor = monsterColors[0];
							if (pUnit->pMonsterData->fMinion) nColor = monsterColors[1];
							if (pUnit->pMonsterData->fBoss){ nColor = monsterColors[2];
							    wchar_t *boss = (wchar_t *)GetUnitNameSTUB((DWORD)pUnit);;
								WideTexthookDraw(Automap, pUnit->pPath->xPos-10, pUnit->pPath->yPos-10, 0, 6, CenterFromX, L"%s",boss);
							}
							
					DrawAutomapCell2(CrossCellFile,pPoint.x-8,pPoint.y+4,nColor); //32는 흰색
					//CrosshookDraw(pUnit->pPath->xPos, pUnit->pPath->yPos, nColor,false); //몹 그리
			}

			
			if (pUnit->dwType == UNIT_TYPE_ITEM){ // 타입이 아이템 이템
				wchar_t wItemName[1024] = L"";
		
				D2Client::GetItemNameString(pUnit,wItemName,sizeof(wItemName));
				POINT itemPoint = {pUnit->pItemPath->dwPosX, pUnit->pItemPath->dwPosY};
				ToAutomap(&itemPoint);

						if (pUnit->pItemData->dwQuality == ITEM_QUALITY_UNIQUE) {
											if (Item_Charm(pUnit) != false) {
												WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -10, pUnit->pItemPath->dwPosY-10,4, 6, CenterFromX,L"%s",wItemName); 
												DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x5e); //78금색
											}
											WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -10, pUnit->pItemPath->dwPosY-10,4, 6, CenterFromX,L"%s",wItemName); 
											DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x5e); //78금색
						}
						if (pUnit->pItemData->dwQuality == ITEM_QUALITY_SET) { 
												WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -10, pUnit->pItemPath->dwPosY-10,2,6, CenterFromX, L"%s",wItemName); 
												DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x81);
													}
							if (pUnit->pItemData->dwQuality == ITEM_QUALITY_MAGIC) 
								if (Item_MagicJewel(pUnit) != false){ 
									WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -10, pUnit->pItemPath->dwPosY-10,3, 6, CenterFromX, L"%s",wItemName); 
									DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x90); //145파랑
							}
							if (pUnit->pItemData->dwQuality == ITEM_QUALITY_RARE) 
								if (Item_RareItem(pUnit) != false){ WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -10, pUnit->pItemPath->dwPosY-10,9, 6, CenterFromX, L"%s",wItemName); 
								DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x6f); //109노랑
							}
							if (pUnit->pItemData->dwQuality == ITEM_QUALITY_NORMAL){
								if (wcsstr(wItemName,L"Rune")){
									char number[6] = "";
									wsprintf(number,"%c%c",D2Common::GetItemText(pUnit->dwTxtFileNo)->szCode[1],D2Common::GetItemText(pUnit->dwTxtFileNo)->szCode[2]);
									if (runenumber <=  atoi(number)) { // 룬이면~
										WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -15, pUnit->pItemPath->dwPosY-15,11, 6, CenterFromX, L"%s",wItemName); 
										DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x9b); //155보라
									   }//룬일경우 보라색
									}
								/*
								if (Item_Ultimative(pUnit) !=false) { WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -10, pUnit->pItemPath->dwPosY-10,1, 6, CenterFromX, L"%s",wItemName); 
										DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x5c); //155보라
									}
									*/
								if (Item_Gem(pUnit) != false) { 
										WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -10, pUnit->pItemPath->dwPosY-10,11, 6, CenterFromX, L"%s",wItemName); 
										DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x5c); //155보라
										//TexthookDraw(Automap,pUnit->pItemPath->dwPosX , pUnit->pItemPath->dwPosY,11, 5, CenterFromX, "x");
								}
								/*
								if (Item_Shrine(pUnit) != false){ WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -10, pUnit->pItemPath->dwPosY-10,8, 6, CenterFromX, L"%s",wItemName);
										DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x5c); //155보라
									}
								if (wcsstr(wItemName,L"Trophy")) { WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -10, pUnit->pItemPath->dwPosY-10,8, 6, CenterFromX, L"%s",wItemName); 
										DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x5c); //155보라
									//TexthookDraw(Automap,pUnit->pItemPath->dwPosX , pUnit->pItemPath->dwPosY,8, 5, CenterFromX, "x");
									}
								if (wcsstr(wItemName,L"Mystic Orb") && wcslen(wItemName) > 25) { WideTexthookDraw(Automap,pUnit->pItemPath->dwPosX -15, pUnit->pItemPath->dwPosY-15,8, 6, CenterFromX, L"%s",wItemName); 
									DrawAutomapCell2(ItemCellFile,itemPoint.x-30,itemPoint.y+10,0x5c); //155보라
									//TexthookDraw(Automap,pUnit->pItemPath->dwPosX , pUnit->pItemPath->dwPosY,8, 5, CenterFromX, "x");
								}
								*/
							}
					}
			if (pUnit->dwType == UNIT_TYPE_MISSILE){

					POINT missilePoint = {pUnit->pPath->xPos, pUnit->pPath->yPos};
					ToAutomap(&missilePoint);

					if (pPlayer->dwUnitId == pUnit->dwOwnerId){
					 missilecolor = 0x1f; // 흰색
					}

					if (pUnit->dwOwnerType == UNIT_TYPE_MONSTER)
					{
						
						missilecolor = 0x0a;
						if (pPlayer->pAct->pRoom1->pRoom2->pLevel->dwLevelNo == 13){
							if (pUnit->pMissileData->SkillNo == 8 && pUnit->pMissileData->wTotalFrames == 50) { //8 = 불데미지/던크 술통 프레임 토탈 프레임 = 50
									if (pUnit->pMissileData->wFrame < 50){ //1839 punit->dwtextno 폭탄
											TexthookDraw(Automap, pUnit->pPath->xPos-13, pUnit->pPath->yPos-20, 1, 1, CenterFromX, "%d",pUnit->pMissileData->wFrame/10); 
											DrawAutomapCell2(BombCellFile,missilePoint.x,missilePoint.y,0x0);
											//CrosshookDraw(pUnit->pPath->xPos, pUnit->pPath->yPos, 0x20,false); //몹 그리
									}
								}
							}
					}

					DrawAutomapCell2(MissileCellFile,missilePoint.x,missilePoint.y,missilecolor); //32는 흰색
					//BoxhookDraw(Automap, pUnit->pPath->xPos - 1, pUnit->pPath->yPos - 1, 2, 2, missilecolor, BTHighlight, None); // 미사일 그리기	
				}
			}
	}

	if (UserAutomapname == true){
			for (size_t j=0; j<AutomapNames[pPlayer->dwAct].size();++j){
							TexthookDraw(Automap, AutomapNames[pPlayer->dwAct][j]->dwX, AutomapNames[pPlayer->dwAct][j]->dwY, AutomapNames[pPlayer->dwAct][j]->levelcolor, 6, LevelName, "%s",AutomapNames[pPlayer->dwAct][j]->szLevelName);
							
		}
		if (!V_CurrentVectors.IsEmpty())
			{
			int Colors[] = {NextLevelVectorColor, PreviousLevelVectorColor, QuestVectorColor, WaypointVectorColor,UberQuestVectorColor,HeroicAreaVectorColor,UberQuestVectorColor2};

			V_CurrentVectors.Lock();

			for (INT i = 0; i < V_CurrentVectors.GetSize(); i++)
			{
				ScreenToAutomap(&targetPosition, V_CurrentVectors[i]->Position.x, V_CurrentVectors[i]->Position.y);
				D2Gfx::DrawLine(myPoint.x, myPoint.y,targetPosition.x, targetPosition.y, Colors[V_CurrentVectors[i]->Type - 1],-1);
				DrawAutomapCell2(CrossCellFile,targetPosition.x-8,targetPosition.y+4,Colors[V_CurrentVectors[i]->Type - 1]);
				//CrosshookDraw(V_CurrentVectors[i]->Position.x, V_CurrentVectors[i]->Position.y,Colors[V_CurrentVectors[i]->Type - 1],true);
			}
			V_CurrentVectors.Unlock();
		}
	}

	return D2Client::DrawAutomap();
}

bool mythUber(UnitAny *pUnit){

	return false;
	/*
	wchar_t *mythmonnameboss = (wchar_t *)GetUnitNameSTUB((DWORD)pUnit);
	if (wcsstr(mythmonnameboss,L"Lamagra")) return true;
	if (wcsstr(mythmonnameboss,L"Angel of the Apocalypse")) return true;
	if (wcsstr(mythmonnameboss,L"Pandora")) return true;
	if (wcsstr(mythmonnameboss,L"Sheeva")) return true;
	if (wcsstr(mythmonnameboss,L"Ursula")) return true;
	if (wcsstr(mythmonnameboss,L"Lilith")) return true;
	if (wcsstr(mythmonnameboss,L"Diablo")) return true;
	if (wcsstr(mythmonnameboss,L"Duriel")) return true;
	if (wcsstr(mythmonnameboss,L"Izual")) return true;
	if (wcsstr(mythmonnameboss,L"Mephisto")) return true;
	if (wcsstr(mythmonnameboss,L"Baal")) return true;
	return false;
	*/
}

void GameAutomapDraw() {

	BnetData* pData = (*D2Launch::BnData);
	UnitAny* pPlayer = D2Client::GetPlayerUnit();

	LevelTxt* pTxt = NULL;

	pTxt = D2Common::GetLevelText(pPlayer->pPath->pRoom1->pRoom2->pLevel->dwLevelNo);
	if(pTxt && pTxt->wName && (levelname == L"" || levelname != pTxt->wName))
		levelname = pTxt->wName;
	pTxt = NULL;
	WideTexthookDraw(InGame, d2res_extend_width + 620, (*D2Client::AutomapYDrawPos), 0, 5, Left, L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p");
	WideTexthookDraw(InGame, 790, (*D2Client::AutomapYDrawPos), 0, 4, Right, L" -  [Diablo 1.13c]");

	if(pData) {
		if(strlen(pData->szGameName))
			TexthookDraw(InGame, 790, (*D2Client::AutomapYDrawPos += 16), 4, 0, Right, "%s: %s",d2locale_kor? "게임 이름" : "Game Name", pData->szGameName);
		if(strlen(pData->szGamePass))
			TexthookDraw(InGame, 790, (*D2Client::AutomapYDrawPos += 16), 4, 0, Right, "%s: %s",d2locale_kor? "게임 비번" : "Game Pass", pData->szGamePass);
	}

	WideTexthookDraw(InGame, 790, (*D2Client::AutomapYDrawPos += 16), 4, 0, Right, levelname);
	if(strlen(pData->szRealmName))
		TexthookDraw(InGame, 790, (*D2Client::AutomapYDrawPos += 16), 3, 0, Right, "%s: %s",d2locale_kor? "렐름" : "Realm", pData->szRealmName);
	TexthookDraw(InGame, 790, (*D2Client::AutomapYDrawPos += 16), 4, 0, Right, "%s: %s",d2locale_kor? "난이도" : "Difficulty", szText[(*D2Client::Difficulty)]);
	
	int pingcolor = *D2Client::Ping > 500 ? 1 : 8;
	WideTexthookDraw(InGame, d2res_extend_width + 680, (*D2Client::AutomapYDrawPos += 16), 4, 0, Left, L"%s: ÿc2%d ÿc4%s: ÿc%d%d",d2locale_kor? L"프레임" : L"Fps", *D2Client::FramesPerSecond,d2locale_kor? L"핑" : L"Ping",pingcolor,*D2Client::Ping);
	
	int nTime = (GetTickCount() - jointime)/1000;
    WideTexthookDraw(InGame, d2res_extend_width + 730, (*D2Client::AutomapYDrawPos += 16), 4, 0, Left, L"ÿc%s %.2d:%.2d:%.2d",nTime>=300?"4":"1",nTime/3600, (nTime/60)%60, nTime%60);

	//TexthookDraw(InGame, 790, (*D2Client::AutomapYDrawPos += 16), 3, 0, Right, "%d" ,pPlayer->pPath->pRoom1->pRoom2->pLevel->dwLevelNo);

	time_t tTime;
	time(&tTime);
	char szTime[128] = "";
	struct tm time;
	localtime_s(&time, &tTime);
	strftime(szTime, sizeof(szTime), "%I:%M:%S %p", &time);

	TexthookDraw(InGame, 790, (*D2Client::AutomapYDrawPos += 16), 9, 0, Right, "%s", szTime);
	/*
	if (Uber_EnteringLevelText() == true){
		if (UberQuest == true && !GetUIVarSTUB(UI_ALTDOWN)){
				WideTexthookDraw(InGame,15, (445 + d2res_extend_height),0,1, Left, L"%s",UberQuestType);
				WideTexthookDraw(InGame,15, (460 + d2res_extend_height),0,1, Left, L"%s",UberQuestBoss);
				WideTexthookDraw(InGame,15, (475 + d2res_extend_height),0,1, Left, L"%s",UberQuestDrop); //보스이름이거나 드랍아이템
				WideTexthookDraw(InGame,50, (490 + d2res_extend_height),0,1, Left, L"%s",UberQuestTrophy); //보스이름이거나 드랍아이템
		}
	}

	for (PresetUnit *pUnit = D2Client::GetPlayerUnit()->pAct->pRoom1->pRoom2->pPreset; pUnit; pUnit = pUnit->pPresetNext) {
		int mCell = -1;
		 if (pUnit->dwType == UNIT_TYPE_MONSTER) {
			 //if(pUnit->dwTxtFileNo == 256 ) AddRoomCell(pUnit->dwPosX,pUnit->dwPosY,300,D2Client::GetPlayerUnit()->pAct->pRoom1->pRoom2);
			 //if(pUnit->dwTxtFileNo == 1653 )AddRoomCell(pUnit->dwPosX,pUnit->dwPosY,300,pRoom2);
			 //if(pUnit->dwTxtFileNo == 1656 )AddRoomCell(pUnit->dwPosX,pUnit->dwPosY,300,pRoom2);
			 //if(pUnit->dwTxtFileNo == 1657 )AddRoomCell(pUnit->dwPosX,pUnit->dwPosY,300,pRoom2);
			 if(pUnit->dwTxtFileNo == 1704 )AddRoomCell(pUnit->dwPosX,pUnit->dwPosY,1704,D2Client::GetPlayerUnit()->pAct->pRoom1->pRoom2);
		 }
	}
	*/

}

void  __declspec(naked) DrawSTUB()
{
	__asm
	{
		PUSHAD;
		CALL GameDraw;
		POPAD;

		POP EBX;
		POP ECX;
		RETN 0x4;
	}
}


DWORD __declspec(naked) __fastcall D2DrawRectFrame_STUB(RECT* pRect)
{
	__asm
	{
		mov eax, ecx
		jmp D2Client::DrawRectFrame
	}
}


BYTE ClickWithdraw[7] = {0x4f, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00}; // 돈 넣기 클릭
bool clickwithdrawok = false;

void GameDraw(void){

	if (QooMapNoticeOK == true){ // on/off보이기
		WideTexthookDraw(InGame,15, 95,0,1, Left, L"%s",QooMapNotice);
		if(GetTickCount() > (DWORD)NoticeTime + 3000) QooMapNoticeOK = false;
	}

	if (GetUIVarSTUB(UI_CHARACTER)){
		if (viewunitstatusok == false) { viewplayerstate = 0; viewunitstatusok = true; }
		DrawRectangle_CharacterStat();
	}else viewunitstatusok = false;

	if (GetUIVarSTUB(UI_PARTY)){
		WideTexthookDraw(InGame,90, 480, 0,1, Left, L"ÿc%sAutoParty",pAutoParty?"9":"5");
	}

	if (GetUIVarSTUB(UI_INVENTORY)){
		WideTexthookDraw(InGame,610, 470, 0,1, Left, L"ÿc%sLockInventory",LockClick?"1":"5");
		WideTexthookDraw(InGame,610, 485, 0,1, Left, L"ÿc%sAutoEatCorpse",AutoEatCorpse?"1":"5");
	}
	/*
	if (GetUIVarSTUB(UI_QUEST)){ //Uberquest hint
		WideTexthookDraw(InGame,90, 480, 0,1, Left, L"ÿc%sUberQuest Info",UberQuest?"2":"5");
	}
	*/

	if (DrawSetting) DrawRectangle_ItemSetting();// 정보 보기

	//AutoTransmuteItems(); //자동 큐브 돌리기

	if (QooMapChicken) {
		int LifePercent = (int)((double)(D2Common::GetUnitStat(D2Client::GetPlayerUnit(), 6,0) >> 8)/(double)(D2Common::GetUnitStat(D2Client::GetPlayerUnit(), 7,0) >> 8)*100);
		WideTexthookDraw(InGame,57, (555 + d2res_extend_height), 0, 5, Left, L"ÿc1%d%% ",LifePercent);
	}
	//WideTexthookDraw(InGame,170, 165, 0, 1, Left, L"ÿc0MouseX : %d , ÿc0MouseY : %d ",*D2Client::MouseX,*D2Client::MouseY);

	WideTexthookDraw(InGame,0, 10, 0, 1, Left, L"%s%s",fakeselect? L"ÿc1W":L"",AlwaysTop? L"ÿc9T":L"");
	//포탈 타면 포탈갯수 확인
	if (myportalonoff == true && myportalnum !=0) WideTexthookDraw(InGame,(370 + (d2res_extend_width/2)), (235 + d2res_extend_height), 0, 1, Left, L"%s = ÿc8%d",d2locale_kor ? L"포탈" : L"Portal",myportalnum);

	if (GetUIVarSTUB(UI_CUBE) || GetUIVarSTUB(UI_STASH)) {
			UnitAny* pPlayer = D2Client::GetPlayerUnit();
			//WideTexthookDraw(InGame,80, 52, 0, 1, Left, L"ÿc8%s : %d",d2locale_kor? L"크래프트 포인트" : L"Craft Point",D2Common::GetUnitStat(pPlayer,183,0));
			/*
			if (GetUIVarSTUB(UI_CUBE)) 
				{
				//WideTexthookDraw(InGame,240, 510, 0, 1, Left, L"ÿc%sAuto Transmute",AutoTransmute?L"9":L"5");
				//if (AutoTransmute) DrawAutomapCell2(AutoTransmuteCellFile,85,500,0xab); //32는 흰색
			}
			*/
			if (GetUIVarSTUB(UI_STASH)){
					if (D2Common::GetUnitStat(pPlayer,14,0) >0 )
						clickwithdrawok = true;
					else clickwithdrawok = false;

					if (clickwithdrawok){
						*(WORD*)&ClickWithdraw[5] = (WORD)D2Common::GetUnitStat(pPlayer,14,0);;

						SendGamePacket(ClickWithdraw,sizeof(ClickWithdraw));	
					}
				//WideTexthookDraw(InGame,150, 510, 0,1, Left, L"ÿc2Iÿc0tem ÿc3Dÿc0ump ÿc1(Save Html File)");
			}
			
			if (myitemview) { //오른쪽버튼 누르고 잇으면 on 떼면 off
	
			int itemcheckunique = 0;
			int itemcheckset = 0;
			int checkstorage;
			wchar_t cube_wItemName[512] = L"";
		
			InventoryLayout* pLayout = NULL;
			pLayout = (InventoryLayout*)D2Client::PlayerLayout;
		
			if(GetUIVarSTUB(UI_CUBE)) {
				pLayout = (InventoryLayout*)D2Client::CubeLayout;
				checkstorage = STORAGE_CUBE;}

			if(GetUIVarSTUB(UI_STASH)) {
				pLayout = (InventoryLayout*)D2Client::StashLayout;
				checkstorage = STORAGE_STASH;}

			for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
				if (pItem->pItemData->InventoryLocation == checkstorage){
					D2Client::GetItemNameString(pItem,cube_wItemName,sizeof(cube_wItemName));
					int x = pItem->pItemPath->dwPosX;
					int y = pItem->pItemPath->dwPosY;

					x =  pLayout->Left + x * pLayout->SlotPixelWidth + 5; 
					y =  pLayout->Top + y * pLayout->SlotPixelHeight + 15;

					if (pItem->pItemData->dwQuality == ITEM_QUALITY_SET) {
						WideTexthookDraw(InGame,x, y, 2, 1, Left, L"%s",d2locale_kor? L"세트":L"Set");
						itemcheckset = itemcheckset++;
					}
					if  (pItem->pItemData->dwQuality == ITEM_QUALITY_UNIQUE) {
							WideTexthookDraw(InGame,x, y, 4, 1, Left, L"%s",d2locale_kor? L"유니크" : L"Unique");
							if (wcsstr(cube_wItemName,L"(Sacred)")) WideTexthookDraw(InGame,x, y+10, 4, 1, Left, L"(%s)",d2locale_kor? L"세크리드" : L"Sacred");
						itemcheckunique = itemcheckunique++;
					}
					//if  (pItem->pItemData->dwQuality == ITEM_QUALITY_CRAFT) 
					//	WideTexthookDraw(InGame,x, y, 8, 1, Left, L"%s",d2locale_kor? L"크래프트" : L"Craft");
					if  (pItem->pItemData->dwQuality == ITEM_QUALITY_RARE) 
						WideTexthookDraw(InGame,x, y, 9, 1, Left, L"%s",d2locale_kor? L"레어" : L"Rare");
					if  (pItem->pItemData->dwFlags & 0x04000000) 
						WideTexthookDraw(InGame,x, y, 5, 1, Left, L"%s",d2locale_kor? L"룬워드" : L"Runeword");
					if  (pItem->pItemData->dwQuality == ITEM_QUALITY_NORMAL){
						char number[6] = "";
						wsprintf(number,"%c%c",D2Common::GetItemText(pItem->dwTxtFileNo)->szCode[1],D2Common::GetItemText(pItem->dwTxtFileNo)->szCode[2]);
						if (wcsstr(cube_wItemName,L"Rune")) { //룬 번호 처리
							WideTexthookDraw(InGame,x, y, 11, 1, Left, L"%d",atoi(number));
							if (atoi(number) > 50 && atoi(number) <= 56) WideTexthookDraw(InGame,x-7, y+10, 0, 6, Left, L"%s",d2locale_kor? L"그레이트룬" : L"Great");
							if (atoi(number) > 56) WideTexthookDraw(InGame,x-7, y+10, 0, 6, Left, L"%s",d2locale_kor? L"시크릿룬" : L"Secret");
						}
						if (wcsstr(cube_wItemName,L"Kabraxis' Stone")) WideTexthookDraw(InGame,x-7, y+10, 0, 6, Left, L"%s",d2locale_kor? L"카브라시스" : L"KABRAXIS");
				      }
					}
				}

				WideTexthookDraw(InGame,240, 47, 4, 1, Left, L"%s = %d",d2locale_kor? L"유니크 아이템" : L"Unique Item",itemcheckunique);
				WideTexthookDraw(InGame,240, 57, 2, 1, Left, L"%s = %d",d2locale_kor? L"세트 아이템" : L"Set Item",itemcheckset);
			}
		}
	else { // 창고/큐브 아니면 표시
		DrawAutomapCell2(BoardCellFile ,167, (d2res_extend_height+ 600),0x00); //1e = 회색 0a적색
		
		DrawAutomapCell2(ItemConfCellFile ,170, (d2res_extend_height+ 575),MenuAllUniqueItem); //
		DrawAutomapCell2(EffectViewCellFile,310, (d2res_extend_height+ 575),EffectView? 0x76 : 0x13); //1e = 회색 0a적색

		DrawAutomapCell2(ScrollMapCellFile,170, (d2res_extend_height+ 585),ScrollMapON? 0x84 : 0x13); //1e = 회색 0a적색
		DrawAutomapCell2(ChickenCellFile,240, (d2res_extend_height+ 585),QooMapChicken? 0x97 : 0x13); //1e = 회색 0a적색
		DrawAutomapCell2(MapNameCellFile,310, (d2res_extend_height+ 585),UserAutomapname? 0x8f : 0x13); //1e = 회색 0a적색

		DrawAutomapCell2(DropNoticeCellFile,170, (d2res_extend_height+ 597),DropNotify? 0x60 : 0x13); //1e = 회색 0a적색
		DrawAutomapCell2(HighresCellFile,240, (d2res_extend_height+ 597),d2extend_ok? 0x4f : 0x13); //1e = 회색 0a적색
		DrawAutomapCell2(SettingCellFile,310, (d2res_extend_height+ 597),DrawSetting? 0x20 : 0x13); //1e = 회색 0a적색
		WideTexthookDraw(InGame,430, (d2res_extend_height+ 600), 0, 1, Left, L"ÿc8Get-Potions");
	}
}

void DrawMonsterText(UnitAny *pUnit,wchar_t *monname,int textcolor,bool targetok){
	
						int setlife = ((int)D2Common::GetUnitStat(pUnit,6,0)*100) / 32768;
							WideTexthookDraw(InGame,140, 520 + d2res_extend_height, 0, 5, Left, L"ÿc9%s : %d %% ÿc%d [%s%d]  %s%s ÿc4[%d]",d2locale_kor ? L"라이프" : L"Life",setlife,textcolor,d2locale_kor? L"레벨":L"Lv",D2Common::GetUnitStat(pUnit,12,0),targetok? L"ÿc3[TARGET]ÿc0":L"ÿc0",monname,pUnit->dwTxtFileNo); //6이 기본폰트사이즈
							WideTexthookDraw(InGame,140, 540 + d2res_extend_height, 4, 4, Left, L"ÿc0%s : ÿc4%d %% ÿc1%d %% ÿc3%d %% ÿc9%d %% ÿc2%d %% ÿc8%d %%",d2locale_kor ? L"이뮨" : L"Immune",D2Common::GetUnitStat(pUnit, 36, 0),D2Common::GetUnitStat(pUnit, 39, 0),D2Common::GetUnitStat(pUnit, 43, 0),D2Common::GetUnitStat(pUnit, 41, 0),D2Common::GetUnitStat(pUnit, 45, 0),D2Common::GetUnitStat(pUnit, 37, 0));
}

bool IsValidMonster(UnitAny *pUnit)
{
	//Have we even been given a unit?
	if (!pUnit)
		return false;


	//Check to see if unit is dead.
	if ((pUnit->dwMode == 0) || (pUnit->dwMode == 12))
		return false;

	if ((pUnit->dwTxtFileNo == 608) && (pUnit->dwMode == 8))
		return false;
	
	if ((pUnit->dwTxtFileNo == 68) && (pUnit->dwMode == 14))// Ignore burrowing maggots
		return false;

	if (((pUnit->dwTxtFileNo == 258) || (pUnit->dwTxtFileNo == 261)) && (pUnit->dwMode == 14))
		return false;

	static DWORD badMonIds[] = {149,151,157,158,159,179,185,227,268,269,318,320, 283, 326, 327, 328, 329, 330, 410, 411, 412, 413, 414, 415, 416, 366, 406,
						 351, 352, 353, 266, 408, 516, 517, 518, 519, 522, 523, 543, 543, 545,1089,1090,1091,1100};

	for (DWORD n = 0; n < sizeof(badMonIds); n++)
		if (pUnit->dwTxtFileNo == badMonIds[n])
			return false;

	if (D2Common::GetUnitStat(pUnit, 172, 0) == 2) 
		return false;

	return true;
}


bool RevealGame() {
	//Checks to see if we have already revealed.
	//if (gameRevealed)
	//	return false;
	//Loop the acts.
	int ActNo = 1;
	int MaxAct = *D2Client::ExpCharFlag ? 5 : 4;
	for (; ActNo <= MaxAct; ActNo++)
		RevealAct(ActNo);
	return true;
}

Level* GetLevel(Act* pAct, DWORD nLevel)
{
	//Insure that the shit we are getting is good.
	if (nLevel < 0 || !pAct)
		return NULL;

	//Loop all the levels in this act
	for(Level* pLevel = pAct->pMisc->pLevelFirst; pLevel; pLevel = pLevel->pNextLevel) {
		//Check if we have reached a bad level.
		if (!pLevel)
			continue;

		//If we have found the level, return it!
		if (pLevel->dwLevelNo == nLevel && pLevel->dwPosX > 0)
			return pLevel;
	}

	//Default old-way of finding level
	return D2Common::LoadLevel(pAct->pMisc, nLevel);
}

bool RevealAct(DWORD dwAct) {
	//Check if we have revealed act.
	UnitAny* unit = D2Client::GetPlayerUnit();

	if (QooMapRevealOK[unit->dwAct]) return false;

	//Load the act to reveal it!
	int nActLevels[6] = {1, 40, 75, 103, 109, 137};
	int diff = *D2Client::Difficulty;
	
	if(!unit || !unit->pAct)
		return false;
	Act* pAct = D2Common::LoadAct(dwAct - 1, unit->pAct->dwMapSeed, *D2Client::ExpCharFlag, 0, diff, NULL, nActLevels[dwAct - 1], D2Client::LoadAct_1, D2Client::LoadAct_2);
	if(!pAct)
		return false;
	for (int n = nActLevels[dwAct - 1]; n < nActLevels[dwAct]; n++) {

		Level* pLevel = GetLevel(pAct, n);
			if(!pLevel)
				return false;
			if (!pLevel->pRoom2First)
				D2Common::InitLevel(pLevel);
				
			RevealLevel(pLevel);
	}

	InitAutomapLayer(nActLevels[dwAct - 1]);
	D2Common::UnloadAct(pAct);
	QooMapRevealOK[unit->dwAct] = true;
	actRevealed[dwAct] = true;

	return true;
}
/*
	Maphack::RevealLevel(Level* pLevel)
		Reveals the given level
*/


bool RevealLevel(Level* pLevel) {

	if (!pLevel)
		return false;

	//Make sure we have revealed the level.
	//if (levelRevealed[pLevel->dwLevelNo])
	//	return false;

	InitAutomapLayer(pLevel->dwLevelNo);

	//Loop the rooms in the level.
	for (Room2* pRoom = pLevel->pRoom2First; pRoom; pRoom = pRoom->pRoom2Next) {
		bool bAdded = false;

		
	
		//Check to see if we have room data, if not add.
		if (!pRoom->pRoom1) {
			D2Common::AddRoomData(pLevel->pMisc->pAct,pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, pRoom->pRoom1);
			if(pRoom->pRoom1) {
				bAdded = true;
			}
			else continue;

			
		}
		
		//Double-check to see if we have room data, since this can be buggy.
		//if (!pRoom->pRoom1)
		//	continue;

		//Reveal room
		D2Client::RevealAutomapRoom(pRoom->pRoom1, TRUE, (*D2Client::CurrentAutomapLayer));
		//
		DrawPresets(pRoom);

		//Remove data if added
		if (bAdded)
			D2Common::RemoveRoomData(pLevel->pMisc->pAct,pLevel->dwLevelNo, pRoom->dwPosX, pRoom->dwPosY, pRoom->pRoom1);
	}


	//Re-initalize old area so that we don't error
	InitAutomapLayer(pLevel->dwLevelNo);

	levelRevealed[pLevel->dwLevelNo] = true;


	return TRUE;

}

AutomapLayer* InitAutomapLayer(DWORD levelno) {

	//Get the  layer for the level.
	AutomapLayer2 *pLayer = D2Common::GetLayer(levelno);
	if (!pLayer)
		return false;

	//Initalize the layer!
	return (AutomapLayer*)D2CLIENT_InitAutomapLayer(pLayer->nLayerNo);
}

DWORD __declspec(naked) __fastcall D2CLIENT_InitAutomapLayer(DWORD nLayerNo)
{
	__asm 
	{
		push eax;
		mov eax, ecx;
		call D2Client::InitAutomapLayer_I;
		pop eax;
		ret;
	}
}

void DrawPresets (Room2 *pRoom2)
{
	for (PresetUnit *pUnit = pRoom2->pPreset; pUnit; pUnit = pUnit->pPresetNext) {
		int mCell = -1;
		 if (pUnit->dwType == UNIT_TYPE_MONSTER){
			 if (pUnit->dwTxtFileNo == 256) AddRoomCell(pUnit->dwPosX,pUnit->dwPosY,300,pRoom2);
		 }
		 if (pUnit->dwType == UNIT_TYPE_OBJECT) { //Objects on Map
			ObjectTxt* obj = NULL;
			if(pUnit->dwTxtFileNo >= 0 && pUnit->dwTxtFileNo < *D2Common::MaxObjId) {
				obj = D2Common::GetObjectText(pUnit->dwTxtFileNo);
				LevelTxt* pTxt = D2Common::GetLevelText(pRoom2->pLevel->dwLevelNo);
				int mylevelcolor=0;
				if(obj && obj->wszName) {
					bool Bad = true;
					
					if (wcsstr(obj->wszName,L"웨이포인트") || wcsstr(obj->wszName,L"Waypoint")) {mylevelcolor = 7; Bad = false;}
					if (wcsstr(obj->wszName,L"Cairn Stone")) {wsprintfW(obj->wszName,L""); Bad = false;}
					if (pUnit->dwTxtFileNo == 30) Bad = false; //Tree of Inifuss
					if (pUnit->dwTxtFileNo == 376) Bad = false; //Tree of Inifuss
					if (pUnit->dwTxtFileNo == 149) Bad = false; //퀘스트 아뮬
					/*
					if (pUnit->dwTxtFileNo == 60 || pUnit->dwTxtFileNo == 61) { //포탈
						if (wcsstr(pTxt->wName,L"스토니 필드") ||wcsstr(pTxt->wName,L"Stony Field") ) {
							wsprintfW(obj->wszName,L"%s",d2locale_kor? L"트리스트람":L"Tristram");
							mylevelcolor = 9;}

						if (wcsstr(pTxt->wName,L"프리지드 하이랜드")||wcsstr(pTxt->wName,L"Frigid Highlands") ) {
							wsprintfW(obj->wszName,L"Toraja (던전우버 #7)");
							mylevelcolor = 4;}

						if (wcsstr(pTxt->wName,L"아리앗 고원")||wcsstr(pTxt->wName,L"Arreat Plateau") ) {
							wsprintfW(obj->wszName,L"Ghosts of Old Bremmtown (우버레벨 #6)");
							mylevelcolor = 8;}

						if (wcsstr(pTxt->wName,L"프로즌 툰드라")||wcsstr(pTxt->wName,L"Frozen Tundra") ){ 
							wsprintfW(obj->wszName,L"Khalimgrad (우버레벨 #3)");
							mylevelcolor = 8;}

						Bad = false;
					}
					*/
				


				if(Bad) {
						//Bad = false;
						continue;
				}
					//wprintf(L"%s %lu\n", obj->wszName, pUnit->dwTxtFileNo);

					AutomapName* pName = new AutomapName;					
					pName->dwLevelId = pRoom2->pLevel->dwLevelNo;
					pName->szLevelName =  UnicodeToAnsi(obj->wszName);
					pName->levelcolor = mylevelcolor;
					pName->dwX = (pUnit->dwPosX + (pRoom2->dwPosX * 5));
					pName->dwY = (pUnit->dwPosY + (pRoom2->dwPosY * 5));
					pName->dwAct = pRoom2->pLevel->pMisc->pAct->dwAct;
					AutomapNames[pName->dwAct].push_back(pName);
				}
			}
			if (mCell == -1 && obj)
				mCell = obj->nAutoMap; //Set the cell number then.

			obj = NULL;
		} else if (pUnit->dwType == UNIT_TYPE_ROOMTILE) {
			for (RoomTile *pTile = pRoom2->pRoomTiles; pTile; pTile = pTile->pNext) {
				//Check if we have found the room tile.
				if (*(pTile->nNum) == pUnit->dwTxtFileNo) {
					//Place the level id in the proper spot.
					LevelTxt* pTxt = D2Common::GetLevelText(pTile->pRoom2->pLevel->dwLevelNo);
					if(pTxt && pTxt->wName) {
						AutomapName* pName = new AutomapName;					
						pName->dwLevelId = pTile->pRoom2->pLevel->dwLevelNo;		
						if (wcsstr(pTxt->wName,L"우버레벨")) pName->levelcolor = 8;
						else if (wcsstr(pTxt->wName,L"히로익"))  pName->levelcolor = 2;
						else if (wcsstr(pTxt->wName,L"던전우버")) pName->levelcolor = 4;
						else  pName->levelcolor = 0;
						pName->szLevelName = UnicodeToAnsi(pTxt->wName);
						pName->dwX = (pUnit->dwPosX + (pRoom2->dwPosX * 5));
						pName->dwY = (pUnit->dwPosY + (pRoom2->dwPosY * 5));
						pName->dwAct = pRoom2->pLevel->pMisc->pAct->dwAct;
						AutomapNames[pName->dwAct].push_back(pName);
					}
					break;
				}
			}
		}


		//Draw the cell if wanted.
		if ((mCell > 0) && (mCell < 1258)) {
			AutomapCell *pCell = D2Client::NewAutomapCell();

			pCell->nCellNo = mCell;
			int pX = (pUnit->dwPosX + (pRoom2->dwPosX * 5));
			int pY = (pUnit->dwPosY + (pRoom2->dwPosY * 5));
			pCell->xPixel = (((pX - pY) * 16) / 10) + 1;
			pCell->yPixel = (((pY + pX) * 8) / 10) - 3;
			D2Client::AddAutomapCell(pCell, &((*D2Client::CurrentAutomapLayer)->pObjects));
		}

	}
}

void ClearAutomapNames()
{
	//vector<AutomapName*>::iterator it = AutomapNames.begin();
	for(size_t i = 0; i < AutomapNames.size(); ++i)
	{
		for(size_t j = 0; j < AutomapNames[i].size(); ++j)
		{
			delete [] AutomapNames[i][j]->szLevelName;
			delete AutomapNames[i][j];
		}
		AutomapNames[i].clear();
	}
}

string lastGameName = "";
string lastGamePass = "";

void __fastcall NextGameNamePatch(Control* box, bool (__stdcall *FunCallBack)(Control*,unsigned,unsigned)) {
	wchar_t *LastName = AnsiToUnicode(lastGameName.c_str());

	D2Win::SetControlText(box, LastName);
	D2Win::SetEditBoxProc(box, FunCallBack);

	delete [] LastName;
}

void __fastcall NextGamePassPatch(Control* box, bool (__stdcall *FunCallBack)(Control*,unsigned,unsigned)) {
	wchar_t *LastPass = AnsiToUnicode(lastGamePass.c_str());

	D2Win::SetControlText(box, LastPass);
	D2Win::SetEditBoxProc(box, FunCallBack);
	
	delete [] LastPass;
}

DWORD __declspec(naked) __fastcall GetUnitNameSTUB(DWORD Unit)
{
	__asm
	{
		MOV EAX, ECX;
		JMP D2Client::GetUnitName_I;
	}
}

UnitAny* GetSelectedUnit_Intercept(void)
{

	if (fakeselect == true) 
		 if(selectunitid !=0)
             return D2Client::FindServerSideUnit(selectunitid, selectunittype);
		 else return D2Client::GetSelectedUnit();

	return D2Client::GetSelectedUnit();
}