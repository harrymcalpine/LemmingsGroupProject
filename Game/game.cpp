#include "game.h"
#include "GameData.h"
#include "drawdata.h"
#include "DrawData2D.h"
#include "gameobject.h"
#include "ObjectList.h"
#include "helper.h"
#include <windows.h>
#include <time.h>
#include "DDSTextureLoader.h"
#include <d3d11shader.h>
#include "ObjectsManager.h"
#include "VBplane.h"
#include "RenderTarget.h"
#include "Cursor.h"
#include "Boundingbox.h"
#include "ImageGO2D.h"

using namespace DirectX;

Game::Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance) :m_playTime(0), m_fxFactory(nullptr), m_states(nullptr)
{
	//Create DirectXTK spritebatch stuff
	ID3D11DeviceContext* pd3dImmediateContext;
	_pd3dDevice->GetImmediateContext(&pd3dImmediateContext);
	m_DD2D = new DrawData2D();
	m_DD2D->m_Sprites.reset(new SpriteBatch(pd3dImmediateContext));
	m_DD2D->m_Font.reset(new SpriteFont(_pd3dDevice, L"italic.spritefont"));
	m_playTime = 0;

	//seed the random number generator
	srand((UINT)time(NULL));

	//Direct Input Stuff
	m_hWnd = _hWnd;
	m_pKeyboard = nullptr;
	m_pDirectInput = nullptr;

	HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pKeyboard->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	hr = m_pMouse->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	//create GameData struct and populate its pointers
	m_GD = new GameData;
	m_GD->m_keyboardState = m_keyboardState;
	m_GD->m_prevKeyboardState = m_prevKeyboardState;
	m_GD->m_GS = GS_LOGO;
	m_GD->m_mouseState = &m_mouseState;

	//set up DirectXTK Effects system
	m_fxFactory = new EffectFactory(_pd3dDevice);

	//Tell the fxFactory to look to the correct build directory to pull stuff in from
#ifdef DEBUG
	((EffectFactory*)m_fxFactory)->SetDirectory(L"../Debug");
#else
	((EffectFactory*)m_fxFactory)->SetDirectory(L"../Release");
#endif

	// Create other render resources here
	m_states = new CommonStates(_pd3dDevice);

	//init render system for VBGOs
	VBGO::Init(_pd3dDevice);

	//find how big my window is to correctly calculate my aspect ratio
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_GD->width = rc.right - rc.left;
	m_GD->height = rc.bottom - rc.top;
	float AR = (float)m_GD->width / (float)m_GD->height;

	//create a base camera
	m_cam = new Camera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3(0.0f, 0.0f, -1000.0f));
	m_cam->SetPos(Vector3(0.0f, 0.0f, 100.0f));
	m_GameObjects.push_back(m_cam);

	//create a base light
	m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.5f, 0.5f, 0.5f, 1.0f));
	m_GameObjects.push_back(m_light);

	//create DrawData struct and populate its pointers
	m_DD = new DrawData;
	m_DD->m_pd3dImmediateContext = nullptr;
	m_DD->m_states = m_states;
	m_DD->m_cam = m_cam;
	m_DD->m_light = m_light;

	//add a player
	Player* pPlayer = new Player("BirdModelV1.cmo", _pd3dDevice, m_fxFactory);
	m_GameObjects.push_back(pPlayer);

	//add a secondary camera
	m_TPScam = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 10.0f, 50.0f));
	m_GameObjects.push_back(m_TPScam);

	Terrain* terrain = new Terrain("table.cmo", _pd3dDevice, m_fxFactory, Vector3(100.0f, 0.0f, 100.0f), 80.0f, 0.0f, 0.0f, 0.25f * Vector3::One);
	//m_GameObjects.push_back(terrain);

	Cursor* cursor = new Cursor("Cursor", _pd3dDevice, ANIMATION_PASS);
	cursor->SetPos(Vector2(400, 300));
	m_GameObjectMouse.push_back(cursor);

	VBCube* cube = new VBCube();
	cube->init(11, _pd3dDevice);
	cube->SetPos(Vector3(100.0f, 0.0f, 0.0f));
	cube->SetScale(1.0f);
	//m_GameObjects.push_back(cube);

	ImageGO2D* Level = new ImageGO2D("Level1", _pd3dDevice, FIRST_PASS);
	Level->SetPos(Vector2(400, 350));
	m_Terrain.push_back(Level);

	ObjectsManager* m_OM = new ObjectsManager(_pd3dDevice, 5, 5, 5);
	m_GD->m_OM = m_OM;

	m_lemManager = new LemmingManager("SpawnerDDS", _pd3dDevice, 20, ANIMATION_PASS, 100.0f);
	m_lemManager->SetPos(Vector2(400, 350));
	m_GameObject2Ds.push_back(m_lemManager);

	//RENDER TARGET INFORMATION AND THE PLANE IT IS LOADED ONTO
	m_RD = new RenderTarget(_pd3dDevice, m_GD->width, m_GD->height);
	m_GD->m_RD = m_RD;

	VBPlane* LBox = new VBPlane();
	LBox->init(100, _pd3dDevice, m_RD->GetShaderResourceView());
	LBox->SetPos(Vector3(100.0f, 0.0f, -10.0f));
	LBox->SetScale(1.0f);
	m_GameObjects.push_back(LBox);

	m_SecRD = new RenderTarget(_pd3dDevice, m_GD->width, m_GD->height);
	m_GD->m_SecRD = m_SecRD;

	VBPlane* GamePlane = new VBPlane();
	GamePlane->init(100, _pd3dDevice, m_SecRD->GetShaderResourceView());
	GamePlane->SetPos(Vector3(0.0f, 0.0f, -50.0f));
	GamePlane->SetScale(1.0f);
	m_GameObjects.push_back(GamePlane);

	m_buttonManager = new ButtonManager(_pd3dDevice, ANIMATION_PASS);
	m_GameObject2DsScreen.push_back(m_buttonManager);

