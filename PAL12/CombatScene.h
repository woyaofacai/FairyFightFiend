#ifndef _COMBATSCENE_H
#define _COMBATSCENE_H

#include "GUIMouseBaseScene.h"
#include "MonsterInfoManager.h"
#include "SoundManager.h"
#include "MonsterRival.h"
#include "HeroRival.h"
#include "SceneMsgID.h"

class MazeBaseScene;

class CombatScene : public GUIMouseBaseScene
{
	friend class MazeBaseScene;
	
	enum CombatStateEnum
	{
		RIVAL_ATTACK,
		SELECT_TACTICS,
		SELECT_RIVAL,
		HERO_ATTACK
	};

public:
	CombatScene(void);
	~CombatScene(void);
	virtual void CreateScene();
	virtual void CreateCamera();
	virtual void SetActive();
	virtual void SetUnactive();
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	//virtual bool keyPressed (const OIS::KeyEvent &arg);
	virtual void OnMessage(Ogre::uint32 id, Ogre::int32 data, void* lpinfo);

	virtual bool mouseMoved (const OIS::MouseEvent &arg);
	virtual bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased (const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	bool OnAttackButtonClicked(const CEGUI::EventArgs& arg);

	void KillMonster();
private:

	void _AddMonsterOnPosition(unsigned int id, int pos);
	void _GetRandomRivalPosition(int* first, int* second, int* third);
	int _GetMonsterPosNumToAttack(const OIS::MouseEvent &arg);
	int _GetFirstValidMonsterPosNum();
	void _ConvertState(CombatStateEnum state);
	void _UpdateCameraPosition(const Ogre::FrameEvent& evt);
	
	Ogre::SceneNode* m_RivalsRootNode;
	Ogre::SceneNode* m_HeroNode;
	Ogre::SceneNode* m_PyramidNode;
	CombatStateEnum m_CurState;

	int m_iMonsterNum;
	unsigned int m_MonsterTypes[3];
	Rival* m_Rivals[3];
	Rival* m_Hero;

	irrklang::ISound* m_BackMusic;
	int m_iRivalFocused;

	CEGUI::Window* m_guiWindow;
	CEGUI::Window* m_AttackBtn;

	Ogre::Vector3 m_CameraControlPoints[3];
	Ogre::Timer m_CameraMoveTimer;

};

#endif

 