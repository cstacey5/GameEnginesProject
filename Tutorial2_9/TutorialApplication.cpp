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
    ////////////////////////////////////////////////////////////////////////////////////////
    // GUI Stuffs
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::ImageManager::getSingleton().loadImageset("TaharezLook.imageset");
    CEGUI::ImageManager::getSingleton().loadImageset("Buttons.imageset");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

    CEGUI::Window* quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
    quit->setText("Quit");
    quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    sheet->addChild(quit);

    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::quit, this));

    // TOWER SELECTION GUI

    CEGUI::FrameWindow* selectionPanel = static_cast<CEGUI::FrameWindow*>(wmgr.createWindow("TaharezLook/FrameWindow", "TDGame/TowerSelectionPane"));
    selectionPanel->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(1, 0)));
    selectionPanel->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85, 0), CEGUI::UDim(0, 0)));
    selectionPanel->setCloseButtonEnabled(false);
    selectionPanel->setVisible(true);

    playerMoneyDisplay = wmgr.createWindow("TaharezLook/StaticText", "TDGame/PlayerMoneyDisplay");
    playerMoneyDisplay->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(0.20, 0)));
    playerMoneyDisplay->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0.05, 0)));
    playerMoneyDisplay->setText("   Current Money:\n    $ 0");
    playerMoneyDisplay->setProperty("HorzFormatting", "WordWrapCentred");
    selectionPanel->addChild(playerMoneyDisplay);

    towerButton1 = static_cast<CEGUI::PushButton*>(wmgr.createWindow("TaharezLook/ImageButton", "tower1Button"));
    towerButton1->setSize(CEGUI::USize(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.30, 0)));
    towerButton1->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.30, 0)));
    towerButton1->setProperty("NormalImage", "Buttons/Tower1Normal");
    towerButton1->setProperty("HoverImage", "Buttons/Tower1Hover");
    towerButton1->setProperty("PushedImage", "Buttons/Tower1Pressed");
    towerButton1Connection = towerButton1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::createTower1, this));
    selectionPanel->addChild(towerButton1);

    towerButton2 = static_cast<CEGUI::PushButton*>(wmgr.createWindow("TaharezLook/ImageButton", "tower2Button"));
    towerButton2->setSize(CEGUI::USize(CEGUI::UDim(0.80, 0), CEGUI::UDim(0.30, 0)));
    towerButton2->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.65, 0)));
    towerButton2->setProperty("NormalImage", "Buttons/Tower2Normal");
    towerButton2->setProperty("HoverImage", "Buttons/Tower2Hover");
    towerButton2->setProperty("PushedImage", "Buttons/Tower2Pressed");
    towerButton2Connection = towerButton2->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::createTower2, this));
    selectionPanel->addChild(towerButton2);

    // ADD SELECTION PANE TO MAIN SHEET
    sheet->addChild(selectionPanel);

    // DUMMY PLANE FOR RAYCAST
    Ogre::MovablePlane* mPlane = new Ogre::MovablePlane("dummy_plane_z");
    mPlane->normal = Ogre::Vector3::UNIT_X;
    Ogre::MeshManager::getSingleton().createPlane("dummy_plane_z", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, *mPlane, 800, 800, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);

    mSceneMgr->createEntity("dummy_plane_z", "dummy_plane_z");
    mSceneMgr->getEntity("dummy_plane_z")->setVisible(false);
    mSceneMgr->getEntity("dummy_plane_z")->setMaterialName("dummy_plane");

    // RAYQUERY
    mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
    ////////////////////////////////////////////////////////////////////////////////////////
  
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
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& fe) {

    playerMoneyDisplay->setText("Current Money:\n\n$ " + std::to_string(playerMoney));

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
    CEGUI::System::getSingleton().injectTimePulse(fe.timeSinceLastFrame);

    //recurring call to handle the path the enemy walks
    handlePath();

    if (mWindow->isClosed())
        return false;

    if (mShutDown)
        return false;


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
    return true;
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


////////////////////////////////////////////////////////////////////////////////////////
bool TutorialApplication::keyPressed(const OIS::KeyEvent& arg)
{
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);

    if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
    else if (arg.key == OIS::KC_UP) addMoney(50);
    else if (arg.key == OIS::KC_DOWN) subtractMoney(25);

    mCameraMan->injectKeyDown(arg);
    return true;
}

