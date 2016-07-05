#include "LemmingManager.h"
#include "GameData.h"
#include "Lemming.h"
#include "RenderTarget.h"
#include "LemmingButton.h"
#include "Exit.h"
#include <iostream>

LemmingManager::LemmingManager(string _filename, ID3D11Device* _GD, int _LemCap, PASS _pass, float _LevelTimer) : ImageGO2D(_filename, _GD, _pass)
{
	for (int i = 0; i < _LemCap; i++)
	{
		myLemmings.push_back(new Lemming("LemmingDDS", _GD, ANIMATION_PASS));
	}
	m_levelTimer = _LevelTimer;
	m_exit = new Exit(_GD, _pass, Vector2(700, 300));
}

LemmingManager::~LemmingManager()
{
	for (list<Lemming *>::iterator it = myLemmings.begin(); it != myLemmings.end(); it++)
	{
		delete (*it);
	}
	myLemmings.clear();
}

void LemmingManager::Tick(GameData* _GD)
{
	m_spawnTimer -= _GD->m_dt;
	m_levelTimer -= _GD->m_dt;
	if (m_levelTimer < 0)
	{
		Nuke();
	}
	if (m_spawnTimer <= 0)
	{
		m_spawnTimer = 3;
		//Find next lemming to spawn
		for (list<Lemming*>::iterator it = myLemmings.begin(); it != myLemmings.end(); it++)
		{
			if (!(*it)->IsAlive())
			{
				(*it)->Spawn(m_pos);
				break;
			}
		}
	}
	if (!m_firstTick)
	{

		for (list<Lemming*>::iterator it = myLemmings.begin(); it != myLemmings.end(); it++)
		{
			(*it)->Tick(_GD);
			/*if (((*it)->JustExploded()) && ((*it)->IsAlive()))
			{
			for (list<Lemming*>::iterator it2 = myLemmings.begin(); it2 != myLemmings.end(); it2++)
			{
			if ((*it != *it2) && ((*it2)->IsAlive()) && (Vector2::Distance((*it)->GetPos(), (*it2)->GetPos()) < 30.0f))
			{
			(*it2)->KillLemming(_GD);
			}
			}
			(*it)->KillLemming(_GD);
			}*/
			if ((*it)->Stopped())
			{
				for (list<Lemming*>::iterator it2 = myLemmings.begin(); it2 != myLemmings.end(); it2++)
				{
					if ((*it)->m_box->CollideBox((*it2)->m_box))		//if a lemming has collided with a stopper...
					{
						(*it2)->SwitchDirection();						//turn that lemming around
					}
				}
			}
			if ((*it)->m_box->CollideBox(m_exit->m_box))	//if they collide with the exit
			{
				if ((*it)->IsAlive())
				{
					(*it)->KillLemming(_GD, true);				//save the lemming
				}
			}
		}
	}
	else
	{
		m_firstTick = false;
	}
}

void LemmingManager::Draw(DrawData2D* _DD)
{
	for (list<Lemming*> ::iterator it = myLemmings.begin(); it != myLemmings.end(); it++)
	{
		if ((*it)->IsAlive())
		{
			(*it)->Draw(_DD);
		}
	}
	m_exit->m_box->Draw(_DD);
	ImageGO2D::Draw(_DD);
}

void LemmingManager::Nuke()
{
	for (list<Lemming*> ::iterator it = myLemmings.begin(); it != myLemmings.end(); it++)
	{
		(*it)->ChangeOccupation(LS_EXPLOSIVE);
	}
}

void LemmingManager::ApplyJob(LemmingState _LS)
{
//	std::cout << _LS;
	if (_LS == LS_NUKE)
	{
		Nuke();
	}
	else if (_LS != LS_NULL)
	{
		for (list<Lemming*> ::iterator it = myLemmings.begin(); it != myLemmings.end(); it++)
		{
			if ((*it)->IsSelected())
			{
				(*it)->ChangeOccupation(_LS);
			}
		}
	}
}