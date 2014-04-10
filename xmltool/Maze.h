#include "irrXML.h"
#include <string.h>
#include <vector>
using namespace irr; // irrXML is located 
using namespace io;  // in the namespace irr::io

struct MonsterPositionInfo
{
	unsigned int id;
	float pos[3];
}; 

struct MonsterTypeAndProbability
{
	unsigned int id;
	float probability;
};

struct MazeInfoHeader
{
	float m_fMazeWidth;
	float m_fMazeDepth;
	float m_fStartPosition[3];
	float m_fRivalNumProbabilities[3];
	int m_iMonsterTileCol;
	int m_iMonsterTileRow;
	int m_iNumMonsterTypes;
	int m_iMonstersTypeOffset;
	int m_iNumMonsters;
	int m_iMonsterOffset;
};

struct MazeInfo
{
	MazeInfoHeader m_Header;
	std::vector<MonsterTypeAndProbability> m_MonsterTypesIncluded;
	std::vector<MonsterPositionInfo> m_MonsterPositionInfos;
};

void ExtractMonsterTypes(IrrXMLReader* xml, MazeInfo* pMaze);
void ExtractMonsterPositions(IrrXMLReader* xml, MazeInfo* pMaze);
MazeInfo* ExtractMazeInfoFromXML(const char* filename);
void SaveMazeInfoToBinaryFile(const char* filename, MazeInfo* pMaze);
