///
/// Camera Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef CAMERA_H
#define CAMERA_H

namespace vEngine
{
	class Camera
	{
	public:
		Camera();
		virtual void Render();
		virtual ~Camera();
	};
}

#endif