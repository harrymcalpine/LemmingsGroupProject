#ifndef _OBJECTS_MANAGER_H_
#define _OBJECTS_MANAGER_H_
#include "GameObject2D.h"
#include <list>

class TerrainObject;

class ObjectsManager : public GameObject2D
{
public:
	ObjectsManager(ID3D11Device* _GD, int _NoOfBricks, int _NoOfExplosives, int _NoOfTunnels);
	~ObjectsManager();

	virtual void Draw(DrawData2D* _DD);
	void AddBricks(DrawData2D* _DD);
	void AddHoles(DrawData2D* _DD);
	void SpawnObject(int _Type, Vector2 _pos);

protected:
	
	list<TerrainObject*> myObjects;
};
#endif