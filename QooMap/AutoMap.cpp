#include "AutoMap.h"
#include "Maphack.h" //gameautomapdraw위해서
#include "D2State.h"
#include "Offset.h"
#include "ScreenHooks.h"

#define D2TXTCODE(x) ( ((x)&0xff)<<24 | ((x)>>8&0xff)<<16 | ((x)>>16&0xff)<<8 | ((x)>>24&0xff) )

#pragma warning ( disable: 4018 )

void __declspec(naked) DrawAutomap_II() //화면 오른편에 정보 표시
{
		__asm
		{
			PUSHAD;
			call GameAutomapDraw; //maphack.cpp에 있음
			POPAD;
			pop edi;
			pop esi;
			ret;
		
		}
}

int runenumber = 1; //ini읽어오는 룬번호
unsigned int monstercolor;
unsigned int xMapShake = 0;
unsigned int yMapShake = 0;

bool ViewJewelMagic;
bool ViewJewelRare;
bool ItemViewAll = false;
bool RareAmulet = false;
bool UniqueItem_Off_TierItem = false;

bool CreepyShrine;
bool AbandonedShrine;
bool QuietShrine;
bool EerieShrine;
bool WeirdShrine;
bool GloomyShrine;
bool TaintedShrine;
bool FascinatingShrine;
bool OrnateShrine;
bool SacredShrine;
bool ShimmeringShrine;
bool SpiritualShrine;
bool MagicalShrine;
bool HiddenShrine;
bool StoneShrine;

bool PerfectAmethyst;
bool PerfectTopaz;
bool PerfectSapphire;
bool PerfectEmerald;
bool PerfectRuby;
bool PerfectDiamond;
bool PerfectSkull;
bool PerfectRainbowStone;
bool PerfectOnyx;
bool PerfectBloodStone;
bool PerfectTurquoise;
bool PerfectAmber;

unsigned int ViewGold_Value;
wchar_t DropItemName[256] = L"";

bool PrintItemName(UnitAny *unit,wchar_t *ItemName,int Color){
			if (DropNotify){
				if (unit->dwMode == ITEM_MODE_BEING_DROPPED){
							wchar_t ColorTemp[10] = L"";

							//if (wcsstr(ItemName,L"Ring of the Five")) return false;

							if (Color !=0){
								wsprintfW(ColorTemp,L"ÿc%d",Color);
								//if (unit->pItemData->dwFlags &0x00000010) return true;
							}
							/*
							if (MenuAllUnique == 1 || MenuAllUnique == 3) {
								if (wcsstr(ItemName,L"(1)") || wcsstr(ItemName,L"(2)") 
								||wcsstr(ItemName,L"(3)") || wcsstr(ItemName,L"(4)")   
								||wcsstr(ItemName,L"(5)") ||wcsstr(ItemName,L"(6)"))
								return false;
							}
							*/

							wsprintfW(DropItemName,L"ÿc8Qÿc2oÿc4oÿc3Mÿc7aÿc9p : %s%s ÿc0Drop!!",ColorTemp,ItemName);
							if (wcsstr(ItemName,L"\n")) {D2Client::PrintGameString(L" ",0);D2Client::PrintGameString(L" ",0);}
							D2Client::PrintGameString(DropItemName,0);
							unit->dwMode = 3;
							return false;
				}
			}
		return true;
}

