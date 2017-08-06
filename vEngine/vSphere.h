///
/// vSphere Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef vSphere_H
#define vSphere_H

#include "vObject.h"

namespace vEngine
{
	class vSphere : public vObject
	{
	public:
		GLuint Stacks;
		GLuint Slices;
		GLuint TextureId;
		GLfloat Size;

		GLvoid Render();

		explicit vSphere();
		explicit vSphere(vfloat3 * position, vfloat3 * angle);
		explicit vSphere(vfloat3 * position);
		~vSphere();

		GLint GetOID();
		static GLint ObjectOID;
	};
}

#endif