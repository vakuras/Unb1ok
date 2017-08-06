///
/// Game Implementation
///
/// Written by Vadim Kuras. 2010.
///

#include "game.h"

//#define GLDEBUG

//configuration
SETTINGS settings;
LPVOID zipLevelsBuffer;
DWORD zipLevelsLength;
LPVOID zipDataBuffer;
DWORD zipDataLength;

//glrender
vEngine::vRender * vr = NULL;
Camera * camera;

//input
BOOL bKeysDown[256];
BOOL bKeysUp[256];
BOOL bRightState = FALSE;
BOOL bLeftState = FALSE;
int mx;
int my;
int ox;
int oy;

//objects
Block ** blocks;
Table * table;
vEngine::vBox * playBox;
vEngine::vBox * fullscreenBox;
vEngine::vBox * soundBox;
vEngine::vBox * exitBox;

//texture
GLuint glCrateTex1;
GLuint glCrateTex2;
GLuint glFSTex;
GLuint glFSONTex;
GLuint glSoundTex;
GLuint glSoundOnTex;

//sound
vEngine::vAudio* audio;
vEngine::vAudioSource intro;
vEngine::vAudioSource hover;
vEngine::vAudioSource selection;
vEngine::vAudioSource finito;
vEngine::vAudioSource pad1;
vEngine::vAudioSource pad2;

//level
int blockCount = 0;
BOOL bLevelLoaded = FALSE;

//menu
BOOL bShowMenu = TRUE;

#ifdef GLDEBUG
clock_t timeNow, lastTime = 0;
int frames=0, fps;
TCHAR fpstr[20];
#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
        case WM_KEYDOWN:
		{
			bKeysDown[wParam] = TRUE;
			break;		
		}

		case WM_KEYUP:
		{
			bKeysDown[wParam] = FALSE;
			bKeysUp[wParam] = FALSE;
			break;
		}

		case WM_SIZE:
		{
			vr->Resize(LOWORD(lParam),HIWORD(lParam));

			if (settings.FullScreen)
				break;

			settings.ResX = LOWORD(lParam);
			settings.ResY = HIWORD(lParam);
			break;
		}

		case WM_RBUTTONDOWN:
		{
			bRightState = TRUE;
			SetCapture(hWnd);
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			break;
		}

		case WM_RBUTTONUP:
		{
			bRightState = FALSE;
			ReleaseCapture();
			break;
		}

		case WM_LBUTTONDOWN:
		{
			bLeftState = TRUE;
			SetCapture(hWnd);
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			break;
		}

		case WM_LBUTTONUP:
		{
			bLeftState = FALSE;
			ReleaseCapture();
			break;
		}

		case WM_MOUSEMOVE:
		{
			ox = mx;
			oy = my;
			mx = LOWORD(lParam);
			my = HIWORD(lParam);

			if(mx & 1 << 15) mx -= (1 << 16);
			if(my & 1 << 15) my -= (1 << 16);
			break;
		}

		case 0x020E:
		case 0x020A:
		{
			short mult = GET_WHEEL_DELTA_WPARAM(wParam)/abs(GET_WHEEL_DELTA_WPARAM(wParam));
			camera->Zoom(mult*0.06f);
			break;
		}

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;
		}

		case WM_DESTROY:	
		{
			PostQuitMessage(0);
			break;
		}
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void UnloadLevel()
{
	if (!bLevelLoaded)
		return;

	for(int i=blockCount-1; i>-1; i--)
	{
		vr->RemoveObject(i+6);
	}

	table->Visible = false;
	bLevelLoaded = false;
}

void ChangeLevel()
{
	TCHAR * tLevel = new TCHAR[8*sizeof(TCHAR)];
	wsprintf(tLevel, TEXT("%03d.lvl"), settings.LastLevelPlayed);

	LoadLevel(tLevel);

	delete [] tLevel;
}

