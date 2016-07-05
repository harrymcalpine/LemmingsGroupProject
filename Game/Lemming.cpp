#include "Lemming.h"
#include "DrawData2D.h"
#include "GameData.h"
#include <algorithm>
#include "Cursor.h"
#include "RenderTarget.h"
#include "Boundingbox.h"

#include <iostream>
Lemming::Lemming(string _filename, ID3D11Device* _GD, PASS _pass) : ImageGO2D(_filename, _GD, _pass)
{
	m_box = new Boundingbox("LemSelectBox", _GD, _pass, 12, 22, m_pos);
}

Lemming::~Lemming()
{

}

void Lemming::Tick(GameData* _GD)
{

	if (m_alive)
	{
		//movement
		if (((_GD->m_RD->GetPixel(m_pos.x, m_pos.y + 37))->A() < 0.3f) & (m_LS != LS_CLIMBER))				//check pixel far enough beneath lemming to fall
		{
			m_LS = LS_FALLING;														//if it is an 'empty' pixel, start this lemming falling
		}
		if (status & ExplodingFlag)
		{
			if (m_explosionTimer < 0)
			{
				_GD->m_OM->SpawnObject(2, m_pos);
				KillLemming(_GD, false);
				//m_justExploded = true;
			}
			else
			{
				m_explosionTimer -= _GD->m_dt;
			}
		}
		switch (m_LS)
		{
		case LS_FALLING:
			if ((_GD->m_RD->GetPixel(m_pos.x, m_pos.y + 19))->A() > 0.3f)			//check pixel beneath lemming
			{
				if (m_jobTimer > 20.5f)
				{
					KillLemming(_GD, false);
				}
				m_LS = LS_MOVING;													//if it is a 'populated' pixel, stop this lemming from falling
				m_jobTimer = 0;
			}
			else
			{
				if (status & FloaterFlag)
				{
					m_pos.y += m_speed / 2;											//fall at half speed if a floater
				}
				else
				{
					m_pos.y += m_speed;
					m_jobTimer += _GD->m_dt;
				}
			}
			break;
		case LS_MOVING:
			for (int i = 0; i < 18; i++)
			{
				if ((_GD->m_RD->GetPixel(m_pos.x + m_direction * 8, m_pos.y - i))->A() > 0.3f)		//check pixel in front of lemming
				{
					if (status & ClimberFlag)
					{
						m_LS = LS_CLIMBER;
					}
					else
					{
						SwitchDirection();												//if it is a 'populated' pixel, switch this lemming's direction
					}
					break;
				}
			}
			m_pos.x += m_direction * m_speed;												//moves lemmming forward
			VerticalAdjustments(_GD);
			break;
		case LS_CLIMBER:
			if ((_GD->m_RD->GetPixel(m_pos.x, m_pos.y - 20))->A() > 0.3f)
			{
				m_LS = LS_FALLING;
			}
			if ((_GD->m_RD->GetPixel(m_pos.x + m_direction * 20, m_pos.y))->A() < 0.3f)
			{
				m_pos.x += 9 * m_direction;											//move lemming forward a bit onto the ledge
				m_LS = LS_MOVING;
				VerticalAdjustments(_GD);
			}
			else
			{
				m_pos.y -= m_speed;
			}
			break;
		case LS_DIGGER:
			if (m_jobTimer > 1)
			{
				_GD->m_OM->SpawnObject(3, Vector2(m_pos.x, m_pos.y + 15));
				m_pos.y += 15.0f;
				m_jobTimer = 0.0f;
			}
			else
			{
				m_jobTimer += _GD->m_dt;
			}
			break;
		case LS_TUNNELER:
			if (m_jobTimer > 1)
			{
				_GD->m_OM->SpawnObject(3, Vector2(m_pos.x + m_direction * 15, m_pos.y));
				m_pos.x += m_direction * 15.0f;
				m_jobTimer = 0.0f;
			}
			else
			{
				m_jobTimer += _GD->m_dt;
			}
			break;
		case LS_MINER:
			if (m_jobTimer > 1)
			{
				_GD->m_OM->SpawnObject(3, Vector2(m_pos.x + m_direction * 7, m_pos.y + 7));
				m_pos += Vector2(m_direction * 7.0f, 7.0f);
				m_jobTimer = 0.0f;
			}
			else
			{
				m_jobTimer += _GD->m_dt;
			}
			break;
		case LS_STEPPER:
			if (m_jobCounter < 12)
			{
				if (m_jobTimer > 1)
				{
					//place a brick
					_GD->m_OM->SpawnObject(1, Vector2(m_pos.x + m_direction * (10 + m_jobCounter * 6.0f), m_pos.y + 15 - m_jobCounter * 4.0f));
					m_pos += Vector2(m_direction * 6.0f, -4.0f);
					//reset timer
					m_jobTimer = 0.0f;
					//increment counter
					m_jobCounter++;
				}
				else
				{
					//decrease timer
					m_jobTimer += _GD->m_dt;
				}
			}
			else
			{
				m_LS = LS_MOVING;
			}
			break;
		case LS_STOPPER:
			//**************************************
			//CODE GOES HERE
			//**************************************
			break;
		};
		m_box->SetPos(m_pos);
		if /*(*/ (m_box->CollideMouse(_GD)) //&& (!status & SelectedFlag))
		{
			status ^= SelectedFlag;
		}
		//************************************
		//attempted hack for limming collision will implement proper once it work=s
		//************************************
		//m_min = m_pos - Vector2(20, 200);
		//m_max = m_min + Vector2(60, 200);
		//if (_GD->m_mouseX > m_min.x &&  _GD->m_mouseX <m_max.x)
		//{
		//	if ((_GD->m_mouseY> m_min.y &&  _GD->m_mouseY < m_max.y))
		//	{
		//		if (_GD->m_mouseState->rgbButtons[0] & 0x80)
		//		{
		//			std::cout << "Lemming";


		//		}
		//	}
		//}

	}
}