#pragma endregion

#pragma region LogoScreen

	ImageGO2D* LogoState = new ImageGO2D("lemmings_logo", _pd3dDevice, ANIMATION_PASS);
	LogoState->SetPos(Vector2(400, 100));
	m_GameObjectLogo.push_back(LogoState);

	ImageGO2D* TeamLogo = new ImageGO2D("Team Logo", _pd3dDevice, ANIMATION_PASS);
	TeamLogo->SetPos(Vector2(400, 350));
	m_GameObjectLogo.push_back(TeamLogo);

	ImageGO2D* Background = new ImageGO2D("Soil-Background", _pd3dDevice, ANIMATION_PASS);
	Background->SetPos(Vector2(0, 0));
	m_GameObjectLogo.push_front(Background);

#pragma endregion

#pragma region Options

	ImageGO2D* optionsState = new ImageGO2D("lemmings_logo", _pd3dDevice, ANIMATION_PASS);
	optionsState->SetPos(Vector2(400, 100));
	m_GameObjectOptions.push_back(optionsState);

	ImageGO2D* TeamLogo2 = new ImageGO2D("Team Logo", _pd3dDevice, ANIMATION_PASS);
	TeamLogo2->SetPos(Vector2(400, 350));
	m_GameObjectOptions.push_back(TeamLogo2);

	ImageGO2D* Background2 = new ImageGO2D("Soil-Background", _pd3dDevice, ANIMATION_PASS);
	Background2->SetPos(Vector2(0, 0));
	m_GameObjectOptions.push_front(Background2);

#pragma endregion

#pragma region GameOver

	ImageGO2D* TeamLogo3 = new ImageGO2D("Team Logo", _pd3dDevice, ANIMATION_PASS);
	TeamLogo3->SetPos(Vector2(400, 350));
	m_GameObjectOptions.push_back(TeamLogo3);

	ImageGO2D* Background3 = new ImageGO2D("Soil-Background", _pd3dDevice, ANIMATION_PASS);
	Background3->SetPos(Vector2(0, 0));
	m_GameObjectOptions.push_front(Background3);

#pragma endregion
}

