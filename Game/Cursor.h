#ifndef  _CURSOR_
#define  _CURSOR_

#include <dinput.h>
#include "ImageGO2D.h"

class Cursor : public ImageGO2D
{
public:
	Cursor(string _fileName, ID3D11Device* _GD, PASS _pass);
	~Cursor();

	virtual void Tick(GameData* _GD);
	void GetMouseLocation(GameData* _GD, int& mouseX, int& mouseY);
	void LeftMouseClick(GameData* _GD);
protected:
	int m_mouseX, m_mouseY;
	int m_screenWidth, m_screenHeight;
	bool m_check;
};

#endif _CURSOR_