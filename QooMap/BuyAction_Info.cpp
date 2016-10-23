#include "Maphack.h"
#include "ScreenHooks.h"
#include "D2State.h"
#include "Offset.h"
#include "AutoMap.h"
#pragma warning(disable : 4996)

#define D2TXTCODE(x) ( ((x)&0xff)<<24 | ((x)>>8&0xff)<<16 | ((x)>>16&0xff)<<8 | ((x)>>24&0xff) )

unsigned int changecolor;
int baseStr = 0;
int baseDex = 0;
int baseVit = 0;
int baseEne = 0;

bool summon_Glorious;
bool summon_Glorious_trophy;
bool summon_Spirit;
bool summon_Scroll;
bool summon_Crystalline_trophy;
bool summon_Crystalline;
bool summon_Legacy;
bool summon_Legacy_trophy;
bool summon_Worldstone;
bool summon_Butcher;
bool summon_Visions;
bool summon_Visions_trophy;
bool summon_Moon;
bool summon_Moon_trophy;

bool dengeon_Cold;
bool dengeon_Cold_trophy;
bool dengeon_Laser;
bool dengeon_Spirit;
bool dengeon_Spirit_trophy;
bool dengeon_Idol;
bool dengeon_Idol_trophy;
bool dengeon_Horazon;
bool dengeon_Optical;
bool dengeon_Azmodan;
bool dengeon_Azmodan_trophy;


bool uber_Eternal;
bool uber_Eternal_trophy;
bool uber_Six;
bool uber_Kurast;
bool uber_Sunstone;
bool uber_Sunstone_trophy;
bool uber_Zakarum;
bool uber_Demonsbane;
bool uber_Demonsbane_trophy;

bool hero_book;
bool hero_Fool;
bool hero_Silver;
bool hero_Silver_trophy;
bool hero_Astrogha;
bool hero_Astrogha_trophy;

bool ultimative_Ancient;
bool ultimative_Brachyceran;
bool ultimative_Slee;
bool ultimative_Soulkeeper;
bool ultimative_Soul;
bool ultimative_Soul_trophy;
bool ultimative_Uldyssian;
bool ultimative_Uldyssian_trophy;
bool ultimative_Chest;

bool challenge_class;
bool challenge_sunstone;
bool challenge_harkon;


HANDLE ClickGotoBeltThread;
HANDLE BuyoilThread;
HANDLE SaveItemThread;
DWORD WINAPI BuyoilThreadFunction(LPVOID pvoid); 
DWORD WINAPI ClickGotoBeltThreadFunction(LPVOID pvoid); 
DWORD WINAPI SaveItemThreadFunction(LPVOID pvoid); 


void FindCharm()
{
	for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
		if (pItem->pItemData->InventoryLocation == STORAGE_INVENTORY){		
					wchar_t wItemName[256] = L"";
					D2Client::GetItemNameString(pItem,wItemName,sizeof(wItemName));
					ItemTxt *itemtxt = D2Common::GetItemText(pItem->dwTxtFileNo);
						if (itemtxt->dwCode == D2TXTCODE('x@x ')) {
							summon_Glorious = true;//return "Glorious Book of Median";}
							summon_Glorious_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0); //c4800 에러시
							}
						if (itemtxt->dwCode == D2TXTCODE('a67 ')) summon_Spirit=true; //return "Spirit of Damnation";}//spritrt
						if (itemtxt->dwCode == D2TXTCODE('a58 ')) summon_Scroll = true;  //return "Scroll of Kings";}//scroll
						if (itemtxt->dwCode == D2TXTCODE('lok ')) {
							summon_Crystalline = true; //return "Crystalline Flame Medallion";}//crystallline
							summon_Crystalline_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}
						if (itemtxt->dwCode == D2TXTCODE('adi ')) {
							summon_Legacy = true; //return "Legacy of Blood";}//regay
							summon_Legacy_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}
						if (itemtxt->dwCode == D2TXTCODE('a61 '))  summon_Worldstone = true;    //return "The Butcher's Tooth";}//worldston			
						if (itemtxt->dwCode == D2TXTCODE('a60 '))  summon_Butcher = true;    //return "The Butcher's Tooth";}
						if (itemtxt->dwCode == D2TXTCODE('a57 ')) {
							summon_Visions = true;//vison
							summon_Visions_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}
						if (itemtxt->dwCode == D2TXTCODE('yqe ')) {
							summon_Moon = true;//return "Moon of the Spider";}
							summon_Moon_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
						}

						if (itemtxt->dwCode == D2TXTCODE('w@w ')) {
							dengeon_Cold = true;  //return "Cold Fusion Schematics";}
							dengeon_Cold_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}	
						if (itemtxt->dwCode == D2TXTCODE('a59 '))  dengeon_Laser = true; // return "Laser Focus Crystal";}
						if (itemtxt->dwCode == D2TXTCODE('yqx ')) {
							dengeon_Spirit = true; //return "Spirit Trance Herb";}
							 dengeon_Spirit_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}
						if (itemtxt->dwCode == D2TXTCODE('a66 ')) {
							dengeon_Idol = true;  //return "Idol of Vanity";}
							dengeon_Idol_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}
						if (itemtxt->dwCode == D2TXTCODE('a68 ')) dengeon_Horazon = true;   //return "Horazon's Focus";}
						if (itemtxt->dwCode == D2TXTCODE('a55 ')) dengeon_Optical = true;		//return "Optical Detecter";}
						if (itemtxt->dwCode == D2TXTCODE('a56 ')) {
							dengeon_Azmodan = true;		//return "Azmodan's Heart";}
							dengeon_Azmodan_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}
	

						if (itemtxt->dwCode == D2TXTCODE('x0x ')) {
							uber_Eternal = true;      //return "Eternal Bone Pile";}
							 uber_Eternal_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}
						if (itemtxt->dwCode == D2TXTCODE('yq2 '))  uber_Six = true;      //return "Six Angel Bag";}
						if (itemtxt->dwCode == D2TXTCODE('0u1 '))  uber_Kurast = true;      //return "Hammer of the Taan Judges";}
						if (itemtxt->dwCode == D2TXTCODE('x4x ')) {
							uber_Sunstone = true;     //return "Sunstone of the Gods";}
							uber_Sunstone_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}
						if (itemtxt->dwCode == D2TXTCODE('yq8 '))  uber_Zakarum = true;       //return "Zakarum's Ear";}
						if (itemtxt->dwCode == D2TXTCODE('t@t ')) {
							uber_Demonsbane = true;  //return "Demonsbane";}
							uber_Demonsbane_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}

						if (itemtxt->dwCode == D2TXTCODE('k@k '))  hero_book = true;      //return "Book of Lies";}
						if (itemtxt->dwCode == D2TXTCODE('az! '))  hero_Fool=true; //return "Fool's Gool";}//	fool
						if (itemtxt->dwCode == D2TXTCODE('aq! ')) {
							hero_Silver = true;      //return "Silver Seal of Ureh";}	
							hero_Silver_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}
						if (itemtxt->dwCode == D2TXTCODE('ab! ')) {
							hero_Astrogha = true;     //return "Astrogha's Venom Stinger";}
							hero_Astrogha_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}

						if (wcsstr(wItemName,L"The Ancient Repositories") != NULL)  ultimative_Ancient = true;//ancident
						if (wcsstr(wItemName,L"Brachyceran's") != NULL)  ultimative_Brachyceran = true;//ancident//	bracy
						if (wcsstr(wItemName,L"The Sleep") != NULL)  ultimative_Slee = true;//ancident//	bracy//	the sleep
						if (itemtxt->dwCode == D2TXTCODE('law '))  ultimative_Soulkeeper = true;    // return "Soul of Kabraxis";}	//soulkeeper
						if (itemtxt->dwCode == D2TXTCODE('bxa ')) {
							ultimative_Soul = true;    // return "Soul of Kabraxis";}	
							ultimative_Soul_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
							}
						if (itemtxt->dwCode == D2TXTCODE('##. ')) {
							ultimative_Uldyssian = true;    //return "ULdyssian's Sword";}
							ultimative_Uldyssian_trophy = (D2Common::GetUnitStat(pItem,219,0) !=0);
						}
						if (wcsstr(wItemName,L"Trader's Chest") != NULL)  ultimative_Chest = true; //chest
						
						if (itemtxt->dwCode == D2TXTCODE('x01 ')) { challenge_class=true;}//return "Sacred Sunstone";}
						if (itemtxt->dwCode == D2TXTCODE('x02 ')) { challenge_class=true;}//return "Shadow Vortex";}
						if (itemtxt->dwCode == D2TXTCODE('x03 ')) { challenge_class=true;}//return "Worldstone Orb";}
						if (itemtxt->dwCode == D2TXTCODE('x04 ')) { challenge_class=true;}//return "Caoi Dulra Fruits (Druid)";}
						if (itemtxt->dwCode == D2TXTCODE('x05 ')) { challenge_class=true;}//return "Soulstone Shard (NecroMancer)";}
						if (itemtxt->dwCode == D2TXTCODE('x06 ')) { challenge_class=true;}///return "Eye of Divinity (Paladin)";}
						if (itemtxt->dwCode == D2TXTCODE('x07 ')) { challenge_class=true;}//return "Nexus Crystal (Sorceress)";}
						if (itemtxt->dwCode == D2TXTCODE('x#x ')) { challenge_sunstone=true;}//return "Sunstone of Elements";}
						if (wcsstr(wItemName,L"Harkon Crystal") != NULL) { challenge_harkon=true;}//harkon

		}
	}
}