Game::~Game()
{
	//delete Game Data & Draw Data
	delete m_GD;
	delete m_DD;

	//tidy up VBGO render system
	VBGO::CleanUp();

	//tidy away Direct Input Stuff
	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
	}
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
	}
	if (m_pDirectInput)
	{
		m_pDirectInput->Release();
	}

	//get rid of the game objects here
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		delete (*it);
	}
	m_GameObjects.clear();

	//and the 2D ones
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		delete (*it);
	}
	m_GameObject2Ds.clear();

	//and the 2D Screen ones
	for (list<GameObject2D *>::iterator it = m_GameObject2DsScreen.begin(); it != m_GameObject2DsScreen.end(); it++)
	{
		delete (*it);
	}
	m_GameObject2DsScreen.clear();
	//clear away CMO render system
	delete m_states;
	delete m_fxFactory;

	delete m_DD2D;
}

bool Game::Update()
{
	//Poll Keyboard & Mouse
	ReadInput();

	//calculate frame time-step dt for passing down to game objects
	DWORD currentTime = GetTickCount();
	m_GD->m_dt = min((float)(currentTime - m_playTime) / 1000.0f, 0.1f);
	m_playTime = currentTime;

	//if paused press return to exit
	if (m_GD->m_GS == GS_PAUSE)
	{
		if (m_keyboardState[DIK_RETURN] & 0x80)
		{
			return false;
		}
	}

	if (m_GD->m_GS == GS_LOGO)
	{
		idleTimer -= m_GD->m_dt;
		if (m_keyboardState[DIK_X] & 0x80)
		{
			m_GD->m_GS = GS_OPTIONS;
			idleTimer = 30.0f;
		}
		if (idleTimer <= 0.1f)
		{
			m_GD->m_GS = GS_OPTIONS;
			idleTimer = 30.0f;
		}
	}

	if ((m_GD->m_lemmingsKilled + m_GD->m_lemmingsSaved) == 20)
	{
		m_GD->m_GS = GS_GAME_OVER;
		idleTimer = 30.0f;
		m_GD->m_lemmingsKilled = 0;
		m_GD->m_lemmingsSaved = 0;
	}

	if (m_GD->m_GS == GS_GAME_OVER)
	{
		idleTimer -= m_GD->m_dt;
		if (m_keyboardState[DIK_X] & 0x80)
		{
			m_GD->m_GS = GS_OPTIONS;
			idleTimer = 30.0f;
		}
		if (idleTimer <= 0.1f)
		{
			m_GD->m_GS = GS_OPTIONS;
			idleTimer = 30.0f;
		}
	}

	if (m_GD->m_GS == GS_OPTIONS)
	{
		idleTimer -= m_GD->m_dt;
		if (m_keyboardState[DIK_RETURN] & 0x80)
		{
			m_GD->m_GS = GS_ATTRACT;
			idleTimer = 30.0f;
		}
		if (idleTimer <= 0.1f)
		{
			m_GD->m_GS = GS_ATTRACT;
			idleTimer = 30.0f;
		}
		if (m_keyboardState[DIK_ESCAPE] & 0x80)
		{
			return false;
		}
	}
	//lock the cursor to the centre of the window
	RECT window;
	GetWindowRect(m_hWnd, &window);

	//	SetCursorPos((window.left + window.right) >> 1, (window.bottom + window.top) >> 1);

	if (m_GD->m_GS == GS_LOGO)
	{
		//update all objects
		for (list<GameObject2D *>::iterator it = m_GameObjectLogo.begin(); it != m_GameObjectLogo.end(); it++)
		{
			(*it)->Tick(m_GD);
		}
		for (list<GameObject2D *>::iterator it = m_GameObjectMouse.begin(); it != m_GameObjectMouse.end(); it++)
		{
			(*it)->Tick(m_GD);
		}
	}

	if (m_GD->m_GS == GS_OPTIONS)
	{
		//update all objects
		for (list<GameObject2D *>::iterator it = m_GameObjectOptions.begin(); it != m_GameObjectOptions.end(); it++)
		{
			(*it)->Tick(m_GD);
		}
		for (list<GameObject2D *>::iterator it = m_GameObjectMouse.begin(); it != m_GameObjectMouse.end(); it++)
		{
			(*it)->Tick(m_GD);
		}
	}
	if (m_GD->m_GS == GS_GAME_OVER)
	{
		//update all objects
		for (list<GameObject2D *>::iterator it = m_GameObjectGameOver.begin(); it != m_GameObjectGameOver.end(); it++)
		{
			(*it)->Tick(m_GD);
		}
		for (list<GameObject2D *>::iterator it = m_GameObjectMouse.begin(); it != m_GameObjectMouse.end(); it++)
		{
			(*it)->Tick(m_GD);
		}
	}
	if ((m_GD->m_GS == GS_ATTRACT) || (m_GD->m_GS == GS_PLAY_TPS_CAM) || (m_GD->m_GS == GS_PAUSE))
	{
		//On Escape press pauses, if paused pressing esc will unpause
		if ((m_keyboardState[DIK_ESCAPE] & 0x80) && !(m_prevKeyboardState[DIK_ESCAPE] & 0x80))
		{
			if (m_GD->m_GS == GS_PAUSE)
			{
				m_GD->m_GS = GS_ATTRACT;
			}
			else
			{
				m_GD->m_GS = GS_PAUSE;
			}
		}
		//apply selected job to selected lemmings
		m_lemManager->ApplyJob(m_buttonManager->CheckButtons(m_GD));
		//update all objects
		for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
		{
			(*it)->Tick(m_GD);
		}
		for (list<GameObject2D *>::iterator it = m_GameObjectMouse.begin(); it != m_GameObjectMouse.end(); it++)
		{
			(*it)->Tick(m_GD);
		}
		for (list<GameObject2D *>::iterator it = m_GameObject2DsScreen.begin(); it != m_GameObject2DsScreen.end(); it++)
		{
			(*it)->Tick(m_GD);
		}
		if (m_GD->m_GS != GS_PAUSE)
		{
			for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
			{
				(*it)->Tick(m_GD);
			}
		}
	}
	//system("cls");
	//	cout << m_GD->m_GS << " " << idleTimer;
	//idleTimer -= m_GD->m_dt;
	return true;
}

