///
/// Table Implementation
///
/// Written by Vadim Kuras. 2010.
///

#include "Table.h"

Table::Table() : vObject() {}
Table::Table(vEngine::vfloat3 * position, vEngine::vfloat3 * angle) : vObject(position, angle) {}
Table::Table(vEngine::vfloat3 * position) : vObject(position) {}

Table::~Table()
{
}

#define SIZE 8

void Table::Render()
{
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);

	glColor4f(Color.R, Color.G, Color.B, Color.A);
	glPushMatrix();	

	glTranslatef(Position->x, Position->y, Position->z);
	glRotatef(Angle->x, 1, 0, 0);
	glRotatef(Angle->y, 0, 1, 0);
	glRotatef(Angle->z, 0, 0, 1);

	glBindTexture(GL_TEXTURE_2D, Texture2);

	glBegin(GL_QUADS);
	for (int i=0; i<SIZE; i++)
	{
		//front
		glTexCoord2f(0.0f, 0.0f); glVertex3f((GLfloat)i,  0.0f,  (GLfloat)SIZE);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f+(GLfloat)i,  0.0f,  (GLfloat)SIZE);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f+(GLfloat)i,  1.0f,  (GLfloat)SIZE);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((GLfloat)i,  1.0f,  (GLfloat)SIZE);
		//right
		if (i!=3)
		{
			glTexCoord2f(1.0f, 0.0f); glVertex3f( (GLfloat)SIZE,  0.0f,  (GLfloat)i);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( (GLfloat)SIZE,  1.0f,  (GLfloat)i);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( (GLfloat)SIZE,  1.0f,  1.0f+(GLfloat)i);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( (GLfloat)SIZE,  0.0f,  1.0f+(GLfloat)i);
		}
		//left
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f,  0.0f,  (GLfloat)i);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f,  0.0f,  1.0f+(GLfloat)i);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f,  1.0f,  1.0f+(GLfloat)i);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f,  1.0f,  (GLfloat)i);
		//back
		glTexCoord2f(1.0f, 0.0f); glVertex3f((GLfloat)i,  0.0f,  0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((GLfloat)i,  1.0f,  (GLfloat)0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f+(GLfloat)i,  1.0f,  (GLfloat)0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f+(GLfloat)i,  0.0f,  (GLfloat)0);
		//bottom
		for (int j=0; j<SIZE; j++)
		{
			glTexCoord2f(1.0f, 1.0f); glVertex3f( (GLfloat)i,  0.0f,  (GLfloat)j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f+(GLfloat)i,  0.0f,  (GLfloat)j);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f+(GLfloat)i,  0.0f,  1.0f+(GLfloat)j);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( (GLfloat)i,  0.0f,  1.0f+(GLfloat)j);
		}
		//top-front
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f+(GLfloat)i,  1.0f,  (GLfloat)(SIZE-1));
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (GLfloat)i,  1.0f,  (GLfloat)(SIZE-1));
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (GLfloat)i,  1.0f,  (GLfloat)SIZE);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f+(GLfloat)i,  1.0f,  (GLfloat)SIZE);
		//top-back
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f+(GLfloat)i,  1.0f,  0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( (GLfloat)i,  1.0f,  0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( (GLfloat)i,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f+(GLfloat)i,  1.0f,  1.0f);
	}

	for(int i=1; i<SIZE-1; i++)
	{
		//front
		glTexCoord2f(1.0f, 0.0f); glVertex3f((GLfloat)i,  0.0f,  (GLfloat)(SIZE-1));
		glTexCoord2f(1.0f, 1.0f); glVertex3f((GLfloat)i,  1.0f,  (GLfloat)(SIZE-1));
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f+(GLfloat)i,  1.0f,  (GLfloat)(SIZE-1));
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f+(GLfloat)i,  0.0f,  (GLfloat)(SIZE-1));
		//back
		glTexCoord2f(0.0f, 0.0f); glVertex3f((GLfloat)i,  0.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f+(GLfloat)i,  0.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f+(GLfloat)i,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((GLfloat)i,  1.0f,  1.0f);
		//left
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  0.0f,  (GLfloat)i);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  (GLfloat)i);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f+(GLfloat)i);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,  0.0f,  1.0f+(GLfloat)i);
		//right
		if (i!=3)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f( (GLfloat)(SIZE-1),  0.0f,  (GLfloat)i);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( (GLfloat)(SIZE-1),  0.0f,  1.0f+(GLfloat)i);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( (GLfloat)(SIZE-1),  1.0f,  1.0f+(GLfloat)i);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( (GLfloat)(SIZE-1),  1.0f,  (GLfloat)i);
		}
		//top-left
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f+1.0f,  1.0f,  1.0f+(GLfloat)i);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f+1.0f,  1.0f,  (GLfloat)i);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f,  1.0f,  (GLfloat)i);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f,  1.0f,  1.0f+(GLfloat)i);
		//top-right
		if (i!=3)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f( (GLfloat)SIZE,  1.0f,  1.0f+(GLfloat)i);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( (GLfloat)SIZE,  1.0f,  (GLfloat)i);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( (GLfloat)(SIZE-1),  1.0f,  (GLfloat)i);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( (GLfloat)(SIZE-1),  1.0f,  1.0f+(GLfloat)i);
		}
	}

	//fix for exit
	glTexCoord2f(0.0f, 0.0f); glVertex3f(7.0f,  0.0f,  3.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(8.0f,  0.0f,  3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(8.0f,  1.0f,  3.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(7.0f,  1.0f,  3.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(8.0f,  0.0f,  4.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(7.0f,  0.0f,  4.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(7.0f,  1.0f,  4.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(8.0f,  1.0f,  4.0f);	
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 8.0f,  0.0f,  4.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 8.0f,  0.0f,  3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 8.0f,  0.2f,  3.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 8.0f,  0.2f,  4.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, Texture1);

	glBegin(GL_QUADS);

	//bottom
	for(int i=1; i<SIZE-1; i++)
	{
		for(int j=1; j<SIZE-1; j++)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f+(GLfloat)i,  0.2f,  (GLfloat)j);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( (GLfloat)i,  0.2f,  (GLfloat)j);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( (GLfloat)i,  0.2f,  1.0f+(GLfloat)j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f+(GLfloat)i,  0.2f,  1.0f+(GLfloat)j);
		}
	}

	glTexCoord2f(0.0f, 0.0f); glVertex3f( 8.0f,  0.2f,  3.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 7.0f,  0.2f,  3.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 7.0f,  0.2f,  4.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 8.0f,  0.2f,  4.0f);

	glEnd();

	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
}