bool LoadLevel(TCHAR * level)
{
	UnloadLevel();

	LPVOID buffer;
	LONG size;

	HZIP hz = vutil::OpenZip(zipLevelsBuffer, zipLevelsLength);
	ZRESULT zr = vutil::UnZip(hz, &buffer, level, &size);

	if (zr != ZR_OK)
		return false;

	std::stringstream ss((char*)buffer);

	ss >> blockCount;

	if (blockCount < 1)
		return false;

	blocks = new Block*[blockCount];

	for(int i=0; i<blockCount; i++)
	{
		GLfloat x;
		GLfloat y;
		GLfloat w;
		GLfloat d;
		int xd;

		ss >> x;
		ss >> y;
		ss >> w;
		ss >> d;
		ss >> xd;

		blocks[i] = new Block(x - 3,y - 3,w,d,!xd == 1);
		blocks[i]->Color.R = blocks[i]->Color.G = blocks[i]->Color.B = blocks[i]->Color.A = 1.0f;
		blocks[i]->Angle->x = blocks[i]->Angle->y = blocks[i]->Angle->z = 0;
		blocks[i]->UseTexture = true;
		blocks[i]->TextureId =  (i==0) ? glCrateTex1 : glCrateTex2;
		blocks[i]->Visible = TRUE;

		vr->AddObject(blocks[i]);
	}

	vutil::FreeBuffer(&buffer);
	vutil::FreeZip(hz);

	bLevelLoaded = true;
	table->Visible = true;
	
	return true;
}

