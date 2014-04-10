#ifndef _TERRAIN_INFO_H 
#define _TERRAIN_INFO_H

#include <OGRE\Ogre.h>

class TerrainInfo
{
public:
	TerrainInfo();
	bool Load(Ogre::String& filename)
	{
		Ogre::ConfigFile cf;
		cf.loadFromResourceSystem(filename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();
		Ogre::String sectionName, typeName, dataString;
		while(sectionIter.hasMoreElements())
		{
			sectionName = sectionIter.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *setting = sectionIter.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator it;
			for (it = setting->begin(); it != setting->end(); it++)
			{
				typeName = it->first;
				dataString = it->second;
				if(typeName == "PageWorldX")
				{
					sscanf(dataString.c_str(), "%f", &m_fPageWorldWidth);
				}
				else if(typeName == "PageSize")
				{
					sscanf(dataString.c_str(), "%d", &m_iSize);
				}
			}
		}

		return true;
	}
	
	float m_fPageWorldWidth;
	int m_iSize;
};

#endif 

