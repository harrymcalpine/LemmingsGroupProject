#include "Boundingbox.h"
#include <iostream>
#include "GameData.h"

Boundingbox::Boundingbox(string _filename, ID3D11Device* _GD, PASS _pass, float _width, float _height, Vector2 _pos) : ImageGO2D(_filename, _GD, _pass)//GameObject2D* _parent,
{
	m_pos = _pos;
	m_height = _height;
	m_width = _width;
}

bool Boundingbox::CollideMouse(GameData* _GD)
{
	if ((_GD->m_mouseX > getMinX()) && (_GD->m_mouseX < getMaxX()))
	{
		if ((_GD->m_mouseY > getMinY()) && (_GD->m_mouseY < getMaxY()))
		{
			if (_GD->m_mouseState->rgbButtons[0] & 0x80)
			{
				//std::cout << m_buttonoutput << endl;
				//std::cout << numberding;
				// return numberding;

				return true;
			}
		}
	}
	return false;
}

bool Boundingbox::CollideBox(Boundingbox* _Box)
{
	if (getMinX() > _Box->getMaxX()) return false;
	if (getMaxX() < _Box->getMinX()) return false;
	if (getMinY() > _Box->getMaxY()) return false;
	if (getMaxY() < _Box->getMinY()) return false;
	return true;
}