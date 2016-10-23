#include <stdio.h>

#include "ItemPatch.h"
#include "Maphack.h"
#include "ScreenHooks.h"
#include "Offset.h"
#include "D2State.h"

#define D2TXTCODE(x) ( ((x)&0xff)<<24 | ((x)>>8&0xff)<<16 | ((x)>>16&0xff)<<8 | ((x)>>24&0xff) )
#pragma warning(disable:4996)

/*
void  __fastcall D2CLIENT_GetItemDesc_ASM(DWORD pUnit, wchar_t* pBuffer)
{
	__asm 
   {
      PUSH EDI
         MOV EDI, EDX
         PUSH NULL
         PUSH 1      // TRUE = New lines, FALSE = Comma between each stat value
         PUSH ECX
         CALL D2Client::GetItemDesc_I;
         POP EDI
         RETN
   }
}

*/

void __declspec(naked) ItemNamePatch_ASM()
{
	__asm {
		mov ecx, edi
		mov edx, ebx
		call ItemNamePatch
		mov al, [ebp+0x12a]
		ret
	}
}

void __declspec(naked) MonsterLifeBarNameSTUB()
{
	__asm
	{
		PUSH EDX;
		MOV EDX, ESP;
		PUSH [ESP + 0x14];
		LEA EAX, [ESP + 0x10];
		PUSH EAX;
		CALL MonsterLifeBarName;
		MOV ECX, EAX;
		POP EDX;
		POP EAX;
		PUSH EBX;
		PUSH ESI;
		MOV ESI, ECX;
		TEST ESI, ESI;
		JMP EAX;
	}
}

wchar_t *myRuneName[34] = {L"", L"El",L"Eld",L"Tir",L"Nef",L"Eth",L"Ith",L"Tal",L"Ral",L"Ort",L"Thul",L"Amn",L"Sol",L"Shael",L"Dol",L"Hel",L"Io"
	,L"Lum",L"Ko",L"Fal",L"Lem",L"Pul",L"Um",L"Mal",L"Ist",L"Gul",L"Vex",L"Ohm",L"Lo",L"Sur",L"Ber",L"Jah",L"Cham",L"Zod"};

