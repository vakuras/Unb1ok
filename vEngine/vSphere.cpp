///
/// vSphere Implementation
///
/// Written by Vadim Kuras. 2010.
///

#include "vSphere.h"

namespace vEngine
{
	GLint vSphere::ObjectOID = OBJ_SPHERE;

	vSphere::vSphere() : vObject() {}
	vSphere::vSphere(vfloat3 * position, vfloat3 * angle) : vObject(position, angle) {}
	vSphere::vSphere(vfloat3* position) : vObject(position) {}

	vSphere::~vSphere()
	{
	}

	void vSphere::Render()
	{
		glEnable(GL_COLOR_MATERIAL);

		if (UseTexture)
		{
			glEnable(GL_TEXTURE_2D);
			gluQuadricTexture(GetQuadric(), TRUE);
			glBindTexture(GL_TEXTURE_2D, TextureId);
		}

		glColor4f(Color.R, Color.G, Color.B, Color.A);
		glPushMatrix();	

		glTranslatef(Position->x, Position->y, Position->z);
		glRotatef(Angle->x, 1, 0, 0);
		glRotatef(Angle->y, 0, 1, 0);
		glRotatef(Angle->z, 0, 0, 1);
		gluQuadricNormals(GetQuadric(), GLU_SMOOTH);
		gluSphere(GetQuadric(), Size, Slices, Stacks);
		glPopMatrix();

		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_TEXTURE_2D);
	}

	GLint vSphere::GetOID()
	{
		return ObjectOID;
	}
}