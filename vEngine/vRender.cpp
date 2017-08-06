///	
/// vRender Implementation
///
/// Written by Vadim Kuras. 2010.
///

#include "vRender.h"

#pragma comment(lib, "opengl32.lib") 
#pragma comment(lib, "glu32.lib")

namespace vEngine
{
	vRender::vRender(HWND hWnd, GLuint width, GLuint height, GLuint bits) : m_hWnd(hWnd), 
																			  m_Width(width),
																              m_Height(height),
																              m_Bits(bits),
																			  m_OwnHWND(FALSE),
																			  m_ClassName(NULL),
																			  m_Title(NULL)
	{
		Trace(TEXT("Begin"));

		m_Quadric = gluNewQuadric();

		Trace(TEXT("End"));
	}

	vRender::vRender(GLuint width, GLuint height, GLuint bits, PTCHAR szClassName, PTCHAR szTitle, WNDPROC WndProc, HINSTANCE hInstance, BOOL fullScreen, HCURSOR hCursor) : m_Width(width),
																																							  m_Height(height),
																																							  m_Bits(bits),
																																							  m_OwnHWND(TRUE),
																																							  m_WndProc(WndProc),
																																							  m_hInstance(hInstance),
																																							  m_FullScreen(fullScreen),
																																							  m_Cursor(hCursor)
	{
		Trace(TEXT("Begin"));

		size_t cnSize = _tcslen(szClassName) + sizeof(TCHAR);
		size_t tSize = _tcslen(szTitle) + sizeof(TCHAR);
		m_ClassName = new TCHAR[cnSize];
		m_Title = new TCHAR[tSize];
		_tcscpy_s(m_ClassName, cnSize, szClassName);
		_tcscpy_s(m_Title, tSize, szTitle);
		m_Quadric = gluNewQuadric();

		Trace(TEXT("End"));
	}

	HWND vRender::GetHWND()
	{
		return m_hWnd;
	}

