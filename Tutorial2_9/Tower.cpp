#include "Tower.h"
#include <iostream>
#include <chrono>
Tower::Tower() {
	towerNode_ = nullptr;
	enemies_ = nullptr;
}
Tower::Tower(SceneNode* towerNode, vector<Enemy>* enemies, int range, int damage, float reload) {
	towerNode_ = towerNode;
	enemies_ = enemies;
	range_ = range;
	damage_ = damage;
	reload_ = reload; //miliseconds

	trigger_ = EnemyTrigger(towerNode_, enemies_, range);
}

int Tower::getRange() const { return range_; }
void Tower::setRange(int range) { trigger_.setRange(range); }

int Tower::getDamage() const { return damage_; }
void Tower::setDamage(int damage) { damage_ = damage; }

EnemyTrigger& Tower::getTrigger() { return trigger_; }

void Tower::fire(){
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShot_).count();
	
	if (elapsed >= reload_) {
		lastShot_ = std::chrono::steady_clock::now();
		vector<Enemy*> enemiesInRange = trigger_.getEnemiesInRange();
		for (Enemy* enemy : enemiesInRange) {
			enemy->takeDamage(damage_);
		}
	}
}