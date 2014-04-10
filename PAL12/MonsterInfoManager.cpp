#include "MonsterInfoManager.h"

struct MonsterTypeNamePair
{
	MonsterTypeEnum type;
	const char* name;
};

MonsterInfoManager::~MonsterInfoManager(void)
{

}

MonsterInfoManager& MonsterInfoManager::GetSingleton()
{
	static MonsterInfoManager mgr;
	return mgr;
}

bool MonsterInfoManager::LoadInformationFromFile(const char* filename)
{
	FILE* fp = fopen(filename, "rb");
	if(!fp)
		return false;

	fread((void*)&m_uiMonsterTypeNum, sizeof(unsigned int), 1, fp);
	m_Infos = new MonsterInfo[m_uiMonsterTypeNum];
	fread((void*)m_Infos, sizeof(MonsterInfo), m_uiMonsterTypeNum, fp);


	for (int i = 0; i < m_uiMonsterTypeNum; i++)
	{
		m_NameInfoMap.insert(std::pair<Ogre::String, MonsterInfo*>(Ogre::String(m_Infos[i].m_Name), &m_Infos[i]));
		m_IdInfoMap.insert(std::pair<unsigned int, MonsterInfo*>(m_Infos[i].m_Id, &m_Infos[i]));
	}

	fclose(fp);
	return false;
}

MonsterInfo* MonsterInfoManager::FindMonsterInfo(Ogre::String& typeName)
{
	std::map<Ogre::String, MonsterInfo*>::iterator it;
	it = m_NameInfoMap.find(typeName);

	if(it == m_NameInfoMap.end())
		return NULL;

	return it->second;
}

MonsterInfo* MonsterInfoManager::FindMonsterInfo(unsigned int id)
{
	std::map<unsigned int, MonsterInfo*>::iterator it;
	it = m_IdInfoMap.find(id);

	if(it == m_IdInfoMap.end())
		return NULL;

	return it->second;
}