char *nnnchar[7] = {"Amazon","Sorceress","NecroMancer","Paladin","BarBarian","Druid","Assasin"};
bool DrawRectangle_CharacterStat(){
	UnitAny* pPlayer = D2Client::GetPlayerUnit();
	BnetData* pData = (*D2Launch::BnData);	
	int nNegRes[3] = {0, -50, -100};

	if(viewplayerstate==0){
	srand(GetTickCount());
	changecolor = rand()%8+1;

	RECT pRect;
	pRect.left = 270;
	pRect.top = 495;
	pRect.right = 395;
	pRect.bottom = 515;
	D2DrawRectFrame_STUB(&pRect);
	D2Gfx::DrawRectangle(pRect.left,pRect.top,pRect.right,pRect.bottom,0x00,255);
	
	WideTexthookDraw(InGame,200, 157, 0, 1, Left, L"ÿc0[ %d ]",baseStr);
	WideTexthookDraw(InGame,200, 220, 0, 1, Left, L"ÿc0[ %d ]",baseDex);
	WideTexthookDraw(InGame,200, 307, 0, 1, Left, L"ÿc0[ %d ]",baseVit);
	WideTexthookDraw(InGame,200, 367, 0, 1, Left, L"ÿc0[ %d ]",baseEne);


	WideTexthookDraw(InGame,100, 395, 0, 1, Left, L"ÿc0Total Points[ ÿc3%d ÿc0]",baseStr+baseDex+baseVit+baseEne-85);


	WideTexthookDraw(InGame,280, 510, changecolor, 1, Left, L"%s",d2locale_kor ? L"     캐릭상태 보기  ": L"Character Status");
	WideTexthookDraw(InGame,130, 510, 0, 1, Left, L"ÿc%s%s",PlayerBuff ? "1" : "5",d2locale_kor? L"플레이어 버프":L"Player Buff");	
	}

	if (viewplayerstate == 1){
			D2Gfx::DrawRectangle(80,62,398,490,0x00,255);	

			int viewplayerstate_ypos = 80;
			TexthookDraw(InGame, 90, viewplayerstate_ypos, 4, 0, Left, "%s : %s (%s)",d2locale_kor?"이름":"Name",pPlayer->pPlayerData->szName,nnnchar[pData->nCharClass]);
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 0, 0, Left, "%s : %d",d2locale_kor?"레벨":"Level",D2Common::GetUnitStat(pPlayer,12,0));
			viewplayerstate_ypos += 15;
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 1, 0, Left, "%s : %d %% / %d %% (Total  = %d %%)",d2locale_kor?"불 저항력":"Fire Resist",D2Common::GetUnitStat(pPlayer,39,0)+nNegRes[pData->nDifficulty],D2Common::GetUnitStat(pPlayer,40,0)+75,D2Common::GetUnitStat(pPlayer,39,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 3, 0, Left, "%s : %d %% / %d %% (Total  = %d %%)",d2locale_kor?"얼음 저항력":"Cold Resist",D2Common::GetUnitStat(pPlayer,43,0)+nNegRes[pData->nDifficulty],D2Common::GetUnitStat(pPlayer,44,0)+75,D2Common::GetUnitStat(pPlayer,43,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 9, 0, Left, "%s : %d %% / %d %% (Total  = %d %%)",d2locale_kor?"전기 저항력":"Lightning Resist",D2Common::GetUnitStat(pPlayer,41,0)+nNegRes[pData->nDifficulty],D2Common::GetUnitStat(pPlayer,42,0)+75,D2Common::GetUnitStat(pPlayer,41,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 2, 0, Left, "%s : %d %% / %d %% (Total  = %d %%)",d2locale_kor?"독 저항력":"Poison Resist",D2Common::GetUnitStat(pPlayer,45,0)+nNegRes[pData->nDifficulty],D2Common::GetUnitStat(pPlayer,46,0)+75,D2Common::GetUnitStat(pPlayer,45,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 8, 0, Left, "%s : %d %% / %d %% (Total  = %d %%)",d2locale_kor?"마법 저항력":"Magic Resist",D2Common::GetUnitStat(pPlayer,37,0)+nNegRes[pData->nDifficulty],D2Common::GetUnitStat(pPlayer,38,0)+75,D2Common::GetUnitStat(pPlayer,37,0));
			viewplayerstate_ypos += 15;
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 0, 0, Left, "%s : %d %%",d2locale_kor?"강한 타격":"Crushing Blow",D2Common::GetUnitStat(pPlayer,136,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 0, 0, Left, "%s : %d %%",d2locale_kor?"크리티컬 공격":"Deadly Strike",D2Common::GetUnitStat(pPlayer,141,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 0, 0, Left, "%s : %d %%",d2locale_kor?"상처 악화":"Open Wounds",D2Common::GetUnitStat(pPlayer,135,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 0, 0, Left, "%s : %d %%",d2locale_kor?"적 둔화":"Slow Target",D2Common::GetUnitStat(pPlayer,150,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 0, 0, Left, "%s : %d %%",d2locale_kor?"적 둔화(근거리)":"Slow Melee Target",D2Common::GetUnitStat(pPlayer,376,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 1, 0, Left, "%s : %d %%",d2locale_kor?"라이프 흡수":"Life Leech",D2Common::GetUnitStat(pPlayer,60,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 3, 0, Left, "%s : %d %%",d2locale_kor?"마나 흡수":"Mana Leech",D2Common::GetUnitStat(pPlayer,62,0));
			viewplayerstate_ypos += 15;
		    TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 7, 0, Left, "%s : %d %%",d2locale_kor?"물리 저항력":"Damage Reduce",D2Common::GetUnitStat(pPlayer,36,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 8, 0, Left, "%s : %d %%",d2locale_kor?"공격 속도":"Attack Speed",D2Common::GetUnitStat(pPlayer,93,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 11, 0, Left, "%s : %d %%",d2locale_kor?"캐스팅 속도":"Fast Cast Rate",D2Common::GetUnitStat(pPlayer,105,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 9, 0, Left, "%s : %d %%",d2locale_kor?"경직 회복 속도":"Fast Hit Recovery",D2Common::GetUnitStat(pPlayer,99,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, "%s : %d %%",d2locale_kor?"블럭 속도":"Fast Block Rate",D2Common::GetUnitStat(pPlayer,102,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 12, 0, Left, "%s : %d %%",d2locale_kor?"달리기/걷기":"Run/Walk",D2Common::GetUnitStat(pPlayer,96,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 12, 0, Left, "%s : %d %%",d2locale_kor?"이속 퍼센트":"VelocityPercent",D2Common::GetUnitStat(pPlayer,67,0));
			viewplayerstate_ypos += 15;
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 10, 0, Left, "+%d %% %s",D2Common::GetUnitStat(pPlayer,121,0),d2locale_kor?"악마(데몬)에게 주는 데미지":"Damage To Demons");
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, "+%d %% %s",D2Common::GetUnitStat(pPlayer,122,0),d2locale_kor?"언데드에게 주는 데미지":"Damage To Undead");
			WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 3, 0, Left, L"%s: %d %%,ÿc4%s: %d %% ",d2locale_kor?L"매직 찬스":L"Magic Find",D2Common::GetUnitStat(pPlayer,80,0),d2locale_kor?L"적에게 얻는 골드량":L"Gold Find",D2Common::GetUnitStat(pPlayer,79,0));
			D2Gfx::DrawRectangle(345,495,395,520,0x00,255);
			WideTexthookDraw(InGame,355, 510, 0, 1, Left, L"%s",d2locale_kor ? L" 다음": L"Next");	
	}

	if (viewplayerstate == 2) { //상태 2번
			D2Gfx::DrawRectangle(80,62,398,490,0x00,255);
			int viewplayerstate_ypos = 65;
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 8, 0, Left, "+%d %% %s",D2Common::GetUnitStat(pPlayer,485,0),d2locale_kor?"스펠데미지(에너지 팩터)":"Energy Factor To Spell Damage");
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 7, 0, Left, "+%d %% %s",D2Common::GetUnitStat(pPlayer,409,0),d2locale_kor?"버프/디버프/얼음상태 지속":"Bonus to Buff/DeBuff/Cold Skill Duration");
			viewplayerstate_ypos += 14;
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 1, 0, Left, "-%d %% %s",(int)D2Common::GetUnitStat(pPlayer,333,0),d2locale_kor?"불 저항력(적) 감소":"To Enemy Fire Resistance");
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 3, 0, Left, "-%d %% %s",(int)D2Common::GetUnitStat(pPlayer,335,0),d2locale_kor?"얼음 저항력(적) 감소":"To Enemy Cold Resistance");
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 9, 0, Left, "-%d %% %s",(int)D2Common::GetUnitStat(pPlayer,334,0),d2locale_kor?"전기 저항력(적) 감소" :"To Enemy Lightning Resistance");
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 2, 0, Left, "-%d %% %s",(int)D2Common::GetUnitStat(pPlayer,336,0),d2locale_kor?"독 저항력(적) 감소":"To Enemy Poison Resistance");
			viewplayerstate_ypos += 14;
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 1, 0, Left, "%s : %d ~ %d",d2locale_kor?"불 데미지":"Fire Damage",(int)D2Common::GetUnitStat(pPlayer,48,0),(int)D2Common::GetUnitStat(pPlayer,49,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 3, 0, Left, "%s : %d ~ %d",d2locale_kor?"얼음 데미지":"Cold Damage",(int)D2Common::GetUnitStat(pPlayer,54,0),(int)D2Common::GetUnitStat(pPlayer,55,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 9, 0, Left, "%s : %d ~ %d",d2locale_kor?"전기 데미지":"Lightning Damage",(int)D2Common::GetUnitStat(pPlayer,50,0),(int)D2Common::GetUnitStat(pPlayer,51,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 2, 0, Left, "%s : %u ~ %u",d2locale_kor?"독 데미지":"Poison Damage",(DWORD)D2Common::GetUnitStat(pPlayer,57,0),(DWORD)D2Common::GetUnitStat(pPlayer,58,0));
			viewplayerstate_ypos += 14;
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 0, 0, Left, "+%d %% %s",D2Common::GetUnitStat(pPlayer,470,0),d2locale_kor?"소환물 데미지 추가":"Bonus to Summoned Minion Damage");
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 0, 0, Left, "+%d %% %s",D2Common::GetUnitStat(pPlayer,444,0),d2locale_kor?"소환물 라이프 추가":"Bonus to Summoned Minion Life");
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 0, 0, Left, "+%d %% %s",D2Common::GetUnitStat(pPlayer,487,0),d2locale_kor?"소환물 저항력 추가":"Bonus to Summoned Minion Resistance");
			viewplayerstate_ypos += 14;
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 2, 0, Left, "%s : %d %%",d2locale_kor?"독 지속시간 감소":"Poison Length Reduced by",D2Common::GetUnitStat(pPlayer,110,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 8, 0, Left, "%s : %d %%",d2locale_kor?"저주 지속시간 감소":"Curse Length Reduced by",D2Common::GetUnitStat(pPlayer,109,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 9, 0, Left, "%s : %.1f sec",d2locale_kor?"스턴 시간":"Stun Length",(float)D2Common::GetUnitStat(pPlayer,66,0)/25);
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 7, 0, Left, "%s : %d %%",d2locale_kor?"상점 가격 인하":"Reduces All Vendor Prices",D2Common::GetUnitStat(pPlayer,87,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 1, 0, Left, "%s : +%d ( %s %.1f %s )",d2locale_kor?"라이프 회복 속도":"Replenish Life",D2Common::GetUnitStat(pPlayer,74,0),d2locale_kor?"1초당":"1 Sec",(float)((25*D2Common::GetUnitStat(pPlayer,74,0)/256)),d2locale_kor?"라이프 회복":"Life Replenish");
			viewplayerstate_ypos += 10;
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 16 , 0, 0, Left, "%s : %d%%,%s : %d%%,%s : %d%%",d2locale_kor?"근거리 회피":"Dodge",D2Common::GetUnitStat(pPlayer,338,0),d2locale_kor?"원거리 회피":"Avoid",D2Common::GetUnitStat(pPlayer,339,0),d2locale_kor?"이동중 회피":"Evade",D2Common::GetUnitStat(pPlayer,340,0));
			viewplayerstate_ypos += 10;
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 4, 0, Left, "%s: +%d %%",d2locale_kor?"무기 데미지 상승":"Enhanced Weapon Damage",D2Common::GetUnitStat(pPlayer,25,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 4, 0, Left, "%s : +%d %%",d2locale_kor?"캐릭터 디펜 증가":"Total Character Defense Plus",D2Common::GetUnitStat(pPlayer,171,0));
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 3, 0, Left, "%d %% %s",D2Common::GetUnitStat(pPlayer,131,0),d2locale_kor?"데미지 반사":"Damage Reflected");
			TexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 9, 0, Left, "+%d %s , +%d %s (Total = +%d Skills)",D2Common::GetUnitStat(pPlayer,127,0),d2locale_kor?"모든 스킬":"All Skills",D2Common::GetUnitStat(pPlayer,83,0),d2locale_kor?"캐릭터 스킬":"Class Skill",(D2Common::GetUnitStat(pPlayer,127,0)+D2Common::GetUnitStat(pPlayer,83,0)));
			D2Gfx::DrawRectangle(345,495,395,520,0x00,255);
			WideTexthookDraw(InGame,355, 510, 0, 1, Left, L"%s",d2locale_kor ? L" 다음": L"Next");
					
	}
	if (viewplayerstate == 3) { //상태 2번
		 summon_Glorious = false;
		summon_Spirit = false;
		summon_Scroll = false;
		summon_Crystalline = false;
		summon_Legacy = false;
		summon_Worldstone = false;
		summon_Butcher = false;
		summon_Visions = false;
		summon_Moon = false;

		dengeon_Cold = false;
		dengeon_Laser = false;
		dengeon_Spirit = false;
		dengeon_Idol = false;
		dengeon_Horazon = false;
		dengeon_Optical = false;
		dengeon_Azmodan = false;


		uber_Eternal = false;
		uber_Six = false;
		uber_Kurast = false;
		uber_Sunstone = false;
		uber_Zakarum = false;
		uber_Demonsbane = false;

		hero_book = false;
		hero_Fool = false;
		 hero_Silver = false;
		 hero_Astrogha = false;

		ultimative_Ancient = false;
		ultimative_Brachyceran = false;
		ultimative_Slee = false;
		ultimative_Soulkeeper = false;
		ultimative_Soul = false;
		ultimative_Uldyssian = false;
		ultimative_Chest = false;

		challenge_class = false;
		challenge_sunstone = false;
		challenge_harkon = false;


				FindCharm(); // 참 읽기
				D2Gfx::DrawRectangle(80,62,398,490,0x00,255);
				int viewplayerstate_ypos = 65;
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 8, 0, Left, L"SUMMONING Uber");
				viewplayerstate_ypos += 10;
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sGlorious Book of Median ÿc5| ÿc%sSpirit of Damnation",summon_Glorious? summon_Glorious_trophy?"1":"4" : "5",summon_Spirit? "4" : "5");
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sScroll of Kings ÿc5| ÿc%sCrystalline Flame Medallion",summon_Scroll? "4" : "5",summon_Crystalline? summon_Crystalline_trophy? "8":"4" : "5");
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sLegacy of Blood ÿc5| ÿc%sWorldstone Key ÿc5| ÿc%sButcher's Tooth",summon_Legacy? summon_Legacy_trophy? "8":"4" : "5",summon_Worldstone? "4" : "5",summon_Butcher? "4" : "5");
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sVisions of Akarat ÿc5| ÿc%sMoon of the Spider",summon_Visions? summon_Visions_trophy? "8":"4" : "5",summon_Moon? summon_Moon_trophy? "8":"4" : "5");
					viewplayerstate_ypos += 10;

				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 0, 0, Left, L"DUNGEON Uber");
				viewplayerstate_ypos += 10;
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sCold Fusion Schematics ÿc5| ÿc%sLaser Focus Crystal", dengeon_Cold? dengeon_Cold_trophy? "8":"4" : "5",dengeon_Laser? "4" : "5");
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sSpirit Trance Herb ÿc5| ÿc%sIdol of Vanity ÿc5| ÿc%sHorazon's Focus",dengeon_Spirit? dengeon_Spirit_trophy?"8":"4" : "5",dengeon_Idol?dengeon_Idol_trophy ?"8":"4" : "5",dengeon_Horazon? "4" : "5");
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sOptical Detector ÿc5| ÿc%sAzmodan's Heart",dengeon_Optical? "4" : "5",dengeon_Azmodan? dengeon_Azmodan_trophy?"8":"4" : "5");

				viewplayerstate_ypos += 10;
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 1, 0, Left, L"UBERQUEST Level");
				viewplayerstate_ypos += 10;
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sEternal Bone Pile ÿc5| ÿc%sSix Angel Bag ÿc5| ÿc%sKurast 3k Hammer",uber_Eternal? uber_Eternal_trophy?"8":"4" : "5",uber_Six? "4" : "5",uber_Kurast? "4" : "5");
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sSunstone of the Gods ÿc5| ÿc%sZakarum's Ear ÿc5| ÿc%sDemonsbane",uber_Sunstone?uber_Sunstone_trophy?"8":"4"  : "5",uber_Zakarum? "4" : "5",uber_Demonsbane? uber_Demonsbane_trophy ?"8":"4": "5");
				viewplayerstate_ypos += 10;
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 2, 0, Left, L"HEROIC Level");
				viewplayerstate_ypos += 10;

				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sAstrogha's Venom Stinger ÿc5| ÿc%sSilver Seal of Ureh",hero_Astrogha? hero_Astrogha_trophy?"8":"4" : "5",hero_Silver? hero_Silver_trophy?"8":"4" : "5");
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sBook of Lies ÿc5| ÿc%sFool's Gold ÿc5",hero_book? "4" : "5",hero_Fool? "4" : "5");
				viewplayerstate_ypos += 10; 
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 8, 0, Left, L"ULTIMATIVE");
				viewplayerstate_ypos += 10;
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sThe Ancient Repositories ÿc5| ÿc%sBrachyceran's Token",ultimative_Ancient? "4" : "5",ultimative_Brachyceran? "4" : "5");
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sThe Sleep ÿc5| ÿc%sSoulkeeper(V7 Only) ÿc5| ÿc%sSoul Of Kabraxis",ultimative_Slee? "4" : "5",ultimative_Soulkeeper? "4" : "5",ultimative_Soul? ultimative_Soul_trophy?"8":"4" : "5");
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sUldyssian's Sword ÿc5| ÿc%sTrader's Chest",ultimative_Uldyssian? ultimative_Uldyssian_trophy ?"8":"4" : "5",ultimative_Chest? "4" : "5");
				viewplayerstate_ypos += 5;
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 3, 0, Left, L"CHALLENGES");
				viewplayerstate_ypos += 5;
				WideTexthookDraw(InGame, 90, viewplayerstate_ypos += 15 , 5, 0, Left, L"ÿc%sClassCharm ÿc5| ÿc%sSunstone-Elements ÿc5| ÿc%sHarkon Crystal",challenge_class? "4" : "5",challenge_sunstone? "4" : "5",challenge_harkon? "4" : "5");
			D2Gfx::DrawRectangle(345,495,395,520,0x00,255);
			WideTexthookDraw(InGame,355, 510, 0, 1, Left, L"%s",d2locale_kor ? L" 닫기": L"Close");
	}


	return true;
}

