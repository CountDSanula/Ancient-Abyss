/// \file Game.h
/// \brief Interface for the game class CGame.

#pragma once

#include "Component.h"
#include "Common.h"
#include "ObjectManager.h"
#include "Settings.h"

/// \brief The game class.

class CGame: 
  public CComponent, 
  public CSettings,
  public CCommon{ 

  private:

	HWND hwnd;
	HCURSOR Cursor;

	list<int> m_listItems; // inventory items
	list<int> m_listWeapon; // inventory items
    bool m_bDrawFrameRate = false; ///< Draw the frame rate.
	bool justDied = true;
	bool m_bGameReset = true;
	int m_nCurLevel = 0;
	int cursorX = 0;
	int cursorY = 0;

	

	float total = 0.0f; // hold total time elapsed for inital sequence
	float t = 0.0f; // hold time

	enum // game screens
	{
		MAIN_GAME,
		INVENTORY,
	};

	int gameScreen = MAIN_GAME;

    void BeginGame(); ///< Begin playing the game.

	bool intro = true; ///< boolean flag to tell if were in intro sequence
	bool upblink = false; ///< used for starting animation
	bool downblink = true; ///< used for starting animation
	int blinks = 0;
	void MouseHandler(); ///< The mouse handler.
    void KeyboardHandler(); ///< The keyboard handler.

    void RenderFrame(); ///< Render an animation frame.
    void DrawFrameRateText(); ///< Draw frame rate text to screen. //might not need
    void CreateObjects(); ///< Create game objects.
	void FollowCamera(); ///< Make camera follow player character.
	void IntroSequence(); ///< Function for intro sequence
	void Deathsequence(); ///< used for death screen and logic

	void WinSequence();
	

  public:
    ~CGame(); ///< Destructor.
	void NextLevel(); ///< Start next level.
    void Initialize(); ///< Initialize the game.
    void ProcessFrame(); ///< Process an animation frame.
	bool InsertWeapon(int item);
	bool RemoveWeapon(int item);
	bool CheckWeapon(int item);
    void Release(); ///< Release the renderer.
	int GetCurLevel() { return m_nCurLevel; };
	bool InsertItem(int item);
	bool RemoveItem(int item);
	bool CheckItem(int item);
	void Inventory();
}; //CGame