BOOL __fastcall InfravisionPatch(UnitAny *unit)
{
	if (!unit) return false;;

	ItemTxt *itemtxt = D2Common::GetItemText(unit->dwTxtFileNo);

	if (unit->dwType == UNIT_TYPE_MISSILE) {
		  if (D2Client::GetPlayerUnit()->dwUnitId == unit->dwOwnerId) {
					return (EffectView) ? true :false; //캐릭효과      
			}
	}

	if (unit->dwType == UNIT_TYPE_MONSTER ){
		if (unit->dwMode == NPC_MODE_DEAD  )  return (viewCorpse) ? true : false;
		} // 0x0c = dead

	if (unit->dwType == UNIT_TYPE_ITEM ) {
				wchar_t Automap_wItemName[256] = L"";

				D2Client::GetItemNameString(unit,Automap_wItemName,sizeof(Automap_wItemName));	
				
					if (wcsstr(Automap_wItemName,L"Rune")){
								char number[6] = "";
								wsprintf(number,"%c%c",D2Common::GetItemText(unit->dwTxtFileNo)->szCode[1],D2Common::GetItemText(unit->dwTxtFileNo)->szCode[2]);
								if (runenumber <=  atoi(number)) {PrintItemName(unit,Automap_wItemName,0); return false;}
					}
					if (Item_Gem(unit)) {PrintItemName(unit,Automap_wItemName,0); return false;}

					if (ViewJewelRare && unit->pItemData->dwQuality == ITEM_QUALITY_RARE) { if (wcsstr(Automap_wItemName,L"[jew]")) return false;}
					if (RareAmulet && unit->pItemData->dwQuality == ITEM_QUALITY_RARE){ if (wcsstr(Automap_wItemName,L"[amu]")) return false;}
					if (RareRing && unit->pItemData->dwQuality == ITEM_QUALITY_RARE){ if (wcsstr(Automap_wItemName,L"[rin]")) return false;}

					if (ViewJewelMagic && unit->pItemData->dwQuality == ITEM_QUALITY_MAGIC) { if (wcsstr(Automap_wItemName,L"[jew]")) return false;}
					/*
					
				    if (wcsstr(Automap_wItemName,L"Mystic Orb") && wcslen(Automap_wItemName) > 25) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Trophy")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (Item_Shrine(unit) != false) {PrintItemName(unit,Automap_wItemName,0); return false;}
					

					if (unit->pItemData->dwFlags & 0x400000 && wcsstr(Automap_wItemName,L"(Sacred)") &&unit->pItemData->dwQuality == ITEM_QUALITY_SUPERIOR) return false;


					if (Item_MagicJewel(unit) != false && unit->pItemData->dwQuality == ITEM_QUALITY_MAGIC) return false; //V
					if (Item_RareItem(unit) != false && unit->pItemData->dwQuality == ITEM_QUALITY_RARE) return false; //V
		
					if (itemtxt->dwCode == D2TXTCODE('leg ')) return false;
					if (itemtxt->dwCode == D2TXTCODE('#X1 ')) return false;
					if (itemtxt->dwCode == D2TXTCODE('#X2 ')) return false;
					if (itemtxt->dwCode == D2TXTCODE('#X3 ')) return false;
					if (itemtxt->dwCode == D2TXTCODE('#X4 ')) return false;
					if (itemtxt->dwCode == D2TXTCODE('#X5 ')) return false;
					if (itemtxt->dwCode == D2TXTCODE('#X6 ')) return false;
					if (itemtxt->dwCode == D2TXTCODE('qul ')) return false; // 아케인 크리스탈
					if (itemtxt->dwCode == D2TXTCODE('zkq ')) return false; // 시그넷
					if (itemtxt->dwCode == D2TXTCODE('dog ')) return false;
					if (itemtxt->dwCode == D2TXTCODE('box ')) return false; // 큐브
					if (wcsstr(Automap_wItemName,L"Sunless Crystal Bird")) {PrintItemName(unit,Automap_wItemName,0); return false; }
					if (wcsstr(Automap_wItemName,L"Lam Esen's Tome")) return false;
					if (wcsstr(Automap_wItemName,L"Scroll of Inifuss")) return false;
					if (wcsstr(Automap_wItemName,L"Gift Box [L")) return false;

					if (itemtxt->dwCode == D2TXTCODE('@## ')){PrintItemName(unit,Automap_wItemName,0); return false; } //  deity's bow 우버 보우
					if (itemtxt->dwCode == D2TXTCODE('r97 ')){PrintItemName(unit,Automap_wItemName,0); return false; } //  카브라시스 
					if (itemtxt->dwCode == D2TXTCODE('r98 ')){PrintItemName(unit,Automap_wItemName,0); return false; }//  카브라시스 
					if (itemtxt->dwCode == D2TXTCODE('r99 ')){PrintItemName(unit,Automap_wItemName,0); return false; } //  카브라시스 

					//엘릭서
					if (wcsstr(Automap_wItemName,L"Elixir of Experience")) return false;
					if (wcsstr(Automap_wItemName,L"경험치의 엘릭서")) return false;

					if (wcsstr(Automap_wItemName,L"Book [L1]")) return false;
					if (wcsstr(Automap_wItemName,L"Broken Crystal [L1]")) return false;
					if (wcsstr(Automap_wItemName,L"Cursed Crystal [L1]")) return false;

					if (wcsstr(Automap_wItemName,L"Signet of Gold")) return false;
					if (wcsstr(Automap_wItemName,L"Signet of Experience")) return false;


					if (wcsstr(Automap_wItemName,L"Signet of Learning")) return false;

					if (wcsstr(Automap_wItemName,L"Arcane Crystal")) return false; //아케인 크리스탈
					if (wcsstr(Automap_wItemName,L"Catalyst of Disenchantment")) return false; //
					if (wcsstr(Automap_wItemName,L"Catalyst of Learning")) return false; //


					if (wcsstr(Automap_wItemName,L"[L1][\\")) {PrintItemName(unit,Automap_wItemName,0); return false;}

					if (wcsstr(Automap_wItemName,L"Earth Stone [L1]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Fire Stone [L1]")){PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Ice Stone [L1]")){PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Magic Stone [L1]")){PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Light Stone [L1]")){PrintItemName(unit,Automap_wItemName,0); return false;}
				    if (wcsstr(Automap_wItemName,L"Poison Stone [L1]")){ PrintItemName(unit,Automap_wItemName,0); return false;}

					// 던크 시질
					if (itemtxt->dwCode == D2TXTCODE('l@l '))   return false;  // aaaa sigil
					if (itemtxt->dwCode == D2TXTCODE('m@m '))   return false; // bbbbb sigil
					if (itemtxt->dwCode == D2TXTCODE('o@o '))   return false;  // ccccc sigil
					if (itemtxt->dwCode == D2TXTCODE('p@p '))   return false; // dddddddd sigil
					if (itemtxt->dwCode == D2TXTCODE('q@q '))   return false;  // eeeeeee sigil

					if (itemtxt->dwCode == D2TXTCODE('2x5 '))   return false;    //"Alpha Brain";}
					if (itemtxt->dwCode == D2TXTCODE('2x6 '))  return false;    //"Beta Brain";}
					if (itemtxt->dwCode == D2TXTCODE('2x7 '))   return false; //"Gamma Brain";}
					if (itemtxt->dwCode == D2TXTCODE('2x8 '))   return false;  //"Delta Brain";}
					if (itemtxt->dwCode == D2TXTCODE('2x9 '))   return false;  //"Epsilon Brain";}

					if (itemtxt->dwCode == D2TXTCODE('!@1 ')){ PrintItemName(unit,Automap_wItemName,0); return false;} //"Evil Eye of the aaaaa";}
					if (itemtxt->dwCode == D2TXTCODE('!@2 ')){ PrintItemName(unit,Automap_wItemName,0); return false;} //"Evil Eye of the bbbbb";}
					if (itemtxt->dwCode == D2TXTCODE('!@3 ')) { PrintItemName(unit,Automap_wItemName,0); return false;}//"Evil Eye of the ccccc";}
					if (itemtxt->dwCode == D2TXTCODE('!@4 ')){ PrintItemName(unit,Automap_wItemName,0); return false;} //"Evil Eye of the ddddd";}
					if (itemtxt->dwCode == D2TXTCODE('!@5 ')) { PrintItemName(unit,Automap_wItemName,0); return false;} //"Evil Eye of the eeeee";}

					if (itemtxt->dwCode == D2TXTCODE('!@6 '))   return false; //"Dogma of aaaaa";}
					if (itemtxt->dwCode == D2TXTCODE('!@7 '))   return false; //"Dogma of bbbbb";}
					if (itemtxt->dwCode == D2TXTCODE('!@8 '))   return false; //"Dogma of ccccc";}
					if (itemtxt->dwCode == D2TXTCODE('!@9 '))   return false; //"Dogma of ddddd";}
					if (itemtxt->dwCode == D2TXTCODE('!@A '))   return false; //"Dogma of eeeee";}
					*/

					if (wcsstr(Automap_wItemName,L"[ma1]"))	return false; //super elixir
					if (wcsstr(Automap_wItemName,L"[ma2]"))	return false; //hero's cigarette
					if (wcsstr(Automap_wItemName,L"[ma3]"))	return false;  //holy drink
					if (wcsstr(Automap_wItemName,L"[g11]"))	return false; // monster's hand
					if (wcsstr(Automap_wItemName,L"[tch]"))	return false; // touch

					if (smallcharm) {if (wcsstr(Automap_wItemName,L"Small Charm")) return false;}
					if (largecharm) {if (wcsstr(Automap_wItemName,L"Large Charm"))	return false;}
					if (grandcharm) {if (wcsstr(Automap_wItemName,L"Grand Charm"))	return false;}
					if (wcsstr(Automap_wItemName,L"Medium Charm")) {PrintItemName(unit,Automap_wItemName,0); return false;}

					if (wcsstr(Automap_wItemName,L"[stt]")) {PrintItemName(unit,Automap_wItemName,0); return false;}

					if (healingpotion) {if (wcsstr(Automap_wItemName,L"Healing Potion [L")) return true;}
					if (manapotion) {if (wcsstr(Automap_wItemName,L"Mana Potion [L")) return true;}

					if (socket3) {if (wcsstr(Automap_wItemName,L"[Sockets = 3]")) {PrintItemName(unit,Automap_wItemName,5); return false;}}
					if (socket4) {if (wcsstr(Automap_wItemName,L"[Sockets = 4]")) {PrintItemName(unit,Automap_wItemName,5); return false;}}
					if (socket5) {if (wcsstr(Automap_wItemName,L"[Sockets = 5]")) {PrintItemName(unit,Automap_wItemName,5); return false;}}
					if (socket6) {if (wcsstr(Automap_wItemName,L"[Sockets = 6]")) {PrintItemName(unit,Automap_wItemName,5); return false;}}

					if (wcsstr(Automap_wItemName,L"Full Rejuvenation Potion"))	return false;
					

					if (wcsstr(Automap_wItemName,L"Glyph")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Elixir of")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					
					if (wcsstr(Automap_wItemName,L"[ol1]")) { return false;} //블랙 오일
					if (wcsstr(Automap_wItemName,L"Adamantium [")) { return false;}
					if (wcsstr(Automap_wItemName,L"Rubidium [")) { return false;}
					if (wcsstr(Automap_wItemName,L"Titanium [")) { return false;}
					if (wcsstr(Automap_wItemName,L"Chromium [")) { return false;}
					if (wcsstr(Automap_wItemName,L"Platinum [")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					//if (wcsstr(Automap_wItemName,L"Silver [")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					

					if (wcsstr(Automap_wItemName,L"[brz]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[scz]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[spe]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[fng]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[sol]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[jaw]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[eyz]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[hrn]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[hrt]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[tal]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[qll]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[flg]")) {PrintItemName(unit,Automap_wItemName,0); return false;}

					//소켓 아이템
					if (wcsstr(Automap_wItemName,L"Acidonus")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Indigo")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Swampex")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Crimson")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Alabaster")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Golden")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Obsidian")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"Nioben")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					
					//인형
					if (wcsstr(Automap_wItemName,L"[do1]")) {PrintItemName(unit,Automap_wItemName,0); return false;} //메피
					if (wcsstr(Automap_wItemName,L"[do2]")) {PrintItemName(unit,Automap_wItemName,0); return false;} //메피
					if (wcsstr(Automap_wItemName,L"[do3]")) {PrintItemName(unit,Automap_wItemName,0); return false;} //메피
					if (wcsstr(Automap_wItemName,L"[do4]")) {PrintItemName(unit,Automap_wItemName,0); return false;} //메피
					if (wcsstr(Automap_wItemName,L"[do6]")) {PrintItemName(unit,Automap_wItemName,0); return false;} //메피
					if (wcsstr(Automap_wItemName,L"[do5]")) {PrintItemName(unit,Automap_wItemName,0); return false;}
					if (wcsstr(Automap_wItemName,L"[do7]")) {PrintItemName(unit,Automap_wItemName,0); return false;} //메피
					if (wcsstr(Automap_wItemName,L"[do8]")) {PrintItemName(unit,Automap_wItemName,0); return false;} //메피
					if (wcsstr(Automap_wItemName,L"[do9]")) {PrintItemName(unit,Automap_wItemName,0); return false;} //메피

					if (unit->pItemData->dwQuality == ITEM_QUALITY_UNIQUE){ PrintItemName(unit,Automap_wItemName,4);}
					if (unit->pItemData->dwQuality == ITEM_QUALITY_SET) { PrintItemName(unit,Automap_wItemName,2);}
	

			if (OnlyUniqueOK == true){
				if (itemtxt->dwCode == D2TXTCODE('gld ')) { return D2Common::GetUnitStat(unit,14,0) >= ViewGold_Value ? false : true;} //골드가 5000이 넘는다면 보임		
				if (unit->pItemData->dwQuality == ITEM_QUALITY_INFERIOR) return true;
				if (unit->pItemData->dwQuality == ITEM_QUALITY_SUPERIOR) return true;
				if (unit->pItemData->dwQuality == ITEM_QUALITY_NORMAL) return true;
				if (unit->pItemData->dwQuality == ITEM_QUALITY_MAGIC) return true;
				if (unit->pItemData->dwQuality == ITEM_QUALITY_RARE) return true;
			}
			/*
			if (MenuAllUnique == 3){
				if (UniqueItem_Off_TierItem == false){
					if (wcsstr(Automap_wItemName,L"(Sacred)")) return false;

					if (unit->pItemData->dwQuality == ITEM_QUALITY_SET) return false;
					if (unit->pItemData->dwQuality == ITEM_QUALITY_UNIQUE) return false;
					return true;
				}	
			}
			*/
	}
	return false;
}



