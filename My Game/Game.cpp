/// \file Game.cpp
/// \brief Code for the game class CGame.

#include "Game.h"

#include "GameDefines.h"
#include "Renderer.h"
#include "ComponentIncludes.h"
#include "ParticleEngine.h"

/// Delete the renderer and the object manager.
CGame::~CGame(){
  delete m_pParticleEngine;
  delete m_pRenderer;
  delete m_pObjectManager;
  delete m_pItems;
  delete m_pSkeleton;
} //destructor


/// Initialize the renderer and the object manager, load 
/// images and sounds, and begin the game.
void CGame::Initialize(){
  m_pRenderer = new CRenderer; 
  m_pRenderer->Initialize(NUM_SPRITES);  
  m_pRenderer->LoadImages(); //load images from xml file list
  m_pObjectManager = new CObjectManager; //set up object manager 
  m_pAudio->Load(); //load the sounds for this game
  m_pItems->gamePtr = this;
  m_pPlayer->gamePtr = this;
  m_pSkeleton->gamePtr = this;
  m_pObjectManager->gamePtr = this;
  m_gGame = this;

  m_pParticleEngine = new CParticleEngine2D((CSpriteRenderer*)m_pRenderer);

  BeginGame();
} //Initialize


/// Release all of the DirectX12 objects by deleting the renderer.
void CGame::Release(){
  delete m_pRenderer;
  m_pRenderer = nullptr; //for safety
} //Release


/// Ask the object manager to create the game objects.
/// Ask the object manager to create a player object
/// and a turret
void CGame::CreateObjects() {
	m_pHealthoutline = m_pObjectManager->create(HEALTHOUTLINE_SPRITE, Vector2(675.0f, 0.0f) + healthoffset);
	m_pHealthbar = m_pObjectManager->create(HEALTHBAR_SPRITE, Vector2(675.0f, 0.0f) + healthoffset );
	
		
	
	if(intro)
		m_pLoadingscreen = m_pObjectManager->create(LOADGAME_SPRITE, m_pPlayer->GetPos());

	//this way works better
	if (m_nCurLevel == 0) {

		m_pObjectManager->create(TUTORIAL_1_SPRITE, Vector2(150.0f, 150.0f)); //use wasd to move
		m_pObjectManager->create(TUTORIAL_2_SPRITE, Vector2(800.0f, 300.0f)); //use L and R click to attack
		m_pObjectManager->create(TUTORIAL_3_SPRITE, Vector2(150.0f, 1200.0f)); //use E to pick up items
		m_pObjectManager->create(TUTORIAL_4_SPRITE, Vector2(800.0f, 1200.0f)); //use I to access your inventory
		m_pObjectManager->create(TUTORIAL_5_SPRITE, Vector2(150.0f, 2100.0f)); //Its dangerous to go alone, take this!
		m_pObjectManager->create(STARTTEXT_SPRITE, Vector2(480.0f, 2500.0f)); //Start
		

		m_pObjectManager->create(SWORDGROUND_SPRITE, Vector2(480.0f, 2000.0f)); //create sword
		m_pObjectManager->create(HEALTH_POTION_GROUND_SPRITE, Vector2(480.0f, 1200.0f)); //create health potion

		m_pAudio->play(INTRO_SOUND);
	}
	if (m_nCurLevel == 1) {

		m_pAudio->stop();
		m_pAudio->play(LEVEL1_SOUND);
		m_pObjectManager->create(CHEST_SPRITE, Vector2(1400.0f, 995.0f)); //Spawns chest

	}		

	if (m_nCurLevel == 2) {

		m_pObjectManager->create(CHEST_SPRITE, Vector2(500.0f, 1375.0f)); //Spawns chest

	}

	if (m_nCurLevel == 3) {

		m_pObjectManager->create(CROSSGROUND_SPRITE, Vector2(1100.0f, 1715.0f));
		m_pObjectManager->create(CHEST_SPRITE, Vector2(500.0f, 1050.0f)); //Spawns chest

	}
	if (m_nCurLevel == 4) {
		m_pObjectManager->create(CHEST_SPRITE, Vector2(300.0f, 770.0f)); //Spawns chest
		m_pObjectManager->create(CHEST_SPRITE, Vector2(225.0f, 775.0f)); //Spawns chest
		m_pObjectManager->create(CHEST_SPRITE, Vector2(175.0f, 775.0f)); //Spawns chest

	}
	if (m_nCurLevel == 6) {
		m_pBossHealthbar = m_pObjectManager->create(BOSSHEALTH_SPRITE, Vector2(512.0f, 300.0f) + healthoffset);

	}
} //CreateObjects



