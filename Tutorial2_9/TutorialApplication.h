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
#include "enemy.h"
#include "Tower.h"
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
    std::vector<Enemy>* enemyVector  = new std::vector<Enemy>;
    
protected:
    virtual void createScene(void);
    virtual void createFrameListener();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);
private:
    Ogre::SceneNode* createPathNode(Ogre::Vector3 pos);
    void createPath();
    void spawnEnemy();
    void createPathVisualizer(Ogre::Vector3 pos, bool rotation);

    ////////////////////////////////////////////////////////////////////////////////////////
    CEGUI::OgreRenderer* mRenderer;
    Ogre::RaySceneQuery* mRaySceneQuery;
    Ogre::Vector3 oldpos;
    Ogre::String selectedObject;
    int entityCount;

    CEGUI::Window* playerMoneyDisplay;
    CEGUI::Window* playerHealthDisplay;
    CEGUI::Window* youLoseGui;

    std::vector<Tower*> towers_;
    ////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////
    // OIS::KeyListener
    virtual bool keyPressed(const OIS::KeyEvent& arg);
    virtual bool keyReleased(const OIS::KeyEvent& arg);
    // OIS::MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent& arg);
    virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

    bool quit(const CEGUI::EventArgs& e);

    Ogre::MovableObject* getNode(float mouseScreenX, float mouseScreenY);
    Ogre::SceneNode* currentObjectNode;
    Tower* newTower = nullptr;

    bool createTower1();
    bool createTower2();
    bool deleteTower();

    const int tower1Cost_ = 50;
    const int tower2Cost_ = 75;
    int towerType;
    bool allowPlacing = false;

    CEGUI::Event::Connection towerButton1Connection, towerButton2Connection;
    //CEGUI::PushButton* towerButton1, * towerButton2;
    CEGUI::Window* towerButton1, * towerButton2;
    ////////////////////////////////////////////////////////////////////////////////////////

    unsigned long long playerMoney = 150;
    void addMoney(unsigned long long amt) { playerMoney += amt; }
    void subtractMoney(unsigned long long amt) {
        unsigned long long temp = playerMoney;
        playerMoney -= amt;
        if (playerMoney > temp) playerMoney = 0;
    }

    int playerHealth = 100;
    void subtractHealth(int amt) {
        playerHealth -= amt;
        if (playerHealth <= 0) {
            playerHealth = 0;
            youLoseGui->setVisible(true);
        }
    }
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