bool Mon_NameUber(int dwact,int TextNo){

	if (TextNo == 871) return true; // la

	if (TextNo == 882) return true; // la
	if (TextNo == 883) return true; // la
	if (TextNo == 884) return true; // la
	if (TextNo == 885) return true; // la

	if (TextNo == 899) return true; // pandora
	if (TextNo == 900) return true; // pandora
	if (TextNo == 901) return true; // pandora



	/*

		if (dwact == 0 ) {
			if (TextNo == 961) return true;     // return d2locale_kor? "인페르날 머신":"Infernal Machine";
			if (TextNo == 2049) return true;     // return "Volrath The Vile"; //레벨 챌린지 0
			if (TextNo == 971) return true;     // return  d2locale_kor? "부쳐":"The Butcher";
			//if (TextNo == 995) return  d2locale_kor? "이나리우스":"Inarius";
			if (TextNo == 1001) return true;     // return  d2locale_kor? "크리스탈 볼":"Crystall Ball";	
			if (TextNo == 1076) return true;     // return "Juris Khan, King of Ureh";
			if (TextNo == 1109) return true;     // return  d2locale_kor? "바르턱":"Bartuc";
			if (TextNo == 1219) return true;     // return  d2locale_kor? "임페리우스":"Imperius";
			if (TextNo == 1220)  return true;     //return  d2locale_kor? "마쎌":"Malthael";
			if (TextNo == 1567) return true;     // return  d2locale_kor? "아스트로가":"Astrogha";
			if (TextNo == 1573) return true;     // return  d2locale_kor? "로데오":"Bull Prince Rodeo";
			if (TextNo == 1612) return true;     // return  d2locale_kor? "아수르":"Assur";
			if (TextNo == 1699) return true;     // return  d2locale_kor? "클로우스톰 테러(엘리트 몹)":"Clawstorm Terror";
			if (TextNo == 1706) return true;     // return  d2locale_kor? "클로우스톰 테러(엘리트 몹)":"Clawstorm Terror";
			if (TextNo == 1704) return true;     // return  d2locale_kor? "위치 퀸 (Danger!)":"Startled Witch...";
		}
		//952 = 안다리엘


		
		if (dwact == 1 ) {
		// 액2
		if (TextNo == 966) return true;     // return  d2locale_kor? "탈라샤":"Tal Rasha";  
		if (TextNo == 1253) return true;     // return  d2locale_kor? "트신":"Quov Tsin";
		if (TextNo == 1004) return true;     // return  d2locale_kor? "크리쳐 오브 프레임":"Creature of Flame";
		if (TextNo == 1083) return true;     // return "Skovos, Priestess of Karcheus";
		if (TextNo == 1084) return true;     // return "Philios, Priestess of Hefaetrus";
		if (TextNo == 1085) return true;     // return "Lycander, Priestess of Zerae";
		if (TextNo == 1864) return true;     // return "Athulua, Amazonian Goddess"; //뉴 우버 퀘스트
		if (TextNo == 2048) return true;     // return "Brachyceran";
		if (TextNo == 1866) return true;     // return "Uldyssian's Memory 1";
		if (TextNo == 1867) return true;     // return "Uldyssian's Memory 2";
		if (TextNo == 1868) return true;     // return "Uldyssian's Memory 3";
		if (TextNo == 1869) return true;     // return "Uldyssian's Memory 4";
		if (TextNo == 1870) return true;     // return "Uldyssian's Memory 5";
		if (TextNo == 1871) return true;     // return "Uldyssian's Memory 6";
		if (TextNo == 1872) return true;     // return "Uldyssian's Memory 7";
		if (TextNo == 1873) return true;     // return "Uldyssian's Memory 8";
		if (TextNo == 1874) return true;     // return "Uldyssian's Memory 9";
		if (TextNo == 1875) return true;     // return "Uldyssian's Memory 10";
		if (TextNo == 1876) return true;     // return "Uldyssian's Memory 11";
		if (TextNo == 1877) return true;     // return "Uldyssian's Memory 12";
		if (TextNo == 1878) return true;     // return "Uldyssian's Memory 13";
		if (TextNo == 1879) return true;     // return "Uldyssian's Memory 14";
		if (TextNo == 1880) return true;     // return "Uldyssian's Memory 15";
		if (TextNo == 1881) return true;     // return "Uldyssian's Memory 16";
		if (TextNo == 1882) return true;     // return "Uldyssian's Memory 17";
		if (TextNo == 1883) return true;     // return "Uldyssian's Memory 18";
		if (TextNo == 1884) return true;     // return "Uldyssian's Memory 19";
		if (TextNo == 1885) return true;     // return "Uldyssian's Memory 20";
		if (TextNo == 1886) return true;     // return "Uldyssian's Memory 21";
		if (TextNo == 1887) return true;     // return "Uldyssian's Memory 22";
		if (TextNo == 1888) return true;     // return "Uldyssian's Memory 23";
		if (TextNo == 1889) return true;     // return "Uldyssian's Memory 24";
		if (TextNo == 1890) return true;     // return "Uldyssian's Memory 25";
		if (TextNo == 1891) return true;     // return "Uldyssian's Memory 26";
		if (TextNo == 1892) return true;     // return "Uldyssian's Memory 27";
		if (TextNo == 1893) return true;     // return "Uldyssian's Memory 28";
		if (TextNo == 1894) return true;     // return "Uldyssian's Memory 29";
		if (TextNo == 1895) return true;     // return "Uldyssian's Memory 30"; // 뉴 우버 퀘스트
		if (TextNo == 1896) return true;     // return  d2locale_kor? "에디렘 킹":"Edyrem King"; //뉴 우버 퀘스트
		if (TextNo == 1095) return true;     // return  d2locale_kor? "알파":"Alpha";
		if (TextNo == 1096) return true;     // return  d2locale_kor? "베타":"Beta";
		if (TextNo == 1097) return true;     // return  d2locale_kor? "감마":"Gamma";
		if (TextNo == 1098) return true;     // return  d2locale_kor? "델타":"Delta";
		if (TextNo == 1099) return true;     // return  d2locale_kor? "엡실론":"Epsilon";
		if (TextNo == 1702) return true;     // return  d2locale_kor? "페어리 위치(엘리트 몹)":"Fairy Witch";
		if (TextNo == 1709) return true;     // return  d2locale_kor? "페어리 위치(엘리트 몹)":"Fairy Witch";
		if (TextNo == 1704) return true;     // return  d2locale_kor? "위치 퀸 (Danger!)":"Startled Witch...";
		if (TextNo == 1088) return true;     // return "Volcanic Shrine";
		}

		if (dwact == 2 ) {

		// 액3
		if (TextNo == 962) return true;     // return  d2locale_kor? "아즈모단":"Azmodan";
		if (TextNo == 964) return true;     // return  d2locale_kor? "아카랏":"Akarat";
		if (TextNo == 969) return true;     // return  d2locale_kor? "데스 프로젝터":"Death Projector";
     	if (TextNo == 1015) return true;     // return  d2locale_kor? "토템":"Totem";
		if (TextNo == 1025) return true;     // return  d2locale_kor? "네크로맨서":"Ennead Necromancer";
		if (TextNo == 1059) return true;     // return  d2locale_kor? "프리머스":"Lord Primus";
		if (TextNo == 1622) return true;     // return  d2locale_kor? "브라더 래즈 (비밀 보스)":"Brother Laz (Secret Boss)";
		if (TextNo == 1629) return true;     // return  d2locale_kor? "마스터 컨트롤 시스템":"Master Control System";
		if (TextNo == 1701) return true;     // return  d2locale_kor? "글로윙 브로브(엘리트 몹)":"Glowing Blob";
		if (TextNo == 1708) return true;     // return  d2locale_kor? "글로윙 브로브(엘리트 몹)":"Glowing Blob"; 
		if (TextNo == 2035) return true;     // return	"Chrysaor (Secret Boss)"; 
		if (TextNo == 2032) return true;     // return  "Chrysaor"; 
		if (TextNo == 2044) return true;     // return "Startled Witch";
		if (TextNo == 2085) return true;     // return "Dul'Mephistos, The Lord Of Hatred";
		if (TextNo == 2086) return true;     // return "Al'Diabalos, The Lord Of Terror";
		if (TextNo == 2083) return true;     // return "Andariel, The Maiden Of Anguish";
		if (TextNo == 2084) return true;     // return "Duriel, The Lord Of Pain";
		if (TextNo == 2087) return true;     // return "Tor'Baalos, The Lord Of Destruction";
		if (TextNo == 2062) return true;     // return "Valthek, The Vizjerei Sorcerer";

		}

		if (dwact == 3 ) {

		// 액4
		 if (TextNo == 1700) return true;     // return  d2locale_kor? "랜드마스(엘리트 몹)":"Landmass";
		 if (TextNo == 1707) return true;     // return  d2locale_kor? "랜드마스(엘리트 몹)":"Landmass";
		}

		if (dwact == 4 ) {
		 //액 5
		if (TextNo == 973) return true;     // return  d2locale_kor? "킹 코쓰":"King Koth";
		if (TextNo == 1030) return true;     // return  d2locale_kor? "말릭":"Malic";
		if (TextNo == 1032) return true;     // return  d2locale_kor? "루시온":"Lucion";
		if (TextNo == 1038) return true;     // return  d2locale_kor? "자카룸의 아바타":"Zakarum's Avatar";
		if (TextNo == 1227) return true;     // return  d2locale_kor? "릴리스":"Lilith";
		if (TextNo == 1246) return true;     // return  d2locale_kor? "다크 스타 드래곤":"Dark Star Dragon";
		if (TextNo == 1069) return true;     // return  d2locale_kor? "알드릭 지탄":"Lord Aldric Jitan";
		if (TextNo == 1596) return true;     // return  d2locale_kor? "베리얼":"Belial, Lord of Lies";
		if (TextNo == 1703) return true;     // return  d2locale_kor? "배인 헌터(엘리트 몹)":"Bane Hunter";
		if (TextNo == 1710) return true;     // return  d2locale_kor? "배인 헌터(엘리트 몹)":"Bane Hunter";
		if (TextNo == 1704) return true;     // return  d2locale_kor? "위치 퀸 (Danger!)":"Startled Witch...";
		}
		//위치퀸 조절 액2가 아니면
		if (TextNo == 1652) return true;     // return  d2locale_kor? "드래곤 애그":"Dragon's Egg";
		if (TextNo == 1653) return true;     // return  d2locale_kor? "드래곤 애그":"Dragon's Egg";
		if (TextNo == 1654) return true;     // return  d2locale_kor? "바알 스폰":"Baalspawn";
		if (TextNo == 1655) return true;     // return  d2locale_kor? "바알 스폰":"Baalspawn";
		if (TextNo == 1656) return true;     // return  d2locale_kor? "드래곤 애그":"Dragon's Egg";
		if (TextNo == 1657) return true;     // return  d2locale_kor? "드래곤 애그":"Dragon's Egg";
		if (TextNo == 1658) return true;     // return  d2locale_kor? "바알 스폰":"Baalspawn";
		if (TextNo == 1659) return true;     // return  d2locale_kor? "바알 스폰":"Baalspawn";
		if (TextNo == 544) return true;     // return  d2locale_kor? "바알":"Baal";
		if (TextNo == 1940) return true;     // return  d2locale_kor? "마르코네크로X (비밀 보스)":"MarcoNecroX (Secret Boss)";
		if (TextNo == 1835) return true;     // return  d2locale_kor? "카브락시스":"Kabraxis, Thief of Hope";
		 if (TextNo == 1833) return true;     // return "Ultra Magic";
		 if (TextNo == 1832) return true;     // return "Ultra Earth";
		 if (TextNo == 1829) return true;     // return "Ultra Light";
		 if (TextNo == 1831) return true;     // return "Ultra Ice";
		 if (TextNo == 1830) return true;     // return "Ultra Fire";
		 if (TextNo == 1834) return true;     //return "pillar of skulls";

		 */

		return false;
}