/// Call this function to start a new game. This
/// should be re-entrant so that you can restart a
/// new game without having to shut down and restart the
/// program.
void CGame::BeginGame() {
	m_pParticleEngine->clear(); //clear old particles
	m_pObjectManager->clear(); //clear old objects
	m_pObjectManager->LoadMap(m_nCurLevel); //load map
	CreateObjects(); //create new objects 
} //BeginGame


/// Increment the level counter and restart the game.
void CGame::NextLevel() {
	m_bNextLevel = false;
	m_nCurLevel = (m_nCurLevel + 1) % 7;
	//if (m_nCurLevel >= 6)// change level number here if more levels added
		//m_bGameWin = true;

	BeginGame();
} //NextLevel


/// Poll the mouse and react to 
/// button presses and the location of the cursor
void CGame::MouseHandler() {
	///SET CURSOR APPEARANCE///
	Cursor = LoadCursor(NULL, IDC_CROSS);
	SetCursor(Cursor);

	///MOUSE POINTER///
	//get the location of the mouse cursor relative to the top left corner of the screen
	POINT mouseCursor;
	hwnd = GetActiveWindow();
	GetCursorPos(&mouseCursor);
	ScreenToClient(hwnd, &mouseCursor);

	//calculate angle from player to cursor, and make player look in direction
	Vector2 mousePoint((float)mouseCursor.x, (float)(m_nWinHeight - mouseCursor.y));
	float mouseAngle = (float)(atan2(mousePoint.y - m_nWinHeight / 2.0f, mousePoint.x - m_nWinWidth / 2.0f) - M_PI / 2);
	m_pPlayer->SetOrientation(mouseAngle);

	///MOUSE BUTTONS///
	if (m_pKeyboard->TriggerDown(VK_LBUTTON)) {
		m_pPlayer->PrimaryAction();
	}

	if (m_pKeyboard->TriggerDown(VK_RBUTTON)) {
		m_pPlayer->SecondaryAction();
	}

}//MouseHandler


/// Poll the keyboard state and respond to the
/// key presses that happened since the last frame.
void CGame::KeyboardHandler() {
	m_pKeyboard->GetState(); //get current keyboard state 

	///MOVE FORWARD///
	if (m_pKeyboard->Down('W')) {
		m_pPlayer->SetSpeed(200.0f);
		m_pPlayer->StrafeForward();
	}

	if (m_pKeyboard->TriggerUp('W'))
		m_pPlayer->SetSpeed(0.0f);

	///MOVE BACK///
	if (m_pKeyboard->Down('S')) {
		m_pPlayer->SetSpeed(-200.0f);
		m_pPlayer->StrafeBack();
	}

	if (m_pKeyboard->TriggerUp('S'))
		m_pPlayer->SetSpeed(0.0f);

	///MOVE RIGHT///
	if (m_pKeyboard->Down('D')) {
		m_pPlayer->StrafeRight();
		m_pPlayer->SetOrthoSpeed(200.0f);
	}

	if (m_pKeyboard->TriggerUp('D'))
		m_pPlayer->SetOrthoSpeed(0.0f);

	///MOVE LEFT///
	if (m_pKeyboard->Down('A')) {
		m_pPlayer->StrafeLeft();
		m_pPlayer->SetOrthoSpeed(-200.0f);
	}
	if(m_pKeyboard->TriggerUp('A'))
		m_pPlayer->SetOrthoSpeed(0.0f);

	if (m_pKeyboard->TriggerDown('I')) // keyboard check to go into inventory
	{
		if (gameScreen == MAIN_GAME)
		{
			gameScreen = INVENTORY;
		}
		else
		{
			gameScreen = MAIN_GAME;
		}
	}

	if(m_bDevMode) { //controls that are limited to dev mode

		if (m_pKeyboard->TriggerDown(VK_F1)) //draw bounding boxes
			m_bDrawAABBs = !m_bDrawAABBs;

		if (m_pKeyboard->TriggerDown(VK_F2)) //force load the next level
			NextLevel();

		if (m_pKeyboard->TriggerDown(VK_F3)) //toggle between absolute and relative controls
			m_bAbsoluteControls = !m_bAbsoluteControls;

		if (m_pKeyboard->TriggerDown(VK_F4)) //simulate player death
			m_pPlayer->kill();

		if (m_pKeyboard->TriggerDown(VK_F5)) // pretty much god mode
			m_pPlayer->health = 1000;
		

	}//if(m_bDevMode)

} //KeyboardHandler


