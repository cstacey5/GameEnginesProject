#ifndef ENEMY_H
#define ENEMY_H

#include "BaseApplication.h"

using Ogre::SceneNode; using Ogre::Entity;

class Enemy {
public:
    Enemy();
    Enemy(SceneNode* node, Ogre::Entity* entity, std::vector<bool> defaultRotateCheck);

    void takeDamage(float value);
    SceneNode* getSceneNode();
    Entity* getEntity();
    int getPathNodeIndex();
    bool getCurrentRotateCheck();
    void toggleCurrentRotateCheck();
    void incrementNodeIndex();
    int getHealth();
    
    float getLastDistance() { return lastDistance; }
    void setLastDistance(float f) { lastDistance = f; }
private:
    float enemyHealth = 100;
    SceneNode* enemyNode;
    Entity* enemyEntity;
    int nextPathNodeIndex = 0;
    std::vector<bool> pathRotateCheck;
    float lastDistance = 0.0; //used to make sure enemy doesn't leave the path due to low framerate
};

#endif