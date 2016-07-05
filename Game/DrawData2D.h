#ifndef _DRAW_DATA_2D_H_
#define _DRAW_DATA_2D_H_

//=================================================================
//Data to be passed by game to all 2D Game Objects via Draw
//=================================================================
#include "SpriteBatch.h"
#include "SpriteFont.h"

using namespace DirectX;
using namespace std;

enum PASS
{
	NO_PASS,
	FIRST_PASS,
	DELETE_PASS,
	ANIMATION_PASS
};

struct DrawData2D
{
	ID3D11DeviceContext* m_pd3dImmediateContext;
	//spritebatch stuff
	unique_ptr<SpriteBatch>	m_Sprites;
	unique_ptr<SpriteFont> m_Font;
	PASS m_Pass = FIRST_PASS;
};

#endif