void Lemming::SwitchDirection()			//toggles which way the lemming is facing
{
	if (m_direction == 1)
	{
		m_direction = -1;
	}
	else
	{
		m_direction = 1;
	}
}

void Lemming::VerticalAdjustments(GameData* _GD)
{
	for (int i = 1; i < 37; i++)
	{
		if ((_GD->m_RD->GetPixel(m_pos.x, m_pos.y + i))->A() > 0.3f)
		{
			m_pos.y -= (18 - i);
			break;
		}
	}
}

void Lemming::ChangeOccupation(LemmingState _Occupation)
{
	//unfortunately, not really any more efficient way of doing this I dont think.
	if (_Occupation == LS_STEPPER)
	{
		m_LS = LS_STEPPER;											//the ones that lay down steps
		m_jobCounter = 0;
	}
	else if (_Occupation == LS_EXPLOSIVE)
	{
		status |= ExplodingFlag;										//since they should carry on what they are doing while they explode.
	}
	else if (_Occupation == LS_FLOATER)
	{
		status |= FloaterFlag;											//the ones that fall slower
	}
	else if (_Occupation == LS_CLIMBER)
	{
		status |= ClimberFlag;
	}
	else
	{
		m_LS = _Occupation;
	}
}

void Lemming::Draw(DrawData2D* _DD)
{
	if (status & SelectedFlag)
	{
		m_box->Draw(_DD);
	}
	ImageGO2D::Draw(_DD);
}

void Lemming::Spawn(Vector2 _pos)
{
	m_pos = _pos;
	m_alive = true;
	m_LS = LS_FALLING;
}

void Lemming::KillLemming(GameData* _GD, bool _saved)
{
	if (_saved)
	{
		_GD->m_lemmingsSaved++;
	}
	else
	{
		_GD->m_lemmingsKilled++;
	}
	m_alive = false;
}

