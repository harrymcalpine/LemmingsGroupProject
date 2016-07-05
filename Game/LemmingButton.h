#ifndef _LEMMING_BUTTON_H_
#define _LEMMING_BUTTON_H_
#include "DrawData2D.h"
#include "LemmingState.h"
#include "Boundingbox.h"

class Boundingbox;

class LemmingButton
{
public:
	LemmingButton(string _filename, ID3D11Device* _GD, PASS _pass, Vector2 _pos, int _LS);
	~LemmingButton();
	void Draw(DrawData2D* _DD) { m_box->Draw(_DD); }
	LemmingState getState() { return m_state; }
	bool checkForHit(GameData* _GD) { return m_box->CollideMouse(_GD); }

protected:
	Boundingbox* m_box;
	LemmingState m_state;
};
#endif
