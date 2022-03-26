/// \file Bow.h
/// \brief Interface for the bow type weapon

#pragma once

#include "Weapon.h"

class Fist : public CWeapon {

private:
	void Punch(CObject* p, eSpriteType bullet, eSoundType sound); ///< Punch action

public:
	Fist(); ///< Default Constructor.
	Fist(eSpriteType t, const Vector2& p); ///< Constructor.

	void PrimaryAction(); ///< Perform the primary action, for a bow, fire an arrow.

	float m_fHitBoxLife = 0.1f;
};

