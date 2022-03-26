#pragma once

/// \file Demon.h
/// \brief An interface for the demon type enemy object

#pragma once

#include "Enemy.h"

class Demon : public CEnemy {

public:

	int log = 0; // for starting animation at 16 
	bool m_bClipping; // becomes false when killed to prevent player contact damage during deathfx

	Demon(eSpriteType t, const Vector2& p); ///< Constructor for demon type enemies

	void move(); ///< Move the enemy

	void DeathFX(); ///< Death special effects.

	void CollisionResponseWall(); ///< Collision response for hitting a wall.
	void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.

	void PrimaryAction();

	//void equipWeapon(CWeapon* w, CObject* o); ///< Equip a weapon to this enemy

	void animate();

};
