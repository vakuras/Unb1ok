///
/// Camera Implementation
///
/// Written by Vadim Kuras. 2010.
///

#include "Camera.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#define PIdiv180 (float)M_PI/180.0f

Camera::Camera(GLfloat xDevide, GLfloat yDevide, GLfloat zoomDevide) :
		m_XDevide(xDevide),
		m_YDevide(yDevide),
		m_ZoomDevide(zoomDevide)
{
	Reset();
}

GLvoid Camera::Reset()
{
	m_Position = vEngine::vfloat3(0.0f, 0.0f, 0.0f);

	m_XSpeed = 0;
	m_YSpeed = 0;
	m_XAngle = 90*PIdiv180;
	m_YAngle = 25*PIdiv180;
	m_ZoomSpeed = 0;
	m_Zoom = 10;
}

GLvoid Camera::RotateX(GLfloat angle)
{
	m_XSpeed = angle;
}

GLvoid Camera::RotateY(GLfloat angle)
{
	m_YSpeed = angle;
}

GLvoid Camera::Render()
{
	m_XAngle += m_XSpeed;
	m_XSpeed /= m_XDevide;

	m_YAngle += m_YSpeed;
	m_YSpeed /= m_YDevide;

	m_Zoom += m_ZoomSpeed;
	m_ZoomSpeed /= m_ZoomDevide;
			
	if (m_Zoom<7)
		m_Zoom = 7;

	if (m_Zoom>20)
		m_Zoom = 20;

	if (m_YAngle>1.0f)
		m_YAngle = 1.0f;

	if (m_YAngle<0.0f)
		m_YAngle = 0.0f;

	m_Position.x = m_Zoom*cos(m_XAngle);
	m_Position.z = m_Zoom*sin(m_XAngle);
	m_Position.y = m_Zoom*sin(m_YAngle);

	gluLookAt(m_Position.x, m_Position.y, m_Position.z,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);

}

GLvoid Camera::SetZoom(GLfloat zoom)
{
	m_Zoom = zoom;
}

GLvoid Camera::Zoom(GLfloat zoom)
{
	m_ZoomSpeed += zoom;
}