#include "EnteringLevelText.h"
#include "D2Structs.h"
#include "Offset.h"
#include "Vector.h"
#include "ScreenHooks.h"

#define UBER_SUMMON 2;

void __declspec(naked) DrawEnteringLevelTextSTUB(){
	__asm
	{
		CALL D2Client::DrawEnteringLevelText;
		CALL Uber_EnteringLevelText;
		RETN;
	}
}

bool UberQuest = true;   //우버퀘스트 정보보기
wchar_t UberQuestType[50] = L"";
wchar_t UberQuestBoss[50] = L"";
wchar_t UberQuestDrop[50] = L"";
wchar_t UberQuestTrophy[20] = L"";
unsigned int dwLevelNo =0;

void UberQuestInfo(wchar_t *type,wchar_t *boss,wchar_t *drop,wchar_t *trophy){
	if (wcslen(drop) != wcslen(UberQuestDrop)){
		wsprintfW(UberQuestType,L"%s",type);
		wsprintfW(UberQuestBoss,L"%s",boss);
		wsprintfW(UberQuestDrop,L"%s",drop);
		wsprintfW(UberQuestTrophy,L"%s",trophy);
	}
}

bool Uber_EnteringLevelText(){ // patch.h 에서 enteringtext주석 제거
	UnitAny* pPlayer = D2Client::GetPlayerUnit();
	dwLevelNo = GetUnitLevel(*D2Client::PlayerUnit)->dwLevelNo;

	if (D2Common::IsLevelATown(pPlayer->pAct->pRoom1->pRoom2->pLevel->dwLevelNo)){
		if (fakeselect == true) *D2Client::AutomapOn = true;
	}

		if (pPlayer->dwAct == 0){ //액트1
			if (dwLevelNo == MAP_A1_INNER_CLOISTER){
				UberQuestInfo(L"ÿc2Summoning UberQuest",L"ÿc1Boss : ÿc0 Malthael,Imperius",L"ÿc0Drop :ÿc4 Crystalline Flame Medallion",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A1_THE_SECRET_COW_LEVEL){
				UberQuestInfo(L"ÿc2Heroic Level",L"ÿc1Boss : ÿc0 Bull Prince Rodeo",L"ÿc0Drop :ÿc4 Fool's Gold",L"");
				return true;}
			if (dwLevelNo == MAP_A1_MAUSOLEUM){
				UberQuestInfo(L"ÿc2Heroic Level",L"ÿc1Boss : ÿc0 Astrogha",L"ÿc0Drop :ÿc4 Astrogha's Venom Stinger",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A1_CRYPT){
				UberQuestInfo(L"ÿc1UberLevel",L"ÿc1Boss : ÿc0 Juris Khan,ÿc;MarcoNecroX (SecretBoss)",L"ÿc4 Eternal Bone Pile,Creators Hand",L"");
				return true;}
			if (dwLevelNo == MAP_A1_PIT_LEVEL_2){
				UberQuestInfo(L"ÿc1Dungeon UberQuest",L"ÿc1Boss : ÿc0 Infernal Machine",L"ÿc0Drop :ÿc4 Optical Detector",L"");
				return true;}
			if (dwLevelNo == MAP_A1_CAVE_LEVEL_2){
				UberQuestInfo(L"ÿc1UberLevel",L"ÿc1Boss : ÿc0 Assur",L"ÿc0Drop :ÿc4 Demonsbane",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A1_UNDERGROUND_PASSAGE_LEVEL_2){
				UberQuestInfo(L"ÿc1Dungeon UberQuest",L"ÿc1Boss : ÿc0 Inarius",L"ÿc0Drop :ÿc4 Idol of Vanity",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A1_FORGOTTEN_TOWER){
				UberQuestInfo(L"ÿc2Summoning UberQuest",L"ÿc1Boss : ÿc0 Bartuc",L"ÿc0Drop :ÿc4 Legacy of Blood",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A1_HOLE_LEVEL_2){
				UberQuestInfo(L"ÿc2Summoning UberQuest",L"ÿc1Boss : ÿc0 Butcher",L"ÿc0Drop :ÿc4 The Butcher's Tooth",L"");
				return true;}
			if (dwLevelNo == MAP_A1_TOWER_CELLAR_LEVEL_5){ //미니게임
				UberQuestInfo(L"",L"ÿc1Mini Game1",L"ÿc0Mission : ÿc4Kill Countess",L"");
				return true;}
			if (dwLevelNo == MAP_A1_TAMOE_HIGHLAND){
				UberQuestInfo(L"",L"ÿc1Mini Game2 Level",L"ÿc0Mission : ÿc4Startled Witch...",L"");
				return true;}

		}
		
		if(pPlayer->dwAct == 1){ 
			if (dwLevelNo == MAP_A2_ROCKY_WASTE){
				UberQuestInfo(L"",L"ÿc1Mini Game2 Level",L"ÿc0Mission : ÿc4Startled Witch... ",L"");
				return true;}
			if (dwLevelNo == MAP_A2_STONY_TOMB_LEVEL_2){
				UberQuestInfo(L"ÿc1UberLevel",L"ÿc0Drop :ÿc4 (N)ÿc:A/B/D/G/E Brain",L"ÿc0Drop :ÿc4 (H)ÿc;Taha,Ghal,Qor Rune",L"");
				return true;}
			if (dwLevelNo == MAP_A2_STONY_TOMB_LEVEL_1){
				UberQuestInfo(L"ÿc2Heroic Level",L"ÿc1Boss : ÿc0 Quov Tsin",L"ÿc0Drop :ÿc4 Silver Seal of Ureh",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A2_VALLEY_OF_SNAKES){
				UberQuestInfo(L"ÿc2Summoning UberQuest",L"ÿc1Boss : ÿc0 Tal Rasha",L"ÿc0Drop :ÿc4 Scroll of Kings",L"");
				return true;}
			if (dwLevelNo == MAP_A2_ANCIENT_TUNNELS){
				UberQuestInfo(L"ÿc1Dungeon UberQuest",L"ÿc1Boss : ÿc0 Creature of Flame",L"ÿc0Drop :ÿc4 Horazon's Focus",L"");
				return true;}
			if (dwLevelNo == MAP_A2_HALLS_OF_THE_DEAD_LEVEL_3){
				UberQuestInfo(L"ÿc1UberLevel",L"ÿc1Boss : ÿc0 Elemental Priestesses",L"ÿc0Drop :ÿc4 (H)Sunstone of the Gods",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A2_MAGGOT_LAIR_LEVEL_3){
				UberQuestInfo(L"ÿc1Dungeon UberQuest",L"ÿc1Boss : ÿc0 Brachyceran",L"ÿc0Drop :ÿc4 Brachyceran's Token",L"");
				return true;}
			if (dwLevelNo == MAP_A2_TAL_RASHAS_CHAMBER){
				UberQuestInfo(L"ÿc2Summoning UberQuest",L"ÿc1Boss : ÿc0 Uldyssian Memory, Edrem",L"ÿc0Drop :ÿc4 Uldyssian's Sword",L"ÿc8 2% Trophy");
				return true;}
	}
		
	if(pPlayer->dwAct == 2){  //액3
			if (dwLevelNo == MAP_A3_SWAMPY_PIT_LEVEL_3){
				UberQuestInfo(L"ÿc1Dungeon Uber/ÿc2Summon Uber",L"ÿc1Boss : ÿc0 Death Projector,ÿc;Brother Laz",L"ÿc0Drop :ÿc4 Laser Focus Crystal,Glorious Book of Median",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A3_FORGOTTEN_RELIQUARY){
				UberQuestInfo(L"ÿc1Dungeon UberQuest",L"ÿc1Boss : ÿc0 Azmodan",L"ÿc0Drop :ÿc4 Azmodan's Heart",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A3_RUINED_FANE){
				UberQuestInfo(L"ÿc1Dungeon UberQuest",L"ÿc1Boss : ÿc0 Primus",L"ÿc0Drop :ÿc4 Spirit Trance Herb",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A3_DISUSED_FANE){
				UberQuestInfo(L"ÿc1Dungeon UberQuest",L"ÿc1Boss : ÿc0 MCS",L"ÿc0Drop :ÿc4 Cold Fusion Schematics",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A3_ARCHNID_LAIR){
				UberQuestInfo(L"ÿc1UberLevel",L"ÿc1Boss : ÿc0 Ennead Necromancers",L"ÿc0Drop :ÿc4 Class Orb/(H)Hammer of the Taan Judges",L"");
				return true;}
			if (dwLevelNo == MAP_A3_KURAST_CAUSEWAY){
				UberQuestInfo(L"ÿc2Summoning UberQuest",L"ÿc1Boss : ÿc0 Akarat",L"ÿc0Drop :ÿc4 Visions of Akarat",L"ÿc8 2% Trophy");
				return true;}
			if (dwLevelNo == MAP_A3_SPIDER_CAVERN){
				UberQuestInfo(L"ÿc1Ultimative UberLevel",L"ÿc1Boss : ÿc0 ???",L"ÿc0Drop :ÿc4 ???",L"");
				return true;}
			if (dwLevelNo == MAP_A3_FLAYER_DUNGEON_LEVEL_3){
				UberQuestInfo(L"ÿc1Ultimative UberLevel",L"ÿc1Boss : ÿc;Startled Witch",L"ÿc0Drop :ÿc4 The Sleep",L"");
				return true;}
			if (dwLevelNo == MAP_A3_SEWERS_LEVEL_2){
				UberQuestInfo(L"ÿc1Ultimative Uber/Summon Level",L"ÿc1Boss : ÿc4Chrysaor/ÿc;Chrysaor (Secret Boss)",L"ÿc0Drop :ÿc4 Trader's Chest/Unknown Skull",L"");
				return true;}

	}
	if(pPlayer->dwAct == 3){ //액4
			if (dwLevelNo == MAP_A4_THE_CHAOS_SANCTUARY){
				UberQuestInfo(L"ÿc2Summoning UberQuest",L"ÿc1Boss : ÿc0 Kabraxis",L"ÿc0Drop :ÿc4 Soul of Kabraxis",L"ÿc8 2% Trophy");
				return true;}
	}
	if(pPlayer->dwAct == 4){ //액5
			if (dwLevelNo == MAP_A5_ICY_CELLAR){
				UberQuestInfo(L"ÿc2Heroic Level",L"ÿc1Boss : ÿc0 Belial",L"ÿc0Drop :ÿc4 Book of Lies",L"");
				return true;}
			if (dwLevelNo == MAP_A5_THRONE_OF_DESTRUCTION){
				UberQuestInfo(L"",L"ÿc1Mini Game3 Level",L"ÿc0Mission : ÿc4SShardspawn... ",L"");
				return true;}
			if (dwLevelNo == MAP_A5_DRIFTER_CAVERN){
				UberQuestInfo(L"ÿc1UberLevel",L"ÿc1Boss : ÿc0 Malic,Lucion",L"ÿc0Drop :ÿc4 Six Angel Bag,Sunless Crystal Bird",L"");
				return true;}
			if (dwLevelNo == MAP_A5_INFERNAL_PIT){
				UberQuestInfo(L"ÿc1UberLevel",L"ÿc1Boss : ÿc0 Zakarum's Avatars",L"ÿc0Drop :ÿc4 Zakarum's Ear(1/12 Chance)",L"");
				return true;}
			if (dwLevelNo == MAP_A5_PIT_OF_ACHERON){
				UberQuestInfo(L"ÿc1UberLevel",L"ÿc1Boss : ÿc0 Dark Star Dragon",L"ÿc0Mission :ÿc4 Upgrade Charm",L"");
				return true;}
			if (dwLevelNo == MAP_A5_ABADDON){
				UberQuestInfo(L"ÿc1Dungeon UberQuest",L"ÿc1Boss : ÿc0 Lilith",L"ÿc0Mission :ÿc4 Upgrade Charm",L"");
				return true;}
			if (dwLevelNo == MAP_A5_ARREAT_SUMMIT){
				UberQuestInfo(L"ÿc2Summoning UberQuest",L"ÿc1Boss : ÿc0 Koth",L"ÿc0Drop :ÿc4 Sacred Worldstone Key",L"");
				return true;}
			if (dwLevelNo == MAP_A5_NIHLATHAKS_TEMPLE){
				UberQuestInfo(L"ÿc2Summoning UberQuest",L"ÿc1Boss : ÿc0 Lord Aldric Jitan",L"ÿc0Drop :ÿc4 Moon of the Spider (1/8 Chance)",L"ÿc8 2% Trophy");
				return true;}

	}

	return false;
}