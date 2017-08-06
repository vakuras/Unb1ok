///
/// vSky Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef vSky_H
#define vSky_H

#include "vObject.h"

namespace vEngine
{
	class vSky : public vObject
	{
	public:
		GLuint TextureLeft;
		GLuint TextureRight;
		GLuint TextureBack;
		GLuint TextureBottom;
		GLuint TextureTop;
		GLuint TextureFront;
		vfloat3 Scale;

		GLvoid Render();

		explicit vSky();
		explicit vSky(vfloat3 * position, vfloat3 * angle);
		explicit vSky(vfloat3 * position);
		~vSky();

		GLint GetOID();
		static GLint ObjectOID;
	};
}

#endif