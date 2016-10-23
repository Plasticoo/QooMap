#include <cstdio>
#include <cstdarg>
#include <string>
#include <iostream>
#include <list>

#include "Maphack.h"
#include "Screenhooks.h"
#include "Offset.h"
#include "D2State.h"

using namespace std;

ScreenHookManager* ScreenHookManager::instance = NULL;

ScreenHookManager* ScreenHookManager::Instance() {
	return instance ? instance : ((instance = new ScreenHookManager));
}

void ScreenHookManager::DrawTextHook(ScreenHook* Hook) {
	//Set wanted font size
	unsigned saveFont = D2Win::SetTextSize(Hook->TextInfo->Font);

	//Get the text width.
	DWORD Width, FileNo;
	D2Win::GetTextWidthFileNo(Hook->TextInfo->String, &Width, &FileNo);

	//Set base x/y to modify
	int nX = Hook->Pos->PosX;
	int nY = Hook->Pos->PosY;

	//Check for automap, if so convert coords
	if (Hook->DrawType == Automap) {
		POINT ptPoint = { Hook->Pos->PosX, Hook->Pos->PosY };
		ToAutomap(&ptPoint);
		nX = ptPoint.x;
		nY = ptPoint.y;
	}
	//Align to center
	if ((Hook->Alignment & Center) && !(Hook->Type == Automap))
		nX = (800 / 2) - (Width / 2);

	//Align to Center From X
	if ((Hook->Alignment & CenterFromX) || (Hook->Alignment & Center) && (Hook->Type == Automap) || (Hook->Alignment & LevelName))
		nX -= (Width / 2);

	if ((Hook->Alignment & LevelName))
		nY -= 16;

	//Align to Right
	if (Hook->Alignment & Right)
		nX = nX = *D2Client::ScreenSizeX_I - D2Win::GetTextWidth_I(Hook->TextInfo->String, wcslen(Hook->TextInfo->String)) - 16;;

	//wprintf(L"%s %d %d %d %d %d\n", Hook->TextInfo->String, Hook->Pos->PosX, Hook->Pos->PosY, Hook->TextInfo->Font, Hook->Colour, Hook->Alignment);
	D2Win::DrawText(Hook->TextInfo->String, Hook->Pos->PosX, Hook->Pos->PosY, Hook->Colour, 0);
	D2Win::SetTextSize(saveFont);
}

void ScreenHookManager::DrawBoxHook(ScreenHook* Hook) {

}

void ScreenHookManager::DrawScreenHooks() {
	bool DrawThisHook = false;
	ScreenHooks::iterator begin = screenHooks.begin();
	ScreenHooks::iterator end = screenHooks.end();
	for(; begin != end; begin++) {
		if((*begin)->DrawType == Perm)
			DrawThisHook = true;

		if(!DrawThisHook) {
			switch(ClientState()) {
			case ClientStateInGame: {
				if((*begin)->DrawType == InGame || (*begin)->DrawType == Automap)
					DrawThisHook = true;
				break;
			}
		
			case ClientStateMenu: {
				if((*begin)->DrawType == OutOfGame)
					DrawThisHook = true;
				break;
			}

			default:
				break;
			}
		}

		if(DrawThisHook) {
			switch((*begin)->Type) {
				case BoxHook: {
					DrawBoxHook((*begin));
					break;
				}

				case TextHook: {
					DrawTextHook((*begin));
					break;
				}
			}
		}

		// Reset our trigger
		DrawThisHook = false;
	}
}

void ScreenHookManager::DrawAutomapHooks() {
	bool DrawThisHook = false;
	ScreenHooks::iterator begin = automapHooks.begin();
	ScreenHooks::iterator end = automapHooks.end();
	for(; begin != end; begin++) {
		if((*begin)->DrawType == Perm)
			DrawThisHook = true;

		if(!DrawThisHook) {
			if((*begin)->DrawType == InGame || (*begin)->DrawType == Automap)
				DrawThisHook = true;
		}

		if(DrawThisHook) {
			switch((*begin)->Type) {
				case BoxHook: {
					DrawBoxHook((*begin));
					break;
				}

				case TextHook: {
					DrawTextHook((*begin));
					break;
				}
			}
		}

		// Reset our trigger
		DrawThisHook = false;
	}
}

unsigned int ScreenHookManager::CreateTextHook(wchar_t* String, HookDrawType DrawType, ShortPoint* Pos, HookAlign Align, unsigned char Colour, unsigned char Font) {

	if(!String || !wcslen(String))
		return 0;

	ScreenHook* newHook = new ScreenHook;
	ShortPoint* pos = new ShortPoint;
	TextData* data = new TextData;

	data->Font = Font;
	data->String = String;
	newHook->TextInfo = data;


	pos->PosX = Pos->PosX;
	pos->PosY = Pos->PosY;
	newHook->Pos = pos;

	newHook->HookId = nextHookId;
	newHook->Type = TextHook;
	newHook->DrawType = DrawType;
	newHook->Alignment = Align;
	newHook->Colour = Colour;

	//printf("adding hook to list");
	screenHooks.push_back(newHook);

	nextHookId++;
	return newHook->HookId;
}

