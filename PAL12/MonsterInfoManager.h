#ifndef _MONSTER_MANAGER_H
#define _MONSTER_MANAGER_H

#include <stdio.h>
#include <ogre/Ogre.h>

enum MonsterTypeEnum
{
	MONSTER_TYPE_ZOMBIE = 0,
	MONSTER_TYPE_SPIDER
};

enum MonsterAnimationEnum
{
	MONSTER_ANIM_WALK = 0,
	MONSTER_ANIM_IDLE,
	MONSTER_ANIM_RUN,
	MONSTER_ANIM_ATTACK,
	MONSTER_ANIM_DIE
};

struct MonsterInfo
{
	unsigned int m_Id;
	char m_Name[20];
	char m_MeshName[30];
	int m_iMaxLife;
	int m_iMinLife;
	float m_fMoveSpeed;
	float m_fScale;
	char m_AnimsName[8][20];
};

struct MonsterGroupOnBattle
{
	int m_iNumMonster;
	int m_MonsterIds[3];
};

class MonsterInfoManager
{
public:
	~MonsterInfoManager(void);
	static MonsterInfoManager& GetSingleton();
	bool LoadInformationFromFile(const char* filename);
	MonsterInfo* FindMonsterInfo(unsigned int id);
	MonsterInfo* FindMonsterInfo(Ogre::String& typeName);
private:
	MonsterInfoManager(){}
	MonsterInfoManager(MonsterInfoManager&){}

	MonsterInfo* m_Infos;
	unsigned int m_uiMonsterTypeNum;
	std::map<unsigned int, MonsterInfo*> m_IdInfoMap;
	std::map<Ogre::String, MonsterInfo*> m_NameInfoMap;
};

#endif