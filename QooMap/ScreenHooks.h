#pragma once

#include <string>
#include <iostream>
#include <list>
//#include <algorithm>

using namespace std;

enum HookType {
	BoxHook,
	DrawHook,
	TextHook
};

enum HookDrawType {
	InGame, 
	OutOfGame, 
	Automap, 
	Perm, 
	Group
};

enum BoxTrans {
	BTThreeFourths, 
	BTOneHalf, 
	BTOneFourth, 
	BTWhite, 
	BTBlack, 
	BTNormal, 
	BTScreen, 
	BTHighlight, 
	BTFull
};

enum HookAlign {
	None=1, 
	Left=1, 
	Center=2, 
	Right=4, 
	Top=8, 
	CenterFromX=16,
	LevelName=32
};

enum HookLayer {
	LayerGame=1,
	LayerAutomap=2
};

struct ShortPoint {
	unsigned short PosX;
	unsigned short PosY;
};

struct TextData {
	wchar_t* String;
	unsigned char Font;
};

struct BoxData {
	BoxTrans Opacity;
	unsigned short SizeX;
	unsigned short SizeY;
};

struct ScreenHook {
	unsigned int HookId;
	ShortPoint* Pos;
	HookLayer Layer;
	HookType Type;
	HookDrawType DrawType;
	HookAlign Alignment;
	unsigned char Colour;
	union {
		BoxData* BoxInfo;
		TextData* TextInfo;
	};
};

typedef list<ScreenHook*> ScreenHooks;

class ScreenHookManager {
protected:
	ScreenHooks screenHooks;
	ScreenHooks automapHooks;
	unsigned int nextHookId;
	static ScreenHookManager* instance;

private:
	ScreenHookManager() : nextHookId(1) {}
	~ScreenHookManager() {}
	ScreenHookManager(const ScreenHookManager &);
	ScreenHookManager & operator= (ScreenHookManager const &);

public:
	static ScreenHookManager * Instance();
	unsigned int CreateTextHook(wchar_t* String, HookDrawType DrawType, ShortPoint* Pos, HookAlign Align, unsigned char Colour, unsigned char Font);
	//void AddHook(ScreenHook* Hook) {} // for now create adds to the list
	//void RemoveHook(ScreenHook* Hook) {}
	void DrawTextHook(ScreenHook* Hook);
	void DrawBoxHook(ScreenHook* Hook);
	void DrawScreenHooks();
	void DrawAutomapHooks();
};

// DEPRECATE
void TexthookDraw(HookDrawType type, unsigned int x, unsigned int y, unsigned int color, unsigned int font, HookAlign align, string text, ...);
void WideTexthookDraw(HookDrawType type, unsigned int x, unsigned int y, unsigned int color, unsigned int font, HookAlign align, wstring text, ...);
void CrosshookDraw(unsigned int x, unsigned int y, unsigned int color,bool Automap);
void BoxhookDraw(HookDrawType type, unsigned int x, unsigned int y, unsigned int xSize, unsigned int ySize, unsigned int color, BoxTrans trans, HookAlign align);
