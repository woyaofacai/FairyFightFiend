#include "Maze.h"

void ExtractMonsterTypes(IrrXMLReader* xml, MazeInfo* pMaze)
{
	bool bFindEndElem = false;
	while(!bFindEndElem && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_ELEMENT_END:
			if(strcmp("monster-types", xml->getNodeName()) == 0)
				bFindEndElem = true;
			break;
		case EXN_ELEMENT:
			if(strcmp("monster-type", xml->getNodeName()) == 0)
			{
				MonsterTypeAndProbability monsterTypeAndPro;
				sscanf(xml->getAttributeValue("id"), "%d", &monsterTypeAndPro.id);
				sscanf(xml->getAttributeValue("probability"), "%f", &monsterTypeAndPro.probability);

				pMaze->m_MonsterTypesIncluded.push_back(monsterTypeAndPro);
			}
			break;
		}
	}
	pMaze->m_Header.m_iNumMonsterTypes = pMaze->m_MonsterTypesIncluded.size();
}

void ExtractMonsterPositions(IrrXMLReader* xml, MazeInfo* pMaze)
{
	bool bFindEndElem = false;
	while(!bFindEndElem && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_ELEMENT_END:
			if(strcmp("monsters", xml->getNodeName()) == 0)
				bFindEndElem = true;
			break;
		case EXN_ELEMENT:
			if(strcmp("monster", xml->getNodeName()) == 0)
			{
				MonsterPositionInfo info;
				sscanf(xml->getAttributeValue("id"), "%d", &info.id);
				sscanf(xml->getAttributeValue("x"), "%f", &info.pos[0]);
				sscanf(xml->getAttributeValue("y"), "%f", &info.pos[1]);
				sscanf(xml->getAttributeValue("z"), "%f", &info.pos[2]);
				pMaze->m_MonsterPositionInfos.push_back(info);
			}
			break;
		}
	}
	pMaze->m_Header.m_iNumMonsters = pMaze->m_MonsterPositionInfos.size();
}

MazeInfo* ExtractMazeInfoFromXML(const char* filename)
{
	MazeInfo* pMaze = new MazeInfo;
	IrrXMLReader* xml = createIrrXMLReader(filename);
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_ELEMENT:
			if(strcmp("rival-numbers", xml->getNodeName()) == 0)
			{
				sscanf(xml->getAttributeValue("one"), "%f", &pMaze->m_Header.m_fRivalNumProbabilities[0]);
				sscanf(xml->getAttributeValue("two"), "%f", &pMaze->m_Header.m_fRivalNumProbabilities[1]);
				sscanf(xml->getAttributeValue("three"), "%f", &pMaze->m_Header.m_fRivalNumProbabilities[2]);
			}
			else if(strcmp("monster-types", xml->getNodeName()) == 0)
			{
				ExtractMonsterTypes(xml, pMaze);
			}
			else if(strcmp("monsters", xml->getNodeName()) == 0)
			{
				ExtractMonsterPositions(xml, pMaze);
			}
			else if(strcmp("monster-tiles", xml->getNodeName()) == 0)
			{
				sscanf(xml->getAttributeValue("col"), "%d", &pMaze->m_Header.m_iMonsterTileCol);
				sscanf(xml->getAttributeValue("row"), "%d", &pMaze->m_Header.m_iMonsterTileRow);
			}
			else if(strcmp("maze-size", xml->getNodeName()) == 0)
			{
				sscanf(xml->getAttributeValue("width"), "%f", &pMaze->m_Header.m_fMazeWidth);
				sscanf(xml->getAttributeValue("depth"), "%f", &pMaze->m_Header.m_fMazeDepth);
			}
			else if(strcmp("start-position", xml->getNodeName()) == 0)
			{
				sscanf(xml->getAttributeValue("x"), "%f", &pMaze->m_Header.m_fStartPosition[0]);
				sscanf(xml->getAttributeValue("y"), "%f", &pMaze->m_Header.m_fStartPosition[1]);
				sscanf(xml->getAttributeValue("z"), "%f", &pMaze->m_Header.m_fStartPosition[2]);
			}
			break;
		}
	}
	delete xml;
	return pMaze;
}

void SaveMazeInfoToBinaryFile(const char* filename, MazeInfo* pMaze)
{
	pMaze->m_Header.m_iMonstersTypeOffset = sizeof(pMaze->m_Header);
	pMaze->m_Header.m_iMonsterOffset = pMaze->m_Header.m_iMonstersTypeOffset + sizeof(MonsterTypeAndProbability) * pMaze->m_Header.m_iNumMonsterTypes;

	FILE* fp = fopen(filename, "wb");
	
	fwrite((void*)&pMaze->m_Header, sizeof(MazeInfoHeader), 1, fp);
	fwrite((void*)&pMaze->m_MonsterTypesIncluded[0], sizeof(MonsterTypeAndProbability), pMaze->m_Header.m_iNumMonsterTypes, fp);
	fwrite((void*)&pMaze->m_MonsterPositionInfos[0], sizeof(MonsterPositionInfo), pMaze->m_Header.m_iNumMonsters, fp);

	fclose(fp);
}