wchar_t *szRuneName[57] = {L"", L"El",L"Eld",L"Tir",L"Nef",L"Eth",L"Ith",L"Tal",L"Ral",L"Ort",L"Thul",L"Amn",L"Sol",L"Shael",L"Dol",L"Hel",L"Io"
	,L"Lum",L"Ko",L"Fal",L"Lem",L"Pul",L"Um",L"Mal",L"Ist",L"Gul",L"Vex",L"Ohm",L"Lo",L"Sur",L"Ber",L"Jah",L"Cham",L"Zod",L"Sha",L"Lah",L"Kur"
	,L"lx",L"Thur",L"Nas",L"Ath",L"Kra",L"Vith",L"No",L"Yul",L"Thai",L"Rha",L"Xar",L"Nih",L"Lai",L"On",L"Taha",L"Ghal",L"Qor",L"Krys",L"Auhe",L"Sha'ad"};

bool DrawRectangle_ItemSetting() {

	srand(GetTickCount());

	D2Gfx::DrawRectangle(80,62,398,490,0x00,255);
	WideTexthookDraw(InGame,195, 75, 0, 1, Left, L"%s",d2locale_kor ? L"아이템 세팅": L"Item Setting");

	WideTexthookDraw(InGame,90, 52, rand()%8+1, 1, Left, L"%s",d2locale_kor ? L"[ 포럼 방문 하기 ]": L"[ Visit Forum ]");	

	WideTexthookDraw(InGame,90, 95, 0, 1, Left, L"%s :  ÿc2<<ÿc0<ÿc;  %d  ÿc0>ÿc2>>",d2locale_kor ? L"ÿc;룬ÿc0번호": L"ÿc;Rÿc0une",runenumber);
	WideTexthookDraw(InGame,225, 95, 0, 1, Left, L"ÿc;%s Rune ÿc0%s",szRuneName[runenumber],d2locale_kor?L"보다 낮은 룬 숨김":L"Hide lower than the runes");
	
	WideTexthookDraw(InGame,90, 110, 0, 1, Left, L"%s",d2locale_kor ? L"ÿc8차ÿc0암": L"ÿc8Cÿc0harm");
	WideTexthookDraw(InGame,105, 125, 0, 6, Left, L"ÿc%ssmall Charm",smallcharm ? "3" : "5");
	WideTexthookDraw(InGame,105, 135, 0, 6, Left, L"ÿc%sLarge Charm",largecharm ? "3" : "5");
	WideTexthookDraw(InGame,105, 145, 0, 6, Left, L"ÿc%sGrand Charm",grandcharm ? "3" : "5");

	WideTexthookDraw(InGame,90, 160, 0, 1, Left, L"%s", L"ÿc1Pÿc0otions");
	WideTexthookDraw(InGame,105, 175, 0, 6, Left, L"ÿc%sHealingPotion",healingpotion ? "1" : "5");
	WideTexthookDraw(InGame,105, 185, 0, 6, Left, L"ÿc%sManaPotion",manapotion ? "3" : "5");

	WideTexthookDraw(InGame,90, 200, 0, 1, Left, L"ÿc5Sÿc0ockets");
	WideTexthookDraw(InGame,105, 215, 0, 6, Left, L"ÿc%sSocket = [3]",socket3 ? "9" : "5");
	WideTexthookDraw(InGame,105, 225, 0, 6, Left, L"ÿc%sSocket = [4]",socket4 ? "9" : "5");
	WideTexthookDraw(InGame,105, 235, 0, 6, Left, L"ÿc%sSocket = [5]",socket5 ? "9" : "5");
	WideTexthookDraw(InGame,105, 245, 0, 6, Left, L"ÿc%sSocket = [6]",socket6 ? "9" : "5");

	/*
	WideTexthookDraw(InGame,225, 95, 0, 1, Left, L"ÿc;%s Rune ÿc0%s",szRuneName[runenumber],d2locale_kor?L"보다 낮은 룬 숨김":L"Hide lower than the runes");
	WideTexthookDraw(InGame,90, 110, 0, 1, Left, L"%s",d2locale_kor ? L"ÿc8쉬ÿc0라인": L"ÿc8Sÿc0hrine");

	WideTexthookDraw(InGame,105, 120, 0, 6, Left, L"ÿc%sCreepy Shrine ÿc0(Strength)",CreepyShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 130, 0, 6, Left, L"ÿc%sAbandoned Shrine ÿc;(Dexterity)",AbandonedShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 140, 0, 6, Left, L"ÿc%sQuiet Shrine ÿc1(Vitality)",QuietShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 150, 0, 6, Left, L"ÿc%sEerie Shrine ÿc3(Energy)",EerieShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 160, 0, 6, Left, L"ÿc%sWeird Shrine ÿc0(Maximum Damage)",WeirdShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 170, 0, 6, Left, L"ÿc%sGloomy Shrine ÿc5(Uninterruptable)",GloomyShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 180, 0, 6, Left, L"ÿc%sTainted Shrine ÿc0(Minion Damage)",TaintedShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 190, 0, 6, Left, L"ÿc%sFascinating Shrine ÿc1(Fire Spell)",FascinatingShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 200, 0, 6, Left, L"ÿc%sOrnate Shrine ÿc9(Lightning Spell)",OrnateShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 210, 0, 6, Left, L"ÿc%sSacred Shrine ÿc3(Cold Spell)",SacredShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 220, 0, 6, Left, L"ÿc%sShimmering Shrine ÿc2(Poison Spell)",ShimmeringShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 230, 0, 6, Left, L"ÿc%sSpiritual Shrine ÿc4(Getting Magic Item)",SpiritualShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 240, 0, 6, Left, L"ÿc%sMagical Shrine ÿc1(Minion Life)",MagicalShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 250, 0, 6, Left, L"ÿc%sHidden Shrine ÿc8(Reanimate Monster)",HiddenShrine ? "8" : "5");
	WideTexthookDraw(InGame,105, 260, 0, 6, Left, L"ÿc%sStone Shrine ÿc0(All ÿc1Reÿc3siÿc9stÿc2s)",StoneShrine ? "8" : "5");
	*/

	WideTexthookDraw(InGame,90, 260, 0, 1, Left, L"%s",d2locale_kor ? L"ÿc4퍼ÿc0팩트 ÿc2보ÿc0석": L"ÿc4Pÿc0erfect ÿc2Gÿc0em");

	WideTexthookDraw(InGame,105, 275, 0, 6, Left, L"ÿc%sFlawless Amethyst",PerfectAmethyst ? ";" : "5");
	WideTexthookDraw(InGame,105, 285, 0, 6, Left, L"ÿc%sFlawless Topaz",PerfectTopaz ? "9" : "5");
	WideTexthookDraw(InGame,105, 295, 0, 6, Left, L"ÿc%sFlawless Sapphire",PerfectSapphire ? "3" : "5");
	WideTexthookDraw(InGame,105, 305, 0, 6, Left, L"ÿc%sFlawless Diamond",PerfectDiamond ? "0" : "5");
	WideTexthookDraw(InGame,105, 315, 0, 6, Left, L"ÿc%sFlawless Emerald",PerfectEmerald ? "2" : "5");
	WideTexthookDraw(InGame,105, 325, 0, 6, Left, L"ÿc%sFlawless Ruby",PerfectRuby ? "1" : "5");
	WideTexthookDraw(InGame,105, 335, 0, 6, Left, L"ÿc%sFlawless Skull",PerfectSkull ? "7" : "5");
	WideTexthookDraw(InGame,105, 345, 0, 6, Left, L"ÿc%sFlawless Ghostly",PerfectGhostly ? "8" : "5");
		/*
	WideTexthookDraw(InGame,105, 360, 0, 6, Left, L"ÿc%sPerfect Rainbowstone",PerfectRainbowStone ? "8" : "5");
	WideTexthookDraw(InGame,105, 370, 0, 6, Left, L"ÿc%sPerfect Onyx",PerfectOnyx ? "0" : "5");
	WideTexthookDraw(InGame,105, 380, 0, 6, Left, L"ÿc%sPerfect Bloodstone",PerfectBloodStone ? "1" : "5");
	WideTexthookDraw(InGame,105, 390, 0, 6, Left, L"ÿc%sPerfect Turquoise",PerfectTurquoise ? ":" : "5");
	WideTexthookDraw(InGame,105, 400, 0, 6, Left, L"ÿc%sPerfect Amber",PerfectAmber ? "4" : "5");
	*/

	WideTexthookDraw(InGame,90, 360, 0, 1, Left, L"ÿc9%s",d2locale_kor? L"추ÿc0가설정":L"Eÿc0tc");
	WideTexthookDraw(InGame,105, 375, 0, 6, Left, L"ÿc%sEnemy Corpse",viewCorpse ? "3" : "5");
	WideTexthookDraw(InGame,105, 385, 0, 6, Left, L"ÿc%sSell Price",SellPriceCheck ? "8" : "5");

	WideTexthookDraw(InGame,90, 400, 0, 1, Left, L"ÿc1Iÿc0tem");

	WideTexthookDraw(InGame,105, 430, 0, 6, Left, L"ÿc%sMagic Jewel ÿc0| ÿc%sRare Jewel",ViewJewelMagic ? "3" : "5",ViewJewelRare? "9":"5");
	WideTexthookDraw(InGame,105, 440, 0, 6, Left, L"ÿc%sRare Amulet ÿc0| ÿc%sRare Ring",RareAmulet ? "9":"5",RareRing ? "9":"5");
	
	WideTexthookDraw(InGame,90, 460, 0, 1, Left, L"ÿc3%s",d2locale_kor? L"치ÿc0킨 설정":L"Cÿc0hicken");
	WideTexthookDraw(InGame,105, 470, 0, 6, Left, L"ÿc%SChicken Life ÿc0= ÿc2<  ÿc4%d%%  ÿc2>",QooMapChicken?"1":"5",QooMapChickenLifePercent);
	WideTexthookDraw(InGame,105, 480, 0, 6, Left, L"ÿc%sChicken -> Town ÿc0| ÿc%sChicken -> LeftGame",QooMapChicken ? QooMapChickenTown? ";":"5":"5",QooMapChicken ?QooMapChickenLeftGame ? "9":"5":"5");

	D2Gfx::DrawRectangle(345,495,395,520,0x00,255);
	WideTexthookDraw(InGame,355, 510, 0, 1, Left, L"%s",d2locale_kor ? L" 저장": L"Save");	

	return true;
}