bool Item_Shrine(UnitAny *item){
	ItemTxt *itemtxt = D2Common::GetItemText(item->dwTxtFileNo);

	if (CreepyShrine == true) { if (itemtxt->dwCode == D2TXTCODE('A0+ '))  return true;}
	if (AbandonedShrine == true) { if (itemtxt->dwCode == D2TXTCODE('B0+ ')) return true;}
	if (QuietShrine == true) { if (itemtxt->dwCode == D2TXTCODE('C0+ ')) return true;}
	if (EerieShrine == true) { if (itemtxt->dwCode == D2TXTCODE('D0+ ')) return true;}
	if (WeirdShrine == true) { if (itemtxt->dwCode == D2TXTCODE('E0+ ')) return true;}
	if (GloomyShrine == true) { if (itemtxt->dwCode == D2TXTCODE('F0+ ')) return true;}
	if (TaintedShrine == true) { if (itemtxt->dwCode == D2TXTCODE('G0+ ')) return true;}
	if (FascinatingShrine == true) { if (itemtxt->dwCode == D2TXTCODE('H0+ '))  return true;}
	if (OrnateShrine == true) { if (itemtxt->dwCode == D2TXTCODE('I0+ '))  return true;}
	if (SacredShrine == true) { if (itemtxt->dwCode == D2TXTCODE('J0+ ')) return true;}
	if (ShimmeringShrine == true) { if (itemtxt->dwCode == D2TXTCODE('K0+ ')) return true;}
	if (SpiritualShrine == true) { if (itemtxt->dwCode == D2TXTCODE('M0+ '))  return true;}
	if (MagicalShrine == true) { if (itemtxt->dwCode == D2TXTCODE('N0+ ')) return true;}
	if (HiddenShrine == true) { if (itemtxt->dwCode == D2TXTCODE('Q0+ ')) return true;}
	if (StoneShrine == true) { if (itemtxt->dwCode == D2TXTCODE('R0+ ')) return true;}

	return false;
}