bool GameInit(HINSTANCE hInstance, UINT windowWidth, UINT windowHeight)
{
	vr = new vEngine::vRender(windowWidth, windowHeight, 32, CLASS_NAME, APP_NAME, WndProc, hInstance, settings.FullScreen, LoadCursor(NULL, IDC_HAND));
	audio = new vEngine::vAudio();
	camera = new Camera(1.1f, 1.1f, 1.1f);

	if (!audio->Init(0))
		return false;
	
	if (!vr->InitializeHWND())
	{
		delete audio;
		return false;
	}

	if (!vr->InitializeGL(0,0,0,0))
	{
		delete audio;
		delete vr;
		return false;
	}

	vEngine::vSky * sky;

	vr->SetCamera(camera);

	HZIP hz = vutil::OpenZip(zipDataBuffer, zipDataLength);

	vr->LoadTexture(hz, _T("crate.bmp"), glCrateTex1, true);
	vr->LoadTexture(hz, _T("super.bmp"), glCrateTex2, true);

	vr->LoadTexture(hz, _T("fs.bmp"), glFSTex, true);
	vr->LoadTexture(hz, _T("fson.bmp"), glFSONTex, true);
	vr->LoadTexture(hz, _T("sound.bmp"), glSoundTex, true);
	vr->LoadTexture(hz, _T("soundon.bmp"), glSoundOnTex, true);

	sky = new vEngine::vSky();

	vr->LoadTexture(hz, _T("sky0_ft.bmp"), sky->TextureFront, false);
	vr->LoadTexture(hz, _T("sky0_lf.bmp"), sky->TextureLeft, false);
	vr->LoadTexture(hz, _T("sky0_rt.bmp"), sky->TextureRight, false);
	vr->LoadTexture(hz, _T("sky0_bk.bmp"), sky->TextureBack, false);
	vr->LoadTexture(hz, _T("sky0_up.bmp"), sky->TextureTop, false);
	vr->LoadTexture(hz, _T("sky0_dn.bmp"), sky->TextureBottom, false);

	sky->Color.R = sky->Color.G = sky->Color.B = sky->Color.A = 1.0f;
	sky->Angle->x = sky->Angle->y = sky->Angle->z = 0;
	sky->Position->x = 0;
	sky->Position->y = -2;
	sky->Position->z = 0;
	sky->Scale.x = 50;
	sky->Scale.y = 50;
	sky->Scale.z = 50;
	sky->UseTexture = TRUE;
	sky->Visible = TRUE;

	vr->AddObject(sky);

	table = new Table();

	vr->LoadTexture(hz, _T("tex1.bmp"), table->Texture1, true);
	vr->LoadTexture(hz, _T("tex2.bmp"), table->Texture2, true);

	table->Color.R = table->Color.G = table->Color.B = table->Color.A = 1.0f;
	table->Angle->x = table->Angle->y = table->Angle->z = 0;
	table->Position->x = -4;
	table->Position->y = -2;
	table->Position->z = -4;
	table->Size = 0.5;
	table->UseTexture = TRUE;
	table->Visible = FALSE;

	vr->AddObject(table);

	playBox = new vEngine::vBox();

	playBox->Color.A = 1.0f;
	playBox->Color.R = 1.0f;
	playBox->Color.G = 1.0f;
	playBox->Color.B = 1.0f;
	playBox->Position->x = 0;
	playBox->Position->y = 3;
	playBox->Position->z = 0;
	playBox->Size.x = 1.0f;
	playBox->Size.y = 1.0f;
	playBox->Size.z = 1.0f;
	playBox->Angle->x = 0;
	playBox->Angle->y = 0;
	playBox->Angle->z = 0;
	playBox->Visible = TRUE;
	vr->LoadTexture(hz, _T("play.bmp"), playBox->TextureId, true);
	playBox->UseTexture = TRUE;

	vr->AddObject(playBox);

	fullscreenBox = new vEngine::vBox();

	fullscreenBox->Color.A = 1.0f;
	fullscreenBox->Color.R = 1.0f;
	fullscreenBox->Color.G = 1.0f;
	fullscreenBox->Color.B = 1.0f;
	fullscreenBox->Position->x = 0;
	fullscreenBox->Position->y = 1;
	fullscreenBox->Position->z = 0;
	fullscreenBox->Size.x = 1.0f;
	fullscreenBox->Size.y = 1.0f;
	fullscreenBox->Size.z = 1.0f;
	fullscreenBox->Angle->x = 0;
	fullscreenBox->Angle->y = 0;
	fullscreenBox->Angle->z = 0;
	fullscreenBox->TextureId = settings.FullScreen ? glFSONTex : glFSTex;
	fullscreenBox->UseTexture = TRUE;
	fullscreenBox->Visible = TRUE;

	vr->AddObject(fullscreenBox);

	soundBox = new vEngine::vBox();

	soundBox->Color.A = 1.0f;
	soundBox->Color.R = 1.0f;
	soundBox->Color.G = 1.0f;
	soundBox->Color.B = 1.0f;
	soundBox->Position->x = 0;
	soundBox->Position->y = -1;
	soundBox->Position->z = 0;
	soundBox->Size.x = 1.0f;
	soundBox->Size.y = 1.0f;
	soundBox->Size.z = 1.0f;
	soundBox->Angle->x = 0;
	soundBox->Angle->y = 0;
	soundBox->Angle->z = 0;
	soundBox->TextureId = settings.Sound ? glSoundOnTex : glSoundTex;
	soundBox->UseTexture = TRUE;
	soundBox->Visible = TRUE;

	vr->AddObject(soundBox);

	exitBox = new vEngine::vBox();

	exitBox->Color.A = 1.0f;
	exitBox->Color.R = 1.0f;
	exitBox->Color.G = 1.0f;
	exitBox->Color.B = 1.0f;
	exitBox->Position->x = 0;
	exitBox->Position->y = -3;
	exitBox->Position->z = 0;
	exitBox->Size.x = 1.0f;
	exitBox->Size.y = 1.0f;
	exitBox->Size.z = 1.0f;
	exitBox->Angle->x = 0;
	exitBox->Angle->y = 0;
	exitBox->Angle->z = 0;
	vr->LoadTexture(hz, _T("exit.bmp"), exitBox->TextureId, true);
	exitBox->UseTexture = TRUE;
	exitBox->Visible = TRUE;

	vr->AddObject(exitBox);

	audio->LoadOgg(hz, TEXT("intro.ogg"), intro, 0);
	audio->LoadOgg(hz, TEXT("select.ogg"), selection, 0);
	audio->LoadOgg(hz, TEXT("hover.ogg"), hover, 0);
	audio->LoadOgg(hz, TEXT("finito.ogg"), finito, 0);
	audio->LoadOgg(hz, TEXT("pad1.ogg"), pad1, 0);
	audio->LoadOgg(hz, TEXT("pad2.ogg"), pad2, 0);

	if (settings.Sound)
		audio->Play(intro);

	vutil::FreeZip(hz);

	return true;
}

int GameLoop()
{
	int selected=-1;
	GLfloat dx, dz;
	GLfloat bx, bz;
	bool tableNoCollision;
	bool blockNoCollision;

    MSG msg = {0};
    while (WM_QUIT != msg.message)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);			
		}	

#ifdef GLDEBUG
		timeNow = clock();
		++frames;
		if(timeNow - lastTime > CLK_TCK*2)
		{
			fps = (frames*CLK_TCK)/(timeNow-lastTime);
			lastTime = timeNow;
			frames = 0;

			swprintf_s(fpstr, TEXT("FPS: %d"), fps); 
			SetWindowText(hWnd, fpstr);
		}