DWORD WINAPI ClickGotoBeltThreadFunction(LPVOID pvoid){
	UnitAny *pInventoryItem = NULL;

		while (FindPotion(STORAGE_INVENTORY) != NULL){
			if((pInventoryItem = FindPotion(STORAGE_INVENTORY)) != NULL) {
							BYTE aPacket[5] = {0};
							aPacket[0] = 0x63;
							*(DWORD*)&aPacket[1] = pInventoryItem->dwUnitId;

							if (belt_potion_id == pInventoryItem->dwUnitId){
								belt_potion_id = 0;
								CloseHandle(ClickGotoBeltThread);
								ClickGotoBeltThread= NULL;
							}else belt_potion_id = pInventoryItem->dwUnitId;

							SendGamePacket(aPacket,sizeof(aPacket));
							
			}
		}
			return 0;
}


BYTE BuyOilPacket[17] = {0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // 조합 클릭

DWORD WINAPI BuyoilThreadFunction(LPVOID pvoid){
	int NowGold = D2Common::GetUnitStat(D2Client::GetPlayerUnit(),14,0) + D2Common::GetUnitStat(D2Client::GetPlayerUnit(),15,0);

	while(NowGold >= 1000 && BuyOilStart) {
			*(DWORD*)&BuyOilPacket[1] = RenewalOilNpc;
			*(DWORD*)&BuyOilPacket[5] = RenewalOilID;
			*(DWORD*)&BuyOilPacket[13] = RenewalOilCost;

			SendGamePacket(BuyOilPacket,sizeof(BuyOilPacket));
	}

	CloseHandle(BuyoilThread);
	BuyoilThread = NULL;
	return 0;
}

DWORD CubeActionNum=0;
DWORD AutoTransmuteTimer;

void Cube_Action(){
		   UnitAny *pShrineItem = NULL;
		   UnitAny *pOilPotion = NULL;

		   UnitAny *pUnit = D2Client::GetPlayerUnit();

		   if ((pOilPotion = FindTownPortalScroll(STORAGE_CUBE)) != NULL) {
			    if (Dragon_RareRenwal == true && AutoTransmute == true){
					 SendGamePacket(ClickTransmute, sizeof(ClickTransmute));	//조합 클릭
				}
		   }
		   else {
					 if (CubeActionNum==1){
						if (pUnit && pUnit->pInventory->pCursorItem != NULL){
	
						BYTE aPacket[17] = {0};
						aPacket[0] = 0x18;
						*(LPDWORD)&aPacket[1] = pUnit->pInventory->pCursorItem->dwUnitId;
						*(LPDWORD)&aPacket[5] = 5;
						*(LPDWORD)&aPacket[9] = 5;
						*(LPDWORD)&aPacket[13] = STORAGE_CUBE; // 인벤토리
						SendGamePacket(aPacket,sizeof(aPacket));
						CubeActionNum = 2; //다음단계 ( 마지막임)

						}
					}

					if(GetTickCount() >= Cube_Timer + 350 && CubeActionNum==0) {// 리뉴 오일 위치 클릭!
							mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0); 
							mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
							CubeActionNum = 1; // 담단계로 이동
					}

					if ((pShrineItem = FindShrine(STORAGE_CUBE)) != NULL || (pOilPotion = FindRenewalOil(STORAGE_CUBE)) != NULL) {
						 SendGamePacket(ClickTransmute, sizeof(ClickTransmute));	//조합 클릭
						 UnitAny *pOilPotion = NULL;
							if((pOilPotion = SearchItem(STORAGE_CUBE)) != NULL) 
								{
								InventoryLayout* pLayout = NULL;
								pLayout = (InventoryLayout*)D2Client::CubeLayout;

								int x = pOilPotion->pItemPath->dwPosX;
								int y = pOilPotion->pItemPath->dwPosY;

								*D2Client::CursorHoverX = x;
								*D2Client::CursorHoverY = y;

								x = pLayout->Left + x * pLayout->SlotPixelWidth + 10; 
								y = pLayout->Top + y * pLayout->SlotPixelHeight + 40;
								SetCursorPos(rect.left+x,rect.top+y); //템 위치

								AutoTransmuteTimer = GetTickCount();
								if (CubeActionNum==2) CubeActionNum = 3;
							}
					}

					if(GetTickCount() >= AutoTransmuteTimer + 100 && CubeActionNum==3) {// 리뉴 오일 위치 클릭!	
							Cube_Start = false;
							if (AutoTransmute == true) CtrlAction(D2Gfx::GetHwnd());

					}
		   }

}

