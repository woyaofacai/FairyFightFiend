#include "irrXML.h"
using namespace irr; // irrXML is located 
using namespace io;  // in the namespace irr::io

#include <string> // we use STL strings to store data
#include <vector>
// in this example

#include "MonsterInfo.h"
#include "Maze.h"

struct MonsterAnimNameEnumPair
{
	const char* name;
	int animIndex;
};

static const MonsterAnimNameEnumPair g_animsNameIndexPairs[] = 
{
	{"Walk", MONSTER_ANIM_WALK},
	{"Idle", MONSTER_ANIM_IDLE},
	{"Run", MONSTER_ANIM_RUN},
	{"Attack", MONSTER_ANIM_ATTACK},
	{"Die", MONSTER_ANIM_DIE},
	{NULL, 0}
};

void ExtractAnimationNames( IrrXMLReader* xml, MonsterInfo* pInfo);
void ExtractMonsterInfo( IrrXMLReader* xml, MonsterInfo& info);
void ExtractMonsterInfosFromXML(const char* filename, std::vector<MonsterInfo>& infos);
bool SaveMonsterInfosToBinaryFile(const char* filename, std::vector<MonsterInfo>& vec);

void ExtractAnimationNames( IrrXMLReader* xml, MonsterInfo* pInfo) 
{
	bool bFindEndElem = false;
	while(!bFindEndElem && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_ELEMENT_END:
			if(strcmp("animations", xml->getNodeName()) == 0)
				bFindEndElem = true;
			break;
		case EXN_ELEMENT:
			if(strcmp("animation", xml->getNodeName()) == 0)
			{
				const MonsterAnimNameEnumPair* pair = g_animsNameIndexPairs;
				const char* name = xml->getAttributeValue("name");
				const char* value = xml->getAttributeValue("value");
				while(pair->name != NULL)
				{
					if(strcmp(name, pair->name) == 0)
					{
						strcpy(pInfo->m_AnimsName[pair->animIndex], value);
						break;
					}
					pair++;
				}
			}
			break;
		}
	}
}

void ExtractMonsterInfo( IrrXMLReader* xml, MonsterInfo& info) 
{
	const char* name = xml->getAttributeValue("name");
	strcpy(info.m_Name, name);

	printf("%s\n", info.m_Name);

	EXML_NODE nodeType;
	bool bFindEndElem = false;

	while(!bFindEndElem && xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_ELEMENT_END:
			if(strcmp("monster", xml->getNodeName()) == 0)
				bFindEndElem = true;
			break;
		case EXN_ELEMENT:
			if(strcmp("life", xml->getNodeName()) == 0)
			{
				sscanf(xml->getAttributeValue("min"), "%d", &info.m_iMinLife);
				sscanf(xml->getAttributeValue("max"), "%d", &info.m_iMaxLife);
			}
			else if(strcmp("move-speed", xml->getNodeName()) == 0)
			{
				sscanf(xml->getAttributeValue("value"), "%f", &info.m_fMoveSpeed);
			}
			else if(strcmp("scale", xml->getNodeName()) == 0)
			{
				sscanf(xml->getAttributeValue("value"), "%f", &info.m_fScale);
			}
			else if(strcmp("animations", xml->getNodeName()) == 0)
			{
				ExtractAnimationNames(xml, &info);
			}
			else if(strcmp("mesh", xml->getNodeName()) == 0)
			{
				sscanf(xml->getAttributeValue("id"), "%d", &info.m_Id);
				strcpy(info.m_MeshName, xml->getAttributeValue("name"));
			}
			break;
		default:
			break;
		}
	}
}

void ExtractMonsterInfosFromXML(const char* filename, std::vector<MonsterInfo>& infos)
{
	IrrXMLReader* xml = createIrrXMLReader("config.xml");
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_ELEMENT:
			if(strcmp("monster", xml->getNodeName()) == 0)
			{
				MonsterInfo info;
				ExtractMonsterInfo(xml, info);
				infos.push_back(info);
			}
			break;
		}
	}
	delete xml;
}

bool SaveMonsterInfosToBinaryFile(const char* filename, std::vector<MonsterInfo>& vec)
{
	FILE* fp = fopen(filename, "wb");
	if(!fp)
		return false;

	unsigned int monsterNum = vec.size();
	if(monsterNum == 0)
		return false;

	fwrite((void*)&monsterNum, sizeof(unsigned int), 1, fp);
	fwrite((void*)&vec[0], sizeof(MonsterInfo), monsterNum, fp);

	fclose(fp);
	return true;
}

MonsterInfo* ReadMonsterInfosFromBinaryFile(const char* filename)
{
	FILE* fp = fopen(filename, "rb");
	if(!fp)
		return NULL;

	unsigned int num;
	fread((void*)&num, sizeof(unsigned int), 1, fp);
	MonsterInfo* pInfos = new MonsterInfo[num];
	fread((void*)pInfos, sizeof(MonsterInfo), num, fp);
	fclose(fp);
	return pInfos;
}


int main()
{
	std::vector<MonsterInfo> monsterInfoVec;
	ExtractMonsterInfosFromXML("config.xml", monsterInfoVec);
	SaveMonsterInfosToBinaryFile("../media/models/monsters.dat", monsterInfoVec);
	//MonsterInfo* monsterInfos = ReadMonsterInfosFromBinaryFile("monsters.dat");
	
	//MazeInfo* pMaze = ExtractMazeInfoFromXML("maze.xml");
	//SaveMazeInfoToBinaryFile("../media/maze/Valley.maze", pMaze);

	system("pause");
	return 0;
}