	vRender::~vRender()
	{
		Trace(TEXT("Begin"));

		for(std::list<vObject*>::iterator it = m_Objects.begin(); it != m_Objects.end(); it++)
			delete *it;

		for(std::list<vLine*>::iterator it = m_Lines.begin(); it != m_Lines.end(); it++)
			delete (*it);

		for(std::list<vText*>::iterator it = m_Texts.begin(); it != m_Texts.end(); it++)
		{
			glDeleteLists((*it)->FontBase, 256);
			delete (*it);
		}

		gluDeleteQuadric(m_Quadric);
		m_Quadric = NULL;

		if (!wglMakeCurrent(NULL,NULL))
			TraceErr(TEXT("wglMakeCurrent [~] failed"));

		if (!wglDeleteContext(m_hRC))
			TraceErr(TEXT("wglDeleteContext [~] failed"));

		if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))
			TraceErr(TEXT("ReleaseDC [~] failed"));

		if (!m_OwnHWND)
		{
			Trace(TEXT("End [no OwnHWND]"));
			return;
		}

		if (m_hWnd && !DestroyWindow(m_hWnd))
			TraceErr(TEXT("DestroyWindow [~] failed"));

		if (!UnregisterClass(m_ClassName, m_hInstance))
			TraceErr(TEXT("UnregisterClass [~] failed"));

		if (m_ClassName)
			delete [] m_ClassName;

		if (m_Title)
			delete [] m_Title;

		Trace(TEXT("End"));
	}

	GLvoid vRender::SetVSync(BOOL vsync)
	{
		typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
		PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
			  
		const GLubyte *extensions = glGetString( GL_EXTENSIONS );

		if( strstr( (char*)extensions, "WGL_EXT_swap_control" ) != 0 )
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

			if( wglSwapIntervalEXT )
				wglSwapIntervalEXT((vsync) ? 1 : 0);
		}
	}

	BOOL vRender::InitializeHWND()
	{
		__try
		{
			Trace(TEXT("Begin"));

			if (!m_OwnHWND)
				return FALSE;

			WNDCLASSEX wcex;
			DWORD dwStyle;
			DWORD dwExStyle;

			wcex.cbSize			= sizeof(WNDCLASSEX);
			wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wcex.lpfnWndProc	= m_WndProc;
			wcex.cbClsExtra		= 0;
			wcex.cbWndExtra		= 0;
			wcex.hInstance		= m_hInstance;
			wcex.hIcon			= NULL;
			wcex.hCursor		= m_Cursor;
			wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
			wcex.lpszMenuName	= NULL;
			wcex.lpszClassName	= m_ClassName;
			wcex.hIconSm		= NULL;
			RegisterClassEx(&wcex);

			if (m_FullScreen)
			{
				DEVMODE dmScreenSettings;
				memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
				dmScreenSettings.dmSize=sizeof(dmScreenSettings);
				dmScreenSettings.dmPelsWidth	= m_Width;
				dmScreenSettings.dmPelsHeight	= m_Height;
				dmScreenSettings.dmBitsPerPel	= m_Bits;
				dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

				m_FullScreen = ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
			}

			if (m_FullScreen)
			{
				dwExStyle=WS_EX_APPWINDOW;
				dwStyle=WS_POPUP;
			}
			else
			{
				dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
				dwStyle=WS_OVERLAPPEDWINDOW;
			}

			RECT rect = { 0, 0, m_Width, m_Height };
			AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);

			m_hWnd = CreateWindow( m_ClassName, 
									m_Title, 
									dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
									CW_USEDEFAULT, 
									CW_USEDEFAULT, 
									rect.right - rect.left, 
									rect.bottom - rect.top, 
									NULL, 
									NULL, 
									m_hInstance, 
									NULL);
   
			if (!m_hWnd) 
				return FALSE;

			ShowCursor(!(m_Cursor == NULL));
			ShowWindow(m_hWnd, SW_SHOW);
			SetForegroundWindow(m_hWnd);
			SetFocus(m_hWnd);
			UpdateWindow(m_hWnd);

			return TRUE;
		}
		__finally
		{
			Trace(TEXT("End"));
		}
	}

	BOOL vRender::InitializeGL(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		__try
		{
			Trace(TEXT("Begin"));

			GLuint pixelFormat;

			PIXELFORMATDESCRIPTOR pfd=
			{
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA,
				m_Bits,
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				16,
				0,
				0,
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};

			if (!(m_hDC=GetDC(m_hWnd)))
			{
				TraceErr(TEXT("Can't create a GL device context"));
				return FALSE;
			}

			if (!(pixelFormat=ChoosePixelFormat(m_hDC,&pfd)))
			{
				TraceErr(TEXT("Can't find a suitable pixelformat"));
				return FALSE;
			}

			if(!SetPixelFormat(m_hDC,pixelFormat,&pfd))
			{
				TraceErr(TEXT("Can't set the pixelformat"));
				return FALSE;
			}

			if (!(m_hRC=wglCreateContext(m_hDC)))
			{
				TraceErr(TEXT("Can't create a gl rendering context"));
				return FALSE;
			}

			if(!wglMakeCurrent(m_hDC,m_hRC))
			{
				TraceErr(TEXT("Can't activate the gl rendering context"));
				return FALSE;
			}

			glShadeModel(GL_SMOOTH);
			glClearColor(r, g, b, a);
			glClearDepth(1.0f);
			glEnable(GL_DEPTH_TEST);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
			glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
			glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Resize(m_Width, m_Height);

			return TRUE;
		}
		__finally
		{
			Trace(TEXT("End"));
		}
	}

	vfloat3 vRender::GL2DTo3D(GLint mx, GLint my)
	{
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
		glGetDoublev( GL_PROJECTION_MATRIX, projection );
		glGetIntegerv( GL_VIEWPORT, viewport );

		winX = (float)mx;
		winY = (float)viewport[3] - (float)my;
		glReadPixels( mx, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

		gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		return vfloat3((GLfloat)posX, (GLfloat)posY, (GLfloat)posZ);
	}

	void vRender::Resize(GLuint newWidth, GLuint newHeight)
	{
		m_Width = newWidth;
		m_Height = newHeight;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45.0f,(GLfloat)m_Width/(GLfloat)m_Height,0.1f,500.0f);

		glMatrixMode(GL_MODELVIEW);
		glViewport(0,0,m_Width,m_Height);
	}

	GLuint vRender::BuildFont(GLint height, DWORD weight, BOOL italic, BOOL underline, BOOL strikeout, LPCTSTR name)
	{
		HFONT font;
		HFONT oldfont;
		GLuint fontBase = glGenLists(256);

		font = CreateFont(	-height,
							0,
							0,
							0,
							weight,
							italic,
							underline,
							strikeout,
							ANSI_CHARSET,
							OUT_TT_PRECIS,
							CLIP_DEFAULT_PRECIS,
							ANTIALIASED_QUALITY,
							FF_DONTCARE|DEFAULT_PITCH,
							name);

		oldfont = (HFONT)SelectObject(m_hDC, font);
		wglUseFontBitmaps(m_hDC, 0, 256, fontBase);
		SelectObject(m_hDC, oldfont);
		DeleteObject(font);

		return fontBase;
	}

	void vRender::glPrint(GLuint base, const PTCHAR fmt, ...)
	{
		TCHAR text[1024];
		va_list	ap;

		if (fmt == NULL)
			return;

		va_start(ap, fmt);
		_vsntprintf_s(text, 1024, 1204, fmt, ap);
		va_end(ap);

		glPushAttrib(GL_LIST_BIT);
		glListBase(base);
		glCallLists((GLsizei)_tcslen(text), sizeof(TCHAR) == sizeof(char) ? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT, text);
		glPopAttrib();
	}

	BOOL vRender::Render()
	{
		if (!m_Camera)
			return false;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		m_Camera->Render();
		
		for(std::list<vObject*>::iterator it = m_Objects.begin(); it != m_Objects.end(); it++)
		{
			if (!(*it)->Visible)
				continue;

			(*it)->Render();
		}
		
		for(std::list<vText*>::iterator it = m_Texts.begin(); it != m_Texts.end(); it++)
		{
			if (!(*it)->Visible)
				continue;

			glLoadIdentity();
			glTranslatef(0.0f,0.0f,-1.0f);
			glColor4f((*it)->Color.R,(*it)->Color.G,(*it)->Color.B, (*it)->Color.A);
			glRasterPos2f((*it)->x,(*it)->y);
			glPrint((*it)->FontBase, (*it)->String);
		}

		for(std::list<vLine*>::iterator it = m_Lines.begin(); it != m_Lines.end(); it++)
		{
			if (!(*it)->Visible)
				continue;

			glLoadIdentity();
			glTranslatef(0.0f,0.0f,-1.0f);
			glColor4f((*it)->Color.R,(*it)->Color.G,(*it)->Color.B, (*it)->Color.A);

			glBegin(GL_LINES);
			glVertex3f((*it)->x1, (*it)->y1, 0.0f);
			glVertex3f((*it)->x2, (*it)->y2, 0.0f);
			glEnd( );
		}

		glFlush();
		SwapBuffers(m_hDC);

		return TRUE;		
	}

	BOOL vRender::LoadTexture(PTCHAR filename, BOOL mipmap, GLuint & id)
	{
		__try
		{
			Trace(TEXT("Begin"));

			HBITMAP hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );

			if (!hBMP)
				return FALSE;

			LoadTexture(hBMP, id, mipmap);
			DeleteObject(hBMP);

			return TRUE;
		}
		__finally
		{
			Trace(TEXT("End"));
		}
	}

	BOOL vRender::LoadTexture(GLint resid, GLuint & id, BOOL mipmap)
	{
		__try
		{
			Trace(TEXT("Begin"));

			HBITMAP hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(resid), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTCOLOR );

			if (!hBMP)
				return FALSE;

			LoadTexture(hBMP, id, mipmap);
			DeleteObject(hBMP);

			return TRUE;
		}
		__finally
		{
			Trace(TEXT("End"));
		}
	}

	BOOL vRender::LoadTexture(LPVOID buffer, GLuint & id, BOOL mipmap)
	{
		__try
		{
			Trace(TEXT("Begin"));

			tagBITMAPFILEHEADER bfh = *(tagBITMAPFILEHEADER*)buffer;
			tagBITMAPINFOHEADER bih = *(tagBITMAPINFOHEADER*)((PBYTE)buffer+sizeof(tagBITMAPFILEHEADER));
			RGBQUAD             rgb = *(RGBQUAD*)((PBYTE)buffer+sizeof(tagBITMAPFILEHEADER)+sizeof(tagBITMAPINFOHEADER));

			BITMAPINFO bi;
			bi.bmiColors[0] = rgb;
			bi.bmiHeader = bih;

			PBYTE pPixels = ((PBYTE)buffer+bfh.bfOffBits);

			PBYTE ppvBits;

			HBITMAP HBMP = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (void**) &ppvBits, NULL, 0);
			SetDIBits(NULL, HBMP, 0, bih.biHeight, pPixels, &bi, DIB_RGB_COLORS);

			LoadTexture(HBMP, id, mipmap);
			DeleteObject(HBMP);

			return TRUE;
		}
		__finally
		{
			Trace(TEXT("End"));
		}
	}

	BOOL vRender::LoadTexture(HZIP zip, PTCHAR filename, GLuint & id, BOOL mipmap)
	{
		__try
		{
			Trace(TEXT("Begin"));

			LPVOID buffer;
			LONG size;
			ZRESULT result = vutil::UnZip(zip, &buffer, filename, &size);
		
			if (result != ZR_OK)
			{
				TraceErrWithCode(result, TEXT("vutil::UnZip failed"));
				return FALSE;
			}

			BOOL ret = LoadTexture(buffer, id, mipmap);
			vutil::FreeBuffer(&buffer);

			return ret;
		}
		__finally
		{
			Trace(TEXT("End"));
		}
	}

	BOOL vRender::LoadTexture(HBITMAP hBMP, GLuint & id, BOOL mipmap)
	{
		__try
		{
			Trace(TEXT("Begin"));
		
			BITMAP BMP;

			glGenTextures(1, &id);

			if (!hBMP)
				return FALSE;

			::GetObject(hBMP, sizeof(BMP), &BMP);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			if (mipmap)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);	
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				gluBuild2DMipmaps( GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits );
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
			}

			return TRUE;
		}
		__finally
		{
			Trace(TEXT("End"));
		}
	}

	size_t vRender::AddObject(vObject * object)
	{
		object->SetQuadric(m_Quadric);
		m_Objects.push_back(object);
		return m_Objects.size()-1;
	}

	vObject * vRender::GetObject(size_t index)
	{
		if (m_Objects.empty() || m_Objects.size() < index)
			return NULL;

		int i = 0;
		for(std::list<vObject*>::iterator it = m_Objects.begin(); it != m_Objects.end(); it++)
		{
			if (i == index)
				return (*it);

			i++;
		}

		return NULL;
	}

	void vRender::RemoveObject(size_t index)
	{
		if (m_Objects.empty() || m_Objects.size() < index)
			return;

		int i = 0;
		for(std::list<vObject*>::iterator it = m_Objects.begin(); it != m_Objects.end(); it++)
		{
			if (i == index)
			{
				delete (*it);
				m_Objects.erase(it);
				return;
			}

			i++;
		}
	}

	size_t vRender::AddText(vText * text)
	{
		m_Texts.push_back(text);
		return m_Texts.size()-1;
	}

	void vRender::RemoveText(size_t index)
	{
		if (m_Texts.empty() || m_Texts.size() < index)
			return;

		int i = 0;
		for(std::list<vText*>::iterator it = m_Texts.begin(); it != m_Texts.end(); it++)
		{
			if (i == index)
			{
				delete (*it);
				m_Texts.erase(it);
				return;
			}

			i++;
		}
	}

	size_t vRender::AddLine(vLine * line)
	{
		m_Lines.push_back(line);
		return m_Lines.size()-1;
	}

	void vRender::RemoveLine(size_t index)
	{
		if (m_Lines.empty() || m_Lines.size() < index)
			return;

		int i = 0;
		for(std::list<vLine*>::iterator it = m_Lines.begin(); it != m_Lines.end(); it++)
		{
			if (i == index)
			{
				delete (*it);
				m_Lines.erase(it);
				return;
			}

			i++;
		}
	}

	void vRender::SetCamera(Camera * camera)
	{
		m_Camera = camera;
	}

	Camera * vRender::GetCamera()
	{
		return m_Camera;
	}

	std::list<vObject*> vRender::GetUnderlyingObjectList()
	{
		return m_Objects;
	}

	std::list<vText*> vRender::GetUnderlyingTextList()
	{
		return m_Texts;
	}

	std::list<vLine*> vRender::GetUnderlyingLineList()
	{
		return m_Lines;
	}

	GLint vRender::Pick(GLint mx, GLint my)
	{
		GLuint	buffer[512];
		GLint	hits;
		GLint	viewport[4];
		glSelectBuffer(512, buffer);
		glGetIntegerv(GL_VIEWPORT, viewport);

		glRenderMode(GL_SELECT);
		glInitNames();
		glPushName(0);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix((GLdouble) mx, (GLdouble) (viewport[3]-my), 1.0, 1.0, viewport);

		gluPerspective(45.0f,(GLfloat)m_Width/(GLfloat)m_Height,0.1f,500.0f);
		glMatrixMode(GL_MODELVIEW);

		int index=7;

		for(std::list<vObject*>::iterator it = m_Objects.begin(); it != m_Objects.end(); it++)
		{
			if ((*it)->GetOID() == vSky::ObjectOID || !(*it)->Visible)
				continue;

			(*it)->Render();
			glLoadName(index);
			index+=7;
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glRenderMode(GL_MODELVIEW);
		hits=glRenderMode(GL_RENDER);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45.0f,(GLfloat)m_Width/(GLfloat)m_Height,0.1f,2000.0f);

		glMatrixMode(GL_MODELVIEW);
		glViewport(0,0,m_Width,m_Height);

		if (hits>0)
		{
			int	choose = buffer[3];
			int depth = buffer[1];

			for (int loop = 1; loop < hits; loop++)
			{
				if (buffer[loop*4+1] < GLuint(depth))
				{
					choose = buffer[loop*4+3];
					depth = buffer[loop*4+1];
				}       
			}

			return choose/7;
		}

		return -1;
	}
}