char createfoldername[260] = "";

char *replaceAll(char *s, const char *olds, const char *news) {
  char *result, *sr;
  size_t i, count = 0;
  size_t oldlen = strlen(olds); if (oldlen < 1) return s;
  size_t newlen = strlen(news);


  if (newlen != oldlen) {
    for (i = 0; s[i] != '\0';) {
      if (memcmp(&s[i], olds, oldlen) == 0) count++, i += oldlen;
      else i++;
    }
  } else i = strlen(s);


  result = (char *) malloc(i + 1 + count * (newlen - oldlen));
  if (result == NULL) return NULL;


  sr = result;
  while (*s) {
    if (memcmp(s, olds, oldlen) == 0) {
      memcpy(sr, news, newlen);
      sr += newlen;
      s  += oldlen;
    } else *sr++ = *s++;
  }
  *sr = '\0';

  return result;
}

void addhtmltag(char *tag){
	FILE *tagfile;
	tagfile = fopen(createfoldername,"a");

	if (tagfile != NULL){
		fputs(tag,tagfile);
		fclose(tagfile);
	}
}

void CreateDir(char* Path)
{
    char DirName[256];  //생성할 디렉초리 이름
    char* p = Path;     //인자로 받은 디렉토리
    char* q = DirName;  
 
    while(*p)
    {
        if (('\\' == *p) || ('/' == *p))   //루트디렉토리 혹은 Sub디렉토리
        {
            if (':' != *(p-1))
            {
                CreateDirectory(DirName, NULL);
            }
        }
        *q++ = *p++;
        *q = '\0';
    }
    CreateDirectory(DirName, NULL);  
}

