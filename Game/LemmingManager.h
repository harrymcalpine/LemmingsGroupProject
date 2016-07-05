#ifndef _LEMMING_MANAGER_H_
#define _LEMMING_MANAGER_H_
#include "ImageGO2D.h"
#include <list>
#include "DrawData2D.h"
#include "Boundingbox.h"
#include "LemmingState.h"

class Lemming;
class Exit;

class LemmingManager : public ImageGO2D
{
public:
	LemmingManager(string _fileName, ID3D11Device* _GD, int _LemCap, PASS _pass, float _LevelTimer);
	~LemmingManager();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);
	void ApplyJob(LemmingState _LS);
	void Nuke();


protected:
	bool m_firstTick = true;
	float m_spawnTimer;
	int m_currentLemmings = 0;
	float m_levelTimer;
	Exit* m_exit;
	list<Lemming*> myLemmings;
	list<Boundingbox*>lembounding;
};


#endif