void TexthookDraw(HookDrawType type, unsigned int x, unsigned int y, unsigned int color, unsigned int font, HookAlign align, string text, ...) {
	//Format the given text.
	char nString[512] =""; 
	va_list arg; 
	va_start(arg, text); 
	vsprintf_s(nString, 512, text.c_str(), arg); 
	va_end(arg); 

	//Convert to Unicode
	wchar_t wString[512];
	MultiByteToWideChar(0, 1, nString, 100, wString, 100);
	//AnsiToUnicode(nString);

	//Set wanted font size
	unsigned dwOldFont = D2Win::SetTextSize(font);

	//Get the text width.
	DWORD Width, FileNo;
	D2Win::GetTextWidthFileNo(wString, &Width, &FileNo);

	//Set base x/y to modify
	int nX = x;
	int nY = y;

	//Check for automap, if so convert coords
	if (type == Automap) {
		POINT ptPoint = {x, y};
		ToAutomap(&ptPoint);
		nX = ptPoint.x;
		nY = ptPoint.y;
	}
	//Align to center
	if ((align & Center) && !(type == Automap))
		nX = (800 / 2) - (Width / 2);

	//Align to Center From X
	if ((align & CenterFromX) || (align & Center) && (type == Automap) || (align & LevelName))
		nX -= (Width / 2);

	if ((align & LevelName))
		nY -= 16;

	//Align to Right
	if (align & Right)
		nX = nX = *D2Client::ScreenSizeX_I - D2Win::GetTextWidth_I(wString, wcslen(wString)) - 16;;

	//Draw and revert font
	D2Win::DrawText(wString, nX, nY, color, 0);
	D2Win::SetTextSize(dwOldFont);

}

void WideTexthookDraw(HookDrawType type, unsigned int x, unsigned int y, unsigned int color, unsigned int font, HookAlign align, wstring text, ...) {
	//Format the given text.
	wchar_t wString[512]; 
	va_list arg; 
	va_start(arg, text);
	vswprintf_s(wString, 512, text.c_str(), arg); 
	va_end(arg); 

	//Set wanted font size
	unsigned OldFont = D2Win::SetTextSize(font);

	//Get the text width.
	DWORD Width, FileNo;
	D2Win::GetTextWidthFileNo(wString, &Width, &FileNo);

	//Set base x/y to modify
	int nX = x;
	int nY = y;

	//Check for automap, if so convert coords
	if (type == Automap) {
		POINT ptPoint = {x, y};
		ToAutomap(&ptPoint);
		nX = ptPoint.x;
		nY = ptPoint.y;
	}
	//Align to center
	if ((align & Center) && !(type == Automap))
		nX = (800 / 2) - (Width / 2);

	//Align to Center From X
	if ((align & CenterFromX) || (align & Center) && (type == Automap) || (align & LevelName))
		nX -= (Width / 2);

	if ((align & LevelName))
		nY -= 16;

	//Align to Right
	if (align & Right)
		nX = *D2Client::ScreenSizeX_I - D2Win::GetTextWidth_I(wString, wcslen(wString)) - 16;
		
	D2Win::DrawText(wString, nX, nY, color, 0);
	D2Win::SetTextSize(OldFont);
}

void BoxhookDraw(HookDrawType type, unsigned int x, unsigned int y, unsigned int xSize, unsigned int ySize, unsigned int color, BoxTrans trans, HookAlign align) {
	 // 미사일 세팅 부분
	//Set base x/y to modify
	int nX = x;
	int nY = y;

	//Check for automap, if so convert coords
	if (type == Automap) {
		POINT ptPoint = {x, y};
		ToAutomap(&ptPoint);
		nX = ptPoint.x;
		nY = ptPoint.y;
	}

	//Align to center
	if ((align & Center) && !(type == Automap))
		nX = (800 / 2) - (xSize / 2);

	//Align to Center From X
	if ((align & CenterFromX) || (align & Center) && (type == Automap))
		nX -= (xSize / 2);

	//Align to Right
	if (align & Right)
		nX = ((type == Automap) ? nX : 800) - xSize;

	D2Gfx::DrawRectangle(nX, nY, nX + xSize, nY + ySize, color, trans);
}

void CrosshookDraw(unsigned int x, unsigned int y, unsigned int color,bool Automap) {
	//CHAR szLines[][2] = {0,-8, -5,0, 5, 0, 0,-8};
	CHAR szLines[][2] = {0,-2, 4,-4, 8,-2, 4,0, 8,2, 4,4, 0,2, -4,4, -8,2, -4,0, -8,-2, -4,-4, 0,-2};

	POINT pPoint = {x,y};
	if (Automap)
	{
		ScreenToAutomap(&pPoint, x, y);
	}
	else ToAutomap(&pPoint);
	for(unsigned int x = 0; x < 12; x++)
		D2Gfx::DrawLine(pPoint.x + szLines[x][0], pPoint.y + szLines[x][1], pPoint.x + szLines[x+1][0], pPoint.y + szLines[x+1][1], color, -1);
}