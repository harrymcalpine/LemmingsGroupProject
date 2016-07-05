#ifndef _GAME_H_
#define _GAME_H_

//=================================================================
//Basic Game Manager Class
//=================================================================

#include <windows.h>
#include <list>
#include <dinput.h>
#include "Effects.h"
#include "CommonStates.h"
#include "Keyboard.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "RenderTarget.h"
#include "ObjectsManager.h"
#include "helper.h"

using std::list;
using std::unique_ptr;
using namespace DirectX;

// Forward declarations
class GameObject;
class GameObject2D;
class Camera;
class TPSCamera;
struct GameData;
struct DrawData;
struct DrawData2D;
class Light;
class ObjectsManager;
class LemmingManager;
class ButtonManager;

class Game
{
public:

	Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance);
	~Game();

	bool Update(); //tick the game state

	void Render(ID3D11DeviceContext* _pd3dImmediateContext); //render the current game state

protected:
	DWORD m_playTime; //amount of time since the game started

	Camera* m_cam; //principle camera
	TPSCamera* m_TPScam;//TPS cam
	Light* m_light; //base light
	LemmingManager* m_lemManager; //Lemming manager
	ButtonManager* m_buttonManager; //On screen buttons

	list<GameObject *> m_GameObjects; //data structure storing all GameObjects of this Game
	list<GameObject2D *> m_GameObject2Ds; //ditto 2D objects
	list<GameObject2D *> m_GameObjectLogo;
	list<GameObject2D *> m_GameObjectMouse;
	list<GameObject2D *> m_GameObjectOptions;
	list<GameObject2D *> m_GameObjectGameOver;
	list<GameObject2D *> m_GameObject2DsScreen;//ditto 2D objects that render to the screen
	list<GameObject2D *> m_Terrain;

	//required for the CMO model rendering system
	CommonStates* m_states;
	IEffectFactory* m_fxFactory;

	//direct input stuff
	bool ReadInput(); //Get current Mouse and Keyboard states
	//void GetMouseLocation(int& mouseX, int& mouseY);
	IDirectInput8*			m_pDirectInput;
	IDirectInputDevice8*	m_pKeyboard;
	IDirectInputDevice8*	m_pMouse;
	unsigned char			m_keyboardState[256];
	unsigned char			m_prevKeyboardState[256];
	DIMOUSESTATE			m_mouseState;
	HWND m_hWnd;

	unique_ptr<SpriteBatch> m_Sprites;
	unique_ptr<SpriteFont> m_Font;

	GameData* m_GD;			//Data to be shared to all Game Objects as they are ticked
	DrawData* m_DD;			//Data to be shared to all Game Objects as they are drawn
	DrawData2D * m_DD2D;	//Data to be passed by game to all 2D Game Objects via Draw
	RenderTarget* m_RD;
	RenderTarget* m_SecRD;
	RenderTarget* m_ThrRD;

	bool firstPass = true;
	float idleTimer = 30.0f;
};

#endif