bool Item_Charm(UnitAny *item){
	wchar_t charm_wItemName[256] = L"";
	D2Client::GetItemNameString(item,charm_wItemName,sizeof(charm_wItemName));

	if (smallcharm) {if (wcsstr(charm_wItemName,L"Small Charm")) return true;}
	if (largecharm) {if (wcsstr(charm_wItemName,L"Large Charm")) return true;}
	if (grandcharm) {if (wcsstr(charm_wItemName,L"Grand Charm")) return true;}

	return false;
}


bool Item_Gem(UnitAny *item){
	wchar_t gem_wItemName[256] = L"";
	D2Client::GetItemNameString(item,gem_wItemName,sizeof(gem_wItemName));
	
	if (PerfectAmethyst == true ) {if (wcsstr(gem_wItemName,L"Flawless Amethyst")) return true;}
	if (PerfectTopaz == true ){if (wcsstr(gem_wItemName,L"Flawless Topaz")) return true;}
	if (PerfectSapphire == true){if (wcsstr(gem_wItemName,L"Flawless Sapphire")) return true;}
	if (PerfectEmerald == true){if (wcsstr(gem_wItemName,L"Flawless Emerald")) return true;}
	if (PerfectRuby == true){if (wcsstr(gem_wItemName,L"Flawless Ruby")) return true;}
	if (PerfectDiamond == true){if (wcsstr(gem_wItemName,L"Flawless Diamond")) return true;}
	if (PerfectSkull == true){if (wcsstr(gem_wItemName,L"Flawless Skull")) return true;}
	if (PerfectGhostly == true){if (wcsstr(gem_wItemName,L"Flawless Ghostly")) return true;}

	if (wcsstr(gem_wItemName,L"[do1]")) return true; //메피
	if (wcsstr(gem_wItemName,L"[do2]")) return true; //메피
	if (wcsstr(gem_wItemName,L"[do3]")) return true; //메피
	if (wcsstr(gem_wItemName,L"[do4]")) return true; //메피
	if (wcsstr(gem_wItemName,L"[do5]")) return true; //메피
	if (wcsstr(gem_wItemName,L"[do6]")) return true; //메피
	if (wcsstr(gem_wItemName,L"[do7]")) return true; //메피
	if (wcsstr(gem_wItemName,L"[do8]")) return true; //메피
	if (wcsstr(gem_wItemName,L"[do9]")) return true; //메피

	if (wcsstr(gem_wItemName,L"Glyph"))  return true;
	if (wcsstr(gem_wItemName,L"Platinum")) return true;
	/*
	if (PerfectOnyx == true){if (wcsstr(gem_wItemName,L"Small Charm")) return true;}
	if (PerfectBloodStone == true){if (wcsstr(gem_wItemName,L"Small Charm")) return true;}
	if (PerfectTurquoise == true){if (wcsstr(gem_wItemName,L"Small Charm")) return true;}
	if (PerfectAmber == true){if (wcsstr(gem_wItemName,L"Small Charm")) return true;}

	*/

	return false;
}

