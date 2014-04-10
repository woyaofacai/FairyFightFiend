#include "Terrain.h"

extern LPDIRECT3DDEVICE9 g_pd3dDevice;

struct VERTEX
{
	float x, y, z;
	float u, v;
	VERTEX(float x, float y, float z, float u, float v):x(x), y(y), z(z), u(u), v(v){}
	VERTEX(){}
	static const DWORD FVF; 
};

const DWORD VERTEX::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

Terrain::Terrain(void)
{
	m_fHeightScale = 0.15f;
	m_heightData = NULL;
	m_iVertexSpace = 1.0f;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pTexture = NULL;
}

Terrain::~Terrain(void)
{
	if(m_heightData != NULL) {
		delete [] m_heightData;
	}
}

HRESULT Terrain::Load(const char* filename, int size, const char* textureName)
{
	HRESULT hr;
	FILE* fp = NULL;
	fp = fopen(filename, "r");

	if(!fp)
		return E_FAIL;

	m_iSize = size;
	m_heightData = new unsigned char[size * size];

	fread((void*)m_heightData, size * size, 1, fp);
	fclose(fp);

	hr = g_pd3dDevice->CreateVertexBuffer(sizeof(VERTEX) * size * size, 
		D3DUSAGE_WRITEONLY, VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);

	if(FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateIndexBuffer(sizeof(WORD) * (size - 1) * (size - 1) * 6, 
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIndexBuffer, NULL);

	if(FAILED(hr))
		return hr;

	D3DXCreateTextureFromFileA(g_pd3dDevice, textureName, &m_pTexture);

	return S_OK;
}


void Terrain::SetHeightScale(float heightScale)
{
	m_fHeightScale = heightScale;
}

float Terrain::GetHeightScale()
{
	return m_fHeightScale;
}

void Terrain::LoadTiles(LPCSTR textureNames[], int numTiles, int tileSize)
{
	m_tiles = new STRN_TEXTURE_TILE[numTiles];
	m_iNumTiles = numTiles;
	m_iTileSize = tileSize;

	int ilastHeight = -255/(numTiles - 1);

	for (int i = 0; i < numTiles; i++)
	{
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		HRESULT hr = D3DXCreateTextureFromFileA(g_pd3dDevice, textureNames[i], &pTexture);
		if(FAILED(hr))
		{
			MessageBoxA(NULL, "Create Tile Texture Failed!", NULL, 0);
		}
		D3DLOCKED_RECT d3dlr;
		pTexture->LockRect(0, &d3dlr, NULL, 0);
		m_tiles[i].imageData = (unsigned char*)malloc(sizeof(DWORD) * tileSize * tileSize);
		memcpy((void*)m_tiles[i].imageData, d3dlr.pBits, sizeof(DWORD) * tileSize * tileSize);
		pTexture->UnlockRect(0);
		SAFE_RELEASE(pTexture);

		m_tiles[i].region.m_iLowHeight = ilastHeight + 1;
		ilastHeight += 255 / (numTiles - 1);

		m_tiles[i].region.m_iOptimalHeight = ilastHeight;
		m_tiles[i].region.m_iHighHeight = 2 * ilastHeight - m_tiles[i].region.m_iLowHeight;
	}

	//m_tiles[0].region.m_iLowHeight = -10;
	//m_tiles[0].region.m_iOptimalHeight = 0;
	//m_tiles[0].region.m_iHighHeight = 10;
	//m_tiles[1].region.m_iLowHeight = 0;
	//m_tiles[1].region.m_iOptimalHeight = 200;
	//m_tiles[1].region.m_iHighHeight = 400;
	//m_tiles[1].region.m_iHighHeight = 255;
	//	m_tiles[numTiles-1].region.m_iHighHeight = 255;
}


HRESULT Terrain::CreateTexture(int size, LPCSTR filename)
{
	HRESULT hr;
	hr = g_pd3dDevice->CreateTexture(size, size, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture, NULL);

	m_iTexSize = size;

	D3DLOCKED_RECT d3dlr;
	m_pTexture->LockRect(0, &d3dlr, NULL, 0);

	DWORD* pBits = (DWORD*)d3dlr.pBits;
	float fHeightToTexRatio = (float)m_iSize / (float)m_iTexSize;

	
	for (int z = 0; z < size; z++)
	{
		for(int x = 0; x < size; x++)
		{
			D3DXCOLOR color(0.0f, 0.0f, 0.0f, 1.0f);
			unsigned char height = InterpolateHeight(x, z, fHeightToTexRatio);
			
			int texCoordX = x, texCoordY = z;
			GetTexCoords(&texCoordX, &texCoordY);

			for (int i = 0; i < m_iNumTiles; i++)
			{
				float blend = RegionPercent(i, height);
				DWORD* pTexBits = (DWORD*)m_tiles[i].imageData;
				D3DXCOLOR c(pTexBits[texCoordY * m_iTileSize + texCoordX]);
				color += c * blend;
			}

			pBits[z * m_iTexSize + x] = (DWORD)color;
		}
	}
	
	m_pTexture->UnlockRect(0);

	D3DXSaveTextureToFileA(filename, D3DXIFF_PNG, m_pTexture, NULL);

	return S_OK;
}


unsigned char Terrain::InterpolateHeight( int x, int z, float fHeightToTexRatio )
{
	unsigned char ucLow, ucHighX, ucHighZ;
	float fScaledX = x * fHeightToTexRatio;
	float fScaledZ = z * fHeightToTexRatio;
	float ucX, ucZ;
	float fInterpolation;

	int x2 = (int)fScaledX;
	int z2 = (int)fScaledZ;

	ucLow = GetTrueHeightAtPoint(x2, z2);
	if(x2 + 1 > m_iSize)
		return ucLow;
	else 
		ucHighX = GetTrueHeightAtPoint(x2 + 1, z2);

	fInterpolation = fScaledX - x2;
	ucX = ucLow * (1.0f - fInterpolation) + ucHighX * fInterpolation;

	if(z2 + 1 > m_iSize)
		return ucLow;
	else 
		ucHighZ = GetTrueHeightAtPoint(x2, z2 + 1);

	fInterpolation = fScaledZ - z2;
	ucZ = ucLow * (1.0f - fInterpolation) + ucHighZ * fInterpolation;

	return (unsigned char)((ucX + ucX)/2);

}

unsigned char Terrain::GetTrueHeightAtPoint(int x, int z)
{
	return m_heightData[z * m_iSize + x];
}

float Terrain::RegionPercent(int i, unsigned char ucHeight)
{
	if(ucHeight < m_tiles[i].region.m_iLowHeight || ucHeight > m_tiles[i].region.m_iHighHeight)
		return 0.0f;

	float temp1, temp2;
	if(ucHeight < m_tiles[i].region.m_iOptimalHeight)
	{
		temp1 = ucHeight - (float)m_tiles[i].region.m_iLowHeight;
		temp2 = (float)(m_tiles[i].region.m_iOptimalHeight - m_tiles[i].region.m_iLowHeight);
	}
	else {
		temp1 = (float)m_tiles[i].region.m_iHighHeight - ucHeight;
		temp2 = (float)(m_tiles[i].region.m_iHighHeight - m_tiles[i].region.m_iOptimalHeight);
	}
	return temp1 / temp2;
}

void Terrain::GetTexCoords(int* x, int* y)
{
	int iRepeatX = -1;
	int iRepeatY = -1;
	int i = 0;

	while(iRepeatX == -1)
	{
		i++;
		if(*x < (m_iTileSize * i))
			iRepeatX = i - 1;
	}

	i = 0;
	while(iRepeatY == -1)
	{
		i++;
		if(*y < (m_iTileSize * i))
			iRepeatY = i - 1;
	}

	*x = *x - (m_iTileSize * iRepeatX);
	*y = *y - (m_iTileSize * iRepeatY);
}