/// Draw the current frame rate to a hard-coded position in the window.
void CGame::DrawFrameRateText(){
  const string s = to_string(m_pStepTimer->GetFramesPerSecond()) + " fps";
  const Vector2 pos(m_nWinWidth - 128.0f, 30.0f);
  m_pRenderer->DrawScreenText(s.c_str(), pos);
} //DrawFrateRateText


/// Ask the object manager to draw the game objects. RenderWorld
/// is notified of the start and end of the frame so
/// that it can let Direct3D do its pipelining jiggery-pokery.
void CGame::RenderFrame() {
	m_pRenderer->BeginFrame();
	m_pRenderer->Draw(BACKGROUND_SPRITE, m_pPlayer->GetPos());

	m_pObjectManager->draw();


	if(m_nCurLevel == 6)
		m_pRenderer->Draw(*(CSpriteDesc2D*)m_pBossHealthbar);

	if (m_bPlayerDeath) {
		m_pRenderer->Draw(DEATHSCREEN_SPRITE, m_pPlayer->GetPos());
	}

	if (intro)  
		m_pRenderer->Draw(*(CSpriteDesc2D*)m_pLoadingscreen);
	
	if (m_bGameWin) {
		m_pRenderer->Draw(WINSCREEN_SPRITE, m_pPlayer->GetPos());
		//m_pRenderer->DrawCenteredText("You Win", Colors::White);
		//m_pRenderer->DrawScreenText("Unlocked: Spreadshot", Vector2(375.0f, 600.0f), Colors::Gold);
		//m_pRenderer->DrawScreenText("Unlocked: Double Health", Vector2(350.0f, 650.0f), Colors::Gold);
	}

		//	m_pRenderer->Draw(LOADGAME_SPRITE, m_pPlayer->GetPos());
	if (gameScreen == INVENTORY) // pulls up inventory
	{
		Inventory();
	}
	//m_pParticleEngine->Draw(); //moved to m_pObjectManager->draw();
	m_pRenderer->EndFrame();
} //RenderFrame


/// Make the camera follow the player
void CGame::FollowCamera() {
	Vector3 vCameraPos(m_pPlayer->GetPos()); //player position

	/*
	if (m_vWorldSize.x > m_nWinWidth) { //world wider than screen
		vCameraPos.x = max(vCameraPos.x, m_nWinWidth / 2.0f); //stay away from the left edge
		vCameraPos.x = min(vCameraPos.x, m_vWorldSize.x - m_nWinWidth / 2.0f);  //stay away from the right edge
	} //if
	else vCameraPos.x = m_vWorldSize.x / 2.0f; //center horizontally.

	if (m_vWorldSize.y > m_nWinHeight) { //world higher than screen
		vCameraPos.y = max(vCameraPos.y, m_nWinHeight / 2.0f);  //stay away from the bottom edge
		vCameraPos.y = min(vCameraPos.y, m_vWorldSize.y - m_nWinHeight / 2.0f); //stay away from the top edge
	} //if
	else vCameraPos.y = m_vWorldSize.y / 2.0f; //center vertically
	*/

	m_pRenderer->SetCameraPos(vCameraPos); //camera to player
} //FollowCamera

