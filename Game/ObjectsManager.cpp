#include "ObjectsManager.h"
#include "GameData.h"
#include "Terrainobject.h"
#include "DrawData2D.h"

ObjectsManager::ObjectsManager(ID3D11Device* _GD, int _NoOfBricks, int _NoOfExplosives, int _NoOfTunnels) : GameObject2D()
{
	for (int i = 0; i < _NoOfBricks; i++)							//12 bricks per bricker
	{
		myObjects.push_back(new TerrainObject("Brick", _GD, 1, ANIMATION_PASS));
	}
	for (int i = 0; i < _NoOfExplosives; i++)
	{
		myObjects.push_back(new TerrainObject("Explosion", _GD, 2, DELETE_PASS));		//change this filename
	}
	for (int i = 0; i < _NoOfTunnels; i++)
	{
		myObjects.push_back(new TerrainObject("Tunnel", _GD, 3, DELETE_PASS));		//change this filename
	}
}

ObjectsManager::~ObjectsManager()
{
	for (list<TerrainObject *>::iterator it = myObjects.begin(); it != myObjects.end(); it++)
	{
		delete (*it);
	}
}

void ObjectsManager::SpawnObject(int _Type, Vector2 _pos)
{
	for (list<TerrainObject *>::iterator it = myObjects.begin(); it != myObjects.end(); it++)
	{
		if (!(*it)->IsAlive())
		{
			if ((*it)->GetType() == _Type)
			{
				(*it)->Spawn(_pos);
				break;
			}
		}
	}
}

void ObjectsManager::AddBricks(DrawData2D* _DD)
{
	for (list<TerrainObject*> ::iterator it = myObjects.begin(); it != myObjects.end(); it++)
	{
		if (((*it)->IsAlive()) && ((*it)->GetType() == 1))
		{
			(*it)->Draw(_DD);
			//(*it)->Kill();
		}
	}
}

void ObjectsManager::AddHoles(DrawData2D* _DD)
{
	for (list<TerrainObject*> ::iterator it = myObjects.begin(); it != myObjects.end(); it++)
	{
		if (((*it)->IsAlive()) && (((*it)->GetType() == 2)||((*it)->GetType() ==3)))
		{
			(*it)->Draw(_DD);
			(*it)->Kill();
		}
	}
}

void ObjectsManager::Draw(DrawData2D* _DD)
{

}