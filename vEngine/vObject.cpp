///
/// Object Implementation
///
/// Written by Vadim Kuras. 2010.
///

#include "vObject.h"

namespace vEngine
{
	GLint vObject::ObjectOID = OBJ_OBJECT;

	vObject::vObject()
	{
		Position = new vfloat3();
		Angle = new vfloat3();
	}

	vObject::vObject(vfloat3 * position, vfloat3 * angle)
	{
		this->Position = position;
		this->Angle = angle;
	}

	vObject::vObject(vfloat3 * position)
	{
		this->Position = position;
		Angle = new vfloat3();
	}

	vObject::~vObject()
	{
		delete Position;
		delete Angle;
	}

	GLvoid vObject::Render()
	{
	}

	GLint vObject::GetOID()
	{
		return ObjectOID;
	}

	GLvoid vObject::SetQuadric(GLUquadric* quadric)
	{
		m_Quadric = quadric;
	}

	GLUquadric * vObject::GetQuadric()
	{
		return m_Quadric;
	}
}