/// \file Goblin.h
/// \brief An interface for the goblin type enemy object

#pragma once

#include "Enemy.h"

class Goblin : public CEnemy{

public:
	Goblin(eSpriteType t, const Vector2& p); ///< Constructor for goblin type enemies

	void move(); ///< Move the enemy

	void DeathFX(); ///< Death special effects.

	int log = 0; // logic for goblin death

	void CollisionResponseWall(); ///< Collision response for hitting a wall.
	void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.

	void PrimaryAction();

	//void equipWeapon(CWeapon* w, CObject* o); ///< Equip a weapon to this enemy

	void animate();

	float m_fTargetX = 0.0f;
	float m_fTargetY = 0.0f;
	float m_fDistance = 0.0f;

	bool m_bClipping; // becomes false when killed to prevent player contact damage during deathfx
};

