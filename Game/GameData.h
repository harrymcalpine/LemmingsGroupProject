#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

//=================================================================
//Data to be passed by game to all Game Objects via Tick
//=================================================================
#include <dinput.h>
#include "Keyboard.h"
#include "Mouse.h"
#include "GameState.h"
#include "RenderTarget.h"
#include "ObjectsManager.h"

using namespace DirectX;

struct GameData
{
	float m_dt;  //time step since last frame
	GameState m_GS; //global GameState
	RenderTarget* m_RD;
	ObjectsManager* m_OM;
	RenderTarget* m_SecRD;
	RenderTarget* m_ThrRD;
	int m_lemmingsKilled = 0;
	int m_lemmingsSaved = 0;
	//player input
	unsigned char* m_keyboardState; //current state of the Keyboard
	unsigned char* m_prevKeyboardState; //previous frame's state of the keyboard
	DIMOUSESTATE* m_mouseState; //current state of the mouse

	UINT width;
	UINT height;
	int m_mouseX = 0, m_mouseY = 0;
};
#endif
