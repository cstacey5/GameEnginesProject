#ifndef ENEMY_H
#define ENEMY_H

#include "..\BaseApplication.h"

using Ogre::SceneNode; using Ogre::Entity;

class Enemy {
public:
    Enemy();
    Enemy(SceneNode* node, Ogre::Entity* entity, std::vector<bool> defaultRotateCheck);

    void takeDamage(int value);
    SceneNode* getSceneNode();
    Entity* getEntity();
    int getPathNodeIndex();
    bool getCurrentRotateCheck();
    void toggleCurrentRotateCheck();
    void incrementNodeIndex();
    int getHealth();

private:
    int enemyHealth = 100;
    SceneNode* enemyNode;
    Entity* enemyEntity;
    int nextPathNodeIndex = 0;
    std::vector<bool> pathRotateCheck;
};

#endif