bool Item_Ultimative(UnitAny *item){
	ItemTxt *itemtxt = D2Common::GetItemText(item->dwTxtFileNo);
	wchar_t Automap_wItemName[1024] = L"";
	D2Client::GetItemNameString(item,Automap_wItemName,sizeof(Automap_wItemName));


	if (wcsstr(Automap_wItemName,L"Book [L1]")) return true;
	if (wcsstr(Automap_wItemName,L"Cursed Crystal [L1]"))  return true;
	if (wcsstr(Automap_wItemName,L"Broken Crystal [L1]")) return true;
	if (wcsstr(Automap_wItemName,L"Earth Stone [L1]")) return true;
	if (wcsstr(Automap_wItemName,L"Fire Stone [L1]")) return true;
	if (wcsstr(Automap_wItemName,L"Ice Stone [L1]")) return true;
	if (wcsstr(Automap_wItemName,L"Magic Stone [L1]")) return true;
	if (wcsstr(Automap_wItemName,L"Light Stone [L1]")) return true;
	if (wcsstr(Automap_wItemName,L"Poison Stone [L1]")) return true;
	if (itemtxt->dwCode == D2TXTCODE('r97 '))   return true; // 체스트 조합때
	if (itemtxt->dwCode == D2TXTCODE('r98 '))  return true; // 카브라시스
	if (itemtxt->dwCode == D2TXTCODE('r99 '))   return true; // 언노운 스쿨
	if (wcsstr(Automap_wItemName,L"Sunless Crystal Bird")) return true;
	
	return false;
}

bool Item_MagicJewel(UnitAny *item){

	wchar_t charm_wItemName[250] = L"";
	D2Client::GetItemNameString(item,charm_wItemName,sizeof(charm_wItemName));

	if (smallcharm) {if (wcsstr(charm_wItemName,L"Small Charm")) return true;}
	if (largecharm) {if (wcsstr(charm_wItemName,L"Large Charm")) return true;}
	if (grandcharm) {if (wcsstr(charm_wItemName,L"Grand Charm")) return true;}

	if (ViewJewelMagic == true) {
		if (D2Common::GetItemText(item->dwTxtFileNo)->dwCode == D2TXTCODE('jew ')) 
			return true;}
	return false;
}

