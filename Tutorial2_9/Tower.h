#ifndef TOWER_H
#define TOWER_H

#include "BaseApplication.h"
#include "Trigger.h"
#include "enemy.h"
#include <vector>
#include <chrono>

using Ogre::SceneNode; using std::vector;

class Tower {
public:
	Tower();
	Tower(SceneNode*, vector<Enemy>*, int range = 0, int damage = 0, float reload = 1);

	int getRange() const;
	void setRange(int);
	int getDamage() const;
	void setDamage(int);
	EnemyTrigger& getTrigger();

	void fire();
private:
	SceneNode* towerNode_;
	int range_ = 0;
	float damage_ = 0;
	float reload_ = 1000; //in milliseconds
	EnemyTrigger trigger_;
	vector<Enemy>* enemies_;
	std::chrono::steady_clock::time_point lastShot_ = std::chrono::steady_clock::now();
};



#endif