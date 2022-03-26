/// \file Items.h
/// \brief Interface for the item object CItems

#pragma once

#include "Renderer.h"
#include "Object.h"
#include "Player.h"
//#include "ObjectManager.h"

//#include <string>


class CGame;

class CItems : public CObject {

protected:

	int health; ///< The health of this enemy.
	int itemLevel = 0;


public:
	CItems(); ///< Default Constructor.
	CItems(eSpriteType t, const Vector2& p); ///< Constructor.

	bool UseAttackSpeedPotion(CObject* p);

	void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.

	bool UseHealthPotion(CObject* p);

	bool UseSpeedPotion(CObject* p);

	float m_fPotionTimer = 0.0f;

	static CGame* gamePtr;
};//CItems