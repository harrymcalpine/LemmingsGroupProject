#include "ButtonManager.h"
#include "LemmingButton.h"
#include "LemmingState.h"

ButtonManager::ButtonManager(ID3D11Device* _GD, PASS _pass)
{
	for (int i = 0; i < 9; i++)
	{
		myButtons.push_back(new LemmingButton("Button" + std::to_string(i + 1), _GD, _pass, Vector2(30 + 38 * i, 500), i + 1));
	}
}

ButtonManager::~ButtonManager()
{
	for (list<LemmingButton *>::iterator it = myButtons.begin(); it != myButtons.end(); it++)
	{
		delete (*it);
	}
	myButtons.clear();
}

void ButtonManager::Draw(DrawData2D* _DD)
{
	for (list<LemmingButton *>::iterator it = myButtons.begin(); it != myButtons.end(); it++)
	{
		(*it)->Draw(_DD);
	}
}

LemmingState ButtonManager::CheckButtons(GameData* _GD)
{
	for (list<LemmingButton*> ::iterator it = myButtons.begin(); it != myButtons.end(); it++)
	{
		if ((*it)->checkForHit(_GD))
		{
			return (*it)->getState();
		}
	}
	return LS_NULL;
}