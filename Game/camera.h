#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "gameobject.h"
#include "GameData.h"
#include <dinput.h>
#include "CUtility.h"
//=================================================================
//Base Camera Class
//=================================================================
class Camera : public GameObject
{
public:
	//Constructor and destructor
	Camera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, Vector3 _up, Vector3 _target = Vector3::Zero);
	~Camera();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);
	void cameraMove(GameData* _GD);

	//Getters
	Matrix GetProj(){ return m_projMat; }
	Matrix GetView(){ return m_viewMat; }

	//Setters

protected:

	//Principle transforms/matrices for this camera
	Matrix m_projMat;
	Matrix m_viewMat;

	//parameters for setting up a camera
	float m_fieldOfView;
	float m_aspectRatio;
	float m_nearPlaneDistance;
	float m_farPlaneDistance;

	Vector3 m_target;
	Vector3 m_up;

	float m_cameraSpeed; 
};

#endif