void SaveFile(char *source,UnitAny *pItem){
	FILE *savefile;
	char *temp = source;
	char html_imagetag[512] = "";
	char *ttt = "</td></tr>";
	wchar_t pitemTemp[1024] = L"";
	wchar_t imagefilename[128] = L"";
	wchar_t *pdest;
	int substr_num;

	D2Client::GetItemNameString(pItem,pitemTemp,sizeof(pitemTemp));

	if (wcsstr(pitemTemp,L"\n")) {
		if (wcsstr(pitemTemp,L"(")) {
			pdest = wcsstr(pitemTemp,L"("); // 티어나 sacred 바로전까지
			substr_num = (int)(pdest - pitemTemp +1);

			if (substr_num >=2) {// 길이가 0이지 않으면 가끔오류남
			wcsncpy(imagefilename,pitemTemp,substr_num-2);
			wsprintf(html_imagetag,"<tr><td width =100 Align=Center><img src=%simage\\item\\%s.jpg onError = 'badimage(this)'></td><td width = 400 Align=Center valign=middle>",szPath,replaceAll(UnicodeToAnsi(imagefilename)," ","%20"),szPath);
			}
			else{
				wcsncpy(imagefilename,pitemTemp,substr_num);
				wsprintf(html_imagetag,"<tr><td width =100 Align=Center><img src=%simage\\item\\%s.jpg onError = 'badimage(this)'></td><td width = 400 Align=Center valign=middle>",szPath,replaceAll(UnicodeToAnsi(imagefilename)," ","%20"),szPath);
				}
		}
		else {
			pdest = wcsstr(pitemTemp,L"\n"); //ring이나 아뮬등등
			substr_num = (int)(pdest - pitemTemp +1);
			wcsncpy(imagefilename,pitemTemp,substr_num-1);
			wsprintf(html_imagetag,"<tr><td width =100 align=center><img src=%simage\\item\\%S.jpg onError = 'badimage(this)'></td><td width = 400 Align=Center valign=middle>",szPath,imagefilename,szPath);
		}

	}
	else {
		if (wcsstr(pitemTemp,L"[")) {
			pdest = wcsstr(pitemTemp,L"["); // 티어나 sacred 바로전까지
			substr_num = (int)(pdest - pitemTemp +1);
			wcsncpy(imagefilename,pitemTemp,substr_num-2);
			wsprintf(html_imagetag,"<tr><td width =100 Align=Center><img src=%s\\image\\item\\%s.jpg onError = 'badimage(this)'></td><td width = 400 Align=Center valign=middle>",szPath,replaceAll(UnicodeToAnsi(imagefilename)," ","%20"),szPath);
		}else {
		wsprintfW(imagefilename,L"%s",pitemTemp);
		wsprintf(html_imagetag,"<tr><td width =100 Align=Center><img src=%s\\image\\item\\%S.jpg onError = 'badimage(this)'></td><td width = 400 Align=Center valign=middle>",szPath,imagefilename,szPath);	
		}
	}

	srand(GetTickCount());
	D2Client::PrintGameString(imagefilename,rand()%8+1);
					savefile = fopen (createfoldername,"a");
					if (savefile != NULL){
					fputs(html_imagetag,savefile);
					temp = replaceAll(temp,"yc3","<font color=#5050ac>");
					temp = replaceAll(temp,"yc1","<font color=#b04434>");
					temp = replaceAll(temp,"yc2","<font color=#18f200>");
					temp = replaceAll(temp,"yc4","<font color=#948064>");
					temp = replaceAll(temp,"yc5","<font color=#505050>");
					temp = replaceAll(temp,"yc0","<font color=white>");
					temp = replaceAll(temp,"yc9","<font color=#d8b864>");
					temp = replaceAll(temp,"yc8","<font color=#d08420>");
					temp = replaceAll(temp,"yc;","<font color=#a420fc>"); //보라색
					temp = replaceAll(temp,"yc:","<font color=#186408>"); //다크그린
					temp =  replaceAll(temp,"\n","<br>");
					fputs(temp,savefile);
					fputs(ttt,savefile);
					fclose(savefile);
					}

}

