#include "LemmingButton.h"
#include "GameData.h"

LemmingButton::LemmingButton(string _filename, ID3D11Device* _GD, PASS _pass, Vector2 _pos, int _LS)
{
	m_box = new Boundingbox(_filename, _GD, _pass, 16, 24, _pos);
	m_state = (LemmingState)_LS;
}

LemmingButton::~LemmingButton()
{

}