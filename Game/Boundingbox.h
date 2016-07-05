#ifndef  _Boundingbox_
#define  _Boundingbox_
#include "ImageGO2D.h"
#include "GameData.h"

class Boundingbox :public ImageGO2D
{
public:
	Boundingbox(string _filename, ID3D11Device* _GD, PASS _pass, float _width, float _height, Vector2 _pos);//GameObject2D* _parent,
	~Boundingbox() {};

	string m_buttonoutput;
	int numberding;

	bool m_check;
	bool CollideMouse(GameData* _GD);
	bool CollideBox(Boundingbox* _Box);
	float getMaxX() { return m_pos.x + m_width; }
	float getMinX() { return m_pos.x - m_width; }
	float getMaxY() { return m_pos.y + m_height; }
	float getMinY() { return m_pos.y - m_height; }
protected:
	float m_height;
	float m_width;
};
#endif _Boundingbox_