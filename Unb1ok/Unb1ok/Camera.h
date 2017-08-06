///
/// Camera Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef UNBLOCK_CAMERA_H
#define UNBLOCK_CAMERA_H

#include "..\..\vEngine\vRender.h"

class Camera : public vEngine::Camera
{
private:
	vEngine::vfloat3	m_Position;
	GLfloat				m_XSpeed;
	GLfloat				m_YSpeed;
	GLfloat				m_XDevide;
	GLfloat				m_YDevide;
	GLfloat				m_XAngle;
	GLfloat				m_YAngle;
	GLfloat				m_Zoom;
	GLfloat				m_ZoomSpeed;
	GLfloat				m_ZoomDevide;
public:
	explicit Camera();
	Camera(GLfloat xDevide, GLfloat yDevide,  GLfloat zoomDevide);
	GLvoid Reset();
	GLvoid RotateX(GLfloat angle);
	GLvoid RotateY(GLfloat angle);
	GLvoid Zoom(GLfloat zoom);
	GLvoid Render();
	GLvoid SetZoom(GLfloat zoom);
};

#endif