/// \file Common.h
/// \brief Interface for the class CCommon.

#pragma once

#include "SndList.h"

//forward declarations to make the compiler less stroppy

class CObjectManager; 
class CRenderer;
class CParticleEngine2D;
class CObject;
class CTileManager;
class CPlayer;
class Skeleton;
class CItems;
class CGame;
class CHealthbar;

/// \brief The common variables class.
///
/// CCommon is a singleton class that encapsulates 
/// things that are common to different game components,
/// including game state variables.
/// Making it a singleton class means
/// that we can avoid passing its member variables
/// around as parameters, which makes the code
/// minisculely faster, and more importantly, reduces
/// function clutter.

class CCommon{
  protected:  
	static CGame* m_gGame; ///< THE GAME
    static CRenderer* m_pRenderer; ///< Pointer to the renderer.
    static CObjectManager* m_pObjectManager; ///< Pointer to the object manager.
	static CParticleEngine2D* m_pParticleEngine; ///< Pointer to the particle engine.
	static CTileManager* m_pTileManager; ///< Pointer to the tile manager.

	static bool m_bDrawAABBs; ///< Whether to draw AABBs.
	static Vector2 m_vWorldSize; ///< World height and width.
	static CPlayer* m_pPlayer; ///< Pointer to player character.
	static Skeleton* m_pSkeleton;
	static CItems* m_pItems; // pointer to items

	// objects that need not class
	static CObject* m_pLoadingscreen; ///< pointer to loading screen object (to walk through frames)
	static CObject* m_pHealthbar; ///< pointer to health bar object
	static CObject* m_pBossHealthbar; ///< pointer to boss health bar object
	static CObject* m_pHealthoutline; ///< pointer to health outline object
	
	const Vector2 healthoffset = Vector2(360.0, -370.0); // health offset
	const Vector2 bosshealthoffset = Vector2(0.0, 350.0); // health offset
	const bool m_bDevMode = true; ///< Whether the game is in development. Change to false on release.
	static bool m_bAbsoluteControls; ///<Determines whether directional controls are absolute or relative to cursor.
	static bool m_bNextLevel; // used in process frame to check if next level trigger hit
	static bool m_bPlayerDeath; // used in process frame to check if player death trigger
	static bool m_bGameWin; // win state for beating the boss

	//for blank game
    static eSoundType m_eDownSnd; ///< Key down sound.
    static eSoundType m_eUpSnd; ///< Key up sound.
}; //CCommon