#endif
	
		if (!vr->Render())
			return EXIT_FAILURE;

		if (bRightState)
		{
			int dx = ox - mx;
			int dy = oy - my;

			GLfloat Anglex = (dx * 180.0f) / 75000.0f;
			GLfloat Angley = (dy * 180.0f) / 75000.0f;

			camera->RotateX(-Anglex);
			camera->RotateY(-Angley);
		}

		for(int i=0; i<256; i++)
		{
			if (!bKeysDown[i])
				continue;

			switch(i)
			{
				case VK_ESCAPE: 
					if (!bShowMenu)
					{
						UnloadLevel();
						playBox->Visible = exitBox->Visible = soundBox->Visible = fullscreenBox->Visible = TRUE;
						bShowMenu = TRUE;
						camera->Reset();
					}
					break;
				case VK_UP:
					camera->Zoom(-0.03f);
					break;
				case VK_DOWN:
					camera->Zoom(0.03f);
					break;
				case VK_LEFT:
					camera->RotateX(-0.03f);
					break;
				case VK_RIGHT:
					camera->RotateX(0.03f);
					break;
				case VK_PRIOR:
					camera->RotateY(0.03f);
					break;
				case VK_NEXT:
					camera->RotateY(-0.03f);
					break;

				case '0':
					vr->SetVSync(false);
					break;

				case '9':
					vr->SetVSync(true);
					break;

				case 0x6B:

					if (bKeysUp[0x6B] || bShowMenu)
						break;

					settings.LastLevelPlayed++;

					if (settings.LastLevelPlayed>=LEVELS)
						settings.LastLevelPlayed = LEVELS-1;

					ChangeLevel();

					bKeysUp[0x6B] = true;
					break;

				case 0x6D:

					if (bKeysUp[0x6D] || bShowMenu)
						break;

					if (settings.LastLevelPlayed>0)
						settings.LastLevelPlayed--;

					ChangeLevel();

					bKeysUp[0x6D] = true;
					break;
			}
		}

		if (bShowMenu)
		{
			static BOOL mUp = TRUE;
			static BOOL mPlayed = FALSE;
			static int lastSelected = -1;
			int mSelected = vr->Pick(mx,my);

			playBox->Color.G = 1;
			playBox->Color.B = 1;

			fullscreenBox->Color.G = 1;
			fullscreenBox->Color.B = 1;

			soundBox->Color.G = 1;
			soundBox->Color.B = 1;

			exitBox->Color.G = 1;
			exitBox->Color.B = 1;

			if (!bLeftState)
				mUp = TRUE;

			if (lastSelected != mSelected)
				mPlayed = FALSE;

			switch(mSelected)
			{
			case 0:
				playBox->Color.G = 0.7f;
				playBox->Color.B = 0.7f;

				if (!mPlayed && settings.Sound)
				{
					audio->Play(hover);
					mPlayed = true;
				}

				if (bLeftState)
				{
					bShowMenu = FALSE;
					playBox->Visible = exitBox->Visible = soundBox->Visible = fullscreenBox->Visible = FALSE;
					ChangeLevel();

					if (settings.Sound)
						audio->Play(selection);
				}

				break;

			case 1:
				fullscreenBox->Color.G = 0.7f;
				fullscreenBox->Color.B = 0.7f;

				if (!mPlayed && settings.Sound)
				{
					audio->Play(hover);
					mPlayed = true;
				}

				if (bLeftState && mUp)
				{
					settings.FullScreen = !settings.FullScreen;
					fullscreenBox->TextureId = settings.FullScreen ? glFSONTex : glFSTex;
					mUp = FALSE;

					if (settings.Sound)
						audio->Play(selection);
				}

				break;

			case 2:
				soundBox->Color.G = 0.7f;
				soundBox->Color.B = 0.7f;

				if (!mPlayed && settings.Sound)
				{
					audio->Play(hover);
					mPlayed = true;
				}

				if (bLeftState && mUp)
				{
					settings.Sound = !settings.Sound;
					soundBox->TextureId = settings.Sound ? glSoundOnTex : glSoundTex;
					mUp = FALSE;

					if (settings.Sound)
						audio->Play(selection);
				}

				break;

			case 3:
				exitBox->Color.G = 0.7f;
				exitBox->Color.B = 0.7f;

				if (!mPlayed && settings.Sound)
				{
					audio->Play(hover);
					mPlayed = true;
				}

				if (bLeftState)
				{
					if (settings.Sound)
					{
						audio->Play(selection);
						Sleep(200);
					}

					return 0;
				}

				break;
			}

			lastSelected = mSelected;
		}

		if (!bLevelLoaded || bShowMenu)
			continue;

		if (blocks[0]->Position->x > 6)
		{
			settings.LevelPassed[settings.LastLevelPlayed] = TRUE;
			settings.LastLevelPlayed++;

			if (settings.LastLevelPlayed >= LEVELS)
				settings.LastLevelPlayed = LEVELS - 1;

			if (settings.Sound)
				audio->Play(finito);

			ChangeLevel();
		}

		for(int i=0; i<blockCount; i++)
			blocks[i]->Color.R = blocks[i]->Color.G = blocks[i]->Color.B = blocks[i]->Color.A = 1.0f;

		if (bLeftState)
		{
			int newselected = vr->Pick(mx,my) - 1;

			if (selected!=newselected)
			{
				if (selected>-1)
					blocks[selected]->Fix(bx,bz);

				selected = newselected;

				if (selected>-1 && selected<blockCount)
				{
					vEngine::vfloat3 Position = vr->GL2DTo3D(mx, my);
					bx = blocks[selected]->Position->x;
					bz = blocks[selected]->Position->z;
					dx = blocks[selected]->Position->x - Position.x;
					dz = blocks[selected]->Position->z - Position.z;
				}
			}

			if (selected>-1 && selected<blockCount)
			{
				blocks[selected]->Color.R = 0.7f;
				blocks[selected]->Color.G = 1.0f;
				blocks[selected]->Color.B = 0.7f;
				blocks[selected]->Color.A = 1.0f;

				vEngine::vfloat3 Position = vr->GL2DTo3D(mx, my);
				blocks[selected]->TryMove(Position.x+dx, Position.z+dz, blocks, blockCount, selected, tableNoCollision, blockNoCollision);

				if (!blockNoCollision && settings.Sound)
					audio->Play(pad1);

				if (!tableNoCollision && settings.Sound)
					audio->Play(pad2);

			}
		}
		else
		{
			if (selected>-1)
				blocks[selected]->Fix(bx,bz);

			selected = -1;
		}
    }

	return (int) msg.wParam;
}