bool TutorialApplication::keyReleased(const OIS::KeyEvent& arg)
{
    //if (CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key)) return true;
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    mCameraMan->injectKeyUp(arg);
    return true;
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
        break;

    case OIS::MB_Right:
        return CEGUI::RightButton;
        break;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
        break;

    default:
        return CEGUI::LeftButton;
        break;
    }
}

bool TutorialApplication::mouseMoved(const OIS::MouseEvent& arg)
{
    if (CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel)) {}
    CEGUI::System& sys = CEGUI::System::getSingleton();
    if (arg.state.Z.rel)
        sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);

    // Make entity follow cursor
    if (selectedObject != "") {
        CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
        arg.state.width = mRoot->getAutoCreatedWindow()->getWidth();
        arg.state.height = mRoot->getAutoCreatedWindow()->getHeight();
        Ogre::Ray mouseRay = mCameraMan->getCamera()->getCameraToViewportRay(mousePos.d_x / arg.state.width, mousePos.d_y / arg.state.height);
        mRaySceneQuery->setRay(mouseRay);
        mRaySceneQuery->setSortByDistance(true);
        Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();

        Ogre::MovableObject* closestObject = nullptr;
        Ogre::Real closestDistance = 100000;

        Ogre::RaySceneQueryResult::iterator rayIterator;

        for (rayIterator = result.begin(); rayIterator != result.end(); rayIterator++)
        {
            if ((*rayIterator).movable != nullptr && closestDistance > (*rayIterator).distance && (*rayIterator).movable->getMovableType() != "TerrainMipMap")
            {

                closestObject = (*rayIterator).movable;

                Ogre::SceneNode* entity = mSceneMgr->getSceneNode(selectedObject);

                Ogre::Vector3 newpos = mouseRay.getPoint((*rayIterator).distance);

                entity->translate(newpos.x - oldpos.x, entity->getPosition().y, newpos.z - oldpos.z);
                oldpos = newpos;
            }
        }
    }
    //mCameraMan->injectMouseMove(arg);
    return true;
}

bool TutorialApplication::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
    if (CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id))) {}
    else mCameraMan->injectMouseDown(arg, id);

    //// Select entity (if applicable)
    //if (id == OIS::MB_Left && selectedObject == "") {
    //    CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
    //    arg.state.width = mRoot->getAutoCreatedWindow()->getWidth();
    //    arg.state.height = mRoot->getAutoCreatedWindow()->getHeight();
    //    Ogre::MovableObject* nodeM = getNode(mousePos.d_x / arg.state.width, mousePos.d_y / arg.state.height);
    //    if (nodeM != nullptr)
    //    {
    //        Ogre::String name = nodeM->getParentSceneNode()->getName();
    //        if (name.find("Moveable") != -1) {
    //            selectedObject = name;
    //            nodeM->getParentSceneNode()->attachObject(mSceneMgr->getEntity("dummy_plane_z"));
    //        }
    //    }
    //}
    if (id == OIS::MB_Left && allowPlacing) {
        selectedObject = "";
        allowPlacing = false;
    }
    return true;
}

bool TutorialApplication::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
    try
    {
        mSceneMgr->getSceneNode(selectedObject)->detachObject("dummy_plane_z");
    }
    catch (...) {}
    if (CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id))) {
        if (allowPlacing) {
            towerButton1Connection->disconnect();
            towerButton2Connection->disconnect();
            towerButton1Connection = towerButton1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::deleteTower, this));
            towerButton2Connection = towerButton2->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::deleteTower, this));
        }
        else {
            towerButton1Connection->disconnect();
            towerButton2Connection->disconnect();
            towerButton1Connection = towerButton1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::createTower1, this));
            towerButton2Connection = towerButton2->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::createTower2, this));
        }
        return true;
    }
    mCameraMan->injectMouseUp(arg, id);
    return true;
}

