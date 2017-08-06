///
/// Table Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef TABLE_H
#define TABLE_H

#include "..\..\vEngine\vRender.h"

class Table : public vEngine::vObject
{
public:
	GLuint Texture1;
	GLuint Texture2;
	GLfloat Size;

	explicit Table();
	explicit Table(vEngine::vfloat3 * pos, vEngine::vfloat3 * angle);
	explicit Table(vEngine::vfloat3 * pos);
	~Table();
	void Render();
};

#endif