///
/// Variables Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef VARS_H
#define VARS_H

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cmath>

#define OBJ_OBJECT 0
#define OBJ_SKY 1
#define OBJ_BOX 2
#define OBJ_SPHERE 3

namespace vEngine
{
	struct vfloat3
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;

		vfloat3() {}
		vfloat3(GLfloat x, GLfloat y, GLfloat z);
		vfloat3 operator+(const vfloat3 & other);
		vfloat3 operator-(const vfloat3 & other);
		vfloat3 operator*(const GLfloat & other);
		GLfloat operator*(const vfloat3 & other);
		GLfloat Length(vfloat3 * v);
		vfloat3 CrossProduct(const vfloat3 & other);
		vfloat3 Normalize();
	};

	struct vuint2
	{
		GLuint x;
		GLuint y;
	};

	struct vRGBA
	{
		GLfloat R;
		GLfloat G;
		GLfloat B;
		GLfloat A;
		
		vRGBA(){}
		vRGBA(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	};

	struct vText
	{
		GLfloat x;
		GLfloat y;
		PTCHAR String;
		vRGBA Color;
		GLuint FontBase;
		BOOL Visible;
	};

	struct vLine
	{
		GLfloat x1;
		GLfloat y1;
		GLfloat x2;
		GLfloat y2;
		vRGBA Color;
		BOOL Visible;
	};

	struct vAudioSource
	{
		PUINT buffers;
		UINT bufferCount;
		UINT source;
	};
}

#endif