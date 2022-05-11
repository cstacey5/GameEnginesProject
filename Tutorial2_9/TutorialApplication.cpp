/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
using namespace Ogre;
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/
#include "TutorialApplication.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
    
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
  
    //Camera
    mCamera->setPosition(200, 300, 400);
    mCamera->lookAt(Ogre::Vector3(0, 0, 0));
    mCamera->setNearClipDistance(5);
	Ogre::Viewport* vp = mWindow->getViewport(0);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
  
   
    //Create Path
    Ogre::SceneNode* pathParentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PathParentNode");
    createPath();


    //Floor
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane(
            "ground",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            plane,
            3000, 3000, 20, 20,
            true,
            1, 5, 5,
            Ogre::Vector3::UNIT_Z);

    Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);

    groundEntity->setCastShadows(false);
    groundEntity->setMaterialName("Examples/Rockwall");


    //! [lightingsset]
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
    mSceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);


    //! [directlight]
    Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);

    //! [directlightcolor]
    directionalLight->setDiffuseColour(Ogre::ColourValue(0.4, 0, 0));
    directionalLight->setSpecularColour(Ogre::ColourValue(0.4, 0, 0));

    //! [directlightdir]
    directionalLight->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Z);

    Ogre::SceneNode* directionalLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    directionalLightNode->attachObject(directionalLight);
    directionalLightNode->setDirection(Ogre::Vector3(0, -1, 1));

    //! [pointlight]
    Ogre::Light* pointLight = mSceneMgr->createLight("PointLight");
    pointLight->setType(Ogre::Light::LT_POINT);

    //! [pointlightcolor]
    pointLight->setDiffuseColour(0.3, 0.3, 0.3);
    pointLight->setSpecularColour(0.3, 0.3, 0.3);

    //! [pointlightpos]
    Ogre::SceneNode* pointLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    pointLightNode->attachObject(pointLight);
    pointLightNode->setPosition(Ogre::Vector3(0, 150, 250));

	
}
//---------------------------------------------------------------------------

void TutorialApplication::createFrameListener() {
    BaseApplication::createFrameListener();
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& fe) {
    bool ret = BaseApplication::frameRenderingQueued(fe);
    


    //recurring call to handle the path the enemy walks
    handlePath();


    //direction vector for enemy movement
    Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;
    dirVec.z += 100;
    
    //spawn enemies
    if (std::time(nullptr) - lastEnemySpawned >= 5) {
        lastEnemySpawned = std::time(nullptr);
        spawnEnemy();
    }

    //move each enemy forward and check health 
    for (auto it = enemyVector.begin(); it != enemyVector.end(); it++) {
        //enemy movement
        it->getSceneNode()->translate(
            dirVec * fe.timeSinceLastFrame,
            Ogre::Node::TS_LOCAL);
        //enemy dies if health hits 0
        if (it->getHealth() <= 0) {
            mSceneMgr->destroyEntity(it->getEntity());
            mSceneMgr->destroySceneNode(it->getSceneNode());
            enemyVector.erase(it);
        }
    }
    return ret;
}


Ogre::SceneNode* TutorialApplication::createPathNode(Ogre::Vector3 pos) {
    pathNodeI += 1;
    std::string nodeName = "PathNode" + std::to_string(pathNodeI);
    //Ogre::Entity* tempEntity = mSceneMgr->createEntity("ogrehead.mesh");
    Ogre::SceneNode* pathNode = mSceneMgr->getSceneNode("PathParentNode")->createChildSceneNode(nodeName, pos);
    //pathNode->attachObject(tempEntity);

    return pathNode;
}

void TutorialApplication::createPath() {
    std::vector<Ogre::SceneNode*> path;
    std::vector<Ogre::Degree> pathTurns;

    Ogre::SceneNode* pathNode1 = createPathNode(Ogre::Vector3(0, 100, 400));
    path.push_back(pathNode1);
    pathTurns.push_back(Ogre::Degree(90));
    defaultPathRotate.push_back(false);
    createPathVisualizer(Ogre::Vector3(0,1,200), true);

    Ogre::SceneNode* pathNode2 = createPathNode(Ogre::Vector3(400, 100, 400));
    path.push_back(pathNode2);
    pathTurns.push_back(Ogre::Degree(-90));
    defaultPathRotate.push_back(false);
    createPathVisualizer(Ogre::Vector3(200, 1, 400), false);

    Ogre::SceneNode* pathNode3 = createPathNode(Ogre::Vector3(400, 100, 800));
    path.push_back(pathNode3);
    pathTurns.push_back(Ogre::Degree(-90));
    defaultPathRotate.push_back(false);
    createPathVisualizer(Ogre::Vector3(400, 1, 600), true);

    Ogre::SceneNode* pathNode4 = createPathNode(Ogre::Vector3(0, 100, 800));
    path.push_back(pathNode4);
    pathTurns.push_back(Ogre::Degree(90));
    defaultPathRotate.push_back(false);
    createPathVisualizer(Ogre::Vector3(200, 1, 800), false);

    Ogre::SceneNode* pathNode5 = createPathNode(Ogre::Vector3(0, 100, 1200));
    path.push_back(pathNode5);
    pathTurns.push_back(Ogre::Degree(-90));
    defaultPathRotate.push_back(false);
    createPathVisualizer(Ogre::Vector3(0, 1, 1000), true);


    createdPath = path;
    createdPathTurns = pathTurns;
}

void TutorialApplication::createPathVisualizer(Ogre::Vector3 pos, bool rotation) {
    Ogre::SceneNode* pathVisualNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(pos);
    pathVisualNode->setScale(Ogre::Vector3(4.5, 0.005, 0.5));
    Ogre::Entity* pathVisualEntity = mSceneMgr->createEntity("cube.mesh");
    pathVisualNode->attachObject(pathVisualEntity);
    if (rotation) {
        pathVisualNode->yaw(Ogre::Degree(90));
    }


}
void TutorialApplication::handlePath() {

    for (auto it = enemyVector.begin(); it != enemyVector.end(); it++) {

        if (it->getSceneNode()->getPosition().distance(createdPath.at(it->getPathNodeIndex())->getPosition()) <= 1 && !it->getCurrentRotateCheck()) {
            it->getSceneNode()->yaw(createdPathTurns.at(it->getPathNodeIndex()));
            it->getSceneNode()->setPosition(createdPath.at(it->getPathNodeIndex())->getPosition());
            it->toggleCurrentRotateCheck();
            it->incrementNodeIndex();
            if (it->getPathNodeIndex() >= createdPath.size()) {
                mSceneMgr->destroyEntity(it->getEntity());
                mSceneMgr->destroySceneNode(it->getSceneNode());
                enemyVector.erase(it);
                break;
                //make player take damage (enemy won)
            }
        }
    }
}



void TutorialApplication::spawnEnemy() {
    std::string enemyName = "Enemy" + std::to_string(enemyCounter);
    enemyCounter++;
    Ogre::SceneNode* enemyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(enemyName, Ogre::Vector3(0, 100, 0));
    enemyNode->setScale(Ogre::Vector3(0.1, 0.1, 0.1));
    Ogre::Entity* enemyEntity = mSceneMgr->createEntity("sphere.mesh");
    enemyNode->attachObject(enemyEntity);
    Enemy spawnedEnemy = Enemy(enemyNode, enemyEntity, defaultPathRotate);
    enemyVector.push_back(spawnedEnemy);
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
