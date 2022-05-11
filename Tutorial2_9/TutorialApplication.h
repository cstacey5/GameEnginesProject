/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include <string>
#include <vector>
#include <ctime>
#include "Tutorial2_9\enemy.h"
//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

    void handlePath();
    int pathNodeI = 0;
    int enemyCounter = 1;
    std::time_t lastEnemySpawned = std::time(nullptr);

    std::vector<bool> defaultPathRotate;
    std::vector<Ogre::SceneNode*> createdPath;
    std::vector<Ogre::Degree> createdPathTurns;
    std::vector<Enemy> enemyVector;
    
protected:
    virtual void createScene(void);
    virtual void createFrameListener();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
private:
    Ogre::SceneNode* createPathNode(Ogre::Vector3 pos);
    void createPath();
    void spawnEnemy();
    void createPathVisualizer(Ogre::Vector3 pos, bool rotation);
};

//class Enemy {
//public:
//    void takeDamage();
//    void getSceneNode();
//private:
//    int enemyHealth = 100;
//    Ogre::SceneNode* enemyNode;
//};

//class Path
//{
//public:
//    Path() {
//        createPath();
//    };
//    void handlePath();
//    int pathNodeI = 0;
//private: 
//    Ogre::SceneNode* createPathNode(Ogre::Vector3 pos);
//    void createPath();
//    std::vector<Ogre::SceneNode*> createdPath;
//    std::vector<Ogre::Degree> createdPathTurns;
//};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
