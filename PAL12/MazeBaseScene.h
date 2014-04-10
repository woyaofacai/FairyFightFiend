#ifndef _MAZE_BASE_SCENE_H
#define _MAZE_BASE_SCENE_H

#include "GameScene.h"
#include "TerrainInfo.h"
#include "Monster.h"

//#include "GameApplication.h"

struct MonsterTypeAndProbability
{
	unsigned int id;
	float probability;
};

class MazeBaseScene : public BaseScene
{
public:
	MazeBaseScene();
	virtual bool keyPressed (const OIS::KeyEvent &arg);
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual void CreateScene(); 
protected:
	int _GetMonsterTile(float x, float z, int* col = NULL, int* row = NULL);
	virtual bool IsHeroPositionValid() = 0;
	void _UpdateMonsters(float deltaTime);
	unsigned int _GetRandomMonsterType();
	int _GenerateMonsterGroupOnBattle(unsigned int monsterEncounter, unsigned int monsterIds[]);

	Monster** m_MonsterTiles;
	TerrainInfo m_TerrainInfo;
	float m_fMonsterTileWidth; 
	float m_fMonsterTileDepth;
	int m_iNumMonsterTileCol;
	int m_iNumMonsterTileRow;
	int m_iNumMonsterTile;
	float m_fMazeWidth;
	float m_fMazeDepth;

	float m_fRivalNumProbabilities[3];
	int m_iNumMonsterType;
	MonsterTypeAndProbability* m_MonsterTypeAndProbs;

	Ogre::SceneNode* m_PivotNode;
	Ogre::SceneNode* m_CamNode;
	Ogre::AnimationState* m_RunAnimState;
	Ogre::AnimationState* m_WalkAnimState;

	Ogre::RaySceneQuery* m_pRaySceneQuery; 
	bool m_bReturnMenu;
};

#endif
