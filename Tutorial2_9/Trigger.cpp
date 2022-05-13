#include "Trigger.h"

//Abstract trigger implementations
AbstractTrigger::AbstractTrigger() {
	triggerNode_ = nullptr;
	range_ = 0;
}


int AbstractTrigger::getRange() const { return range_; }
void AbstractTrigger::setRange(int range) { range_ = range; }
void AbstractTrigger::setEntity(Ogre::Entity* entity) { visualizationEntity_ = entity; }

void AbstractTrigger::showVisualization() {
	if (!visualizationEntity_) {
		return;
	}
	static int nameId = 0;

	if (setupVisualizationNodes) {
		setupVisualizationNodes = false;
		for (int i = 1; i <= 24; ++i) {
			SceneNode* newNode = triggerNode_->createChildSceneNode();
			newNode->setScale(Ogre::Vector3(0.1, 0.1, 0.1));
			Ogre::Entity* clone = visualizationEntity_->clone("clone" + std::to_string(nameId));
			newNode->attachObject(clone);
			visualizationNodes_.push_back(newNode);
			++nameId;
		}
	}

	float x, y, theta = 0.0;
	for (SceneNode* node : visualizationNodes_) {
		x = range_ * (1/triggerNode_->getScale().x) * Ogre::Math::Cos(theta); //remove affect of local scale
		y = range_ * (1/triggerNode_->getScale().z) * Ogre::Math::Sin(theta);
		node->setPosition(x, 0, y);

		theta += Ogre::Math::TWO_PI / visualizationNodes_.size();
	}
}

//Normal trigger implementations
Trigger::Trigger(SceneNode* triggerNode, vector<SceneNode*>* activatorNodes, int range) {
	triggerNode_ = triggerNode;
	activatorNodes_ = activatorNodes;
	range_ = range;
}

vector<SceneNode*> Trigger::getNodesInRange() const {
	vector<SceneNode*> result;

	Ogre::Vector3 triggerPos = triggerNode_->getPosition();
	for (SceneNode* node : *activatorNodes_) {
		if (triggerPos.distance(node->getPosition()) <= range_) {
			result.push_back(node);
		}
	}
	return result;
}

//Enemy trigger implementations
EnemyTrigger::EnemyTrigger(SceneNode* triggerNode, vector<Enemy>* enemyList, int range) {
	triggerNode_ = triggerNode;
	enemyList_ = enemyList;
	range_ = range;
}

vector<Enemy*> EnemyTrigger::getEnemiesInRange() const {
	vector<Enemy*> result;

	Ogre::Vector3 triggerPos = triggerNode_->getPosition();
	for (Enemy& enemy : *enemyList_) {
		if (triggerPos.distance(enemy.getSceneNode()->getPosition()) <= range_) {
			result.push_back(&enemy);
		}
	}
	return result;
}