void CGame::IntroSequence() {
	
	t = m_pStepTimer->GetElapsedSeconds();
	total += t;


	if (m_pLoadingscreen->m_nCurrentFrame == 0 && total > 2) {
		m_pLoadingscreen->m_nCurrentFrame++;
		total = 0.0f;
	}
	
	if (total > .12 && downblink && m_pLoadingscreen->m_nCurrentFrame != 0) {
		m_pLoadingscreen->m_nCurrentFrame++;
		total = 0.0f;
	}

	if (total > .12 && upblink) {
		m_pLoadingscreen->m_nCurrentFrame--;
		total = 0.0f;
	}

	if (m_pLoadingscreen->m_nCurrentFrame == 0 && upblink == true) {
		downblink = true;
		upblink = false;
		blinks++;

	}
	if (m_pLoadingscreen->m_nCurrentFrame == 3 && upblink == false) {
		downblink = false;
		upblink = true;

	}

	if (blinks == 3) {
		m_pLoadingscreen->m_bDead = true;
		intro = false;

	}

}

void CGame::Deathsequence() {


	if (justDied) {
		m_pAudio->stop();
		m_nCurLevel = 0;
		BeginGame();
		m_pAudio->play(DEMONLAUGH_SOUND);
		justDied = false;
	}
	t = m_pStepTimer->GetElapsedSeconds();
	total += t;

	if (total > 4) {
		m_bPlayerDeath = false;
		justDied = true;
		total = 0;
	}
}

void CGame::WinSequence() {

	if (m_bGameReset)
	{
		
		m_nCurLevel = 0;
		BeginGame();
		m_pAudio->stop();
		m_pAudio->play(WINSCREEN_SOUND);
		m_bGameReset = false;
		m_pObjectManager->m_bPlayerSpreadShot = true;
		m_pPlayer->healthMax = 10;
		m_pPlayer->health += m_pPlayer->healthMax;
	}
	
	t = m_pStepTimer->GetElapsedSeconds();
	total += t;

	if (total > 12.0f) {
		m_pAudio->stop();
		m_pAudio->play(INTRO_SOUND);
		m_bGameWin = false;
		m_bGameReset = true;
		total = 0;
	}
}

/// Handle keyboard input, move the game objects and render 
/// them in their new positions and orientations. Notify the 
/// audio player at the start of each frame so that it can 
/// prevent multiple copies of a sound from starting on the
/// same frame. Notify the timer of the start and end of the
/// frame so that it can calculate frame time. 
void CGame::ProcessFrame() {
	
	if (intro) {
		IntroSequence();
	}
	else if (m_bNextLevel)
	{
		NextLevel(); // if next level triggered through player interaction with door
		//m_bNextLevel = false;
	}
	else if (m_bPlayerDeath)
	{
		Deathsequence();
	}

	else if (m_bGameWin)
	{
		WinSequence();
	}
	
	else{
		MouseHandler(); //handle mouse input
		KeyboardHandler(); //handle keyboard input
	}

	m_pAudio->BeginFrame(); //notify audio player that frame has begun

	m_pStepTimer->Tick([&]() {
		m_pObjectManager->move(); //move all objects
		FollowCamera(); //make camera follow player
		m_pParticleEngine->step(); //advance particle animation
	});

	RenderFrame(); //render a frame of animation
	

} //ProcessFrame

bool CGame::InsertWeapon(int item) // inserts item into inventory
{
	if (m_listWeapon.size() < 2) { // max number of items that can be held
		m_listWeapon.push_back(item);
		return true;
	}
	else {
		return false; // item not inserted
	}
}

