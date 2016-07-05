//base camera class
#include "camera.h"
#include "Cursor.h"

Camera::Camera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, Vector3 _up, Vector3 _target)
{
	m_fieldOfView = _fieldOfView;
	m_aspectRatio = _aspectRatio;
	m_nearPlaneDistance = _nearPlaneDistance;
	m_farPlaneDistance = _farPlaneDistance;

	m_target = _target;
	m_up = _up;
	m_cameraSpeed = 25.0f;
}

Camera::~Camera()
{
}

void Camera::Tick(GameData* _GD)
{
	Camera::cameraMove(_GD);
	//calculate standard transforms for a camera
	m_projMat = Matrix::CreatePerspectiveFieldOfView(m_fieldOfView, m_aspectRatio, m_nearPlaneDistance, m_farPlaneDistance);
	m_viewMat = Matrix::CreateLookAt(m_pos, m_target, m_up);
	GameObject::Tick(_GD);
}

////////////////////////////////////////////////////////////////
// Camera Move function, move the camera using WASD
// or when the mouse is at the corner of screen
//
//To do set up so that if beyond game size cannot move further
////////////////////////////////////////////////////////////////
void Camera::cameraMove(GameData* _GD)
{
	switch (_GD->m_GS)
	{
	case GS_ATTRACT:

		//Move camera Up and down
		if ((_GD->m_keyboardState[DIK_W] & 0x80) ||
			(_GD->m_mouseY < 5))
		{
			m_pos.y += _GD->m_dt * m_cameraSpeed;
			m_target.y += _GD->m_dt * m_cameraSpeed;
		}

		if (_GD->m_keyboardState[DIK_S] & 0x80 ||
			(_GD->m_mouseY >(_GD->height - 5)))
		{
			m_pos.y -= _GD->m_dt * m_cameraSpeed;
			m_target.y -= _GD->m_dt * m_cameraSpeed;
		}

		//Move Camera left and right
		if (_GD->m_keyboardState[DIK_A] & 0x80 ||
			(_GD->m_mouseX < 5))
		{
			m_pos.x -= _GD->m_dt * m_cameraSpeed;
			m_target.x -= _GD->m_dt * m_cameraSpeed;
		}

		if (_GD->m_keyboardState[DIK_D] & 0x80 ||
			(_GD->m_mouseX >(_GD->width - 5)))
		{
			m_pos.x += _GD->m_dt * m_cameraSpeed;
			m_target.x += _GD->m_dt * m_cameraSpeed;
		}
		break;
	}

	float length = m_pos.Length();
	float maxLength = 500.0f;
	if (length > maxLength)
	{
		m_pos *= maxLength / length;
	}
}

void Camera::Draw(DrawData* _DD)
{
	//standard camera doesn't draw ANYTHING
}