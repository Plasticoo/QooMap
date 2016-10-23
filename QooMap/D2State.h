#pragma once

enum ClientGameState
{
	ClientStateNull, ClientStateMenu, ClientStateInGame, ClientStateBusy
};

struct Module
{	
	union {
		HMODULE hModule;
		DWORD dwBaseAddress;
	};
	DWORD _1;
	char szPath[MAX_PATH];
};    

ClientGameState ClientState(void);
extern WNDPROC OldWndProc;
LONG WINAPI WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SendGamePacket(unsigned char* Packet, unsigned PacketSize);
void GamePacketReceived_I(void);
DWORD __fastcall HandleGamePacketReceived(BYTE* pPacket, DWORD dwSize);
void OnGamePacketSentSTUB();

//오토파티
void AutoParty();
UnitAny* __fastcall ClickParty_ASM(RosterUnit* RosterUnit, unsigned Mode);
DWORD __fastcall GetUIVarSTUB(DWORD UI);
extern DWORDLONG Phys; //물리메모리
UnitAny* FindItem(DWORD Code, int nLoc);
UnitAny* SearchItem(int Storage);
UnitAny* FindShrine(int Storage);
UnitAny* FindPortal(int Storage);
UnitAny* FindPotion(int Storage);
UnitAny* FindRenewalOil(int Storage);
UnitAny* FindTownPortalScroll(int Storage);
UnitAny* FindCube();
extern bool fakeselect;
extern unsigned int myportalnum;
extern bool AlwaysTop;
extern bool myportalonoff;
extern unsigned int d2res_extend_width;
extern unsigned int d2res_extend_height;
extern bool d2res_on;
extern char pFile[65535];
extern bool PlayerBuff;
extern bool RareRing;
extern bool Dragon_RareRenwal;
extern char szPath[260];

void ViewInventorySTUB1();
void ViewInventorySTUB2();
void ViewInventorySTUB3();
void CtrlAction(HWND hWnd);
bool ReadProcessBYTES(HANDLE hProcess, DWORD lpAddress, void* buf, int len);
extern bool inGame;

extern UnitAny *viewunit;
extern HINSTANCE instance;
extern bool viewunitstatusok;
extern BYTE MenuAllUniqueItem;
extern unsigned int MenuAllUnique;
extern bool EffectView;
extern bool OnlyUniqueOK;
extern bool UniqueItem_Off_TierItem;
extern bool viewCorpse;
extern bool joingame;
extern bool FastTownPortal;
extern bool ScrollMapON;
extern bool SellPriceCheck;
extern unsigned int AlwaysAltDown;
extern BYTE TakeCorpse[9];
extern bool DrawSetting;
extern bool AutoEatCorpse;
extern bool QooMapChicken;
extern bool QooMapChickenTown;
extern bool QooMapChickenLeftGame;
extern unsigned int QooMapChickenLifePercent;
extern bool DieOK;
extern bool Cube_Start;
extern DWORD CubeActionNum;
extern DWORD Cube_Timer;
extern RECT rect;
extern bool myitemview;//아이템 set/unique/ring/등 표시할때
extern bool d2locale_kor;
extern bool d2extend_ok;
extern bool d2extend_test;
extern bool togglelight;
void d2gfx_10060 (DWORD unknown);
extern bool QooMapNoticeOK;
extern wchar_t QooMapNotice[150];
extern DWORD NoticeTime;
void __fastcall NewResolution(DWORD width,DWORD height);
extern bool pAutoParty;
extern bool DropNotify;
extern HWND WindowTitlehWnd;
extern bool LockClick;
extern int playercorpsex;
extern int playercorpsey;
extern unsigned int belt_potion_id;
extern BYTE ClickTransmute[7]; // 조합 클릭
extern DWORD RenewalOilID;
extern DWORD RenewalOilCost;
extern DWORD RenewalOilNpc;
extern bool AutoTransmute;
extern bool BuyOilStart;
extern bool smallcharm;
extern bool largecharm;
extern bool grandcharm;
extern bool healingpotion;
extern bool manapotion;

extern bool socket3;
extern bool socket4;
extern bool socket5;
extern bool socket6;

extern HANDLE SaveItemThread;
extern HANDLE BuyoilThread;
extern HANDLE ClickGotoBeltThread;

DWORD WINAPI SaveItemThreadFunction(LPVOID pvoid);
DWORD WINAPI BuyoilThreadFunction(LPVOID pvoid); 
DWORD WINAPI ClickGotoBeltThreadFunction(LPVOID pvoid); 

void ItemBasicStatSTUB();
void FakeClick(int x,int y);
void Delay(DWORD dwMillisecond);

enum {  	UNIT_TYPE_PLAYER = 0,		// Player
		UNIT_TYPE_MONSTER,		// Monster or Npc
		UNIT_TYPE_OBJECT,		// Static object(wp, stash, etc)
		UNIT_TYPE_MISSILE,		// Missle
		UNIT_TYPE_ITEM,			// Item
		UNIT_TYPE_ROOMTILE };		// Room tile


