#include <OGRE\ExampleApplication.h>

#pragma comment(lib, "OgreMain_d.lib")
#pragma comment(lib, "OIS_d.lib")

class Example1FrameListener : public FrameListener
{
public:
	Example1FrameListener(Entity* ent)
	{
		mAnim = ent->getAnimationState("Walk");
		mAnim->setEnabled(true);
		mAnim->setLoop(true);
	}

	bool frameStarted(const FrameEvent& evt)
	{
		mAnim->addTime(evt.timeSinceLastFrame);
		return true;
	}
private:
	AnimationState* mAnim;
};

class Example1 : public ExampleApplication
{
public:
	void createScene()
	{
		Entity* ent = mSceneMgr->createEntity("jaiqua.mesh");
		SceneNode* node = mSceneMgr->createSceneNode();
		node->attachObject(ent);
		mSceneMgr->getRootSceneNode()->addChild(node);

		AnimationStateSet* animSet = ent->getAllAnimationStates();
		AnimationStateIterator iter = animSet->getAnimationStateIterator();
		while(iter.hasMoreElements())
		{
			std::cout << iter.getNext()->getAnimationName() << std::endl;
		}

		Example1FrameListener* listener = new Example1FrameListener(ent);
		mRoot->addFrameListener(listener);
	}
};

int main()
{
	Example1 app;
	app.go();
	return 0;
}