static wchar_t itemdesc[1500] = L"";

void FindItemHtml(int Storage){

	for(UnitAny* pItem = D2Common::GetFirstItemFromInventory(D2Client::GetPlayerUnit()->pInventory); pItem; pItem = D2Common::GetNextItemFromInventory(pItem)) {
				if (pItem->pItemData->InventoryLocation == Storage) {
					InventoryLayout* pLayout = NULL;
					if (Storage == STORAGE_CUBE) pLayout = (InventoryLayout*)D2Client::CubeLayout;
					if (Storage == STORAGE_STASH) pLayout = (InventoryLayout*)D2Client::StashLayout;
					if (Storage == STORAGE_INVENTORY) pLayout = (InventoryLayout*)D2Client::PlayerLayout;
					int x = pItem->pItemPath->dwPosX;
					int y = pItem->pItemPath->dwPosY;

					*D2Client::CursorHoverX = x;
					*D2Client::CursorHoverY = y;

					x = pLayout->Left + x * pLayout->SlotPixelWidth + 20; 
					y = pLayout->Top + y * pLayout->SlotPixelHeight + 40;

					SetCursorPos(rect.left+x,rect.top+y); //리뉴오일 위치

					Sleep(50);

					if (ReadProcessBYTES(GetCurrentProcess(), 0x6F9A9E58,itemdesc, 1500)){ //아이템 속성 얻어오기  D2Win::ItemDescBuffer 랑 같음

						if (wcslen(itemdesc) !=0){
							SaveFile(UnicodeToAnsi(itemdesc),pItem);					
						}
					}
					
				}

	}
}

