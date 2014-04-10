#include <d3d9.h>
#include <d3dx9.h>  
#include <tchar.h> 
#include <mmsystem.h>
#include "D3DUtil.h"
#include "Terrain.h"

#pragma comment(lib,"d3d9.lib")  
#pragma comment(lib,"d3dx9.lib")  
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")  
#pragma comment(lib, "winmm.lib")

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
HRESULT Game_Init(HWND hwnd);
HRESULT Objects_Init();
void Game_Update(float);
void Game_Render();
void Game_Exit();
float Get_FPS();
void Draw_FPS_Text();
void Matrix_Set();

LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPD3DXFONT g_pFont = NULL;

int WINAPI WinMain (HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int iCmdShow)

{
	HWND hwnd; 
	MSG msg; 
	WNDCLASS wndclass; 
	static TCHAR szClassName[ ] = TEXT("WindowsApp");

	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = szClassName;
	wndclass.lpfnWndProc = WindowProcedure; 
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; 
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;

	if (!RegisterClass (&wndclass))
		return 0;

	RECT rc = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(
		szClassName, 
		TEXT("MyApp"), 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		rc.right - rc.left,   
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	ShowWindow (hwnd, iCmdShow);
	UpdateWindow(hwnd);

	Game_Init(hwnd);

	float lastTime = timeGetTime() * 0.001f;
	float currentTime = timeGetTime() * 0.001f;
	float delta;

	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			currentTime = timeGetTime() * 0.001f;
			delta = currentTime - lastTime;
			Game_Update(delta);
			Game_Render();
			lastTime = currentTime;
		}
		Sleep(1);
	}

	Game_Exit();

	return msg.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)  
	{
	case WM_DESTROY:
		PostQuitMessage (0); 
		break;
	default:
		return DefWindowProc (hwnd, message, wParam, lParam);
	}

	return 0;
}

HRESULT Game_Init(HWND hwnd)
{
	LPDIRECT3D9  pD3D = NULL; 
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) 
		return E_FAIL;

	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
	{
		return E_FAIL;
	}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING; 
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = SCREEN_WIDTH;
	d3dpp.BackBufferHeight           = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	if(Objects_Init() != S_OK) return E_FAIL;

	SAFE_RELEASE(pD3D)

	return S_OK;
}

HRESULT Objects_Init()
{
	if(FAILED(D3DXCreateFont(g_pd3dDevice, 30, 0, 0, 1, FALSE, DEFAULT_CHARSET,   
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("ËÎÌו"), &g_pFont)))  
		return E_FAIL;  

	Terrain* g_pTerrain = new Terrain;
	//	g_pTerrain->Load("height128_0.raw", 128, "Data/grass_1.tga");
	g_pTerrain->Load("terrain-heightmap.raw", 257, "Data/grass_1.tga");

	//LPCSTR tileTextues[] = {"Data/lowestTile.tga", "Data/lowTile.tga", "Data/highTile.tga", "Data/highestTile.tga"};
	LPCSTR tileTextues[] = {"Data/lowestTile.tga", "Data/lowTile.tga", "Data/lowTile.tga", "Data/lowTile.tga", "Data/lowTile.tga"};
	g_pTerrain->LoadTiles(tileTextues, 5, 256);
	g_pTerrain->CreateTexture(512, "hill.png");

	return S_OK;
}

void Game_Update(float delta)
{

}


void Game_Render()
{
	Matrix_Set();

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0); 

	g_pd3dDevice->BeginScene();

	Draw_FPS_Text();

	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Draw_FPS_Text()
{
	RECT rect;
	rect.top = 0;
	rect.bottom = SCREEN_HEIGHT;
	rect.left = 0;
	rect.right = SCREEN_WIDTH;

	float fps = Get_FPS();

	TCHAR fps_str[50];
	_stprintf(fps_str, TEXT("FPS:%f"), fps);

	g_pFont->DrawText(0, fps_str, -1, &rect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(0, 255, 0));
}

void Game_Exit()
{
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pd3dDevice);
}

float Get_FPS()
{
	static float currentTime = timeGetTime() * 0.001f;
	static float lastTime = 0;
	static float fps = 0.0f;
	static int frameCount = 0;

	frameCount++;
	currentTime = timeGetTime() * 0.001f;
	if(currentTime - lastTime > 1.0f)
	{
		fps = frameCount / (currentTime - lastTime);
		frameCount = 0;
		lastTime = currentTime;
	}
	return fps;
}



void Matrix_Set()
{
	//--------------------------------------------------------------------------------------  
	D3DXMATRIX matView;
	D3DXVECTOR3 vEye(0.0f, 0.0f, -200.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f); 
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); 

	D3DXMATRIX matProj; 
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f); 
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj); 
}