void __fastcall ItemNamePatch(wchar_t *name, UnitAny *item)
{
	ItemTxt *itemtxt = D2Common::GetItemText(item->dwTxtFileNo);

	if (itemtxt->dwCode == D2TXTCODE('box '))
		wsprintfW(name,L"ÿc1%s",d2locale_kor ? L"호라드릭 큐브": L"Horadric Cube");

	if (itemtxt->dwCode == D2TXTCODE('leg ')) 
		wsprintfW(name,L"ÿc3Mÿc7rÿc0.ÿc8%s",d2locale_kor ? L"쿠ÿc2우ÿc0의 다리": L"Qÿc2oÿc4oÿc0's Leg");

	if (wcsstr(name,L"Rune")) //룬 번호 처리
		wsprintfW(name,L"%s [%c%c]",name,itemtxt->szCode[1],itemtxt->szCode[2]);
	
	/*
	if (itemtxt->dwCode == D2TXTCODE('dog '))
		wsprintfW(name,L"ÿc1%s",d2locale_kor ? L"사과": L"Aÿc2pÿc4pÿc0lÿc3e");
	if (itemtxt->dwCode == D2TXTCODE('r97 ')) //언노운 스쿨2
		wsprintfW(name,L"ÿc1Unknown Skull %s",d2locale_kor ? L"(하수구 2층)": L"(Sewers 2)");
	if (itemtxt->dwCode == D2TXTCODE('r99 ')) //언노운 스쿨1
		wsprintfW(name,L"ÿc1Unknown Skull %s",d2locale_kor ? L"(하수구 1층)": L"(Sewers 1)");

	if (wcsstr(name,L"Arcane Shards #1")) //아케인 샤드
		wsprintfW(name,L"ÿc0Cube Reagent\nÿc8Arcane Shards ÿc2#1ÿc8");
	if (wcsstr(name,L"Arcane Shards #2")) //아케인 샤드
		wsprintfW(name,L"ÿc0Cube Reagent\nÿc8Arcane Shards ÿc2#2ÿc8");
	if (wcsstr(name,L"Arcane Shards #3")) //아케인 샤드
		wsprintfW(name,L"ÿc0Cube Reagent\nÿc8Arcane Shards ÿc2#3ÿc8");
	if (wcsstr(name,L"Arcane Shards #4")) //아케인 샤드
		wsprintfW(name,L"ÿc0Cube Reagent\nÿc8Arcane Shards ÿc2#4ÿc8");

	if (itemtxt->dwCode == D2TXTCODE('!@1 ')){ wsprintfW(name,L"%s ÿc0[Act1] ÿc:",name);} //"Evil Eye of the aaaaa";}
	if (itemtxt->dwCode == D2TXTCODE('!@4 ')){ wsprintfW(name,L"%s ÿc0[Act2] ÿc:",name);} //"Evil Eye of the ddddd";}
	if (itemtxt->dwCode == D2TXTCODE('!@3 ')) { wsprintfW(name,L"%s ÿc0[Act3] ÿc:",name);}//"Evil Eye of the ccccc";}
	if (itemtxt->dwCode == D2TXTCODE('!@2 ')){ wsprintfW(name,L"%s ÿc0[Act4] ÿc:",name);} //"Evil Eye of the bbbbb";}
	if (itemtxt->dwCode == D2TXTCODE('!@5 ')) { wsprintfW(name,L"%s ÿc0[Act5] ÿc:",name);} //"Evil Eye of the eeeee";}

	if (itemtxt->dwCode == D2TXTCODE('!@6 '))   wsprintfW(name,L"%s ÿc0[andariel] ÿc:",name); //"Dogma of aaaaa";}
	if (itemtxt->dwCode == D2TXTCODE('!@7 '))   wsprintfW(name,L"%s ÿc0[Duriel] ÿc:",name); //"Dogma of bbbbb";}
	if (itemtxt->dwCode == D2TXTCODE('!@8 '))   wsprintfW(name,L"%s ÿc0[Mephisto] ÿc:",name); //"Dogma of ccccc";}
	if (itemtxt->dwCode == D2TXTCODE('!@9 '))  wsprintfW(name,L"%s ÿc0[Diablo] ÿc:",name); //"Dogma of ddddd";}
	if (itemtxt->dwCode == D2TXTCODE('!@A '))  wsprintfW(name,L"%s ÿc0[Baal] ÿc:",name); //"Dogma of eeeee";}
	
	*/
	if (wcsstr(name,L"Random Glyph")) { //룬 번호 처리
		char glyphtemp[16] = "";
		if (item->dwTxtFileNo == 885) wsprintf(glyphtemp,"Strong");
		if (item->dwTxtFileNo == 886) wsprintf(glyphtemp,"Poison"); 
		if (item->dwTxtFileNo == 887) wsprintf(glyphtemp,"Fire"); 
		if (item->dwTxtFileNo == 888) wsprintf(glyphtemp,"Cold"); 
		if (item->dwTxtFileNo == 889) wsprintf(glyphtemp,"Lightning");
		if (item->dwTxtFileNo == 890) wsprintf(glyphtemp,"Massive");
		if (item->dwTxtFileNo == 891) wsprintf(glyphtemp,"Thorns");
		if (item->dwTxtFileNo == 892) wsprintf(glyphtemp,"Vampire");
		if (item->dwTxtFileNo == 893) wsprintf(glyphtemp,"Crypt");
		if (item->dwTxtFileNo == 894) wsprintf(glyphtemp,"Strike");
		if (item->dwTxtFileNo == 895) wsprintf(glyphtemp,"Mage's");
		if (item->dwTxtFileNo == 896) wsprintf(glyphtemp,"Warriior's");
		if (item->dwTxtFileNo == 897) wsprintf(glyphtemp,"Ancient");
		if (item->dwTxtFileNo == 898) wsprintf(glyphtemp,"Cobra");
		if (item->dwTxtFileNo == 899) wsprintf(glyphtemp,"Gold");
		if (item->dwTxtFileNo == 900) wsprintf(glyphtemp,"Defensive");

		 wsprintfW(name,L"ÿc8%S Glyph (ÿc1Rÿc8aÿc9nÿc:dÿc3oÿc;mÿc8)ÿc0",glyphtemp);
	}
	if (wcsstr(name,L"Random Rune")) { //룬 번호 처리	
		char runetemp[6] = "";
		wsprintf(runetemp,"%c%c",itemtxt->szCode[1],itemtxt->szCode[2]);
		wsprintfW(name,L"ÿc8%s Rune (ÿc1Rÿc8aÿc9nÿc:dÿc3oÿc;mÿc8)ÿc0",myRuneName[atoi(runetemp)]);
	}


	wsprintfW(name,L"%s [L%i][%c%c%c]",name, item->pItemData->dwItemLevel,itemtxt->szCode[0],itemtxt->szCode[1],itemtxt->szCode[2]); //찰시
	

	if (D2Common::GetUnitStat(item, 194, 0) != 0) wsprintfW(name,L"%s ÿc9[%s = %d]",name,d2locale_kor? L"소켓 수":L"Sockets",D2Common::GetUnitStat(item, 194, 0));
	/*
	if (item->pItemData->dwFlags & 0x00002000) { //npc 스토어
		if (wcsstr(name,L"Oil of Renewal")) {
			if (RenewalOilID != item->dwUnitId) {
				RenewalOilID = item->dwUnitId; // item id 얻음
			}
		}
	}
	*/
	
	if (item->pItemData->dwFlags & 0x400000) wsprintfW(name,L"%s ÿc8(%s)",name,d2locale_kor ? L"*에테리얼":L"Ethereal");

	if (SellPriceCheck && D2Common::GetItemPrice(D2Client::GetPlayerUnit(),item,*D2Client::Difficulty,D2Client::ItemPriceList,154,1) >= 2500) wsprintfW(name,L"%s ÿc8$%d",name,D2Common::GetItemPrice(D2Client::GetPlayerUnit(),item,*D2Client::Difficulty,D2Client::ItemPriceList,154,1));
}