bool CGame::RemoveWeapon(int item) // removes used item from inventory
{
	if (item != NULL)
	{
		m_listWeapon.erase(find(m_listWeapon.begin(), m_listWeapon.end(), item));
		return true;
	}
	else
	{
		return false; // item not removed
	}
}

bool CGame::CheckWeapon(int item) // checks if a certain item is in inventory
{
	if (item != NULL)
	{
		return find(m_listWeapon.begin(), m_listWeapon.end(), item) != m_listWeapon.end();
	}
	else
	{
		return false;
	}
}

bool CGame::InsertItem(int item) // inserts item into inventory
{
	if (m_listItems.size() < 6) { // max number of items that can be held
		m_listItems.push_back(item);
		return true;
	}
	else {
		return false; // item not inserted
	}
}

bool CGame::RemoveItem(int item) // removes used item from inventory
{
	if (item != NULL)
	{
		m_listItems.erase(find(m_listItems.begin(), m_listItems.end(), item));
		return true;
	}
	else
	{
		return false; // item not removed
	}
}

bool CGame::CheckItem(int item) // checks if a certain item is in inventory
{
	if (item != NULL)
	{
		return find(m_listItems.begin(), m_listItems.end(), item) != m_listItems.end();
	}
	else
	{
		return false;
	}
}