enum {	PLAYER_MODE_DEATH = 0,				// death
		PLAYER_MODE_STAND_OUTTOWN,			// standing outside town
		PLAYER_MODE_WALK_OUTTOWN,			// walking outside town
		PLAYER_MODE_RUN,					// running
		PLAYER_MODE_BEING_HIT,				// being hit
		PLAYER_MODE_STAND_INTOWN,			// standing inside town
		PLAYER_MODE_WALK_INTOWN,			// walking outside town
		PLAYER_MODE_ATTECK1,				// attacking 1
		PLAYER_MODE_ATTACK2,				// attacking 2
		PLAYER_MODE_BLOCK,					// blocking
		PLAYER_MODE_CAST,					// casting spell
		PLAYER_MODE_THROW,					// throwing
		PLAYER_MODE_KICK,					// kicking (assassin)
		PLAYER_MODE_USESKILL1,				// using skill 1
		PLAYER_MODE_USESKILL2,				// using skill 2
		PLAYER_MODE_USESKILL3,				// using skill 3
		PLAYER_MODE_USESKILL4,				// using skill 4
		PLAYER_MODE_DEAD,					// dead
		PLAYER_MODE_SEQUENCE,				// sequence
		PLAYER_MODE_BEING_KNOCKBACK };		// being knocked back

enum StorageBuffer {
	STORAGE_INVENTORY	= 0x00,
	STORAGE_EQUIP		= 0x01,
	STORAGE_TRADE		= 0x02,
	STORAGE_CUBE		= 0x03,
	STORAGE_STASH		= 0x04,
	STORAGE_BELT		= 0x05,
	STORAGE_NULL		= 0xFF

};


#define NODEPAGE_STORAGE	1
#define NODEPAGE_BELTSLOTS	2
#define NODEPAGE_EQUIP		3

////////////////////////////////////////////////////////////////////
// NPC Mode Definition
////////////////////////////////////////////////////////////////////
enum {  NPC_MODE_DEATH = 0,		// NPC death
		NPC_MODE_STAND,			// NPC standing still
		NPC_MODE_WALK,			// NPC walking
		NPC_MODE_BEING_HIT,		// NPC getting hit
		NPC_MODE_ATTACK1,		// NPC attacking 1
		NPC_MODE_ATTACK2,		// NPC attacking 2
		NPC_MODE_BLOCK,			// NPC blocking
		NPC_MODE_CAST,			// NPC casting spell skill
		NPC_MODE_USESKILL1,		// NPC using skill 1
		NPC_MODE_USESKILL2,		// NPC using skill 2
		NPC_MODE_USESKILL3,		// NPC using skill 3
		NPC_MODE_USESKILL4,		// NPC using skill 4
		NPC_MODE_DEAD,			// NPC dead
		NPC_MODE_BEING_KNOCKBACK,	// NPC being knocked back
		NPC_MODE_SEQUENCE,		// NPC sequence
		NPC_MODE_RUN };			// NPC running

enum 
{  
	ITEM_MODE_INV_STASH_CUBE_STORE,		// Item inven stash cube store
	ITEM_MODE_EQUIPPED,					// Item equipped self or merc
	ITEM_MODE_IN_BELT,					// Item in belt
	ITEM_MODE_ON_GROUND,				// Item on ground
	ITEM_MODE_ON_CURSOR,				// Item on cursor
	ITEM_MODE_BEING_DROPPED,			// Item being dropped
	ITEM_MODE_SOCKETED_IN_ITEM 			// Item socketed in item
};

#define	ITEM_QUALITY_INFERIOR			0x01
#define	ITEM_QUALITY_NORMAL				0x02
#define	ITEM_QUALITY_SUPERIOR			0x03
#define	ITEM_QUALITY_MAGIC				0x04
#define	ITEM_QUALITY_SET				0x05
#define	ITEM_QUALITY_RARE				0x06
#define	ITEM_QUALITY_UNIQUE				0x07
#define	ITEM_QUALITY_CRAFT				0x08

#define UI_INVENTORY			0x01
#define UI_CHARACTER			0x02
#define UI_MINISKILL			0x03
#define UI_SKILL				0x04
#define UI_CHAT					0x05
#define UI_NPCMENU				0x08	
#define UI_MAINMENU				0x09
#define UI_AUTOMAP				0x0a
#define UI_CONFIG				0x0b
#define UI_NPCSHOP				0x0c
#define UI_ALTDOWN				0x0d			
#define UI_ANVIL				0x0e
#define UI_QUEST				0x0f
#define UI_QUESTLOG				0x11
#define UI_STATUSAREA			0x12
#define UI_WPMENU				0x14
#define UI_MINIPANEL			0x15
#define UI_PARTY				0x16
#define UI_TRADE				0x17
#define UI_MSGS					0x18
#define UI_STASH				0x19
#define UI_CUBE					0x1a
#define UI_BELT					0x1f
#define UI_HELP					0x21
#define UI_MERC					0x24
#define UI_SCROLL				0x25


#define	STAT_IAS					93		// IAS
#define	STAT_FASTERRUNWALK			96		// faster run/walk
#define	STAT_FASTERHITRECOVERY		99		// faster hit recovery
#define	STAT_FASTERBLOCK			102		// faster block rate
#define	STAT_FASTERCAST				105		// faster cast rate
#define	STAT_VELOCITYPERCENT		67		// effective run/walk


#define PARTY_NOT_IN_PARTY 0x00
#define PARTY_IN_PARTY 0x01
#define PARTY_INVITED_YOU 0x02
#define PARTY_INVITED_BY_YOU 0x04

#define clickParty_Loot 0
#define clickParty_Accept 1
#define clickParty_Invite 2
#define clickParty_Leave 3

#define PLAYER_TYPE_HARDCORE	4
#define PLAYER_TYPE_EXPAC		32
#define PLAYER_TYPE_LADDER		64
