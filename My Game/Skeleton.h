/// \file Skeleton.h
/// \brief Interface for the skeleton type enemy

#pragma once

#include "Enemy.h"

class Skeleton : public CEnemy {

public:
	Skeleton(eSpriteType t, const Vector2& p); ///< Constructor for skeleton type enemies

	int log = 0; // logic for skeleton death

	float deathTimer = 0.0f; // time used for death animation
	float total = 0.0f; // adds up time
	bool m_bClipping; // becomes false when killed to prevent player contact damage during deathfx
	bool m_bSpreadShot; // gives skeleton spreadshot arrows

	void move(); ///< Move the enemy

	void DeathFX(); ///< Death special effects.

	void CollisionResponseWall(); ///< Collision response for hitting a wall.
	void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.

	void PrimaryAction();

	//void equipWeapon(CWeapon* w, CObject* o); ///< Equip a weapon to this enemy

	void animate();

	static CGame* gamePtr;
};

