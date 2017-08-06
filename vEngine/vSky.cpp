///
/// vSky Implementation
///
/// Written by Vadim Kuras. 2010.
///

#include "vSky.h"

namespace vEngine
{
	GLint vSky::ObjectOID = OBJ_SKY;

	vSky::vSky() : vObject() {}
	vSky::vSky(vfloat3 * position, vfloat3 * angle) : vObject(position, angle) {}
	vSky::vSky(vfloat3 * position) : vObject(position) {}

	vSky::~vSky()
	{
	}

	void vSky::Render()
	{
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glColor4f(Color.R, Color.G, Color.B, Color.A);
		glPushMatrix();	

		glTranslatef(Position->x, Position->y, Position->z);
		glRotatef(Angle->x, 1, 0, 0);
		glRotatef(Angle->y, 0, 1, 0);
		glRotatef(Angle->z, 0, 0, 1);
		glScalef(Scale.x, Scale.y, Scale.z);

		// Render the front quad
		glBindTexture(GL_TEXTURE_2D, TextureFront);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
			glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
			glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
			glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
		glEnd();

		// Render the right quad
		glBindTexture(GL_TEXTURE_2D, TextureRight);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
			glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
			glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
			glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
		glEnd();

		// Render the back quad
		glBindTexture(GL_TEXTURE_2D, TextureBack);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f(  0.5f, -0.5f,  0.5f );
			glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
			glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
			glTexCoord2f(0, 1); glVertex3f(  0.5f,  0.5f,  0.5f );

		glEnd();

		// Render the left quad
		glBindTexture(GL_TEXTURE_2D, TextureLeft);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f,  0.5f );
			glTexCoord2f(1, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
			glTexCoord2f(1, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
			glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
		glEnd();

		// Render the top quad
		glBindTexture(GL_TEXTURE_2D, TextureTop);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f,  0.5f );
			glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f, -0.5f );
			glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f, -0.5f );
			glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f,  0.5f );
		glEnd();

		// Render the bottom quad
		glBindTexture(GL_TEXTURE_2D, TextureBottom);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1); glVertex3f( -0.5f,  -0.5f, -0.5f );
			glTexCoord2f(0, 0); glVertex3f( -0.5f,  -0.5f, 0.5f );
			glTexCoord2f(1, 0); glVertex3f(  0.5f,  -0.5f, 0.5f );
			glTexCoord2f(1, 1); glVertex3f(  0.5f,  -0.5f, -0.5f );
		glEnd();

		glPopMatrix();

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_TEXTURE_2D);
	}

	GLint vSky::GetOID()
	{
		return ObjectOID;
	}
}