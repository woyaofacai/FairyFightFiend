enum MonsterAnimationEnum
{
	MONSTER_ANIM_WALK = 0,
	MONSTER_ANIM_IDLE,
	MONSTER_ANIM_RUN,
	MONSTER_ANIM_ATTACK,
	MONSTER_ANIM_DIE
};

enum MonsterTypeEnum
{
	MONSTER_TYPE_ZOMBIE = 0,
	MONSTER_TYPE_SPIDER = 1
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