bool Item_RareItem(UnitAny *item){
	wchar_t charm_wItemName[250] = L"";
	D2Client::GetItemNameString(item,charm_wItemName,sizeof(charm_wItemName));

	if (smallcharm) {if (wcsstr(charm_wItemName,L"Small Charm")) return true;}
	if (largecharm) {if (wcsstr(charm_wItemName,L"Large Charm")) return true;}
	if (grandcharm) {if (wcsstr(charm_wItemName,L"Grand Charm")) return true;}
	if (ViewJewelRare == true) {if (D2Common::GetItemText(item->dwTxtFileNo)->dwCode == D2TXTCODE('jew ')) return true;}
	if (RareAmulet == true){ if (D2Common::GetItemText(item->dwTxtFileNo)->dwCode == D2TXTCODE('amu ')) return true;}
	if (RareRing == true){ if (D2Common::GetItemText(item->dwTxtFileNo)->dwCode == D2TXTCODE('rin ')) return true;}
	return false;
}


bool Item_SacredOnly(UnitAny *item){
	//ItemTxt *itemtxt = D2Common::GetItemText(item->dwTxtFileNo);

	wchar_t Automap_wItemName[1024] = L"";
	D2Client::GetItemNameString(item,Automap_wItemName,sizeof(Automap_wItemName));

	if (wcsstr(Automap_wItemName,L"Ring of the Five")) return false;

	if (MenuAllUnique == 1 ||  MenuAllUnique == 0) {// All Unique
		if (wcsstr(Automap_wItemName,L"(1)") || wcsstr(Automap_wItemName,L"(2)") 
		||wcsstr(Automap_wItemName,L"(3)") || wcsstr(Automap_wItemName,L"(4)")   
		||wcsstr(Automap_wItemName,L"(5)") ||wcsstr(Automap_wItemName,L"(6)")) {return false;}
	}
	return true;
	/*
	//우버참
	
	if (itemtxt->dwCode == D2TXTCODE('amu ')) {return "Amulet";}
	if (itemtxt->dwCode == D2TXTCODE('rin ')) { return "Ring";}
	if (itemtxt->dwCode == D2TXTCODE('jew ')) {return "Jewel";}

	if (itemtxt->dwCode == D2TXTCODE('aqv ')) { return "Arrow Quiver";}
	if (itemtxt->dwCode == D2TXTCODE('cqv ')) { return "Bolt Quiver";}

	if (itemtxt->dwCode == D2TXTCODE('a55 ')) {		return "Optical Detecter";}
	if (itemtxt->dwCode == D2TXTCODE('a56 ')) {		return "Azmodan's Heart";}
	if (itemtxt->dwCode == D2TXTCODE('a59 ')) {       return "Laser Focus Crystal";}
	if (itemtxt->dwCode == D2TXTCODE('a60 ')) {       return "The Butcher's Tooth";}
	if (itemtxt->dwCode == D2TXTCODE('a66 ')) {       return "Idol of Vanity";}
	if (itemtxt->dwCode == D2TXTCODE('a68 ')) {       return "Horazon's Focus";}
	if (itemtxt->dwCode == D2TXTCODE('yq1 ')) {       return "Sunless Crystal Bird";}
	if (itemtxt->dwCode == D2TXTCODE('yq2 ')) {        return "Six Angel Bag";}
	if (itemtxt->dwCode == D2TXTCODE('yq7 ')) {       return "Hard Mode Charm";}
	if (itemtxt->dwCode == D2TXTCODE('yq8 ')) {       return "Zakarum's Ear";}
	if (itemtxt->dwCode == D2TXTCODE('yqx ')) {       return "Spirit Trance Herb";}
	if (itemtxt->dwCode == D2TXTCODE('yqe ')) {       return "Moon of the Spider";}
	if (itemtxt->dwCode == D2TXTCODE('x0x ')) {       return "Eternal Bone Pile";}
	if (itemtxt->dwCode == D2TXTCODE('x1x ')) {       return "Sunstone of Fire";}
	if (itemtxt->dwCode == D2TXTCODE('x2x ')) {       return "Sunstone of Ice";}
	if (itemtxt->dwCode == D2TXTCODE('x3x ')) {       return "Sunstone of Thunder";}
	if (itemtxt->dwCode == D2TXTCODE('x4x ')) {       return "Sunstone of the Gods";}
	if (itemtxt->dwCode == D2TXTCODE('0u1 ')) {       return "Hammer of the Taan Judges";}
	if (itemtxt->dwCode == D2TXTCODE('w@w ')) {       return "Cold Fusion Schematics";}
	if (itemtxt->dwCode == D2TXTCODE('x@x ')) {       return "Glorious Book of Median";}
	if (itemtxt->dwCode == D2TXTCODE('x#x ')) {       return "Sunstone of Elements";}
	if (itemtxt->dwCode == D2TXTCODE('aq! ')) {       return "Silver Seal of Ureh";}
	if (itemtxt->dwCode == D2TXTCODE('ab! ')) {       return "Astrogha's Venom Stinger";}
	if (itemtxt->dwCode == D2TXTCODE('k@k ')) {       return "Book of Lies";}
	if (itemtxt->dwCode == D2TXTCODE('bxa ')) {       return "Soul of Kabraxis";}
	if (itemtxt->dwCode == D2TXTCODE('##. ')) {       return "ULdyssian's Sword";}
	if (itemtxt->dwCode == D2TXTCODE('x01 ')) {       return "Sacred Sunstone (Amazon)";}
	if (itemtxt->dwCode == D2TXTCODE('x02 ')) {       return "Shadow Vortex (Assasin)";}
	if (itemtxt->dwCode == D2TXTCODE('x03 ')) {       return "Worldstone Orb (BarBarian)";}
	if (itemtxt->dwCode == D2TXTCODE('x04 ')) {       return "Caoi Dulra Fruits (Druid)";}
	if (itemtxt->dwCode == D2TXTCODE('x05 ')) {       return "Soulstone Shard (NecroMancer)";}
	if (itemtxt->dwCode == D2TXTCODE('x06 ')) {       return "Eye of Divinity (Paladin)";}
	if (itemtxt->dwCode == D2TXTCODE('x07 ')) {       return "Nexus Crystal (Sorceress)";}
	if (itemtxt->dwCode == D2TXTCODE('t@t ')) {    return "Demonsbane";}

	*/
	//return NULL;
}

