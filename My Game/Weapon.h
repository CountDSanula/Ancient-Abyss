/// \file Weapon.h
/// \brief Interface for the weapon object class CWeapon

#pragma once

#include "Object.h"

class CWeapon : public CObject {

protected:
	CObject* owner; ///< The owner of this weapon

	int damage; ///< The amount of damage dealt by the weapon
	float range; ///< The range of the weapon, bows have unlimited range.
	float viewAngle; ///< Half the angle the weapon can attack from

	float m_fAttackTimer = 0.0f; ///< Attack timer, counts time between attacks.

	float attackDelay = 1.0f; ///< The delay between each of this weapon's attacks use constructor to adjust timing for weap types

public:
	CWeapon(); ///< Default Constructor, req. for inheritance.
	CWeapon(eSpriteType t, const Vector2& p); ///< Constructor

	CObject* getOwner(); ///< Returns the owner of this weapon.
	void setOwner(CObject* o); ///< Changes the owner of this weapon.

	void move(); ///< Move the weapon, in general should follow owner.

	virtual void CollisionResponseWall(); ///< Collision response for hitting a wall.
	virtual void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.

	virtual void PrimaryAction();
	virtual void SecondaryAction();

};//CWeapon

