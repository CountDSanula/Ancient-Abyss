/// \file Bow.h
/// \brief Interface for the bow type weapon

#pragma once

#include "Weapon.h"

class Bow : public CWeapon {

private:
	void FireProjectile(CObject* p, eSpriteType bullet, eSoundType sound); ///< Fire object's gun.

	void FireProjectileSpread(CObject* pObj, eSpriteType largefireball, eSoundType sound);

public:
	Bow(); ///< Default Constructor.
	Bow(eSpriteType p, float delay = 1.0f); ///Constructor that takes the type of projectile to throw.
	Bow(eSpriteType t, const Vector2& p, float delay = 1.0f); ///< Constructor.

	void PrimaryAction(); ///< Perform the primary action, for a bow, fire an arrow.

};

