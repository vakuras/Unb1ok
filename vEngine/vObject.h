///
/// Object Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef vObject_H
#define vObject_H

#include "Variables.h"

namespace vEngine
{
	class vObject
	{
	private:
		GLUquadric * m_Quadric;
	public:
		vfloat3 * Position;
		vfloat3 * Angle;
		vRGBA Color;
		BOOL UseTexture;
		BOOL Visible;

		vObject();
		vObject(vfloat3 * position, vfloat3 * angle);
		vObject(vfloat3 * position);

		virtual ~vObject();

		GLvoid SetQuadric(GLUquadric* quadric);
		GLUquadric* vObject::GetQuadric();

		virtual GLvoid Render();

		static GLint ObjectOID;
		virtual GLint GetOID();
	};
}

#endif