/// \file FireBall.h
/// \brief Interface for the bow type weapon

#pragma once

#include "Weapon.h"
#include "Boss.h"

class FireBall : public CWeapon {

private:
	void FireProjectile(CObject* p, eSpriteType bullet, eSoundType sound); ///< Fire object's gun.

	void FireProjectileSpread(CObject* pObj, eSpriteType largefireball, eSoundType sound);

public:
	FireBall(); ///< Default Constructor.
	FireBall(eSpriteType t, const Vector2& p); ///< Constructor.

	void PrimaryAction(); ///< Perform the primary action
	void SecondaryAction();
};

