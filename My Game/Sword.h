/// \file Sword.h
/// \brief Interfacel for the sword type weapon

#pragma once

#include "Weapon.h"

class Sword : public CWeapon {

protected:
	//bool isSwinging = false;

public:
	Sword(); ///< Default Constructor
	Sword(eSpriteType t, const Vector2& p); ///< Constructor

	void move();

	bool onenemy = false;
	bool logic = true;
	void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.

	void PrimaryAction();
	//void SecondaryAction();

};

