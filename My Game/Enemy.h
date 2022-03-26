/// \file Enemy.h
/// \brief Interface for the enemy object CEnemy

#pragma once

#include "Object.h"
#include "Weapon.h"

class CEnemy: public CObject{

protected:

	CWeapon* equipedWeapon; ///< The weapon this enemy has equiped.

	int health; ///< The health of this enemy.
	float t; // time used for death animation
	float total = 0.0f; // adds up time for death
	bool m_bClipping;

	float m_fDesiredRoll = 0; ///< Desired amount of roll.
	bool m_bRolling = false; ///< Object is rolling.
	float m_fSlerpControlFactor = 0; ///< Slerp control factor in [0, 1].

	void RotateToFacePlayer(); ///< Rotate to face player.

public:
	CEnemy(); ///< Default Constructor.
	CEnemy(eSpriteType t, const Vector2& p); ///< Constructor.

	virtual void move(); ///< Move the enemy.

	void DeathFX(); ///< Death special effects.

	void CollisionResponseWall(); ///< Collision response for hitting a wall.
	void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.

	void Damage(int d); ///< Deal damage equal to d to this enemy.

	void PrimaryAction();

	void equipWeapon(CWeapon* w, CObject* o); ///< Equip a weapon to this enemy

};//CEnemy.

