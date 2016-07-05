#ifndef _LEMMING_H_
#define _LEMMING_H_
#include "ImageGO2D.h"
#include "Cursor.h"
#include "LemmingState.h"
#include <list>
#include "Boundingbox.h"

class Boundingbox;

class Lemming : public ImageGO2D
{
	const unsigned char SelectedFlag = 0x01;
	const unsigned char FloaterFlag = 0x02;
	const unsigned char ClimberFlag = 0x04;
	const unsigned char ExplodingFlag = 0x08;
	using LemmingStatus = unsigned short;

public:
	Lemming(string _fileName, ID3D11Device* _GD, PASS _pass);
	~Lemming();

	Boundingbox* m_box;
	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);
	void Spawn(Vector2 _pos);
	void SwitchDirection();
	void VerticalAdjustments(GameData* _GD);
	void ChangeOccupation(LemmingState _Occupation);
	void KillLemming(GameData* _GD, bool _saved);
	bool IsAlive() { return m_alive; }
	bool IsSelected() { return status & SelectedFlag; }
	bool Stopped() { if (m_LS == LS_STOPPER) return true; else return false; }

protected:
	LemmingStatus status = 0;
	int m_direction = 1;			//is this lemming travelling left or right  (-1 = left, 1 = right)
	const float m_speed = 0.5f;		//movement speed
	LemmingState m_LS; 				//lemmings current occupation
	int m_jobCounter = 0;			//Counter to be used with lemming abilities
	float m_jobTimer = 0;			//Timer for use with lemming abilities
	float m_explosionTimer = 5;		//Timer for explosions
	bool m_alive = false;

};
#endif

