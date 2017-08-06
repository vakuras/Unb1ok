///
/// Variables Implementation
///
/// Written by Vadim Kuras. 2010.
///

#include "Variables.h"

#define SQR(x) (x*x)

namespace vEngine
{
	vRGBA::vRGBA(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		R = red;
		G = green;
		B = blue;
		A = alpha;
	}

	vfloat3::vfloat3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z)
	{}

	vfloat3 vfloat3::operator+(const vfloat3 & other)
	{
		return vfloat3(x + other.x, y + other.y, z + other.z);
	}

	vfloat3 vfloat3::operator-(const vfloat3 & other)
	{
		return vfloat3(x - other.x, y - other.y, z - other.z);
	}

	vfloat3 vfloat3::operator*(const GLfloat & other)
	{
		return vfloat3(x * other, y * other, z * other);
	}
		
	GLfloat vfloat3::operator*(const vfloat3 & other)
	{
		return GLfloat(x * other.x + y * other.y + z * other.z);
	}

	GLfloat vfloat3::Length(vfloat3 * v)
	{
		return (GLfloat)(sqrt(SQR(v->x)+SQR(v->y)+SQR(v->z)));
	}

	vfloat3 vfloat3::CrossProduct(const vfloat3 & other)
	{
		return vfloat3(y*other.z - z*other.y, z*other.x - x*other.z,  x*other.y - y*other.x);
	}
			
	vfloat3 vfloat3::Normalize()
	{
		vfloat3 res(0.0f, 0.0f, 0.0f);
		GLfloat len = Length(this);
		if (len == 0.0f) 
			return vfloat3(0.0f, 0.0f, 0.0f);
		res.x = x / len;
		res.y = y / len;
		res.z = z / len;
		return res;
	}
}