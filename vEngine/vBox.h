///
/// vBox Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef vBOX_H
#define vBOX_H

#include "vObject.h"

namespace vEngine
{
	class vBox : public vObject
	{
	public:
		vfloat3 Size;
		GLuint TextureId;

		GLvoid Render();

		explicit vBox();
		explicit vBox(vfloat3 * Position, vfloat3 * angle);
		explicit vBox(vfloat3 * Position);
		~vBox();

		GLint GetOID();
		static GLint ObjectOID;
	};
}

#endif