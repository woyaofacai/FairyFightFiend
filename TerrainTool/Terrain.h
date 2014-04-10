#ifndef TERRAIN_CLASS_H
#define TERRAIN_CLASS_H

#include <windows.h>
#include <d3dx9.h>
#include <stdio.h>
#include "D3DUtil.h"
#include "Terrain.h"

struct STRN_TEXTURE_REGIONS
{
	int m_iLowHeight;
	int m_iOptimalHeight;
	int m_iHighHeight;
};

struct STRN_TEXTURE_TILE
{
	unsigned char* imageData;
	STRN_TEXTURE_REGIONS region;
};

typedef struct
{
	byte	identsize;              // Size of ID field that follows header (0)
	byte	colorMapType;           // 0 = None, 1 = paletted
	byte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;          // First colour map entry
	unsigned short	colorMapLength;         // Number of colors
	unsigned char 	colorMapBits;   // bits per palette entry
	unsigned short	xstart;                 // image x origin
	unsigned short	ystart;                 // image y origin
	unsigned short	width;                  // width in pixels
	unsigned short	height;                 // height in pixels
	byte	bits;                   // bits per pixel (8 16, 24, 32)
	byte	descriptor;             // image descriptor
} TGAHEADER;


class Terrain
{
public:
	Terrain();
	HRESULT Load(const char* filename, int size, const char* textureName);
	void LoadTiles(LPCSTR textureNames[], int numTiles, int tileSize);
	//void Render();
	void SetHeightScale(float heightScale);
	float GetHeightScale();
	HRESULT CreateTexture(int size, LPCSTR filename);
	unsigned char GetTrueHeightAtPoint(int x, int z);
	unsigned char InterpolateHeight( int x, int z, float fHeightToTexRatio );
	float RegionPercent(int i, unsigned char ucHeight);
	void GetTexCoords(int* x, int* y);
	~Terrain(void);
private:
	int m_iSize;
	unsigned char* m_heightData;
	float m_fHeightScale;
	float m_iVertexSpace;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	LPDIRECT3DTEXTURE9 m_pTexture;
	STRN_TEXTURE_TILE* m_tiles;
	int m_iNumTiles;
	int m_iTileSize;
	int m_iTexSize;
};

#endif
