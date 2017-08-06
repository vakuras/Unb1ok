///
/// Block Implementation
///
/// Written by Vadim Kuras. 2010.
///

#include "Block.h"

Block::Block(GLfloat x, GLfloat z, GLfloat w, GLfloat d, bool xdir) : vObject() 
{
	Position->x = x;
	Position->z = z;
	Position->y = -1.8f;
	Width = w;
	Depth = d;
	XDirection = xdir;
	DirPosition = (xdir) ? &(Position->x) : &(Position->z);
}

Block::Block(GLfloat x, GLfloat z, GLfloat w, GLfloat d, bool xdir, vEngine::vfloat3 * pos, vEngine::vfloat3 * angle) 
	: vObject(pos, angle) 
{
	Position->x = x;
	Position->z = z;
	Position->y = -1.8f;
	Width = w;
	Depth = d;
	XDirection = xdir;
	DirPosition = (xdir) ? &(Position->x) : &(Position->z);
}
Block::Block(GLfloat x, GLfloat z, GLfloat w, GLfloat d, bool xdir, vEngine::vfloat3 * pos) 
	: vObject(pos) 
{
	Position->x = x;
	Position->z = z;
	Position->y = -1.8f;
	Width = w;
	Depth = d;
	XDirection = xdir;
	DirPosition = (xdir) ? &(Position->x) : &(Position->z);
}

Block::~Block()
{
}

#define DEC 0.05f

void Block::Render()
{
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureId);

	glColor4f(Color.R, Color.G, Color.B, Color.A);
	glPushMatrix();	

	glTranslatef(Position->x, Position->y, Position->z);
	glRotatef(Angle->x, 1, 0, 0);
	glRotatef(Angle->y, 0, 1, 0);
	glRotatef(Angle->z, 0, 0, 1);

	GLfloat deca = 0.0f;
	GLfloat decb = 0.0f;
	GLfloat decc = 0.0f;
	GLfloat decd = 0.0f;

	glBegin(GL_QUADS);
	for (int i=0; i<Width; i++)
	{
		if (i==Width-1)
			decb = DEC;
		else
			decb = 0.0f;

		if (i==0)
			deca = DEC;
		else
			deca = 0.0f;

		//front
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f+(GLfloat)i-decb,  0.0f,  0.0f+DEC);
		glTexCoord2f(1.0f, 0.0f); glVertex3f((GLfloat)i+deca,  0.0f,  0.0f+DEC);
		glTexCoord2f(1.0f, 1.0f); glVertex3f((GLfloat)i+deca,  1.0f,  0.0f+DEC);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f+(GLfloat)i-decb,  1.0f,  0.0f+DEC);
		//back
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f+(GLfloat)i-decb,  0.0f,  Depth-DEC);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f+(GLfloat)i-decb,  1.0f,  Depth-DEC);
		glTexCoord2f(0.0f, 1.0f); glVertex3f((GLfloat)i+deca,  1.0f,  Depth-DEC);
		glTexCoord2f(0.0f, 0.0f); glVertex3f((GLfloat)i+deca,  0.0f,  Depth-DEC);
	}

	for (int i=0; i<Depth; i++)
	{
		if (i==Depth-1)
			decb = DEC;
		else
			decb = 0.0f;

		if (i==0)
			deca = DEC;
		else
			deca = 0.0f;

		//left
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.0f+DEC,  0.0f,  1.0f+(GLfloat)i-decb);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.0f+DEC,  1.0f,  1.0f+(GLfloat)i-decb);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f+DEC,  1.0f,  (GLfloat)i+deca);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f+DEC,  0.0f,  (GLfloat)i+deca);
		//right
		glTexCoord2f(0.0f, 0.0f); glVertex3f( Width-DEC,  0.0f,  1.0f+(GLfloat)i-decb);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( Width-DEC,  0.0f,  (GLfloat)i+deca);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( Width-DEC,  1.0f,  (GLfloat)i+deca);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( Width-DEC,  1.0f,  1.0f+(GLfloat)i-decb);
	}

	for(int i=0; i<Width; i++)
		for(int j=0; j<Depth; j++)
		{
			if (i==Width-1)
				decb = DEC;
			else
				decb = 0.0f;

			if (i==0)
				deca = DEC;
			else
				deca = 0.0f;

			if (j==Depth-1)
				decd = DEC;
			else
				decd = 0.0f;

			if (j==0)
				decc = DEC;
			else
				decc = 0.0f;

			//top
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f+(GLfloat)i-decb,  1.0f,  (GLfloat)j+decc);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( (GLfloat)i+deca,  1.0f,  (GLfloat)j+decc);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( (GLfloat)i+deca,  1.0f,  1.0f+(GLfloat)j-decd);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f+(GLfloat)i-decb,  1.0f,  1.0f+(GLfloat)j-decd);
		}
							
	glEnd();

	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
}

void Block::TryMove(GLfloat x, GLfloat z, Block ** blocks, int count, int current, bool& tableNoCollision, bool& blockNoCollision)
{
	GLfloat xpos = ((XDirection) ? x : Position->x);
	GLfloat zpos = ((XDirection) ? z : Position->z);
	GLfloat tpos = ((XDirection) ? x : z);
	GLfloat tpossub = ((XDirection) ? Width : Depth);

	tableNoCollision = ((current == 0 && XDirection) || tpos<3-tpossub) && tpos>-3;
	blockNoCollision = !TestCollision(xpos, zpos, blocks, count, current);

	if (tableNoCollision && blockNoCollision)
		*DirPosition = tpos;
}

bool Block::TestCollision(GLfloat x, GLfloat z, Block ** blocks, int count, int current)
{
	for(int i=0; i<count; i++)
	{
		if (i==current)
			continue;

		if (Collision(x,z,Width, Depth, blocks[i]))
			return true;
	}

	return false;
}

bool Block::Collision(GLfloat x, GLfloat z, GLfloat w, GLfloat d, Block * block)
{
	GLfloat x2 = block->Position->x + 0.1f;
	GLfloat z2 = block->Position->z + 0.1f;
	GLfloat w1 = block->Width - 0.1f;
	GLfloat d1 = block->Depth - 0.1f;

	GLfloat x1 = (x + 0.1f) + Width - 0.1f;
	GLfloat z1 = (z + 0.1f) + Depth - 0.1f;
	GLfloat x3 = x2 + w1;
	GLfloat z3 = z2 + d1;
	
	return !((x1<x2) | (x3<(x + 0.1f)) | (z1<z2) | (z3<(z + 0.1f)));
}

void Block::Fix(GLfloat beforeX, GLfloat beforeZ)
{
	bool movedir = (XDirection) ? (beforeX > Position->x) : (beforeZ > Position->z);

	GLfloat part = *DirPosition - (int)*DirPosition;

	if (part<0.25)
		*DirPosition = floor(*DirPosition);
	else if (part>0.75)
		*DirPosition = ceil(*DirPosition);
	else
		*DirPosition = (movedir) ? floor(*DirPosition) : ceil(*DirPosition);
}