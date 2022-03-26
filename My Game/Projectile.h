/// \file Projectile.h
/// \brief Interface for the projectile object CProjectile

#pragma once

#include "Weapon.h"

class CProjectile : public CWeapon {

public:
	CProjectile(eSpriteType t, const Vector2& p, CObject* o); ///< Constructor, not compat. w/ ObjectManager.create()

	void move(); ///< Move projectile

	void FistLifeTime();
	void CollisionResponseWall(); ///< Collision response for hitting a wall.
	void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.

	float m_fHitBoxLife = 0.2f;
};//CProjectile