wchar_t * __fastcall MonsterLifeBarName(WCHAR * Old, LPDWORD TextSize, LPDWORD Size, DWORD Flag)
{
	UnitAny *pUnit = D2Client::GetSelectedUnit();

	static wchar_t MonLifeBar[256]= L"";
	static wchar_t monlifebartmp[256]= L"";

		if (pUnit && pUnit->dwType == UNIT_TYPE_MONSTER){

			DWORD LevelNo = D2Common::GetRoom1LevelNo(D2Client::GetPlayerUnit()->pPath->pRoom1);

			if (!GetUIVarSTUB(UI_MINISKILL)) {
				if(!D2Common::IsLevelATown(LevelNo)) { // 마을일경우
				wsprintfW(monlifebartmp,L"%s",Old);

				if (wcslen(monlifebartmp) < 50) {//몹 이름이 50자 이하일경우 실행
					if (d2locale_kor) {
						wsprintfW(MonLifeBar,L"%s[L%d] ÿc0[ÿc4물리:%d%% ÿc3콜드:%d%% ÿc1파이어:%d%% ÿc9라이트닝:%d%% ÿc2포이즌:%d%% ÿc8매직:%d%%ÿc0] ÿc8[%d]",Old,D2Common::GetUnitStat(pUnit,12,0),(int)D2Common::GetUnitStat(pUnit, 36, 0),(int)D2Common::GetUnitStat(pUnit, 43, 0),(int)D2Common::GetUnitStat(pUnit, 39, 0),(int)D2Common::GetUnitStat(pUnit, 41, 0),(int)D2Common::GetUnitStat(pUnit, 45, 0),(int)D2Common::GetUnitStat(pUnit, 37, 0),pUnit->dwTxtFileNo);
						*TextSize = *TextSize -wcslen(MonLifeBar) -110;}
					else{
						wsprintfW(MonLifeBar,L"%s[L%d] ÿc0[ÿc4Ph:%d%% ÿc3C:%d%% ÿc1F:%d%% ÿc9L:%d%% ÿc2P:%d%% ÿc8M:%d%%ÿc0] ÿc8[%d]",Old,D2Common::GetUnitStat(pUnit,12,0),(int)D2Common::GetUnitStat(pUnit, 36, 0),(int)D2Common::GetUnitStat(pUnit, 43, 0),(int)D2Common::GetUnitStat(pUnit, 39, 0),(int)D2Common::GetUnitStat(pUnit, 41, 0),(int)D2Common::GetUnitStat(pUnit, 45, 0),(int)D2Common::GetUnitStat(pUnit, 37, 0),pUnit->dwTxtFileNo);
						*TextSize = *TextSize -wcslen(MonLifeBar) -30;
					}
					
				}
					else wsprintfW(MonLifeBar,L"%s[L%d] [%d]",Old,D2Common::GetUnitStat(pUnit,12,0),pUnit->dwTxtFileNo);

					return MonLifeBar;
				} else { //ncp 등등 일경우

				if (RenewalOilNpc != pUnit->dwUnitId) RenewalOilNpc =pUnit->dwUnitId;//오일사기위해 npc id 얻음 

				/*
				if (pUnit->dwTxtFileNo == 511){
					wsprintfW(MonLifeBar,L"ÿc1%s ÿc4[ÿc8%sÿc4]",Old,d2locale_kor?L"미스틱 오브 & 카탈리스트 & 오일":L"Mystic Orb & Catalyst & Oil");}
				else if (pUnit->dwTxtFileNo == 252){
					wsprintfW(MonLifeBar,L"ÿc1%s ÿc4[ÿc8%sÿc4]",Old,d2locale_kor?L"미스틱 오브 & 오일 & 용병 고용":L"Mystic Orb & Oil & Hireling");}
				else if (pUnit->dwTxtFileNo	== 512 || pUnit->dwTxtFileNo	== 405 ||pUnit->dwTxtFileNo	== 253 //카탈 오일
							||pUnit->dwTxtFileNo	== 202 ||pUnit->dwTxtFileNo	== 178 || pUnit->dwTxtFileNo	== 154){ //파는 NPC
					wsprintfW(MonLifeBar,L"ÿc1%s ÿc4[ÿc8%sÿc4]",Old,d2locale_kor?L"카탈리스트 & 오일":L"Catalyst & Oil");}
				else if (pUnit->dwTxtFileNo	== 257 || pUnit->dwTxtFileNo	== 177 || pUnit->dwTxtFileNo	== 148 ){
					wsprintfW(MonLifeBar,L"ÿc1%s ÿc4[ÿc8%sÿc4]",Old,d2locale_kor?L"미스틱 오브":L"Mystic Orb");} //미스틱 오브 파는 NPC
				else if (pUnit->dwTxtFileNo == 150 || pUnit->dwTxtFileNo	== 198 || pUnit->dwTxtFileNo	== 515){
					wsprintfW(MonLifeBar,L"ÿc1%s ÿc4[ÿc8%sÿc4]",Old,d2locale_kor?L"용병 고용":L"Hireling");}
				*/
				
				else {wsprintfW(MonLifeBar,L"ÿc1%s",Old);}

				if (wcsstr(MonLifeBar,L"ÿc1 ")) //이름 깨지는거 수정 
					return Old;
				else return MonLifeBar;
				}
			}
		}

		if (pUnit && pUnit->dwType == UNIT_TYPE_OBJECT){ // wp,chest등
			wsprintfW(MonLifeBar,L"ÿc9%s",Old);

				return MonLifeBar;
		}
		return Old; //기본값 리턴
}