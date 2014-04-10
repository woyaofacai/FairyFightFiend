#ifndef _TESTSCENE_H
#define _TESTSCENE_H

#include "GameScene.h"
#include "GameApplication.h"
#include "Monster.h"
#include "TerrainInfo.h"
#include "MonsterInfoManager.h"

#define HEROINE_WALK_SPEED 20.0f
#define HEROINE_RUN_SPEED 40.0f
#define MONSTER_TILE_NUM 64
#define MONSTER_TILE_COL_NUM 8
#define MONSTER_TILE_SIZE 32

#define GENERATE_HILLS_CONFIG_FILE

class TestScene : public BaseScene
{
public:
	virtual bool keyPressed (const OIS::KeyEvent &arg);
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual void CreateCamera();
	virtual void CreateScene(); 
	virtual void CreateSceneManager();
private:
	void _CreateMonsters();
	float _GetTerrainHeight(float x, float z);
	int _GetMonsterTile(float x, float z, int* col = NULL, int* row = NULL);
	Ogre::SceneNode* m_PivotNode;
	Ogre::SceneNode* m_CamNode;
	Ogre::AnimationState* m_HeroAnimState;
	Ogre::AnimationState* m_RunAnimState;
	Ogre::AnimationState* m_WalkAnimState;
	bool m_bOver;
	float m_fTimeStartRunning;
	float m_fTimeEndRunning;
	Ogre::RaySceneQuery* m_pRaySceneQuery; 
	std::list<Monster*> m_Monsters;
	CEGUI::Window* m_fpsText;
	Monster* m_MonsterTiles[MONSTER_TILE_NUM * MONSTER_TILE_NUM];
	TerrainInfo m_TerrainInfo;
	float m_fMonsterTileWidth;

#ifdef GENERATE_HILLS_CONFIG_FILE
	Ogre::Vector3 _GetRandomPos();
	void _GenerateMonsterPositions(LPCSTR filename, int num);
#endif
};

#endif

