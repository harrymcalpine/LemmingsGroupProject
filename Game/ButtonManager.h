#ifndef  _ButtonManager_
#define  _ButtonManager_
#include <list>
#include "GameObject2D.h"
#include "LemmingState.h"
#include "DrawData2D.h"

class LemmingButton;

class ButtonManager : public GameObject2D
{
public:
	ButtonManager(ID3D11Device* _GD, PASS _pass);
	~ButtonManager();

	virtual void Draw(DrawData2D* _DD);
	LemmingState CheckButtons(GameData* _GD);
protected:
	list<LemmingButton*> myButtons;
};
#endif _ButtonManager_