bool TutorialApplication::quit(const CEGUI::EventArgs& e)
{
    mShutDown = true;
    return true;
}

// For picking the node to be dragged (may not be required)
Ogre::MovableObject* TutorialApplication::getNode(float mouseScreenX, float mouseScreenY)
{
    Ogre::Ray mouseRay = mCameraMan->getCamera()->getCameraToViewportRay(mouseScreenX, mouseScreenY);
    mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);
    Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();

    Ogre::MovableObject* closestObject = nullptr;
    Ogre::Real closestDistance = 100000;

    Ogre::RaySceneQueryResult::iterator rayIterator;

    for (rayIterator = result.begin(); rayIterator != result.end(); rayIterator++)
    {
        if ((*rayIterator).movable != nullptr && closestDistance > (*rayIterator).distance && (*rayIterator).movable->getMovableType() != "TerrainMipMap")
        {
            closestObject = (*rayIterator).movable;
            closestDistance = (*rayIterator).distance;
            oldpos = mouseRay.getPoint((*rayIterator).distance);
            // originalPos = oldpos;
        }
    }

    return closestObject;
}

bool TutorialApplication::createTower1() {

    // Handle money
    if (playerMoney < tower1Cost_) return true;
    else subtractMoney(tower1Cost_);

    Ogre::Entity* towerEnt = mSceneMgr->createEntity("ninja.mesh");
    towerEnt->setCastShadows(true);

    CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
    Ogre::Ray mouseRay = mCameraMan->getCamera()->getCameraToViewportRay(mousePos.d_x / mRoot->getAutoCreatedWindow()->getWidth(), mousePos.d_y / mRoot->getAutoCreatedWindow()->getHeight());
    mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);
    Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator rayIterator = --result.end();
    Ogre::Vector3 pos = mouseRay.getPoint((*rayIterator).distance);
    oldpos = pos;
    towerType = 1;
    allowPlacing = true;

    selectedObject = "Tower1Node" + Ogre::StringConverter::toString(entityCount++) + ":Moveable"; // Name of the tower node (REQUIRED FOR MOUSE TRACKING)
    currentObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(selectedObject, pos); // Node of the tower (REQUIRED FOR MOUSE TRACKING)

    currentObjectNode->attachObject(towerEnt);

    return true;
}

bool TutorialApplication::createTower2() {

    // Handle money
    if (playerMoney < tower2Cost_) return true;
    else subtractMoney(tower2Cost_);

    Ogre::Entity* towerEnt = mSceneMgr->createEntity("penguin.mesh");
    towerEnt->setCastShadows(true);

    CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
    Ogre::Ray mouseRay = mCameraMan->getCamera()->getCameraToViewportRay(mousePos.d_x / mRoot->getAutoCreatedWindow()->getWidth(), mousePos.d_y / mRoot->getAutoCreatedWindow()->getHeight());
    mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);
    Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator rayIterator = --result.end();
    Ogre::Vector3 pos = mouseRay.getPoint((*rayIterator).distance);
    oldpos = pos;
    towerType = 2;
    allowPlacing = true;

    selectedObject = "Tower2Node" + Ogre::StringConverter::toString(entityCount++) + ":Moveable"; // Name of the tower node (REQUIRED FOR MOUSE TRACKING)
    currentObjectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(selectedObject, pos); // Node of the tower (REQUIRED FOR MOUSE TRACKING)

    currentObjectNode->attachObject(towerEnt);

    return true;
}

bool TutorialApplication::deleteTower() {
    if (towerType == 1) addMoney(tower1Cost_);
    else if (towerType == 2) addMoney(tower2Cost_);
    selectedObject = "";
    Ogre::Entity* tempEnt = static_cast<Ogre::Entity*>(currentObjectNode->getAttachedObject(0));
    currentObjectNode->detachAllObjects();
    mSceneMgr->destroyEntity(tempEnt);
    mSceneMgr->destroySceneNode(currentObjectNode);
    towerType = 0;
    allowPlacing = false;
    return true;
}
////////////////////////////////////////////////////////////////////////////////////////

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
