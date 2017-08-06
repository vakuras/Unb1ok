///
/// vRender Header
///
/// Written by Vadim Kuras. 2010.
///

#ifndef vRender_H
#define vRender_H

#include "../vutil/vutil.h"
#include <list>
#include "Camera.h"
#include "vObject.h"
#include "vSphere.h"
#include "vBox.h"
#include "vSky.h"

namespace vEngine
{
	class vRender
	{
	private:
		HCURSOR						m_Cursor;
		BOOL						m_OwnHWND;
		WNDPROC						m_WndProc;
		PTCHAR						m_ClassName;
		PTCHAR						m_Title;
		HINSTANCE					m_hInstance;
		BOOL						m_FullScreen;
		HWND						m_hWnd;
		GLuint						m_Bits;
		HDC							m_hDC;
		HGLRC						m_hRC;
		GLuint						m_Height;
		GLuint						m_Width;
		Camera*						m_Camera;
		GLUquadric*					m_Quadric;
		std::list<vObject*>			m_Objects;
		std::list<vText*>			m_Texts;
		std::list<vLine*>			m_Lines;

	public:
		vRender(HWND hWnd, GLuint width, GLuint height, GLuint bits);
		vRender(GLuint width, GLuint height, GLuint bits, PTCHAR szClassName, PTCHAR szTitle, WNDPROC WndProc, HINSTANCE hInstance, BOOL fullScreen, HCURSOR hCursor);
		~vRender();
		BOOL InitializeHWND();
		BOOL InitializeGL(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
		BOOL Render();
		GLvoid Resize(GLuint newWidth, GLuint newHeight);
		BOOL LoadTexture(LPVOID buffer, GLuint & id, BOOL mipmap);
		BOOL LoadTexture(PTCHAR filename, BOOL mipmap, GLuint & id);
		BOOL LoadTexture(HBITMAP HBMP, GLuint & id, BOOL mipmap);
		BOOL LoadTexture(GLint resid, GLuint & id, BOOL mipmap);
		BOOL LoadTexture(HZIP zip, PTCHAR filename, GLuint & id, BOOL mipmap);
		GLvoid SetVSync(BOOL vsync);
		size_t AddObject(vObject * object);
		size_t AddText(vText * text);
		size_t AddLine(vLine * line);
		vObject * GetObject(size_t index);
		vText * GetText(size_t index);
		vLine * GetLine(size_t index);
		GLvoid RemoveObject(size_t index);
		GLvoid RemoveText(size_t index);
		GLvoid RemoveLine(size_t index);
		GLvoid SetCamera(Camera * camera);
		Camera * GetCamera();
		GLuint BuildFont(GLint height, DWORD weight, BOOL italic, BOOL underline, BOOL strikeout, LPCTSTR name);
		GLint Pick(GLint mx, GLint my);
		vfloat3 GL2DTo3D(GLint mx, GLint my);
		HWND GetHWND();
		std::list<vObject*> GetUnderlyingObjectList();
		std::list<vText*> GetUnderlyingTextList();
		std::list<vLine*> GetUnderlyingLineList();
	protected:
		GLvoid glPrint(GLuint, const PTCHAR, ...);
	};
}

#endif