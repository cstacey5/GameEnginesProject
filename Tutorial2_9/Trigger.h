#ifndef TRIGGER_H
#define TRIGGER_H

#include "BaseApplication.h"
#include "enemy.h"
#include <vector>
#include <string>

using Ogre::SceneNode; using std::vector;

class AbstractTrigger {
public:
	AbstractTrigger();

	int getRange() const;
	void setRange(int);
	void setEntity(Ogre::Entity*);

	void showVisualization();

protected:
	SceneNode* triggerNode_;
	vector<SceneNode*> visualizationNodes_;
	int range_;
	Ogre::Entity* visualizationEntity_ = nullptr;
	bool setupVisualizationNodes = true;
};

class Trigger : public AbstractTrigger {
public:
	Trigger() : AbstractTrigger() {}
	Trigger(SceneNode*, vector<SceneNode*>*, int=0);
	vector<SceneNode*> getNodesInRange() const;

private:
	vector<SceneNode*>* activatorNodes_ = nullptr;
};

class EnemyTrigger : public AbstractTrigger {
public:
	EnemyTrigger() : AbstractTrigger() {}
	EnemyTrigger(SceneNode*, vector<Enemy>*, int = 0);
	vector<Enemy*> getEnemiesInRange() const;

private:
	vector<Enemy>* enemyList_ = nullptr;
};


#endif