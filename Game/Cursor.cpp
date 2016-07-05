#include "Cursor.h"
#include "ImageGO2D.h"
#include "DrawData2D.h"
#include "GameData.h"
#include <iostream>
#include "game.h"
#include <mutex>
#include <DirectXCollision.h>

Cursor::Cursor(string _filename, ID3D11Device* _GD, PASS _pass) : ImageGO2D(_filename, _GD, _pass)
{
	m_mouseX = 0;
	m_mouseY = 0;
	//BoundingSphere(Vector3(0.0f, 0.0f, 0.0f), 5.0f);
}

Cursor::~Cursor()
{
}

void Cursor::Tick(GameData* _GD)
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	_GD->m_mouseX += _GD->m_mouseState->lX;
	_GD->m_mouseY += _GD->m_mouseState->lY;

	m_screenHeight = _GD->height;
	m_screenWidth = _GD->width;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (_GD->m_mouseX < 0)  { _GD->m_mouseX = 0; }
	if (_GD->m_mouseY < 0)  { _GD->m_mouseY = 0; }

	if (_GD->m_mouseX > m_screenWidth)  { _GD->m_mouseX = m_screenWidth; }
	if (_GD->m_mouseY > m_screenHeight) { _GD->m_mouseY = m_screenHeight; }

	m_pos.x = _GD->m_mouseX;
	m_pos.y = _GD->m_mouseY;

	// For debugging where the cursor is in the window
	//cout << _GD->m_mouseX << " " << _GD->m_mouseY << endl;

	LeftMouseClick(_GD);
	GameObject2D::Tick(_GD);
}

void Cursor::GetMouseLocation(GameData* _GD, int& mouseX, int& mouseY)
{
	mouseX = _GD->m_mouseX;
	mouseY = _GD->m_mouseY;
}

void Cursor::LeftMouseClick(GameData* _GD)
{
	if (_GD->m_mouseState->rgbButtons[0] & 0x80)
	{
			m_check = true;
			GetMouseLocation(_GD, _GD->m_mouseX, _GD->m_mouseY);	
	}
}
