/// \file Player.h
/// \brief Interface for the player object class CPlayer.

#pragma once

#include "Object.h"
#include "Weapon.h"
//#include "Game.h"

/// \brief Player object.
///
/// Its the player object and should contian everything the player can do.

class CGame;

class CPlayer: public CObject{
private:
	CWeapon* equipedWeapon = nullptr; ///< The weapon the player has equipped.
	CWeapon* equipedWeapon2 = nullptr; ///< The sencodary the player has equipped.

	//float m_fAnimTimer = 0.0f; ///< Timer for animations.

public:
	CPlayer(eSpriteType t, const Vector2& p); ///< Constructor.
	void move(); ///< Move the player.
	void CollisionResponseWall(); ///< Collision response for hitting a wall.
	void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.
	void ContactDamage();

	void PrimaryAction();
	void SecondaryAction();

	void RandomGrunt(); ///< plays random grunt sound

	void equipWeapon(CWeapon* w, CObject* o); ///< Equip a weapon to the player.
	void equipWeapon2(CWeapon* w, CObject* o); ///< Equip a sencondary weapon to the player.

	void animate(); ///<Animate the player.

	float m_fHitTimer = 0.0f; // keeps track of how long since last hit
	float m_fHitDelay = 0.5f; // length of time between contact damage
	float m_fStatusTimer = 0.0f; // keeps track of how long speed potion lasts and other statuses

	int health; ///< The player's health
	int healthMax; ///< The player's max health

	bool m_bSpreadShot = false;
	float m_fPlayerSpeed = 300.0f; // variable to now hold player speed

	bool isSwinging = false; ///< player is swinging their sword.
	bool isThrowing = false; ///< playyer is throwing cross.

	static CGame* gamePtr;
};//CPlayer

