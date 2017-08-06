///
/// Game Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "..\..\vEngine\vAudio.h"
#include "Camera.h"
#include "Table.h"
#include "Block.h"
#include <time.h>
#include <sstream>
#include "resource.h"
#include "..\..\vutil\vutil.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int GameLoop();
bool GameInit(HINSTANCE hInstance, UINT windowWidth, UINT windowHeight);
void GameCleanup();
bool LoadLevel(TCHAR * level);

#define CLASS_NAME TEXT("Unb1okClass")
#define APP_NAME TEXT("Unb1ok")
#define FULLSCREEN_Q TEXT("Use fullscreen?")

#define DEFXRES 800
#define DEFYRES 600

#define LEVELS 1200

typedef struct
{
	UINT ResX;
	UINT ResY;
	BOOL FullScreen;
	UINT LastLevelPlayed;
	BOOL LevelPassed[LEVELS];
	BOOL Sound;
} SETTINGS, *PSETTINGS;

#endif