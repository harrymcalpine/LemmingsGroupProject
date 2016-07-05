#ifndef _EXIT_H_
#define _EXIT_H_
#include "Boundingbox.h"

class Boundingbox;

class Exit
{
public:
	Exit(ID3D11Device* _GD, PASS _pass, Vector2 _pos);
	~Exit();

	Boundingbox* m_box;


protected:

};

#endif
