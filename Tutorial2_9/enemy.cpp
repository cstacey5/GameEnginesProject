#include "enemy.h"


Enemy::Enemy() {
	enemyNode = nullptr;
	enemyEntity = nullptr;
}

Enemy::Enemy(Ogre::SceneNode* node, Ogre::Entity* entity, std::vector<bool> defaultRotateCheck) {
	enemyNode = node;
	enemyEntity = entity;
	pathRotateCheck = defaultRotateCheck;
}

void Enemy::takeDamage(float value) {
	enemyHealth -= value;
	float healthPercentage = enemyHealth / 100.0;
	enemyNode->setScale(0.4 * healthPercentage, 0.4 * healthPercentage, 0.4 * healthPercentage);
}

SceneNode* Enemy::getSceneNode() {
	return enemyNode;
}

Entity* Enemy::getEntity() {
	return enemyEntity;
}

int Enemy::getPathNodeIndex() {
	return nextPathNodeIndex;
}

bool Enemy::getCurrentRotateCheck() {
	return pathRotateCheck.at(nextPathNodeIndex);
}

void Enemy::toggleCurrentRotateCheck() {
	pathRotateCheck.at(nextPathNodeIndex) = true;
}

void Enemy::incrementNodeIndex() {
	nextPathNodeIndex++;
}

int Enemy::getHealth() {
	return enemyHealth;
}