void Game::Render(ID3D11DeviceContext* _pd3dImmediateContext)
{
	//set immediate context of the graphics device
	m_DD->m_pd3dImmediateContext = _pd3dImmediateContext;

	//set which camera to be used
	m_DD->m_cam = m_cam;
	if (m_GD->m_GS == GS_PLAY_TPS_CAM)
	{
		m_DD->m_cam = m_TPScam;
	}

	//update the constant buffer for the rendering of VBGOs
	VBGO::UpdateConstantBuffer(m_DD);

	if (m_GD->m_GS == GS_LOGO)
	{
		m_DD2D->m_Pass = ANIMATION_PASS;
		//update all objects
		m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred);// , m_states->Additive());
		for (list<GameObject2D *>::iterator it = m_GameObjectLogo.begin(); it != m_GameObjectLogo.end(); it++)
		{
			(*it)->Draw(m_DD2D);
			//m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar("Logo State"), Vector2(0, 0), Colors::Yellow);

			m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar("Press X To Go To The Menu"), Vector2(150, 500), Colors::Yellow);
		}
		m_DD2D->m_Sprites->End();
	}

	if (m_GD->m_GS == GS_GAME_OVER)
	{
		m_DD2D->m_Pass = ANIMATION_PASS;
		//update all objects
		m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred);// , m_states->Additive());
		for (list<GameObject2D *>::iterator it = m_GameObjectLogo.begin(); it != m_GameObjectLogo.end(); it++)
		{
			(*it)->Draw(m_DD2D);
			//m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar("Logo State"), Vector2(0, 0), Colors::Yellow);
			m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar("Game Over!"), Vector2(150, 100), Colors::Yellow);
			m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar("Press X To Go To The Menu"), Vector2(150, 500), Colors::Yellow);
		}
		m_DD2D->m_Sprites->End();
	}

	if (m_GD->m_GS == GS_OPTIONS)
	{
		m_DD2D->m_Pass = ANIMATION_PASS;
		//update all objects
		m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred);// , m_states->Additive());
		for (list<GameObject2D *>::iterator it = m_GameObjectOptions.begin(); it != m_GameObjectOptions.end(); it++)
		{
			(*it)->Draw(m_DD2D);
			//m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar("Options State"), Vector2(0, 0), Colors::Yellow);
			m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar("Press Enter To Play The Game"), Vector2(150, 500), Colors::Yellow);
			m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar("Or ESC To Exit The Game"), Vector2(150, 550), Colors::Yellow);
		}
		m_DD2D->m_Sprites->End();
	}

	if ((m_GD->m_GS == GS_ATTRACT) || (m_GD->m_GS == GS_PLAY_TPS_CAM) || (m_GD->m_GS == GS_PAUSE))
	{
		for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
		{
			(*it)->Draw(m_DD);
		}

		if (firstPass)
		{
			m_DD2D->m_Pass = FIRST_PASS;
			if (m_RD->GetMapped())
			{
				m_RD->Unmap(m_DD->m_pd3dImmediateContext);
			};

			m_RD->Begin(m_DD->m_pd3dImmediateContext);
			m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred);// , m_states->Additive());
			for (list<GameObject2D *>::iterator it = m_Terrain.begin(); it != m_Terrain.end(); it++)
			{
				(*it)->Draw(m_DD2D);
			}
			ImageGO2D* terrain = new ImageGO2D(m_RD->GetShaderResourceView(), ANIMATION_PASS);
			m_GameObject2Ds.push_back(terrain);
			m_DD2D->m_Sprites->End();
			m_RD->End(m_DD->m_pd3dImmediateContext);
			m_RD->Map(m_DD->m_pd3dImmediateContext);
			firstPass = false;
		}

		//Delete Pass
		//Anything Drawn in here will render as deleted space, taking chunks out of
		m_DD2D->m_Pass = DELETE_PASS;
		if (m_RD->GetMapped())
		{
			m_RD->Unmap(m_DD->m_pd3dImmediateContext);
		};
		m_RD->Begin(m_DD->m_pd3dImmediateContext);
		m_DD2D->m_Sprites->Begin(SpriteSortMode_Texture, m_RD->GetDigBlend());
		for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
		{
			(*it)->Draw(m_DD2D);
		}
		m_GD->m_OM->AddHoles(m_DD2D);
		m_DD2D->m_Sprites->End();
		m_RD->End(m_DD->m_pd3dImmediateContext);
		m_RD->Map(m_DD->m_pd3dImmediateContext);

		m_SecRD->ClearRenderTarget(_pd3dImmediateContext, 0.0f, 0.0f, 0.0f, 0.0f);

		m_DD2D->m_Pass = ANIMATION_PASS;
		if (m_SecRD->GetMapped())
		{
			m_SecRD->Unmap(m_DD->m_pd3dImmediateContext);
		};
		m_SecRD->Begin(m_DD->m_pd3dImmediateContext);
		m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred);
		for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
		{
			(*it)->Draw(m_DD2D);
		}
		m_GD->m_OM->AddBricks(m_DD2D);				//Brick McAddems
		m_DD2D->m_Sprites->End();
		m_SecRD->End(m_DD->m_pd3dImmediateContext);
		m_SecRD->Map(m_DD->m_pd3dImmediateContext);

		m_DD2D->m_Sprites->Begin();
		for (list<GameObject2D *>::iterator it = m_GameObject2DsScreen.begin(); it != m_GameObject2DsScreen.end(); it++)
		{
			(*it)->Draw(m_DD2D);
			m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar("To exit press ESC then Enter"), Vector2(100, 100), Colors::Yellow);
		}
		m_DD2D->m_Sprites->End();
	}
	m_DD2D->m_Sprites->Begin();
	for (list<GameObject2D *>::iterator it = m_GameObjectMouse.begin(); it != m_GameObjectMouse.end(); it++)
	{
		(*it)->Draw(m_DD2D);
	}
	m_DD2D->m_Sprites->End();
	//drawing text screws up the Depth Stencil State, this puts it back again!
	_pd3dImmediateContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
}

bool Game::ReadInput()
{
	//copy over old keyboard state
	memcpy(m_prevKeyboardState, m_keyboardState, sizeof(unsigned char) * 256);

	//clear out previous state
	ZeroMemory(&m_keyboardState, sizeof(unsigned char) * 256);
	ZeroMemory(&m_mouseState, sizeof(DIMOUSESTATE));

	// Read the keyboard device.
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	// Read the Mouse device.
	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(hr))
	{
		// If the Mouse lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}