DWORD WINAPI SaveItemThreadFunction(LPVOID pvoid){
	GetWindowRect(D2Gfx::GetHwnd(),&rect);

	BnetData* pData = *D2Launch::BnData;
	char htmlTemp[1024] = "";

	wsprintf(createfoldername,"%s",pData->szAccountName);

	if (strlen(createfoldername) >= 2) wsprintf(createfoldername,"%s\\[BattleNet] - %s",szPath,pData->szAccountName);
	else wsprintf(createfoldername,"%s\\[Single Player]",szPath);

	CreateDir(createfoldername);
	wsprintf(createfoldername,"%s\\%s - Lv%d (%s).htm",createfoldername,pData->szPlayerName,D2Common::GetUnitStat(D2Client::GetPlayerUnit(),12,0),nnnchar[pData->nCharClass]);
	FILE *file;
	file = fopen(createfoldername,"w");
	if (file != NULL) {
		wsprintf(htmlTemp,"<html>\n<body bgcolor=black>\n<font color = white><Center>\n<script language='javascript'>function badimage(myimg){this.src='%simage\\item\\no_image.jpg';}</script>",szPath);
		fputs(htmlTemp,file);
		fclose(file);
	}

	UnitAny *pFindCube = NULL;
	if ((pFindCube = FindCube()) != NULL){
			InventoryLayout* pLayout = NULL;
			pLayout = (InventoryLayout*)D2Client::PlayerLayout;

			int x = pFindCube->pItemPath->dwPosX;
			int y = pFindCube->pItemPath->dwPosY;

			*D2Client::CursorHoverX = x;
			*D2Client::CursorHoverY = y;

			x = pLayout->Left + x * pLayout->SlotPixelWidth + 10; 
			y = pLayout->Top + y * pLayout->SlotPixelHeight + 40;

			SetCursorPos(rect.left+x,rect.top+y); //리뉴오일 위치
			Sleep (100);

			mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0); 
			mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	}

	Sleep (100);

	addhtmltag("<br><Font Size = 30 Color = orange>Cube </font><br><table border=1>");

	FindItemHtml(STORAGE_CUBE);

	Sleep(100);
	if (GetUIVarSTUB(UI_CUBE)) D2Client::SetUIVar(UI_CUBE,1,0); //  큐브 닫기	

	addhtmltag("</table><br><Font Size = 30 Color = White>Inventory </font><br><table border=1>");

	FindItemHtml(STORAGE_INVENTORY);

	addhtmltag("</table><br><Font Size = 30 Color = Green>Stash </font><br><table border=1>");

	FindItemHtml(STORAGE_STASH);

	addhtmltag("</table>");
	ShellExecute(NULL, "open", createfoldername, NULL, "", SW_SHOW);
	CloseHandle(SaveItemThread);
	SaveItemThread = NULL;
	return 0;
}