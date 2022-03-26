/// \file ObjectManager.h
/// \brief Interface for the object manager CObjectManager.

#pragma once

#include <list>

#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Projectile.h"
#include "Goblin.h"
#include "Demon.h"
#include "Skeleton.h"
#include "Boss.h"
#include "Fist.h"
#include "Sword.h"
#include "Bow.h"
#include "FireBall.h"
#include "Items.h"
#include "Lamp.h"

#include "Game.h"
#include "Component.h"
#include "Common.h"
#include "Settings.h"
#include "TileManager.h"

using namespace std;

/// \brief The object manager.
///
/// A collection of all of the game objects.

class CObjectManager :
	public CComponent,
	public CCommon,
	public CSettings {

private:
	list<CObject*> m_stdObjectList; ///< Object list.

	void BroadPhase(); ///< Broad phase collision detection and response.
	void NarrowPhase(CObject* p0, CObject* p1); ///< Narrow phase collision detection and response.

	void CullDeadObjects(); ///< Cull dead objects.

public:
	///< Constructors and Destructors
	CObjectManager(); ///< Constructor.
	~CObjectManager(); ///< Destructor.

	///< Map Creation
	void LoadMap(int level); ///< Load a level.

	///< Object Creation
	CPlayer* createPlayer(Vector2 pos);///< Create the player object.
	CObject* create(eSpriteType t, const Vector2& v); ///< Create new object.
	//void PickUpSword(Vector2 pos);

	void addObject(CObject* p); ///< Add an object to stdObjectList, for when the object has more params.

	///< Functions that do stuff
	void clear(); ///< Reset to initial conditions.
	void move(); ///< Move all objects.
	void draw(); ///< Draw all objects.

	bool m_bPlayerFist = true; // player starts weapons, just fists
	bool m_bPlayerSword = false;
	bool m_bPlayerBow = false;
	bool m_bPlayerFireBall = false;

	int m_nPlayerHealth = 10; // keeps player health consistent between levels
	bool m_bPlayerSpreadShot = false; // keeps spreadshot consistent between levels

	static CGame* gamePtr;
}; //CObjectManager