void GameCleanup()
{
	audio->FreeAudioSource(intro);
	audio->FreeAudioSource(selection);
	audio->FreeAudioSource(hover);
	audio->FreeAudioSource(finito);
	audio->FreeAudioSource(pad1);
	audio->FreeAudioSource(pad2);

	delete vr;
	delete camera;
	delete audio;
}

int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	FILE * file = NULL;
	BOOL bSettingsSuccess = FALSE;

	vutil::SetAppName(APP_NAME);
	vutil::InitTraceFileTarget();

	Trace(TEXT("Begin"));

	bSettingsSuccess = vutil::ReadStructFromFile(TEXT("unb1ok.cfg"), &settings, sizeof(SETTINGS)) == 0;
	
	if (!bSettingsSuccess)
	{
		ZeroMemory(&settings, sizeof(SETTINGS));
		settings.FullScreen = MessageBox(NULL, FULLSCREEN_Q, APP_NAME, MB_YESNO | MB_ICONQUESTION) == IDYES;
		settings.ResX = DEFXRES;
		settings.ResY = DEFYRES;
		settings.Sound = TRUE;
	}

	int windowWidth = settings.ResX;	
	int windowHeight = settings.ResY;

	if (settings.FullScreen)
	{
		RECT desktop;
		GetWindowRect(GetDesktopWindow(), &desktop);
		windowWidth = desktop.right;
		windowHeight = desktop.bottom;
	}

	zipLevelsBuffer = vutil::GetResourceBuffer(hInstance, MAKEINTRESOURCE(IDR_LEVELS), RT_RCDATA, &zipLevelsLength); 
	zipDataBuffer = vutil::GetResourceBuffer(hInstance, MAKEINTRESOURCE(IDR_DATA), RT_RCDATA, &zipDataLength);

	if (!zipLevelsBuffer || !zipDataBuffer)
		return EXIT_FAILURE;

	if (!GameInit(hInstance, windowWidth, windowHeight))
		return EXIT_FAILURE;

	int result = GameLoop();

	GameCleanup();

	vutil::WriteStructToFile(TEXT("unb1ok.cfg"), &settings, sizeof(SETTINGS));

	Trace(TEXT("End"));

	vutil::DestryTraceFileTarget();

	return result;
}
