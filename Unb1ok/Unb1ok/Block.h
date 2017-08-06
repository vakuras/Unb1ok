///
/// Block Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef BLOCK_H
#define BLOCK_H

#include "..\..\vEngine\vRender.h"

class Block : public vEngine::vObject
{
private:
	GLfloat * DirPosition;

public:
	GLfloat Width;
	GLfloat Depth;
	bool XDirection;
	GLuint TextureId;

	void Fix(GLfloat beforeX, GLfloat beforeZ);
	void TryMove(GLfloat x, GLfloat z, Block ** blocks, int count, int current, bool& tableNoCollision, bool& blockNoCollision);
	bool Collision(GLfloat x, GLfloat z, GLfloat w, GLfloat d, Block * block);
	bool TestCollision(GLfloat x, GLfloat z, Block ** blocks, int count, int current);
	explicit Block(GLfloat x, GLfloat z, GLfloat w, GLfloat d, bool xdir);
	explicit Block(GLfloat x, GLfloat z, GLfloat w, GLfloat d, bool xdir, vEngine::vfloat3 * pos, vEngine::vfloat3 * angle);
	explicit Block(GLfloat x, GLfloat z, GLfloat w, GLfloat d, bool xdir, vEngine::vfloat3 * pos);
	~Block();
	void Render();
};

#endif