void __declspec(naked) InfravisionPatch_ASM() //벽뒤  몹보기
{
	__asm {
		mov ecx, esi
		call InfravisionPatch
		add dword ptr [esp], 0x72
		ret
	}
}

void __declspec(naked) GameFailToJoin_STUB()
{
	__asm {
		cmp esi, 4000
		ret
	}	
}

void __declspec(naked) GameShake1_STUB()
{
	__asm
	{
		je OldCode;

		add dword ptr [esp], 0xC;
		ret;

OldCode:
		mov [ecx], esi;
		mov ecx, [esp + 0x1C];
		ret;
	}

}

void __stdcall GameShakePatch(LPDWORD lpX, LPDWORD lpY)
{
	if (ScrollMapON == true) {
		*lpX += xMapShake;
		*lpY += yMapShake;

		*D2Client::xShake = xMapShake;
		*D2Client::yShake = yMapShake;
	}
	else {
		*D2Client::xShake = 0;
		*D2Client::yShake = 0;
	}

}

void ScrollMap(char x,char y){
	xMapShake += x*16;
    yMapShake += y*16;
}


CellFile *InitCellFile(CellFile *File)
{
	if (File) D2Cmp::InitCellFile(File, &File, "?", 0, -1, "?");
	return File;
}

HANDLE OpenFileRead(LPSTR filename)
{
	return CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

DWORD ReadFile(HANDLE hFile, LPVOID buf, DWORD len)
{
	DWORD numdone = 0;
	ReadFile(hFile, buf, len, &numdone, NULL);
	return numdone;
}


LPBYTE AllocReadFile(LPSTR FileName)
{
	HANDLE hFile = OpenFileRead(FileName);
	INT FileSize = GetFileSize(hFile, 0);
	
	if (FileSize <= 0)
		return 0;

	LPBYTE Buffer = new BYTE[FileSize];
	ReadFile(hFile, Buffer, FileSize);

	CloseHandle(hFile);
	return Buffer;
}

VOID * memcpy2(LPVOID Destination, CONST VOID * Source, size_t Count)
{
	return (LPSTR)memcpy(Destination, Source, Count) + Count;
}

CellFile *LoadBMPCellFile(BYTE * Buffer, INT Width, INT Height)
{
	/*BYTE *buf2 = new BYTE[(Width*Height*2)+Height], *dest = buf2;

	for (int i = 0; i < Height; i++) {
		BYTE *src = Buffer+(i*((Width+3)&-4)), *limit = src+Width;
		while (src < limit) {
			BYTE *start = src, *limit2 = min(limit, src+0x7f), trans = !*src;
			do src++; while ((trans == (BYTE)!*src) && (src < limit2));
			if (!trans || (src < limit)) *dest++ = (trans?0x80:0)+(src-start);
			if (!trans) while (start < src) *dest++ = *start++;
		}
		*dest++ = 0x80;
	}

	static DWORD dc6head[] = {6, 1, 0, 0xeeeeeeee, 1, 1, 0x1c,  0, -1, -1, 0, 0, 0, -1, -1};
	dc6head[8] = Width;
	dc6head[9] = Height;
	BYTE *ret = new BYTE[dc6head[13] = sizeof(dc6head)+(dc6head[14] = dest-buf2)+3];
	memset(memcpy2(memcpy2(ret, dc6head, sizeof(dc6head)), buf2, dc6head[14]), 0xee, 3);
	delete buf2;

	return (CellFile *)ret;*/
	BYTE * Buffer2 = new BYTE[(Width * Height * 2) + Height], *Destination = Buffer2;

	for (INT i = 0; i < Height; i++)
	{
		BYTE * Source = Buffer + (i * ((Width + 3) & -4)), *Limit = Source + Width;
		
		while (Source < Limit)
		{
			BYTE * Start = Source, *Limit2 = min(Limit, Source + 0x7F), Transparency = !*Source;
			do Source++; while ((Transparency == (BYTE)!*Source) && (Source < Limit2));

			if (!Transparency || (Source < Limit)) *Destination++ = (BYTE)((Transparency ? 0x80 : 0) + (Source - Start));
			if (!Transparency) while (Start < Source) *Destination++ = *Start++;
		}
		
		*Destination++ = 0x80;
	}

	INT DC6[] = {6, 1, 0, 0xEEEEEEEE, 1, 1, 0x1C,  0, -1, -1, 0, 0, 0, -1, -1};
	DC6[8] = Width;
	DC6[9] = Height;

	BYTE * Return = new BYTE[DC6[13] = sizeof(DC6) + (DC6[14] = (DWORD)(Destination - Buffer2)) + 3];
	memset(memcpy2(memcpy2(Return, DC6, sizeof(DC6)), Buffer2, DC6[14]), 0xEE, 3);
	delete Buffer2;

	return (CellFile *)Return;
}


CellFile *LoadBMPCellFile(LPSTR FileName)
{
	/*BYTE *ret = 0;
		
	BYTE *buf1 = AllocReadFile(FileName);
	BITMAPFILEHEADER *bmphead1 = (BITMAPFILEHEADER *)buf1;
	BITMAPINFOHEADER *bmphead2 = (BITMAPINFOHEADER *)(buf1+sizeof(BITMAPFILEHEADER));
	if (buf1 && (bmphead1->bfType == 'MB') && (bmphead2->biBitCount == 8) && (bmphead2->biCompression == BI_RGB)) {
		ret = (BYTE *)LoadBMPCellFile(buf1+bmphead1->bfOffBits, bmphead2->biWidth, bmphead2->biHeight);
	}
	delete buf1;

	return (CellFile *)ret;*/
	LPBYTE Return = 0;
	LPBYTE Buffer = AllocReadFile(FileName);
	
	BITMAPFILEHEADER * BMP1 = (BITMAPFILEHEADER *)Buffer;
	BITMAPINFOHEADER * BMP2 = (BITMAPINFOHEADER *)(Buffer + sizeof(BITMAPFILEHEADER));
	
	if (Buffer && (BMP1->bfType == 'MB') && (BMP2->biBitCount == 8) && (BMP2->biCompression == BI_RGB))
		Return = (LPBYTE)LoadBMPCellFile(Buffer + BMP1->bfOffBits, BMP2->biWidth, BMP2->biHeight);

	delete Buffer;
	return (LPCELLFILE)Return;
}