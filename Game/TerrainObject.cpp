#include "TerrainObject.h"

TerrainObject::TerrainObject(string _fileName, ID3D11Device* _GD, int _Type, PASS _pass) : ImageGO2D(_fileName, _GD, _pass)
{
	m_type = _Type;
}

TerrainObject::~TerrainObject()
{

}

void TerrainObject::Spawn(Vector2 _pos)
{
	m_pos = _pos;
	m_alive = true;
}