void CGame::Inventory()
{
	m_pRenderer->Draw(BACKGROUND_SPRITE, m_pPlayer->GetPos());
	m_pRenderer->DrawScreenText("INVENTORY", Vector2(425.0f, 70.0f), Colors::White);
	CPlayer* z = static_cast<CPlayer*> (m_pPlayer);
	const string s1 = to_string(z->health) + " / ";
	const string s2 = to_string(z->healthMax) + " HP";
	const string s3 = "Dungeon Level " + to_string(m_nCurLevel + 1);
	const Vector2 pos1(m_nWinWidth - 138.0f, 30.0f);
	const Vector2 pos2(m_nWinWidth - 85.0f, 30.0f);
	const Vector2 pos3(10.0f, 30.0f);

	m_pRenderer->DrawScreenText(s1.c_str(), pos1, Colors::White);
	m_pRenderer->DrawScreenText(s2.c_str(), pos2, Colors::White);
	m_pRenderer->DrawScreenText(s3.c_str(), pos3, Colors::White);

	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-250.0f, 150.0f), m_pPlayer->GetPos() + Vector2(-150.0f, 150.0f)); // first item slot
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-150.0f, 150.0f), m_pPlayer->GetPos() + Vector2(-150.0f, 50.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-150.0f, 50.0f), m_pPlayer->GetPos() + Vector2(-250.0f, 50.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-250.0f, 50.0f), m_pPlayer->GetPos() + Vector2(-250.0f, 150.0f));

	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-50.0f, 150.0f), m_pPlayer->GetPos() + Vector2(50.0f, 150.0f)); // second item slot
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(50.0f, 150.0f), m_pPlayer->GetPos() + Vector2(50.0f, 50.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(50.0f, 50.0f), m_pPlayer->GetPos() + Vector2(-50.0f, 50.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-50.0f, 50.0f), m_pPlayer->GetPos() + Vector2(-50.0f, 150.0f));

	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(150.0f, 150.0f), m_pPlayer->GetPos() + Vector2(250.0f, 150.0f)); // third item slot
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(250.0f, 150.0f), m_pPlayer->GetPos() + Vector2(250.0f, 50.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(250.0f, 50.0f), m_pPlayer->GetPos() + Vector2(150.0f, 50.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(150.0f, 50.0f), m_pPlayer->GetPos() + Vector2(150.0f, 150.0f));

	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-250.0f, -50.0f), m_pPlayer->GetPos() + Vector2(-150.0f, -50.0f)); // fourth item slot
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-150.0f, -50.0f), m_pPlayer->GetPos() + Vector2(-150.0f, -150.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-150.0f, -150.0f), m_pPlayer->GetPos() + Vector2(-250.0f, -150.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-250.0f, -150.0f), m_pPlayer->GetPos() + Vector2(-250.0f, -50.0f));

	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-50.0f, -50.0f), m_pPlayer->GetPos() + Vector2(50.0f, -50.0f)); // fifth item slot
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(50.0f, -50.0f), m_pPlayer->GetPos() + Vector2(50.0f, -150.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(50.0f, -150.0f), m_pPlayer->GetPos() + Vector2(-50.0f, -150.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(-50.0f, -150.0f), m_pPlayer->GetPos() + Vector2(-50.0f, -50.0f));

	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(150.0f, -50.0f), m_pPlayer->GetPos() + Vector2(250.0f, -50.0f)); // sixth item slot
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(250.0f, -50.0f), m_pPlayer->GetPos() + Vector2(250.0f, -150.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(250.0f, -150.0f), m_pPlayer->GetPos() + Vector2(150.0f, -150.0f));
	m_pRenderer->DrawLine(WHITELINE_SPRITE, m_pPlayer->GetPos() + Vector2(150.0f, -150.0f), m_pPlayer->GetPos() + Vector2(150.0f, -50.0f));

	if (m_pKeyboard->TriggerDown(VK_LEFT))
	{
		if (cursorX == 0)
		{
			cursorX = 2;
		}
		else
		{
			cursorX--;
		}
	}
	if (m_pKeyboard->TriggerDown(VK_RIGHT))
	{
		if (cursorX == 2)
		{
			cursorX = 0;
		}
		else
		{
			cursorX++;
		}
	}
	if (m_pKeyboard->TriggerDown(VK_DOWN)) // reversed because list goes top left to bottom right
	{
		if (cursorY == 1)
		{
			cursorY = 0;
		}
		else
		{
			cursorY++;
		}
	}
	if (m_pKeyboard->TriggerDown(VK_UP)) // reversed because list goes top left to bottom right
	{
		if (cursorY == 0)
		{
			cursorY = 1;
		}
		else
		{
			cursorY--;
		}
	}

	if (cursorX == 0 && cursorY == 0) // highlight first item slot 
	{
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-250.0f, 150.0f), m_pPlayer->GetPos() + Vector2(-150.0f, 150.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-150.0f, 150.0f), m_pPlayer->GetPos() + Vector2(-150.0f, 50.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-150.0f, 50.0f), m_pPlayer->GetPos() + Vector2(-250.0f, 50.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-250.0f, 50.0f), m_pPlayer->GetPos() + Vector2(-250.0f, 150.0f));
	}

	if (cursorX == 1 && cursorY == 0) // highlight second item slot
	{
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-50.0f, 150.0f), m_pPlayer->GetPos() + Vector2(50.0f, 150.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(50.0f, 150.0f), m_pPlayer->GetPos() + Vector2(50.0f, 50.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(50.0f, 50.0f), m_pPlayer->GetPos() + Vector2(-50.0f, 50.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-50.0f, 50.0f), m_pPlayer->GetPos() + Vector2(-50.0f, 150.0f));
	}

	if (cursorX == 2 && cursorY == 0) // highlight third item slot
	{
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(150.0f, 150.0f), m_pPlayer->GetPos() + Vector2(250.0f, 150.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(250.0f, 150.0f), m_pPlayer->GetPos() + Vector2(250.0f, 50.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(250.0f, 50.0f), m_pPlayer->GetPos() + Vector2(150.0f, 50.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(150.0f, 50.0f), m_pPlayer->GetPos() + Vector2(150.0f, 150.0f));
	}

	if (cursorX == 0 && cursorY == 1) // highlight fourth item slot
	{
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-250.0f, -50.0f), m_pPlayer->GetPos() + Vector2(-150.0f, -50.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-150.0f, -50.0f), m_pPlayer->GetPos() + Vector2(-150.0f, -150.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-150.0f, -150.0f), m_pPlayer->GetPos() + Vector2(-250.0f, -150.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-250.0f, -150.0f), m_pPlayer->GetPos() + Vector2(-250.0f, -50.0f));
	}

	if (cursorX == 1 && cursorY == 1) // highlight fifth item slot
	{
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-50.0f, -50.0f), m_pPlayer->GetPos() + Vector2(50.0f, -50.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(50.0f, -50.0f), m_pPlayer->GetPos() + Vector2(50.0f, -150.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(50.0f, -150.0f), m_pPlayer->GetPos() + Vector2(-50.0f, -150.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(-50.0f, -150.0f), m_pPlayer->GetPos() + Vector2(-50.0f, -50.0f));
	}

	if (cursorX == 2 && cursorY == 1) // highlight sixth item slot
	{
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(150.0f, -50.0f), m_pPlayer->GetPos() + Vector2(250.0f, -50.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(250.0f, -50.0f), m_pPlayer->GetPos() + Vector2(250.0f, -150.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(250.0f, -150.0f), m_pPlayer->GetPos() + Vector2(150.0f, -150.0f));
		m_pRenderer->DrawLine(REDLINE_SPRITE, m_pPlayer->GetPos() + Vector2(150.0f, -150.0f), m_pPlayer->GetPos() + Vector2(150.0f, -50.0f));
	}

	int selectedHealthX = 4;
	int selectedHealthY = 4;
	int selectedSpeedX = 4;
	int selectedSpeedY = 4;

	//bool selectedSpeedBoost = false;
	int i = 0;

	for (auto& items : m_listItems) // interate through items and draw to screen at correct placements
	{
		int x = i % 3;
		int y = i / 3;
		i++;

		if (items == 1)
		{
			m_pRenderer->Draw(HEALTH_POTION_INVENTORY_SPRITE, m_pPlayer->GetPos() + Vector2(-200.0f + x * 200.0f, 100.0f + y * -200.0f));
			if (cursorX == x && cursorY == y)
			{
				selectedHealthX = x;
				selectedHealthY = y;
			}
		}
		if (items == 2)
		{
			m_pRenderer->Draw(SPEED_POTION_INVENTORY_SPRITE, m_pPlayer->GetPos() + Vector2(-200.0f + x * 200.0f, 100.0f + y * - 200.0f));
			if (cursorX == x && cursorY == y)
			{
				selectedSpeedX = x;
				selectedSpeedY = y;
			}
		}
	}
	
	if (m_pKeyboard->TriggerDown(VK_RETURN) && cursorX == selectedHealthX && cursorY == selectedHealthY && (cursorX != selectedSpeedX && cursorY != selectedSpeedY)) {
		if (CheckItem(1)) // Health potion
		{
			m_pItems->UseHealthPotion(m_pPlayer);
			RemoveItem(1);
		}
	}
	if (m_pKeyboard->TriggerDown(VK_RETURN) && cursorX == selectedSpeedX && cursorY == selectedSpeedY) {
		if (CheckItem(2)) // Attack speed potion
		{
			m_pItems->UseSpeedPotion(m_pPlayer);
			RemoveItem(2);
		}
	}

	

	for (auto& weapon : m_listWeapon) // interate through items and draw to screen at correct placements
	{
		int x = i % 3;
		int y = i / 3;
		i++;
		
		if (weapon == 1)
		{
			m_pRenderer->Draw(SWORDINVENTORY_SPRITE, m_pPlayer->GetPos() + Vector2(-400.0f, 25.0f));
		}
		if (weapon == 2)
		{
			m_pRenderer->Draw(CROSSINVENTORY_SPRITE, m_pPlayer->GetPos() + Vector2(400.0f, 0.0f));
		}
		if (weapon == 3)
		{
			m_pRenderer->Draw(LARGEFIREBALL_SPRITE, m_pPlayer->GetPos() + Vector2(-200.0f + x * 200.0f, 100.0f + y * -200.0f));
		}
	}
}