#include "Exit.h"
#include "Boundingbox.h"
#include "GameData.h"
#include "DrawData2D.h"

Exit::Exit(ID3D11Device* _GD, PASS _pass, Vector2 _pos)
{
	m_box = new Boundingbox("Exit", _GD, _pass, 32, 24, _pos);
}

Exit::~Exit()
{

}